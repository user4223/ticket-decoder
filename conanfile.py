from conans import ConanFile, CMake

class TicketDecoderConan(ConanFile):
   settings = "os", "compiler", "build_type", "arch"
   generators = "cmake"
   requires = [
                ("gtest/1.14.0"),
                ("opencv/4.5.5"),
                ("zxing-cpp/2.1.0"),
                ("nlohmann_json/3.11.2"),
                ("easyloggingpp/9.97.0"),
                ("pugixml/1.13"),
                ("botan/2.19.3"),
                ("tclap/1.2.5"),
                # some overrides to get quite old libpoppler running without conflicts
                #("libpng/1.6.40", "override"),
                #("zlib/1.3", "override"),
                #("libjpeg/9e", "override"),
                #("poppler/21.07.0")
                ]
   default_options = {
                "*:shared": False,
                "opencv:parallel": False,
                "opencv:with_ffmpeg": False,
                "opencv:with_jpeg2000": False,
                "opencv:with_tiff": False,
                "opencv:with_webp": False,
                "opencv:with_quirc": False,
                "opencv:with_openexr": False,
                "opencv:with_ade": False,
                "opencv:with_eigen": False,
                "zxing-cpp:enable_encoders": False,
                "easyloggingpp:enable_default_logfile": False
                }

   #def imports(self):
   #   self.copy("*.dll", dst="bin", src="bin") # From bin to bin
   #   self.copy("*.dylib*", dst="bin", src="lib") # From lib to bin

   #def build(self):
   #   cmake = CMake(self)
   #   cmake.configure()
   #   cmake.build()
