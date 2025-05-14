# glfw
add_subdirectory(glfw)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set_target_properties(glfw update_mappings PROPERTIES FOLDER "third_party/GLFW3")

# glad

set(glad_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glad" CACHE STRING "")
#使用 file(GLOB ...) 命令收集 Glad 源代码目录中所有 .c 文件，并将它们存储到变量 source 中。
file(GLOB source
  "${glad_SOURCE_DIR}/src/*.c"
)
#创建一个名为 glad 的静态库或动态库，其源文件由变量 source 指定,	默认创建静态库（如需动态库需显式指定 SHARED）
add_library(glad ${source})
#将 Glad 的头文件目录添加到 glad 库的公共包含目录中。这意味着任何链接了 glad 库的目标都可以访问这些头文件
target_include_directories(glad
  PUBLIC "${glad_SOURCE_DIR}/include"
)
set_target_properties(glad PROPERTIES FOLDER "third_party")

# imgui
set(imgui_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui" CACHE STRING "")
set(imgui_file_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ImGuiFileDialog" CACHE STRING "")
file(GLOB source
    "${imgui_SOURCE_DIR}/*.h"
    "${imgui_SOURCE_DIR}/*.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.*"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.*"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3_loader.*"
    "${imgui_file_SOURCE_DIR}/*.h"
    "${imgui_file_SOURCE_DIR}/*.cpp"
)
add_library(imgui ${source})
target_include_directories(imgui
    PUBLIC "${imgui_SOURCE_DIR}"
    PUBLIC "${imgui_SOURCE_DIR}/backends"
    PUBLIC "${imgui_file_SOURCE_DIR}"
)
#将 glfw 库链接到 imgui 库
target_link_libraries(imgui PUBLIC glfw)
set_target_properties(imgui PROPERTIES FOLDER "third_party")

