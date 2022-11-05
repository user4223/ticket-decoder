# Run cmake somehow like this:
# cmake -DCMAKE_TOOLCHAIN_FILE=.../clang12.cmake  ...

SET (CMAKE_C_COMPILER   "/usr/bin/clang-12")
SET (CMAKE_CXX_COMPILER "/usr/bin/clang++-12")
SET (CMAKE_AR           "/usr/bin/llvm-ar-12")
SET (CMAKE_LINKER       "/usr/bin/llvm-link-12")
SET (CMAKE_NM           "/usr/bin/llvm-nm-12")
SET (CMAKE_OBJDUMP      "/usr/bin/llvm-objdump-12")
SET (CMAKE_RANLIB       "/usr/bin/llvm-ranlib-12")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
