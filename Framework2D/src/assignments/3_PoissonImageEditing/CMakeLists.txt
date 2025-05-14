project(3_PoissonImageEditing)
# 查找 OpenMP 包
find_package(OpenMP REQUIRED)

file(GLOB source
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.h" 
  "${CMAKE_CURRENT_SOURCE_DIR}/shapes/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/shapes/*.h" 
)
add_executable(${PROJECT_NAME} ${source})
target_include_directories(${PROJECT_NAME} PUBLIC
 ${CMAKE_CURRENT_SOURCE_DIR} 
 ${THIRD_PARTY_DIR}/Eigen
 )
# 添加 OpenMP 支持
if(OpenMP_CXX_FOUND)
    message(STATUS "Found OpenMP ${OpenMP_VERSION}")
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
    target_compile_options(${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/openmp>)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES 
  DEBUG_POSTFIX "_d"
  RUNTIME_OUTPUT_DIRECTORY "${BINARY_DIR}"
  LIBRARY_OUTPUT_DIRECTORY "${LIBRARY_DIR}"
  ARCHIVE_OUTPUT_DIRECTORY "${LIBRARY_DIR}") 
  
target_link_libraries(${PROJECT_NAME} PUBLIC common) 
target_compile_definitions(${PROJECT_NAME} PRIVATE -DDATA_PATH="${FRAMEWORK2D_DIR}/../Homeworks/3_poisson_image_editing/data")