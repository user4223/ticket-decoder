project(ticket-decoder)

file(GLOB PROJECT_INCLUDES "include/*.h")
file(GLOB PROJECT_ETC "etc/*")

IF (WITH_TICKET_ANALYZER)

  add_executable("ticket-analyzer" "source/analyzer.cpp" ${PROJECT_INCLUDES} ${PROJECT_ETC})
  target_include_directories("ticket-analyzer" PRIVATE)
  target_link_libraries("ticket-analyzer"
    tclap::tclap
    opencv::opencv_core_alias
    easyloggingpp::easyloggingpp
    nlohmann_json::nlohmann_json
    ticket-decoder-api
    ticket-decoder-barcode
    ticket-decoder-dip-detection
    ticket-decoder-dip-filtering
    ticket-decoder-dip-utility
    ticket-decoder-uic918
    ticket-decoder-utility
    ticket-decoder-ui)
ENDIF()

add_executable("ticket-decoder" "source/decoder.cpp" ${PROJECT_INCLUDES} ${PROJECT_ETC})
target_include_directories("ticket-decoder" PRIVATE)
target_link_libraries("ticket-decoder"
  tclap::tclap
  opencv::opencv_core_alias
  easyloggingpp::easyloggingpp
  ticket-decoder-api)

add_custom_target(${PROJECT_NAME}.Copy ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory $<TARGET_FILE_DIR:ticket-decoder-dip-detection>/etc/dip $<TARGET_FILE_DIR:${PROJECT_NAME}>/etc/dip
  DEPENDS ${PROJECT_NAME}
  COMMENT "Copying configuration files")
