link_directories( ${GTKMM_LIBRARY_DIRS} )

include_directories( ${GTKMM_INCLUDE_DIRS} )

add_executable(test-gtkmm base.cpp)
target_link_libraries(
  test-gtkmm
  ${GTKMM_LIBRARIES}
)