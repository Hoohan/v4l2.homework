link_directories( ${GTKMM_LIBRARY_DIRS} )

include_directories( ${GTKMM_INCLUDE_DIRS} )

add_executable(test-gtkmm-imageShow ImageShow.cpp)
target_link_libraries(
  test-gtkmm-imageShow
  ${GTKMM_LIBRARIES}
)

add_executable(test-gtkmm-helloworld HelloWorld.cpp)
target_link_libraries(
  test-gtkmm-helloworld
  ${GTKMM_LIBRARIES}
)

add_executable(test-gtkmm-combo-box ComboBox.cpp)
target_link_libraries(
  test-gtkmm-combo-box
  ${GTKMM_LIBRARIES}
)