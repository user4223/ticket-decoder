PROJECT(ticket-decoder-lib-uic918)

AUX_SOURCE_DIRECTORY("api/source" PROJECT_SOURCE)
AUX_SOURCE_DIRECTORY("detail/source" PROJECT_SOURCE)
AUX_SOURCE_DIRECTORY("gen/v1.3" PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES
   "api/include/*.h"
   "detail/include/*.h"
   "gen/v1.3/*.h")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/gen/v1.3")
target_link_libraries(${PROJECT_NAME} PRIVATE 
   botan::botan
   nlohmann_json::nlohmann_json
   easyloggingpp::easyloggingpp
   pugixml::pugixml)
