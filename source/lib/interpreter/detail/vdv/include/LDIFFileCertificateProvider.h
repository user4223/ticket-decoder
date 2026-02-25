// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "lib/interpreter/api/include/CertificateProvider.h"

#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/infrastructure/include/Logger.h"

#include <filesystem>
#include <memory>

namespace interpreter::detail::vdv
{
  /* Use 'Apache Directory Studio' and connect to public ldap 'ldaps://ldap-vdv-ion.telesec.de:636'
     to get company and root certificates.
     - Install 'Apache Directory Studio' or another useful tool
       - on macos via 'brew install apache-directory-studio'
       - on linux see: https://directory.apache.org/studio/download/download-linux.html
     - Create a new connection by using the host:port from above
     - Navigate to 'c=de,o=VDV Kernapplikations GmbH,ou=VDV KA'
     - Right click to 'ou=VDV KA' and select 'Export' and 'LDIF Export'
     - Click continue and select a file name and continue until the export starts,
       use '${workspaceFolder}/cert/VDV_Certificates.ldif' to make it working with default used below

     See: https://www.telesec.de/assets/downloads/Public-Key-Service/PKS-LDAP_Schnittstelle-V2.1-DE.pdf
  */
  class LDIFFileCertificateProvider : public api::CertificateProvider
  {
    infrastructure::Logger logger;
    struct Internal;
    std::shared_ptr<Internal> internal;

  public:
    LDIFFileCertificateProvider(infrastructure::Context &context, std::filesystem::path vdvCertificateLdifFile);

    virtual std::vector<std::string> getAuthorities() override;

    /* The value in 'authority' should match exactly one very specific entry in
       the list of exported certificates from public LDAP server identified by
       'cn=<authority>,ou=VDV KA,o=VDV Kernapplikations GmbH,c=de'
       4555564456xxxxxx -> EUVDVxxxxxx
       4445564456xxxxxx -> DEVDVxxxxxx
    */
    virtual std::optional<api::CertificateRawData> get(std::string authority) override;
  };
}
