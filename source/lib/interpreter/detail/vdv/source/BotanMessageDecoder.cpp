// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/BotanMessageDecoder.h"
#include "../include/VDVUtility.h"

#include "lib/interpreter/detail/common/include/Context.h"

#include "lib/infrastructure/include/Logging.h"

#include <botan/pkcs8.h>
#include <botan/pubkey.h>
#include <botan/x509cert.h>
#include <botan/auto_rng.h>
#include <botan/data_src.h>

namespace interpreter::detail::vdv
{
    BotanMessageDecoder::BotanMessageDecoder(infrastructure::LoggerFactory &lf, CertificateProvider &cp)
        : logger(CREATE_LOGGER(lf)),
          certificateProvider(cp)
    {
    }

    std::optional<std::vector<std::uint8_t>> BotanMessageDecoder::decode(Envelop const &envelop)
    {
        auto context = common::Context(envelop.certificate);
        auto const certificateTag = getTag(context);
        auto const certificateLength = getLength(context);
        auto const certificate = context.consumeBytes(certificateLength);
        auto const exponentTag = getTag(context);
        auto const exponentLength = getLength(context);
        auto const exponent = context.consumeBytes(exponentLength);

        auto const rootCertificate = certificateProvider.get("4555564456100106");
        auto const companyCertificate = certificateProvider.get(envelop.authority);

        /* The value in 'authority' should match exactly one very specific entry in
           the list of exported certificates from public LDAP server identified by
           'cn=<authority>,ou=VDV KA,o=VDV Kernapplikations GmbH,c=de'
           4555564456xxxxxx -> EUVDVxxxxxx
           4445564456xxxxxx -> DEVDVxxxxxx
        */

        if (companyCertificate && rootCertificate)
        {
            /*
            auto rng = std::make_unique<Botan::AutoSeeded_RNG>();
            auto dataSource = Botan::DataSource_Memory(companyCertificate->certificate.data(), companyCertificate->certificate.size());
            auto const key = Botan::PKCS8::load_key(dataSource, *rng);
            // auto const key = Botan::X509_Certificate(dataSource);
            auto decoder = Botan::PK_Decryptor_EME(*key, *rng, "RSA-1984(SHA-1)");
            auto const message = decoder.decrypt(certificate.data(), certificate.size());
            */
        }

        return std::nullopt;
    }
}
