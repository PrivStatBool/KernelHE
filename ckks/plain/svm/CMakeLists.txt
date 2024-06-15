add_executable(svm svm.cpp)
add_executable(svm_kernel svm_kernel.cpp)

# Link the OpenFHE and OpenMP libraries
target_link_libraries(svm ${OpenFHE_SHARED_LIBRARIES} ${OpenMP_CXX_LIBRARIES})
target_link_libraries(svm_kernel ${OpenFHE_SHARED_LIBRARIES} ${OpenMP_CXX_LIBRARIES})

