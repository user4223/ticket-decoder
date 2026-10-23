# SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

from pathlib import Path
from urllib.request import urlopen


def main() -> None:
    project_root = Path(__file__).resolve().parents[3]
    certificate_path = project_root / "cert" / "UIC_PublicKeys.xml"
    certificate_path.parent.mkdir(parents=True, exist_ok=True)

    with urlopen("https://railpublickey.uic.org/download.php", timeout=30) as response:
        certificate = response.read()
    if not certificate:
        raise RuntimeError("The UIC public-key download was empty")
    certificate_path.write_bytes(certificate)


if __name__ == "__main__":
    main()
