from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout


class ZxingCppConan(ConanFile):
    name = "zxing-cpp"
    version = "1.2.0"
    license = "Apache License 2.0"
    url = "https://github.com/nu-book/zxing-cpp"
    description = "ZxingCpp"
    topics = ("barcode")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = ["CMakeLists.txt",
                       "zxing.pc.in", "core/*", "test/unit/*"]

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure(
            variables=[
                ("BUILD_SHARED_LIBS", "false"),
                ("BUILD_EXAMPLES", "false"),
                ("BUILD_PYTHON_MODULE", "false"),
                ("BUILD_BLACKBOX_TESTS", "false")])
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.names["cmake_find_package"] = "ZXing"
        self.cpp_info.names["cmake_find_package_multi"] = "ZXing"
        self.cpp_info.names["pkg_config"] = "zxing"
        self.cpp_info.libs = ["ZXing"]
