#!/usr/bin/env bash
# Download VDV certificates from the public LDAP server
source venv/bin/activate

python -c "
import ldap3
server = ldap3.Server('ldaps://ldap-vdv-ion.telesec.de:636', connect_timeout=10)
conn = ldap3.Connection(server, authentication=ldap3.ANONYMOUS, auto_bind=True)
conn.search('o=VDV Kernapplikations GmbH,c=DE', '(objectClass=*)', attributes=['*'])
with open('cert/VDV_Certificates.ldif', 'w') as f:
    for entry in conn.entries:
        f.write(str(entry.entry_to_ldif()) + '\n')
print(f'Done - {len(conn.entries)} entries exported')
"
