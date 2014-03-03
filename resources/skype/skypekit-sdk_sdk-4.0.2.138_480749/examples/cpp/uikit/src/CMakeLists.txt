include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(model)

add_subdirectory(presenter)

if (QT4_FOUND)
  add_subdirectory(view)
endif (QT4_FOUND)
