# Add the executable for the distance computation
add_executable(similarity similarity.cpp)
add_executable(similarity_kernel similarity_kernel.cpp)

# Link the OpenFHE and OpenMP libraries
target_link_libraries(similarity ${OpenFHE_SHARED_LIBRARIES} ${OpenMP_CXX_LIBRARIES})
target_link_libraries(similarity_kernel ${OpenFHE_SHARED_LIBRARIES} ${OpenMP_CXX_LIBRARIES})

