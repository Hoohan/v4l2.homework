add_executable(test-v4l2 test-v4l2.cpp)
target_link_libraries(
  test-v4l2
  PUBLIC v4l2)