from unittest import TestCase
from json import loads
from pathlib import Path
from base64 import b64encode
from ticket_decoder import decode_uic918

class TestDecodeUIC918(TestCase):

    @staticmethod
    def loadRaw(file_name):
        path = Path(__file__).parent.parent / 'uic918' / 'etc' / file_name
        with open(path, mode='rb') as file:
            return b64encode(file.read())

    def test_decode_uic918(self):
        result = loads(decode_uic918(TestDecodeUIC918.loadRaw('Muster 918-9 CityTicket.raw')))
        assert result['records']['U_FLEX']['transportDocuments'][0]['openTicket']['fromStationName'] == 'Kassel+City'
        assert result['validated'] == 'false'
