add_subdirectory(gtkmm)

add_executable(test-v4l2 test-v4l2.cpp)
add_executable(test-Camera 
  test-Camera.cpp
  ${PROJECT_SOURCE_DIR}/src/Camera.cpp
)