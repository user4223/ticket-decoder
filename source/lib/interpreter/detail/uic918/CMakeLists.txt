PROJECT(ticket-decoder-interpreter-detail-uic918)

add_subdirectory(u_flex/v1.3)
add_subdirectory(u_flex/v3.0)

AUX_SOURCE_DIRECTORY("source" PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_include_directories(${PROJECT_NAME} PRIVATE)
target_link_libraries(${PROJECT_NAME} PRIVATE
   nlohmann_json::nlohmann_json
   easyloggingpp::easyloggingpp
   ticket-decoder-interpreter-detail-common
   ticket-decoder-interpreter-detail-uic918-u_flex13
   ticket-decoder-interpreter-detail-uic918-u_flex30)
