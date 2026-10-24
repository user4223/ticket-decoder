#!/usr/bin/env python
#
# Download public UIC and VDV sample tickets
#

from pathlib import Path
from urllib.request import urlopen
from io import BytesIO
from zipfile import ZipFile

def main() -> None:

    project_root = Path(__file__).resolve().parents[1]
    image_root = project_root / "images"
    image_root.mkdir(parents=True, exist_ok=True)

    for entry in [
        {'folder': 'Muster-UIC918-3', 'url': 'https://assets.static-bahn.de/dam/jcr:c362849f-210d-4dbe-bb18-34141b5ba274/mdb_320951_muster-tickets_nach_uic_918-3_2.zip'},
        {'folder': 'Muster-UIC918-9', 'url': 'https://assets.static-bahn.de/dam/jcr:ec74454d-557b-438f-8ed9-689abcc276f5/Muster%20918-9.zip'},
        {'folder': 'Muster-VDV-KA', 'url': 'https://www.kcd-nrw.de/fileadmin/user_upload/2023-05-05_Testtickets.zip'}]:

        destination_root = image_root / entry['folder']

        with urlopen(entry['url']) as response:
            with ZipFile(BytesIO(response.read())) as zip:
                for info in zip.infolist():
                    if info.is_dir():
                        continue

                    info.filename = Path(info.filename).name
                    zip.extract(info, destination_root)


if __name__ == "__main__":
    main()
