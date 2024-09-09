from ticket_decoder import DecoderFacade
import json

decoder_facade = DecoderFacade()
for result in decoder_facade.decode_file("images/Muster-UIC918-9"):
    print(result[0] + ": " + json.dumps(json.loads(result[1]), indent=3))

for result in decoder_facade.decode_file("images/Muster-UIC918-3"):
    print(result[0] + ": " + json.dumps(json.loads(result[1]), indent=3))
