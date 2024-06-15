# Add the executable for the distance computation
add_executable(distance distance.cpp)
add_executable(distance_kernel distance_kernel.cpp)

# Link the OpenFHE and OpenMP libraries
target_link_libraries(distance ${OpenFHE_SHARED_LIBRARIES} ${OpenMP_CXX_LIBRARIES})
target_link_libraries(distance_kernel ${OpenFHE_SHARED_LIBRARIES} ${OpenMP_CXX_LIBRARIES})

