from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout


class TicketDecoderConan(ConanFile):
   name = 'ticket-decoder'
   version = 'v0.15'
   settings = "os", "compiler", "build_type", "arch"
   generators = "CMakeDeps"
   options = {
               "with_analyzer": [True, False],
               "with_python_module": [True, False]
             }
   default_options = {
               # global
               "*:shared": False,
               # ticket-decoder
               "with_analyzer": True,
               "with_python_module": True,
            }

   def requirements(self):
      # https://conan.io/center/recipes/opencv
      self.requires("opencv/4.12.0")
      # https://conan.io/center/recipes/zxing-cpp
      self.requires("zxing-cpp/2.3.0")
      # https://conan.io/center/recipes/nlohmann_json
      self.requires("nlohmann_json/3.12.0")
      # https://conan.io/center/recipes/easyloggingpp
      self.requires("easyloggingpp/9.97.1")
      # https://conan.io/center/recipes/pugixml
      self.requires("pugixml/1.15")
      # https://conan.io/center/recipes/botan
      # version 3.x is available but has breaking changes
      self.requires("botan/2.19.5")
      # https://conan.io/center/recipes/tclap
      self.requires("tclap/1.2.5")
      # https://conan.io/center/recipes/poppler
      self.requires("poppler-cpp/25.10.0")
      # https://conan.io/center/recipes/gtest
      self.requires("gtest/1.17.0")
      #
      # CONDITIONAL dependencies
      #
      if self.options.with_python_module == True:
         # https://conan.io/center/recipes/boost
         self.requires("boost/1.88.0")
      #
      # OVERWRITES
      #
      # https://conan.io/center/recipes/libiconv
      self.requires("libiconv/1.18", override=True)

   def generate(self):
      tc = CMakeToolchain(self)

      if self.options.with_analyzer == True:
         tc.variables["WITH_TICKET_ANALYZER"] = "TRUE"

      if self.options.with_python_module == True:
         tc.variables["WITH_PYTHON_MODULE"] = "TRUE"

      # tc.preprocessor_definitions["WITH_TICKET_ANALYZER"] = "TRUE"
      tc.generate()

   def config_options(self):
      if self.options.with_python_module:
         TicketDecoderConan.config_options_boost(
            self.options["boost"])

      TicketDecoderConan.config_options_opencv(
         self.options['opencv'],
         self.options.with_analyzer)

      TicketDecoderConan.config_options_zxing(
         self.options['zxing-cpp'])

      TicketDecoderConan.config_options_easylogging(
         self.options['easyloggingpp'])

   def build(self):
      cmake = CMake(self)
      cmake.configure()
      cmake.build()

   def layout(self):
      cmake_layout(self)

   @staticmethod
   def config_options_easylogging(easylogging_options):
      easylogging_options.enable_default_logfile = False

   @staticmethod
   def config_options_zxing(zxing_options):

      zxing_options.enable_encoders = False
      zxing_options.enable_c_api = False

   @staticmethod
   def config_options_opencv(opencv_options, with_analyzer: bool):

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
      opencv_options.calib3d = True # Required by objdetect
      opencv_options.photo = False
      opencv_options.stitching = False
      opencv_options.with_imgcodec_hdr = False
      opencv_options.with_imgcodec_pfm = False
      opencv_options.with_imgcodec_pxm = False
      opencv_options.with_imgcodec_sunraster = False

   @staticmethod
   def config_options_boost(boost_options):

      boost_options.pch = False
      boost_options.header_only = False
      boost_options.without_atomic = True
      boost_options.without_charconv = True
      boost_options.without_chrono = True
      boost_options.without_cobalt = True
      boost_options.without_container = False
      boost_options.without_context = True
      boost_options.without_contract = True
      boost_options.without_coroutine = True
      boost_options.without_date_time = True
      boost_options.without_exception = True
      boost_options.without_fiber = True
      boost_options.without_filesystem = True
      boost_options.without_graph = False
      boost_options.without_graph_parallel = True
      boost_options.without_iostreams = True
      boost_options.without_json = True
      boost_options.without_locale = True
      boost_options.without_log = True
      boost_options.without_math = False
      boost_options.without_mpi = True
      boost_options.without_nowide = True
      boost_options.without_process = True
      boost_options.without_program_options = True
      boost_options.without_python = False           # <-- that's what you are looki = for
      boost_options.without_random = False
      boost_options.without_regex = False
      boost_options.without_serialization = False
      boost_options.without_stacktrace = True
      boost_options.without_system = False
      boost_options.without_test = True
      boost_options.without_thread = True
      boost_options.without_timer = True
      boost_options.without_type_erasure = True
      boost_options.without_url = True
      boost_options.without_wave = True
