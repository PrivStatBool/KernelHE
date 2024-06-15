add_executable(variance variance.cpp)
add_executable(variance_kernel variance_kernel.cpp)

# Link the OpenFHE and OpenMP libraries
target_link_libraries(variance ${OpenFHE_SHARED_LIBRARIES} ${OpenMP_CXX_LIBRARIES})
target_link_libraries(variance_kernel ${OpenFHE_SHARED_LIBRARIES} ${OpenMP_CXX_LIBRARIES})

