
// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "CertificateProvider.h"

#include "lib/infrastructure/include/Logger.h"

#include <filesystem>
#include <map>

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
  */
  class LDIFFileCertificateProvider : public CertificateProvider
  {
    infrastructure::Logger logger;
    std::optional<std::map<std::string, Certificate>> const entries;

    static std::optional<std::map<std::string, Certificate>> import(std::filesystem::path file);

  public:
    LDIFFileCertificateProvider(infrastructure::LoggerFactory &loggerFactory, std::filesystem::path file = "cert/VDV_Certificates.ldif");

    virtual std::optional<Certificate> get(std::string commonName) override;
  };
}
