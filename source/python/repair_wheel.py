# SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

import os
from pathlib import Path
import subprocess
import sys


def main() -> None:
    wheel, destination = sys.argv[1:]
    conan_home = Path(os.environ.get("CONAN_HOME", Path.home() / ".conan2"))
    coann_package_cache = conan_home / "p" / "b"
    dll_directories = sorted({str(path.parent) for path in coann_package_cache.rglob("*.dll")})
    if not dll_directories:
        return

    subprocess.run(
        [
            "delvewheel",
            "repair",
            "--add-path",
            os.pathsep.join(dll_directories),
            "-w",
            destination,
            wheel,
        ],
        check=True,
    )


if __name__ == "__main__":
    main()
