#!/usr/bin/env python
#
# Download UIC public keys from public key server to allow verification of UIC tickets
#

from pathlib import Path
from urllib.request import urlopen
from datetime import date
from xml.dom import minidom


def main() -> None:

    project_root = Path(__file__).resolve().parents[1]
    certificate_root = project_root / "cert"
    certificate_root.mkdir(parents=True, exist_ok=True)

    with urlopen("https://railpublickey.uic.org/download.php", timeout=30) as response:
        key_content = response.read().decode("utf-8")

    if not key_content:
        raise RuntimeError("No UIC public keys found")

    key_file = certificate_root / "UIC_PublicKeys.xml"
    if key_file.exists():
        key_file.replace(certificate_root / f"UIC_PublicKeys_{date.today().isoformat()}.xml")

    with minidom.parseString(key_content) as key_xml:
        key_file.write_text(key_xml.toprettyxml(newl='\n'))
        print(f'Installed {len(key_xml.getElementsByTagName('publicKey'))} UIC keys in: {str(key_file)}')


if __name__ == "__main__":
    main()
