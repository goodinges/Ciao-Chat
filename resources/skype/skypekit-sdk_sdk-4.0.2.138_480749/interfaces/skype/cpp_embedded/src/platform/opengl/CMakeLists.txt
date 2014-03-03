add_definitions(-DGL_WINDOW)

include_directories(${SID_IPC_DIR}
					${SID_IPC_DIR}/VideoBuffers
                   )

# source files for reference_opengl_lib
aux_source_directory(${SID_DIR}/reference/openglvideorenderer OPENGL_LIB_SRCS)

add_library(reference_opengl_lib ${OPENGL_LIB_SRCS})

install(TARGETS reference_opengl_lib DESTINATION ${CPPWRAPPER_INSTALL_DIR})
