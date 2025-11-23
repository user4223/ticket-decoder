PROJECT(ticket-decoder-decoder-detail-zxing)

find_package(ZXing REQUIRED)

AUX_SOURCE_DIRECTORY("source" PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_include_directories(${PROJECT_NAME} PRIVATE)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    opencv::opencv_core_alias
    ZXing::ZXing
    easyloggingpp::easyloggingpp
    ticket-decoder-decoder-api)
