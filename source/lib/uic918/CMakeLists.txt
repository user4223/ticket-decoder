PROJECT(ticket-decoder-uic918)

AUX_SOURCE_DIRECTORY("api/source" PROJECT_SOURCE)
AUX_SOURCE_DIRECTORY("detail/source" PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES
   "api/include/*.h"
   "detail/include/*.h")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_include_directories(${PROJECT_NAME} PRIVATE)
target_link_libraries(${PROJECT_NAME} PRIVATE
   botan::botan
   nlohmann_json::nlohmann_json
   easyloggingpp::easyloggingpp
   pugixml::pugixml
   ticket-decoder-uic918-u_flex13
   ticket-decoder-uic918-u_flex30)
