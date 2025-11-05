PROJECT(ticket-decoder-api)

AUX_SOURCE_DIRECTORY("source" PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_include_directories(${PROJECT_NAME} PRIVATE)
target_link_libraries(${PROJECT_NAME} PRIVATE
    opencv::opencv_core_alias
    easyloggingpp::easyloggingpp
    nlohmann_json::nlohmann_json
    ticket-decoder-utility
    ticket-decoder-infrastructure
    ticket-decoder-dip-detection
    ticket-decoder-dip-filtering
    ticket-decoder-barcode
    ticket-decoder-interpreter-api
    ticket-decoder-interpreter-detail-common
    ticket-decoder-interpreter-detail-uic918
    ticket-decoder-interpreter-detail-sbb
    ticket-decoder-interpreter-detail-vdv
    ticket-decoder-io-api)
