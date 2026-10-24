#!/usr/bin/env python
#
# Download VDV certificates from the public LDAP server
#

from pathlib import Path
from ldap3 import Server, Connection, ANONYMOUS
from datetime import date


def main() -> None:

    project_root = Path(__file__).resolve().parents[1]
    certificate_root = project_root / "cert"
    certificate_root.mkdir(parents=True, exist_ok=True)

    server = Server('ldaps://ldap-vdv-ion.telesec.de:636', connect_timeout=30)
    with Connection(server, authentication=ANONYMOUS, auto_bind=True) as connection:
        connection.search('o=VDV Kernapplikations GmbH,c=DE', '(objectClass=*)', attributes=['*'])

        if not connection.entries:
            raise RuntimeError("No matching VDV certificates found")

        certificate_file = certificate_root / "VDV_Certificates.ldif"
        if certificate_file.exists():
            certificate_file.replace(certificate_root / f"VDV_Certificates_{date.today().isoformat()}.ldif")

        certificate_file.write_text(connection.response_to_ldif(line_separator='\n'))
        print(f'Installed {len(connection.entries)} VDV certificates in: {str(certificate_file)}')


if __name__ == "__main__":
    main()
