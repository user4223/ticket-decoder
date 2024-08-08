PROJECT(ticket-decoder-api)

AUX_SOURCE_DIRECTORY("source" PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_include_directories(${PROJECT_NAME} PRIVATE)
target_link_libraries(${PROJECT_NAME} PRIVATE
    opencv::opencv_core_alias
    easyloggingpp::easyloggingpp
    ticket-decoder-utility
    ticket-decoder-dip-detection
    ticket-decoder-dip-filtering
    ticket-decoder-barcode
    ticket-decoder-uic918
    ticket-decoder-io-api)
