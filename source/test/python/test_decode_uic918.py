from unittest import TestCase, skipIf
from json import loads
from pathlib import Path
from base64 import b64encode
from ticket_decoder import DecoderFacade

class TestDecodeUIC918(TestCase):

    @staticmethod
    def loadRaw(file_name) -> str:
        path = Path(__file__).parent.parent / 'uic918' / 'etc' / file_name
        with open(path, mode='rb') as file:
            return b64encode(file.read())

    def test_decode_uic918(self):
        decoder_facade = DecoderFacade()
        result = loads(decoder_facade.decode_uic918(TestDecodeUIC918.loadRaw('Muster 918-9 CityTicket.raw')))
        assert result['records']['U_FLEX']['transportDocuments'][0]['openTicket']['fromStationName'] == 'Kassel+City'
        assert result['validated'] == 'false'

    def test_decode_uic918_fail(self):
        with self.assertRaisesRegex(RuntimeError, '^Decoding failed with: Interpretation failed, this could be due to version mismatch or missing implementation for the given type or just due to an error:.*'):
            decoder_facade = DecoderFacade()
            decoder_facade.decode_uic918('no base64 encoded UIC918 data')

    input_file = 'images/Muster-UIC918-9/Muster 918-9 CityTicket.pdf'

    @skipIf(not Path(input_file).exists(), "Missing input file: " + input_file)
    def test_decode_files(self):
        decoder_facade = DecoderFacade()
        result = decoder_facade.decode_files(self.input_file)
        assert len(result) == 1
        records = loads(result[0][1])
        assert records['records']['U_FLEX']['transportDocuments'][0]['openTicket']['fromStationName'] == 'Kassel+City'
        assert records['validated'] == 'false'

    def test_decode_files_not_existing(self):
        with self.assertRaisesRegex(RuntimeError, '^Decoding failed with: Path to load input elements from does not exist: Not existing file$'):
            decoder_facade = DecoderFacade()
            decoder_facade.decode_files('Not existing file')

    def test_decode_files_without_aztec_code(self):
        decoder_facade = DecoderFacade()
        result = decoder_facade.decode_files('source/test/io/etc/minimal.pdf')
        assert len(result) == 0

    def test_two_instances(self):
        decoder_facadeA = DecoderFacade()
        decoder_facadeB = DecoderFacade()
