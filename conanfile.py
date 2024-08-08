from conans import ConanFile, tools
from conan.tools.cmake import CMake, cmake_layout

class TicketDecoderConan(ConanFile):
   name = 'ticket-decoder'
   version = '0.7'
   settings = "os", "compiler", "build_type", "arch"
   generators = "CMakeToolchain", "CMakeDeps"
   options = {
               "with_analyzer": [True, False]
             }
   requires = [
               # https://conan.io/center/recipes/opencv
               ("opencv/4.8.1"),
               # https://conan.io/center/recipes/zxing-cpp
               ("zxing-cpp/2.2.1"),
               # https://conan.io/center/recipes/nlohmann_json
               ("nlohmann_json/3.11.3"),
               # https://conan.io/center/recipes/easyloggingpp
               ("easyloggingpp/9.97.1"),
               # https://conan.io/center/recipes/pugixml
               ("pugixml/1.14"),
               # https://conan.io/center/recipes/botan
               ("botan/2.19.3"),
               # https://conan.io/center/recipes/tclap
               ("tclap/1.2.5"),
               # https://conan.io/center/recipes/boost
               ("boost/1.85.0"),
               # Override some requirements for poppler, since it uses older dependencies than opencv for same libs
               # ("zlib/1.3", "override"),        # Remove this direct override when poppler gets updated
               # ("libpng/1.6.42", "override"),   # Remove this direct override when freetype/2.12.1 vs. opencv/4.8.1 gets updated
               # ("expat/2.6.0", "override"),     # Remove this direct override when fontconfig/2.13.93 vs. wayland/1.22.0 gets updated
               ("fontconfig/2.15.0", "override"),
               # https://conan.io/center/recipes/poppler
               ("poppler/21.07.0"),
               # https://conan.io/center/recipes/gtest
               ("gtest/1.14.0"),
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
                # "botan:with_bzip2": True,
                # "botan:with_zlib": True,
                # boost
                "boost:pch": False,
                "boost:header_only": False,
                "boost:without_atomic": True,
                "boost:without_chrono": True,
                "boost:without_container": True,
                "boost:without_context": True,
                "boost:without_contract": True,
                "boost:without_coroutine": True,
                "boost:without_date_time": True,
                "boost:without_exception": True,
                "boost:without_fiber": True,
                "boost:without_filesystem": True,
                "boost:without_graph": True,
                "boost:without_graph_parallel": True,
                "boost:without_iostreams": True,
                "boost:without_json": True,
                "boost:without_locale": True,
                "boost:without_log": True,
                "boost:without_math": True,
                "boost:without_mpi": True,
                "boost:without_nowide": True,
                "boost:without_program_options": True,
                "boost:without_python": False,          # <-- that's what you are looking for
                "boost:without_random": True,
                "boost:without_regex": True,
                "boost:without_serialization": True,
                "boost:without_stacktrace": True,
                "boost:without_system": True,
                "boost:without_test": True,
                "boost:without_thread": True,
                "boost:without_timer": True,
                "boost:without_type_erasure": True,
                "boost:without_url": True,
                "boost:without_wave": True,
                # poppler
                "poppler:fontconfiguration": "fontconfig",
                "poppler:with_lcms": False,
                "poppler:with_libcurl": False,
                "poppler:with_nss": False,
                "poppler:with_libjpeg": False,
                "poppler:with_openjpeg": False,
                "poppler:with_png": False,
                "poppler:with_qt": False,
                "poppler:with_tiff": False,
                "poppler:with_zlib": False
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
