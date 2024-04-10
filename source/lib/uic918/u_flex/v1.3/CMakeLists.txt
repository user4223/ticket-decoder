PROJECT(ticket-decoder-lib-uic918-u_flex13)

AUX_SOURCE_DIRECTORY("source" PROJECT_SOURCE)
AUX_SOURCE_DIRECTORY("gen" PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES
   "include/*.h"
   "gen/*.h")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/gen")
target_link_libraries(${PROJECT_NAME} PRIVATE 
   nlohmann_json::nlohmann_json
   easyloggingpp::easyloggingpp)
