add_executable(pca pca.cpp)
add_executable(pca_kernel pca_kernel.cpp)

# Link the OpenFHE and OpenMP libraries
target_link_libraries(pca ${OpenFHE_SHARED_LIBRARIES} ${OpenMP_CXX_LIBRARIES})
target_link_libraries(pca_kernel ${OpenFHE_SHARED_LIBRARIES} ${OpenMP_CXX_LIBRARIES})

