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
    ticket-decoder-dip
    ticket-decoder-decoder-api
    ticket-decoder-detector-api
    ticket-decoder-interpreter-api
    ticket-decoder-io-api)
