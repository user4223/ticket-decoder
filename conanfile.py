from conans import ConanFile, tools
from conan.tools.cmake import CMake, cmake_layout

class TicketDecoderConan(ConanFile):
   settings = "os", "compiler", "build_type", "arch"
   generators = "CMakeToolchain", "CMakeDeps"
   requires = [
               # https://conan.io/center/recipes/opencv
               ("opencv/4.5.5"),
               # https://conan.io/center/recipes/zxing-cpp
               ("zxing-cpp/2.1.0"),
               # https://conan.io/center/recipes/nlohmann_json
               ("nlohmann_json/3.11.2"),
               # https://conan.io/center/recipes/easyloggingpp
               ("easyloggingpp/9.97.1"),
               # https://conan.io/center/recipes/pugixml
               ("pugixml/1.14"),
               # https://conan.io/center/recipes/botan
               ("botan/2.19.3"),
               # https://conan.io/center/recipes/tclap
               ("tclap/1.2.5"),
               # https://conan.io/center/recipes/gtest
               ("gtest/1.14.0"),
               ]
   default_options = {
                "*:shared": False,
                # opencv
                "opencv:parallel": False,
                "opencv:stitching": False,
                "opencv:video": False, # Disables video processing only, required video-io keeps enabled 
                "opencv:with_ffmpeg": False,
                "opencv:with_jpeg2000": False,
                "opencv:with_tiff": False,
                "opencv:with_webp": False,
                "opencv:with_quirc": False,
                "opencv:with_openexr": False,
                "opencv:gapi": False,
                "opencv:with_eigen": False,
                "opencv:with_tesseract": False,
                # zxing-cpp
                "zxing-cpp:enable_encoders": False,
                # easyloggingpp
                "easyloggingpp:enable_default_logfile": False,
                }
                
   def build(self):
      cmake = CMake(self)
      cmake.configure()
      cmake.build()

   def layout(self):
      cmake_layout(self)
