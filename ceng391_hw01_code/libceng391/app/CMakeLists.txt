add_executable(image-test image_test.cpp)


set(app_targets image-test)

foreach(target ${app_targets})
  target_link_libraries(${target} PRIVATE ceng391)
  target_compile_features(${target} PRIVATE cxx_std_17)
  target_compile_options(${target} PRIVATE -Wall -Wextra)
  set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)
endforeach()


