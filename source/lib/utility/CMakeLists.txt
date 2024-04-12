PROJECT(ticket-decoder-lib-utility)

AUX_SOURCE_DIRECTORY("source" PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(${PROJECT_NAME} PRIVATE)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    opencv::opencv_core_alias
    botan::botan
    nlohmann_json::nlohmann_json
    easyloggingpp::easyloggingpp)
