add_executable(4_list_Polynomial main.cpp)

target_include_directories(4_list_Polynomial PUBLIC ${PROJECT_SOURCE_DIR}/include)
# 打印项目源目录
#message(STATUS "Debug || Project source dir: ${PROJECT_SOURCE_DIR}")
target_link_libraries(4_list_Polynomial PolynomialListLib)

set_target_properties(4_list_Polynomial PROPERTIES ${OUTPUT_PROP})