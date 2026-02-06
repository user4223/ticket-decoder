// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/LDIFFileCertificateProvider.h"
#include "../include/VDVUtility.h"

#include "lib/interpreter/detail/common/include/Context.h"
#include "lib/interpreter/detail/common/include/InterpreterUtility.h"

#include "lib/utility/include/Base64.h"

#include "lib/infrastructure/include/Logging.h"

#include <stdexcept>
#include <fstream>
#include <regex>
#include <iterator>
#include <algorithm>

namespace interpreter::detail::vdv
{
    struct LDIFCertificate
    {
        std::string commonName;
        std::string distinguishedName;
        std::string description;
        std::vector<std::uint8_t> data;
        std::optional<Certificate> mutable certificate; // Cache a certificate extracted once

        std::optional<Certificate> get() const
        {
            if (certificate)
            {
                return certificate;
            }

            auto context = common::Context(data);
            auto payload = consumeExpectedTagValue(context, {0x7f, 0x21});
            ensureEmpty(context);

            auto content = std::span<std::uint8_t const>{};
            auto signature = std::span<std::uint8_t const>{};
            auto remainder = std::span<std::uint8_t const>{};

            auto payloadContext = common::Context(payload);
            while (!payloadContext.isEmpty())
            {
                auto const tag = consumeTag(payloadContext);
                auto const value = payloadContext.consumeBytes(consumeLength(payloadContext));
                if (tag == TagType{0x5f, 0x4e})
                {
                    content = value;
                }
                else if (tag == TagType{0x5f, 0x37})
                {
                    signature = value;
                }
                else if (tag == TagType{0x5f, 0x38})
                {
                    remainder = value;
                }
                else
                {
                    throw std::runtime_error(std::string("Unexpected tag found: ") + common::bytesToHexString(tag));
                }
            }

            certificate = std::make_optional(Certificate{commonName, description, Signature{signature, remainder}, content});
            return certificate;
        }
    };

    struct LDIFFileCertificateProvider::Internal
    {
        std::optional<std::map<std::string, LDIFCertificate>> const entries;

        static std::string getValue(std::vector<std::string> const &entryLines, std::string key)
        {
            auto const result = std::find_if(std::begin(entryLines), std::end(entryLines), [&](auto const &line)
                                             { return line.starts_with(key); });
            return result == entryLines.end()
                       ? std::string{}
                       : result->substr(key.size(), result->size() - key.size());
        }

        static std::optional<std::map<std::string, LDIFCertificate>> import(std::filesystem::path file)
        {
            if (!std::filesystem::exists(file) || !std::filesystem::is_regular_file(file))
            {
                return std::nullopt;
            }

            auto entryLines = std::vector<std::vector<std::string>>();
            auto entry = std::vector<std::string>();
            auto stream = std::ifstream(file);
            auto count = 1;
            for (auto line = std::string(); std::getline(stream, line); count++)
            {
                if (line.starts_with('#')) // Ignore comments
                {
                    continue;
                }

                if (line.empty()) // An empty line (2 directly following newlines) indicate a new entry
                {
                    entryLines.emplace_back(std::move(entry));
                    entry = std::vector<std::string>();
                    continue;
                }

                if (line.starts_with(' ')) // Line starting with space indicates line-folding, that means the following line belongs the the line before
                {
                    if (entry.empty())
                    {
                        throw std::runtime_error(std::string("Expect line-folding after regular line only but found it as a starting line: ") + std::to_string(count));
                    }
                    (*entry.rbegin()) += line.erase(0, 1);
                    continue;
                }

                entry.push_back(line);
            }

            /* Remove all entries not having a distinguished name matching the following pattern
             */
            auto const entryStart = std::regex("dn[:] cn[=]\\w+,ou[=]VDV KA,o[=]VDV Kernapplikations GmbH,c[=]de", std::regex::icase);
            std::erase_if(entryLines, [&](auto const &lines)
                          { return lines.end() == std::find_if(std::begin(lines), std::end(lines),
                                                               [&](auto const &line)
                                                               { return std::regex_match(line, entryStart); }); });

            /* Given file might be in LDIF format but might not contain desired entries matching the distinguished name above
             */
            if (entryLines.empty())
            {
                return std::nullopt;
            }

            std::map<std::string, LDIFCertificate> entries;
            std::transform(std::begin(entryLines), std::end(entryLines), std::inserter(entries, entries.begin()), [](auto const &lines)
                           {
                        auto commonName = getValue(lines, "cn: ");
                        return std::make_pair(commonName, LDIFCertificate{
                                                              commonName,
                                                              getValue(lines, "dn: "),
                                                              getValue(lines, "description: "),
                                                              utility::base64::decode(getValue(lines, "cACertificate:: "))}); });

            return entries;
        }
    };

    LDIFFileCertificateProvider::LDIFFileCertificateProvider(infrastructure::LoggerFactory &loggerFactory, std::filesystem::path file)
        : logger(CREATE_LOGGER(loggerFactory)),
          internal(std::make_shared<Internal>(Internal::import(file)))
    {
        if (!internal->entries)
        {
            LOG_INFO(logger) << "Failed to import certificates from given LDIF file: " << file.string();
        }
        else
        {
            LOG_DEBUG(logger) << "Imported no of certificates: " << internal->entries->size();
        }
    }

    std::vector<std::string> LDIFFileCertificateProvider::getAuthorities()
    {
        if (!internal->entries)
        {
            return {};
        }

        auto keys = std::vector<std::string>{};
        std::transform(std::begin(*(internal->entries)), std::end(*(internal->entries)), std::back_inserter(keys), [](auto const &entry)
                       { return entry.first; });
        return keys;
    }

    std::optional<Certificate> LDIFFileCertificateProvider::get(std::string authority)
    {
        if (!internal->entries)
        {
            return std::nullopt;
        }

        auto const entry = internal->entries->find(authority);
        return entry == internal->entries->end()
                   ? std::nullopt
                   : entry->second.get();
    }
}
