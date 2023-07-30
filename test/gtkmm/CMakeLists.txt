link_directories( ${GTKMM_LIBRARY_DIRS} )

include_directories( ${GTKMM_INCLUDE_DIRS} )

add_executable(test-gtkmm-imageShow imageShow.cpp)
target_link_libraries(
  test-gtkmm-imageShow
  ${GTKMM_LIBRARIES}
)

add_executable(test-gtkmm-helloworld helloworld.cpp)
target_link_libraries(
  test-gtkmm-helloworld
  ${GTKMM_LIBRARIES}
)