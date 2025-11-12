project(ticket-decoder)

find_package(tclap REQUIRED)

file(GLOB PROJECT_INCLUDES "include/*.h")
file(GLOB PROJECT_ETC "etc/*")

IF (WITH_TICKET_ANALYZER)

  add_executable("ticket-analyzer" "source/analyzer.cpp" "source/InteractionController.cpp" ${PROJECT_INCLUDES} ${PROJECT_ETC})
  target_include_directories("ticket-analyzer" PRIVATE)
  target_link_libraries("ticket-analyzer" PRIVATE
    tclap::tclap
    opencv::opencv_core_alias
    easyloggingpp::easyloggingpp
    nlohmann_json::nlohmann_json
    ticket-decoder-api
    ticket-decoder-barcode
    ticket-decoder-utility
    ticket-decoder-ui)
ENDIF()

add_executable("ticket-decoder" "source/decoder.cpp" ${PROJECT_INCLUDES} ${PROJECT_ETC})
target_include_directories("ticket-decoder" PRIVATE)
target_link_libraries("ticket-decoder" PRIVATE
  tclap::tclap
  opencv::opencv_core_alias
  easyloggingpp::easyloggingpp
  ticket-decoder-api
  ticket-decoder-dip)
