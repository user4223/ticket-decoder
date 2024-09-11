from ticket_decoder import DecoderFacade
from json import loads
from jsonpath2.path import Path
from typing import Tuple

UNIQUE_KEY_EXPRESSION = Path.parse_str('$..["firstName", "uniqueTicketKey", "issuerPNR", "editionTime"]')

def get_details(result: str) -> str:
    json_result = loads(result)
    return str({x.current_value for x in UNIQUE_KEY_EXPRESSION.match(json_result)})

def get_source_and_details(result: Tuple[str,str]) -> str:
    return result[0] + ": " + get_details(result[1])


decoder_facade = DecoderFacade()

print("\n### UIC918-9")
for result in decoder_facade.decode_file("images/Muster-UIC918-9"):
    print(get_source_and_details(result))

print("\n### UIC918-3")
for result in decoder_facade.decode_file("images/Muster-UIC918-3"):
    print(get_source_and_details(result))

print("\n### Raw input")
# Does not make sense in real world since it decodes the data twice, but it shows whats possible when uic918-data is available from plain aztec-decoder
base64_encoded_raws = [loads(item[1])['raw'] for item in decoder_facade.decode_file("images/")]
for raw in base64_encoded_raws:
    print(get_details(decoder_facade.decode_uic918(raw)))
