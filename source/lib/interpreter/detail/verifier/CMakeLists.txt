PROJECT(ticket-decoder-interpreter-detail-verifier)

find_package(pugixml REQUIRED)
find_package(botan REQUIRED)

AUX_SOURCE_DIRECTORY("source" PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_include_directories(${PROJECT_NAME} PRIVATE)
target_link_libraries(${PROJECT_NAME} PRIVATE
   easyloggingpp::easyloggingpp
   nlohmann_json::nlohmann_json
   botan::botan
   pugixml::pugixml)
