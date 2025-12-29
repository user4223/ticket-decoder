# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout


class TicketDecoderConan(ConanFile):
   name = 'ticket-decoder'
   version = 'v0.16'
   settings = "os", "compiler", "build_type", "arch"
   generators = "CMakeDeps"
   options = {
               "shared": [True, False], 
               "fPIC": [True, False],
               "with_analyzer": [True, False],
               "with_python_module": [True, False],
               "with_square_detector": [True, False],
               "with_classifier_detector": [True, False],
               "with_barcode_decoder": [True, False],
               "with_pdf_input": [True, False],
               "with_signature_verifier": [True, False],
               "with_uic_interpreter": [True, False],
               "with_sbb_interpreter": [True, False],
             }
   default_options = {
               "shared": False,
               "fPIC": True,
               # ticket-decoder
               "with_analyzer": True,
               "with_python_module": True,
               "with_square_detector": True,
               "with_classifier_detector": True,
               "with_barcode_decoder": True,
               "with_pdf_input": True,
               "with_signature_verifier": True,
               "with_uic_interpreter": True,
               "with_sbb_interpreter": True,
            }

   def requirements(self):
      # https://conan.io/center/recipes/opencv
      self.requires("opencv/4.12.0")
      # https://conan.io/center/recipes/nlohmann_json
      self.requires("nlohmann_json/3.12.0")
      # https://conan.io/center/recipes/easyloggingpp
      self.requires("easyloggingpp/9.97.1")
      # https://conan.io/center/recipes/tclap
      self.requires("tclap/1.2.5")
      # https://conan.io/center/recipes/boost
      self.requires("boost/1.88.0")
      #
      # CONDITIONAL dependencies
      #
      if self.options.with_signature_verifier:
         # https://conan.io/center/recipes/pugixml
         self.requires("pugixml/1.15")
         # https://conan.io/center/recipes/botan
         # - version 3.x is available but has breaking changes
         self.requires("botan/2.19.5")
      if self.options.with_pdf_input:
         # https://conan.io/center/recipes/poppler
         self.requires("poppler-cpp/25.10.0")
      if self.options.with_barcode_decoder:
         # https://conan.io/center/recipes/zxing-cpp
         self.requires("zxing-cpp/2.3.0")
      if self.options.with_uic_interpreter:
         # https://conan.io/center/recipes/zlib
         self.requires("zlib/1.3.1")
      if self.options.with_sbb_interpreter:
         # https://conan.io/center/recipes/protobuf
         self.requires("protobuf/6.32.1")
      #
      # OVERWRITES
      #
      # https://conan.io/center/recipes/libiconv
      self.requires("libiconv/1.18", override=True)

   def build_requirements(self):
      self.tool_requires("cmake/[>=3.22]")
      # https://conan.io/center/recipes/gtest
      self.test_requires("gtest/1.17.0")

   def generate(self):
      toolchain = CMakeToolchain(self)

      if self.options.with_analyzer:
         TicketDecoderConan.add_config_switch(toolchain, "WITH_TICKET_ANALYZER")

      if self.options.with_python_module:
         TicketDecoderConan.add_config_switch(toolchain, "WITH_PYTHON_MODULE")

      if self.options.with_square_detector:
         TicketDecoderConan.add_config_switch(toolchain, "WITH_SQUARE_DETECTOR")

      if self.options.with_classifier_detector:
         TicketDecoderConan.add_config_switch(toolchain, "WITH_CLASSIFIER_DETECTOR")

      if self.options.with_barcode_decoder:
         TicketDecoderConan.add_config_switch(toolchain, "WITH_BARCODE_DECODER")

      if self.options.with_pdf_input:
         TicketDecoderConan.add_config_switch(toolchain, "WITH_PDF_INPUT")

      if self.options.with_signature_verifier:
         TicketDecoderConan.add_config_switch(toolchain, "WITH_SIGNATURE_VERIFIER")

      if self.options.with_uic_interpreter:
         TicketDecoderConan.add_config_switch(toolchain, "WITH_UIC_INTERPRETER")

      if self.options.with_sbb_interpreter:
         TicketDecoderConan.add_config_switch(toolchain, "WITH_SBB_INTERPRETER")

      toolchain.generate()

   def configure(self):
      self.output.highlight("with_analyzer: " + str(self.options.with_analyzer))
      self.output.highlight("with_python_module: " + str(self.options.with_python_module))
      self.output.highlight("with_square_detector: " + str(self.options.with_square_detector))
      self.output.highlight("with_classifier_detector: " + str(self.options.with_classifier_detector))
      self.output.highlight("with_barcode_decoder: " + str(self.options.with_barcode_decoder))
      self.output.highlight("with_pdf_input: " + str(self.options.with_pdf_input))
      self.output.highlight("with_signature_verifier: " + str(self.options.with_signature_verifier))
      self.output.highlight("with_uic_interpreter: " + str(self.options.with_uic_interpreter))
      self.output.highlight("with_sbb_interpreter: " + str(self.options.with_sbb_interpreter))

      TicketDecoderConan.config_options_boost(
         self.options["boost"],
         self.options.with_python_module)

      TicketDecoderConan.config_options_opencv(
         self.options['opencv'],
         self.options.with_analyzer,
         self.options.with_square_detector,
         self.options.with_classifier_detector)

      TicketDecoderConan.config_options_zxing(
         self.options['zxing-cpp'],
         self.options.with_barcode_decoder)

      TicketDecoderConan.config_options_easylogging(
         self.options['easyloggingpp'])

   def build(self):
      cmake = CMake(self)
      cmake.configure()
      cmake.build()

   def layout(self):
      cmake_layout(self)

   @staticmethod
   def add_config_switch(toolchain: CMakeToolchain, option_name: str):
      toolchain.variables[option_name] = "TRUE"
      toolchain.preprocessor_definitions[option_name] = "TRUE"

   @staticmethod
   def config_options_easylogging(easylogging_options):
      easylogging_options.enable_default_logfile = False

   @staticmethod
   def config_options_zxing(zxing_options, with_barcode_decoder: bool):
      if not with_barcode_decoder:
         return

      zxing_options.enable_encoders = False
      zxing_options.enable_c_api = False

   @staticmethod
   def config_options_opencv(opencv_options, with_analyzer: bool, with_square_detector: bool, with_classifier_detector: bool):
      opencv_options.highgui = True if with_analyzer else False
      opencv_options.videoio = True if with_analyzer else False
      opencv_options.parallel = False
      opencv_options.stitching = False
      opencv_options.video = False # Disables video processing only, required video-io keeps enabled
      opencv_options.with_ffmpeg = False
      opencv_options.with_jpeg2000 = False
      opencv_options.with_tiff = False
      opencv_options.with_webp = False
      opencv_options.with_quirc = False
      opencv_options.with_openexr = False
      opencv_options.with_eigen = False
      opencv_options.with_tesseract = False
      opencv_options.with_protobuf = False
      opencv_options.gapi = False
      opencv_options.ml = False
      opencv_options.dnn = False
      opencv_options.calib3d = True if with_classifier_detector else False # Required by objdetect
      opencv_options.objdetect = True if with_classifier_detector else False
      opencv_options.photo = False
      opencv_options.stitching = False
      opencv_options.with_imgcodec_hdr = False
      opencv_options.with_imgcodec_pfm = False
      opencv_options.with_imgcodec_pxm = False
      opencv_options.with_imgcodec_sunraster = False

   @staticmethod
   def config_options_boost(boost_options, with_python_module: bool):
      boost_options.pch = True # Precompiled headers may speed up compilation
      boost_options.header_only = False if with_python_module else True # Without python modules we do need the headers only

      boost_options.without_atomic = True
      boost_options.without_charconv = True
      boost_options.without_chrono = True
      boost_options.without_cobalt = True
      boost_options.without_container = False if with_python_module else True # Required by python
      boost_options.without_context = True
      boost_options.without_contract = True
      boost_options.without_coroutine = True
      boost_options.without_date_time = True
      boost_options.without_exception = True
      boost_options.without_fiber = True
      boost_options.without_filesystem = True
      boost_options.without_graph = False if with_python_module else True # Required by python
      boost_options.without_graph_parallel = True
      boost_options.without_iostreams = True
      boost_options.without_json = True
      boost_options.without_locale = True
      boost_options.without_log = True
      boost_options.without_math = False if with_python_module else True # Required by python
      boost_options.without_mpi = True
      boost_options.without_nowide = True
      boost_options.without_process = True
      boost_options.without_program_options = True
      boost_options.without_python = False if with_python_module else True # Actual direct dependency
      boost_options.without_random = False if with_python_module else True # Required by python
      boost_options.without_regex = False if with_python_module else True # Required by python
      boost_options.without_serialization = False if with_python_module else True # Required by python
      boost_options.without_stacktrace = True
      boost_options.without_system = False if with_python_module else True # Required by python
      boost_options.without_test = True
      boost_options.without_thread = True
      boost_options.without_timer = True
      boost_options.without_type_erasure = True
      boost_options.without_url = True
      boost_options.without_wave = True
