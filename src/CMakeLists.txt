link_directories( ${GTKMM_LIBRARY_DIRS} )

include_directories( ${GTKMM_INCLUDE_DIRS} )

aux_source_directory(
    . SRCS_LIST
)

add_executable(grabber ${SRCS_LIST})
target_link_libraries(
    grabber
    ${GTKMM_LIBRARIES}
)