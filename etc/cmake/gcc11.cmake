# Run cmake somehow like this:
# cmake -DCMAKE_TOOLCHAIN_FILE=.../gcc11.cmake  ...

SET (CMAKE_C_COMPILER   "/usr/bin/gcc-11")
SET (CMAKE_CXX_COMPILER "/usr/bin/g++-11")
SET (CMAKE_AR           "/usr/bin/gcc-ar-11")
SET (CMAKE_LINKER       "/usr/bin/ld")
SET (CMAKE_NM           "/usr/bin/gcc-nm-11")
SET (CMAKE_OBJDUMP      "/usr/bin/objdump")
SET (CMAKE_RANLIB       "/usr/bin/gcc-ranlib-11")
