project(aztec-detector-test)

aux_source_directory(source PROJECT_SOURCE)
aux_source_directory("dip/source" PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

find_package(GTest)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_link_libraries(${PROJECT_NAME} aztec-detector-lib ${CONAN_LIBS})

add_custom_target(${PROJECT_NAME}.Copy ALL
   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/etc $<TARGET_FILE_DIR:${PROJECT_NAME}>/../etc
   DEPENDS ${PROJECT_NAME}
   COMMENT "Copying resource files")
