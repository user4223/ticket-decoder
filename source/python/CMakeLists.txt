PROJECT(ticket_decoder)

find_package(Python3 COMPONENTS Interpreter Development.Module)
find_package(Boost REQUIRED COMPONENTS python)

message("Python3_VERSION: " ${Python3_VERSION})

AUX_SOURCE_DIRECTORY("source" PROJECT_SOURCE)

# Important to have python package name as output folder inside bin to place __init__.py beside
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME})

ADD_LIBRARY(${PROJECT_NAME} SHARED ${PROJECT_SOURCE})

# Important to have no "lib" prefix!
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES PREFIX "")
# Important to have no ".dylib" postfix on macos!
# TODO ".pyd" should be platform independent?
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES SUFFIX ".so")
target_link_options(${PROJECT_NAME} PUBLIC ${Python3_LINK_OPTIONS})
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS} ${Python3_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE
    Boost::python
    Python3::Module
    easyloggingpp::easyloggingpp
    opencv::opencv_core_alias
    ticket-decoder-api)

file(INSTALL ${PROJECT_SOURCE_DIR}/etc/__init__.py DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/)
#
# fails in pipeline since there is no permission to install
# so we do need an explicit install command
#
# file(INSTALL ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} DESTINATION ${Python3_SITEARCH}/)
