# Add the executable for the distance computation
add_executable(norm norm.cpp)
add_executable(norm_kernel norm_kernel.cpp)

# Link the OpenFHE and OpenMP libraries
target_link_libraries(norm ${OpenFHE_SHARED_LIBRARIES} ${OpenMP_CXX_LIBRARIES})
target_link_libraries(norm_kernel ${OpenFHE_SHARED_LIBRARIES} ${OpenMP_CXX_LIBRARIES})

