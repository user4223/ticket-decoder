// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/BotanMessageDecoder.h"

#include "lib/infrastructure/include/Logging.h"

#include <botan/x509_ca.h>

namespace interpreter::detail::vdv
{
    BotanMessageDecoder::BotanMessageDecoder(infrastructure::LoggerFactory &lf, CertificateProvider &cp)
        : logger(CREATE_LOGGER(lf)),
          certificateProvider(cp)
    {
    }

    std::optional<std::vector<std::uint8_t>> BotanMessageDecoder::decode(Envelop const &envelop)
    {
        /* The value in 'authority' should match exactly one very specific entry in
           the list of exported certificates from public LDAP server identified by
           'cn=<authority>,ou=VDV KA,o=VDV Kernapplikations GmbH,c=de'
           4555564456xxxxxx -> EUVDVxxxxxx
           4445564456xxxxxx -> DEVDVxxxxxx
        */
        auto const companyCertificate = certificateProvider.get(envelop.authority);
        auto const rootCertificate = certificateProvider.get("4555564456100106");

        return std::nullopt;
    }
}
