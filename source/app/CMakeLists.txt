project(ticket-decoder)

file(GLOB PROJECT_INCLUDES "include/*.h")
file(GLOB PROJECT_ETC "etc/*")

add_executable("analyzer" "source/analyzer.cpp" ${PROJECT_INCLUDES} ${PROJECT_ETC})
target_link_libraries("analyzer"
  ${CONAN_LIBS}
  ticket-decoder-lib-barcode
  ticket-decoder-lib-dip
  ticket-decoder-lib-uic918
  ticket-decoder-lib-utility)

add_executable("ticket-decoder" "source/decoder.cpp" ${PROJECT_INCLUDES} ${PROJECT_ETC})
target_link_libraries("ticket-decoder"
  ${CONAN_LIBS}
  ticket-decoder-lib-barcode
  ticket-decoder-lib-dip
  ticket-decoder-lib-uic918
  ticket-decoder-lib-utility)
