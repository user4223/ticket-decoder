import ticket_decoder
import json

for result in ticket_decoder.decode_file("images/Muster-UIC918-9"):
    print(result[0] + ": " + json.dumps(json.loads(result[1]), indent=3))

for result in ticket_decoder.decode_file("images/Muster-UIC918-3"):
    print(result[0] + ": " + json.dumps(json.loads(result[1]), indent=3))
