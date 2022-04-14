set(ceng391_sources
  image.cpp)

set(ceng391_headers
  ../include/ceng391/image.hpp)

add_library(ceng391 STATIC ${ceng391_sources} ${ceng391_headers})
target_include_directories(ceng391 PUBLIC ../include)
target_compile_features(ceng391 PRIVATE cxx_std_17)
target_compile_options(ceng391 PRIVATE -Wall -Wextra)
set_target_properties(ceng391 PROPERTIES CXX_EXTENSIONS OFF)
