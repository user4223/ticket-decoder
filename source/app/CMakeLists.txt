project(ticket-decoder)

file(GLOB PROJECT_INCLUDES "include/*.h")
file(GLOB PROJECT_ETC "etc/*")

add_executable("analyzer" "source/analyzer.cpp" ${PROJECT_INCLUDES} ${PROJECT_ETC})
target_link_libraries("analyzer" ticket-decoder-lib ${CONAN_LIBS})

add_executable("decoder" "source/decoder.cpp" ${PROJECT_INCLUDES} ${PROJECT_ETC})
target_link_libraries("decoder" ticket-decoder-lib ${CONAN_LIBS})
