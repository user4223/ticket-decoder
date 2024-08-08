PROJECT(ticket-decoder-barcode)

AUX_SOURCE_DIRECTORY("api/source" PROJECT_SOURCE)
AUX_SOURCE_DIRECTORY("detail/source" PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES 
    "api/include/*.h" 
    "detail/include/*.h")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_include_directories(${PROJECT_NAME} PRIVATE)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    opencv::opencv_core_alias
    ZXing::ZXing
    easyloggingpp::easyloggingpp)
