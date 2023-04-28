from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps

class TicketDecoder(ConanFile):
    name = "ticket-decoder"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    default_options = {
        "*/*:shared": False,
        "opencv/*:parallel": False,
        "opencv/*:with_ffmpeg": False,
        "opencv/*:with_jpeg2000": False,
        "opencv/*:with_tiff": False,
        "opencv/*:with_webp": False,
        "opencv/*:with_quirc": False,
        "opencv/*:with_openexr": False,
        "opencv/*:with_ade": False,
        "opencv/*:with_eigen": False,
        "zxing-cpp/*:enable_encoders": False,
        "easyloggingpp/*:enable_default_logfile": False
    }

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def requirements(self):
        self.requires("gtest/1.13.0")
        self.requires("opencv/4.5.5")
        self.requires("zxing-cpp/1.4.0")
        self.requires("nlohmann_json/3.11.2")
        #self.requires("easyloggingpp/9.97.0")
        self.requires("pugixml/1.13")
        #self.requires("botan/2.19.3")
        #self.requires("tclap/1.2.5")
