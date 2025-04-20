from conans import ConanFile
from conan.tools.cmake import CMake, cmake_layout

class TicketDecoderConan(ConanFile):
   name = 'ticket-decoder'
   version = 'v0.10'
   settings = "os", "compiler", "build_type", "arch"
   generators = "CMakeToolchain", "CMakeDeps"
   options = {
               "with_analyzer": [True, False]
             }
   requires = [
               # https://conan.io/center/recipes/opencv
               ("opencv/4.11.0"),
               # https://conan.io/center/recipes/zxing-cpp
               ("zxing-cpp/2.3.0"),
               # https://conan.io/center/recipes/nlohmann_json
               ("nlohmann_json/3.11.3"),
               # https://conan.io/center/recipes/easyloggingpp
               ("easyloggingpp/9.97.1"),
               # https://conan.io/center/recipes/pugixml
               ("pugixml/1.15"),
               # https://conan.io/center/recipes/botan
               ("botan/2.19.5"),
               # https://conan.io/center/recipes/tclap
               ("tclap/1.2.5"),
               # https://conan.io/center/recipes/boost
               ("boost/1.87.0"),
               # https://conan.io/center/recipes/poppler
               ("poppler-cpp/25.4.0"),
               # https://conan.io/center/recipes/gtest
               ("gtest/1.15.0"),
               ]
   default_options = {
                # global
                "*:shared": False,
                # ticket-decoder
                "with_analyzer": True,
                # opencv
                "opencv:highgui": True,
                "opencv:parallel": False,
                "opencv:stitching": False,
                "opencv:video": False, # Disables video processing only, required video-io keeps enabled
                "opencv:with_ffmpeg": False,
                "opencv:with_jpeg2000": False,
                "opencv:with_tiff": False,
                "opencv:with_webp": False,
                "opencv:with_quirc": False,
                "opencv:with_openexr": False,
                "opencv:with_eigen": False,
                "opencv:with_tesseract": False,
                "opencv:with_protobuf": False,
                "opencv:gapi": False,
                "opencv:ml": False,
                "opencv:dnn": False,
                "opencv:calib3d": True, # Required by objdetect
                "opencv:photo": False,
                "opencv:stitching": False,
                # zxing-cpp
                "zxing-cpp:enable_encoders": False,
                # easyloggingpp
                "easyloggingpp:enable_default_logfile": False,
                # botan
                # --
                # boost
                "boost:pch": False,
                "boost:header_only": False,
                "boost:without_atomic": True,
                "boost:without_charconv": True,
                "boost:without_chrono": True,
                "boost:without_cobalt": True,
                "boost:without_container": False,
                "boost:without_context": True,
                "boost:without_contract": True,
                "boost:without_coroutine": True,
                "boost:without_date_time": True,
                "boost:without_exception": True,
                "boost:without_fiber": True,
                "boost:without_filesystem": True,
                "boost:without_graph": False,
                "boost:without_graph_parallel": True,
                "boost:without_iostreams": True,
                "boost:without_json": True,
                "boost:without_locale": True,
                "boost:without_log": True,
                "boost:without_math": False,
                "boost:without_mpi": True,
                "boost:without_nowide": True,
                "boost:without_process": True,
                "boost:without_program_options": True,
                "boost:without_python": False,           # <-- that's what you are looking for
                "boost:without_random": False,
                "boost:without_regex": False,
                "boost:without_serialization": False,
                "boost:without_stacktrace": True,
                "boost:without_system": False,
                "boost:without_test": True,
                "boost:without_thread": True,
                "boost:without_timer": True,
                "boost:without_type_erasure": True,
                "boost:without_url": True,
                "boost:without_wave": True,
                # poppler
                # --
                }

   def configure(self):
      if self.options.with_analyzer == False:
         self.options['opencv'].highgui = False
         self.options['opencv'].videoio = False

   def build(self):
      cmake = CMake(self)
      cmake.configure()
      cmake.build()

   def layout(self):
      cmake_layout(self)
