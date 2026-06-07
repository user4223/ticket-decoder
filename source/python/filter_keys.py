# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

from ticket_decoder import DecoderFacade
from json import loads
from jsonpath_ng.ext import parse

decoder_facade = DecoderFacade()


UIC918_9_EXPRESSION = parse("$..[firstName, lastName, uniqueTicketKey, issuerPNR, editionTime]")
result = decoder_facade.decode_files("images/Muster-UIC918-9", splitting_mode='21')
print(f'### UIC918-9 items found: {len(result)}')
for origin, value in result.items():
    print(f'{origin}: {", ".join([x.value for x in UIC918_9_EXPRESSION.find(loads(value))])}')


UIC918_3_EXPRESSION = parse("$..fields[S023, S015, S016].value")
result = decoder_facade.decode_files("images/Muster-UIC918-3", rotation_degree=2, splitting_mode='42')
print(f'### UIC918-3 items found: {len(result)}')
for origin, value in result.items():
    print(f'{origin}: {", ".join([x.value for x in UIC918_3_EXPRESSION.find(loads(value))])}')


result = decoder_facade.decode_files("images")
verified = 0
for origin, value in result.items():
    verified += 1 if loads(value).get('validated') == 'true' else 0
print(f'### Overall items found: {len(result)} (verified: {verified})')
