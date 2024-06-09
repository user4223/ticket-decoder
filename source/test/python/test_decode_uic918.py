from unittest import TestCase, skipIf
from json import loads
from pathlib import Path
from base64 import b64encode
from ticket_decoder import decode_uic918, decode_file

class TestDecodeUIC918(TestCase):

    @staticmethod
    def loadRaw(file_name) -> str:
        path = Path(__file__).parent.parent / 'uic918' / 'etc' / file_name
        with open(path, mode='rb') as file:
            return b64encode(file.read())

    def test_decode_uic918(self):
        result = loads(decode_uic918(TestDecodeUIC918.loadRaw('Muster 918-9 CityTicket.raw')))
        assert result['records']['U_FLEX']['transportDocuments'][0]['openTicket']['fromStationName'] == 'Kassel+City'
        assert result['validated'] == 'false'

    def test_decode_uic918_fail(self):
        with self.assertRaisesRegex(RuntimeError, '^Decoding failed with: No UIC918 structured data found, version not matching or implemented, or interpretation failed:.*'):
            decode_uic918('no base64 encoded UIC918 data')

    input_file = 'images/Muster_918-9_2024/Muster 918-9 CityTicket.pdf'

    @skipIf(not Path(input_file).exists(), "Missing input file: " + input_file)
    def test_decode_file(self):
        result = decode_file(self.input_file)
        assert len(result) == 1
        records = loads(result[0])
        assert records['records']['U_FLEX']['transportDocuments'][0]['openTicket']['fromStationName'] == 'Kassel+City'
        assert records['validated'] == 'false'

    def test_decode_file_not_existing(self):
        with self.assertRaisesRegex(RuntimeError, '^Decoding failed with: Path to load input elements from does not exist: Not existing file$'):
            decode_file('Not existing file')

    def test_decode_file_without_aztec_code(self):
        result = decode_file('source/test/io/etc/minimal.pdf')
        assert len(result) == 0
