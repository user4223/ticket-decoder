PROJECT(aztec-detector-lib)

AUX_SOURCE_DIRECTORY(source PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_include_directories(${PROJECT_NAME} PRIVATE "${CONAN_INCLUDE_DIRS_ZXING-CPP}/ZXing")
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

add_custom_target(${PROJECT_NAME}.Copy ALL
   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/etc $<TARGET_FILE_DIR:${PROJECT_NAME}>/../etc
   DEPENDS ${PROJECT_NAME}
   COMMENT "Copying configuration files")

