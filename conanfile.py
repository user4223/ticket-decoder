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
               # OVERWRITES quite old dependency from libpoppler to use the same and newer dependency from libopencv
               # ("zlib/1.3", "override"),
               # https://conan.io/center/recipes/poppler
               # ("poppler/21.07.0"),
               # https://conan.io/center/recipes/gtest
               ("gtest/1.14.0"),
               ]
   default_options = {
                "*:shared": False,
                # opencv
                "opencv:parallel": False,
                "opencv:calib3d": False,
                "opencv:stitching": False,
                "opencv:video": False, # Disables video processing only video-io keeps enabled 
                "opencv:with_ffmpeg": False,
                "opencv:with_jpeg2000": False,
                "opencv:with_tiff": False,
                "opencv:with_webp": False,
                "opencv:with_quirc": False,
                "opencv:with_openexr": False,
                "opencv:with_ade": False,
                "opencv:with_eigen": False,
                "opencv:with_tesseract": False,
                # zxing-cpp
                "zxing-cpp:enable_encoders": False,
                # easyloggingpp
                "easyloggingpp:enable_default_logfile": False,
                # poppler
                "poppler:splash": False,
                "poppler:with_cairo": False,
                "poppler:with_glib": False,
                "poppler:with_gobject_introspection": False,
                "poppler:with_gtk": False,
                "poppler:with_lcms": False,
                "poppler:with_libcurl": False,
                "poppler:with_libiconv": False,
                "poppler:with_libjpeg": False,
                "poppler:with_nss": False,
                "poppler:with_openjpeg": False,
                "poppler:with_png": False,
                "poppler:with_qt": False,
                "poppler:with_tiff": False,
                "poppler:with_zlib": False,
                }
                
   def build(self):
      cmake = CMake(self)
      cmake.configure()
      cmake.build()

   def layout(self):
      cmake_layout(self)
