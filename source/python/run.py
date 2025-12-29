# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

from ticket_decoder import DecoderFacade
from json import loads
from jsonpath2.path import Path
from typing import Tuple

UNIQUE_KEY_EXPRESSION = Path.parse_str('$..["firstName", "uniqueTicketKey", "issuerPNR", "editionTime"]')

def get_details(result: str) -> str:
    json_result = loads(result)
    return str({x.current_value for x in UNIQUE_KEY_EXPRESSION.match(json_result)}) if 'records' in json_result else "{}"

def get_source_and_details(result: Tuple[str,str]) -> str:
    return result[0] + ": " + get_details(result[1])


decoder_facade = DecoderFacade(fail_on_interpreter_error = False, public_key_file = "cert/UIC_PublicKeys.xml")

print("\n### UIC918-9")
for result in decoder_facade.decode_files("images/Muster-UIC918-9"):
    print(get_source_and_details(result))

print("\n### UIC918-3")
for result in decoder_facade.decode_files("images/Muster-UIC918-3"):
    print(get_source_and_details(result))

print("\n### Raw input")
# Does not make sense in real world since it decodes the data twice, but it shows whats possible when uic918-data is available from plain aztec-decoder
results = [loads(item[1]) for item in decoder_facade.decode_files("images/")]
for result in results:
    if 'raw' in result:
        print(get_details(decoder_facade.decode_uic918(result['raw'])))

validated = 0
for result in results:
    if 'validated' in result and result['validated'] == "true":
        validated += 1
print("\n### Validated: " + str(validated) + "/" + str(len(results)))
