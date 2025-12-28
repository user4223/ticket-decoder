PROJECT(ticket-decoder-interpreter-api)

AUX_SOURCE_DIRECTORY("source" PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_include_directories(${PROJECT_NAME} PRIVATE)
target_link_libraries(${PROJECT_NAME} PRIVATE
   easyloggingpp::easyloggingpp
   nlohmann_json::nlohmann_json
   ticket-decoder-interpreter-detail-common
   ticket-decoder-interpreter-detail-uic918
   ticket-decoder-interpreter-detail-vdv)

if (WITH_SBB_INTERPRETER)
   target_link_libraries(${PROJECT_NAME} PRIVATE ticket-decoder-interpreter-detail-sbb)
ENDIF()

if (WITH_SIGNATURE_VERIFIER)
   target_link_libraries(${PROJECT_NAME} PRIVATE ticket-decoder-interpreter-detail-verifier)
ENDIF()
