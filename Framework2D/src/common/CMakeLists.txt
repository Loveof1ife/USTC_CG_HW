project(common)
file(GLOB source
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${INCLUDE_DIR}/common/*.h"
)
add_library(${PROJECT_NAME} ${source})
set_target_properties(${PROJECT_NAME} PROPERTIES 
  DEBUG_POSTFIX "_d"
  RUNTIME_OUTPUT_DIRECTORY "${BINARY_DIR}"
  LIBRARY_OUTPUT_DIRECTORY "${LIBRARY_DIR}"
  ARCHIVE_OUTPUT_DIRECTORY "${LIBRARY_DIR}") 

#public:
#common 自身需要这些库（编译时需要头文件);任何链接 common 的目标也会自动获得这些库，包括头文件路径和库文件链接（传递性依赖）
#在src/assignments/3_PoissonImageEditing/CMakeLists.txt中，显式链接了 common，但由于 common 用 PUBLIC 链接了 glad、glfw、imgui，所以3_PoissonImageEditing 也会自动获得 glad、glfw、imgui 的链接。
target_link_libraries(${PROJECT_NAME} PUBLIC ${THIRD_PARTY_LIBS} glfw)
target_include_directories(${PROJECT_NAME} 
  PUBLIC ${INCLUDE_DIR} 
  PRIVATE ${THIRD_PARTY_DIR}/glad/include  # 显式指定 glad 路径
  PRIVATE ${THIRD_PARTY_DIR}
)