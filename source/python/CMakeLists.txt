PROJECT(ticket_decoder)

find_package(Python3 COMPONENTS Development.Module)
find_package(Boost REQUIRED COMPONENTS python)

AUX_SOURCE_DIRECTORY("source" PROJECT_SOURCE)

ADD_LIBRARY(${PROJECT_NAME} SHARED ${PROJECT_SOURCE})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES PREFIX "")
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS} ${Python3_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE
    opencv::opencv_core_alias
    nlohmann_json::nlohmann_json
    easyloggingpp::easyloggingpp
    Boost::python
    Python3::Module)
