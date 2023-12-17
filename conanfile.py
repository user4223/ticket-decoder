from conans import ConanFile, tools
from conan.tools.cmake import CMake, cmake_layout

class TicketDecoderConan(ConanFile):
   settings = "os", "compiler", "build_type", "arch"
   generators = "CMakeToolchain", "CMakeDeps"
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
               ("boost/1.83.0"),              # poppler requires boost but the recipe fails when boost has to be built as dependency for poppler (probably the recipe has a bug, since even when splash is disabled, the boost requirement remains unfortunately)
               # Override some requirements for poppler, since it uses older dependencies than opencv for same libs
               # ("zlib/1.3", "override"),      # Remove this direct override when poppler gets updated
               # ("libpng/1.6.40", "override"), # Remove this direct override when poppler gets updated
               # ("libjpeg/9e", "override"),    # Remove this direct override when poppler gets updated
               # https://conan.io/center/recipes/poppler
               ("poppler/21.07.0"),
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
                "opencv:with_eigen": False,
                "opencv:with_tesseract": False,
                "opencv:with_protobuf": False,
                "opencv:gapi": False,
                "opencv:ml": False,
                "opencv:dnn": False,
                "opencv:calib3d": False,
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
                "boost:header_only": True, # When we have this dependency due to poppler, then as small as possible, so header only libs
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
                
   def build(self):
      cmake = CMake(self)
      cmake.configure()
      cmake.build()

   def layout(self):
      cmake_layout(self)
