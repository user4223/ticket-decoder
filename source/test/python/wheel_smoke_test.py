# SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

import json
import os
from pathlib import Path

from ticket_decoder import DecoderFacade


def main() -> None:
    os.chdir(Path(__file__).resolve().parents[3])
    decoder = DecoderFacade()
    result = decoder.decode_base64(
        "I1VUMDExMDgwMDAwMDEwLQIUNDW6hIQDALJXkeinsN8LGG6JJ9kCFQCbqcKtFige6dO0zRFzeaDKaTWlgwAAADAzNTh4nAuN93B1dDEwNDAwNTY0sDCwCIkwc3cxCGRAAgZmhgZGBkZGhibGFgYuri6uofEhPo6RQE1GRmYBPo6eBgZAnSZA0tDIAATMvXMSi4tTrQxMgRwjiJhZQGpRsZ4VmG1oAbLQwNItMaMoO7GoJBXMhWg2NHI/vCenJDM9OzWzxMrA0NAUZAJQ3sQwLD9PwchAz9BAD+QaBQNLoGlcTpnFCkaGCEFjkBUmhpYg46CmmgJ5RlCeqYEx3AFGpsHJGTmpxeWZ6boe+TnFJamZebohmcnZqSWh8W4+rhGGxiD7kzYxtD3kPbjo9A1RQY5GVpkWi5WJWyp6NBZwNTZwzDj07AXfsmMXnlw686KMsaFJiUGeoUGQr5STL+lMk7A0X1/Uh08vp7AvPNYgmMF0wKFBEqeVoDAMCwXHhBFDCjDYWRn1+pp0JUqYf0mUO4Z4MDiGSjAwCBqABNnvsAoIdzAIAAAFHHyh"
    )
    if not isinstance(json.loads(result), dict):
        raise AssertionError("Decoder smoke test did not return a JSON object")


if __name__ == "__main__":
    main()
