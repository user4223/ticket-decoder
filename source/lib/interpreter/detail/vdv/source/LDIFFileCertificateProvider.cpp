
// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/LDIFFileCertificateProvider.h"

#include "lib/infrastructure/include/Logging.h"

#include "lib/utility/include/Base64.h"

#include <stdexcept>
#include <fstream>
#include <regex>
#include <iterator>

namespace interpreter::detail::vdv
{
    static std::string getValue(std::vector<std::string> const &entryLines, std::string key)
    {
        auto const result = std::find_if(std::begin(entryLines), std::end(entryLines), [&](auto const &line)
                                         { return line.starts_with(key); });
        return result == entryLines.end()
                   ? std::string{}
                   : result->substr(key.size(), result->size() - key.size());
    }

    std::optional<std::map<std::string, Certificate>> LDIFFileCertificateProvider::import(std::filesystem::path file)
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

        std::map<std::string, Certificate> entries;
        std::transform(std::begin(entryLines), std::end(entryLines), std::inserter(entries, entries.begin()), [](auto const &lines)
                       {
                        auto commonName = getValue(lines, "cn: "); 
                        return std::make_pair(commonName, Certificate{
                             commonName,
                             getValue(lines, "dn: "),
                             getValue(lines, "description: "),
                             utility::base64::decode(getValue(lines, "cACertificate:: "))}); });

        return entries;
    }

    LDIFFileCertificateProvider::LDIFFileCertificateProvider(infrastructure::LoggerFactory &loggerFactory, std::filesystem::path file)
        : logger(CREATE_LOGGER(loggerFactory)),
          entries(import(file))
    {
        if (!entries)
        {
            LOG_INFO(logger) << "Failed to import certificates from given LDIF file: " << file.string();
        }
        else
        {
            LOG_DEBUG(logger) << "Imported no of certificates: " << entries->size();
        }
    }

    std::optional<Certificate> LDIFFileCertificateProvider::get(std::string commonName)
    {
        if (!entries)
        {
            return std::nullopt;
        }

        auto const entry = entries->find(commonName);
        return entry == entries->end()
                   ? std::nullopt
                   : std::make_optional(entry->second);
    }
}
