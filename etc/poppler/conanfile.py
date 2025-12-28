# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout
from conan.tools.scm import Git
from conan.tools.files import copy
import os
import shutil


class PopplerCppConan(ConanFile):
    """ This is a minimal poppler-cpp package recipe to avoid huge dependencies and complexity for a recipe
        compatible with all options, platforms, compilers and architectures.
    """

    name = "poppler-cpp"
    version = "25.10.0"
    license = "GPL-2.0-or-later", "GPL-3.0-or-later"
    homepage = "https://poppler.freedesktop.org/"
    url = "https://github.com/user4223/ticket-decoder"
    description = "Poppler is a PDF rendering library based on the xpdf-3.0 code base - but this contains CPP bindings only"
    topics = ("poppler", "pdf", "rendering")

    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False], 
        "fPIC": [True, False],
        # Following options are there only to make this executable from conan-instal.sh with same parameters, they are ignored in libpoppler
        "with_analyzer": None,
        "with_python_module": None,
        "with_square_detector": None,
        "with_classifier_detector": None,
        "with_barcode_decoder": None,
        "with_pdf_input": None,
        "with_signature_verifier": None,
        "with_sbb_interpreter": None,
    }
    default_options = {
        "shared": False,
        "fPIC": True,
    }
    exports_sources = ["CMakeLists.txt", "ConfigureChecks.cmake", "config.h.cmake", "cmake/*",
                       "poppler.pc.cmake", "poppler-cpp.pc.cmake",
                       "goo/*", "fofi/*", "splash/*", "utils/*", "cpp/*", "poppler/*"]

    def source(self):
        git = Git(self)
        git.clone(url="https://gitlab.freedesktop.org/poppler/poppler.git", target='.', args=["--depth", "1", "--branch", "poppler-25.10.0"])

    def requirements(self):
        # https://conan.io/center/recipes/fontconfig
        self.requires("fontconfig/2.15.0")
        # https://conan.io/center/recipes/libiconv
        self.requires("libiconv/1.18")
        # https://conan.io/center/recipes/libjpeg
        self.requires("libjpeg/9f")
        # https://conan.io/center/recipes/libpng
        self.requires("libpng/1.6.50")

    def build_requirements(self):
        # https://conan.io/center/recipes/pkgconf
        self.build_requires("pkgconf/2.2.0")

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        self.options.rm_safe("with_analyzer")
        self.options.rm_safe("with_python_module")
        self.options.rm_safe("with_square_detector")
        self.options.rm_safe("with_classifier_detector")
        self.options.rm_safe("with_barcode_decoder")
        self.options.rm_safe("with_pdf_input")
        self.options.rm_safe("with_signature_verifier")
        self.options.rm_safe("with_sbb_interpreter")

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
                ("ENABLE_CPP", "ON"),
                ("BUILD_GTK_TESTS", "OFF"),
                ("BUILD_QT5_TESTS", "OFF"),
                ("BUILD_QT6_TESTS", "OFF"),
                ("BUILD_CPP_TESTS", "OFF"),
                ("BUILD_MANUAL_TESTS", "OFF"),
                ("ENABLE_BOOST", "OFF"),
                ("ENABLE_UTILS", "OFF"),
                ("ENABLE_GLIB", "OFF"),
                ("ENABLE_GOBJECT_INTROSPECTION", "OFF"),
                ("ENABLE_GTK_DOC", "OFF"),
                ("ENABLE_QT5", "OFF"),
                ("ENABLE_QT6", "OFF"),
                ("ENABLE_LCMS", "OFF"),
                ("ENABLE_LIBCURL", "OFF"),
                ("ENABLE_LIBTIFF", "OFF"),
                ("ENABLE_LIBOPENJPEG", "none"),
                ("ENABLE_NSS3", "OFF"),
                ("ENABLE_GPGME", "OFF"),
                ("ENABLE_PGP_SIGNATURES", "OFF"),
                ("ENABLE_ZLIB_UNCOMPRESS", "OFF"),
                ("USE_FLOAT", "OFF"),
                ("BUILD_SHARED_LIBS", "OFF"),
                ("RUN_GPERF_IF_PRESENT", "OFF"),
                ("INSTALL_GLIB_DEMO", "OFF")])
        cmake.build()

    def package(self):
        copy(self, "COPYING*", dst="licenses", src=self.source_folder)
        cmake = CMake(self)
        cmake.install()
        shutil.rmtree(os.path.join(self.package_folder, "lib", "pkgconfig"), ignore_errors=True)

    def package_info(self):
        self.cpp_info.libs = ["poppler-cpp", "poppler"]
        self.cpp_info.includedirs.append(os.path.join("include", "poppler", "cpp"))
        self.cpp_info.set_property("cmake_file_name", "Poppler")
        self.cpp_info.set_property("cmake_target_name", "Poppler::Poppler")
        self.cpp_info.set_property("pkg_config_name", "poppler-cpp")

        self.cpp_info.requires.append("fontconfig::fontconfig")
        self.cpp_info.requires.append("libjpeg::libjpeg")
        self.cpp_info.requires.append("libpng::libpng")
        self.cpp_info.requires.append("libiconv::libiconv")
