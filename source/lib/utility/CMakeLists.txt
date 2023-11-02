PROJECT(ticket-decoder-lib-utility)

AUX_SOURCE_DIRECTORY("source" PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${botan_INCLUDE_DIRS}
    ${nlohmann_json_INCLUDE_DIRS}
    ${easyloggingpp_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${OpenCV_LIBS}
    ${botan_LIBRARIES}
    ${easyloggingpp_LIBRARIES})
