project(qtsimpleclient)

include_directories(${SKT_CPP_DIR}/src/api
                    ${SKT_CPP_DIR}/src/ipc
                    ${SKT_CPP_DIR}/src/types
                    ${SKT_CPP_DIR}/src/platform/threading
                    ${SKT_DIR}/ipc/cpp
                    ${SKT_DIR}/ipc/cpp/platform/se
                    ${SKT_DIR}/ipc/cpp/ssl/cyassl/ctaocrypt/include
                    ${SKT_DIR}/ipc/cpp/ssl/cyassl/include
                   )

IF(WIN32)
    include_directories(${SKT_CPP_DIR}/src/platform/threading/win)
ELSE(WIN32)
    include_directories(${SKT_CPP_DIR}/src/platform/threading/pthread)
ENDIF(WIN32)

QT4_WRAP_CPP(MOC_SRCS qtsimpleclientapp.h)

ADD_EXECUTABLE(qtsimpleclient MACOSX_BUNDLE main.cpp qtsimpleclientapp.cpp ${MOC_SRCS})
	
TARGET_LINK_LIBRARIES(qtsimpleclient
	PocoFoundation
	PocoXML
	PocoData
	Mvp-presenter
	Mvp-model
	Mvp-view
	${skt_libs}
	${QT_LIBRARIES})

#--------------------------------------------------------------------------------
# Now the installation stuff below
#--------------------------------------------------------------------------------

SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "0")

# identify the skypekit runtime in the SkypeKit distribution
SET(DISTRO_DIR ${SKT_DIR})
IF(APPLE)
	FIND_FILE(skyperuntime mac-x86-skypekit PATHS ${SKT_DIR}/bin/mac-x86)
ELSEIF(UNIX)
	FIND_FILE(skyperuntime linux-x86-skypekit PATHS ${DISTRO_DIR}/bin/linux-x86)
ELSEIF(WIN32)
	FIND_FILE(skyperuntime windows-x86-skypekit.exe PATHS ${DISTRO_DIR}/bin/windows-x86)
ENDIF(APPLE)

IF(NOT skyperuntime)
	MESSAGE(FATAL_ERROR "Need skype runtime!")
ENDIF()

# set package format specific CPack variables
SET(CPACK_PACKAGE_NAME "SkypeUikit")
SET(CPACK_PACKAGE_DESCRIPTION "Skypekit UiKit Reference client")
SET(CPACK_PACKAGE_VERSION "0.0.1.pre-alpha")
SET(CPACK_PACKAGE_VENDOR "Skype Technologies S.A.")
SET(CPACK_PACKAGE_CONTACT "info@skype.net")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Skype UiKit reference client for SkypeKit")
SET(CPACK_MONOLITHIC_INSTALL True)

SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/Copyright.txt")

IF(APPLE)
	# OS X Packaging parameters
	SET(uikit_icon ${CMAKE_SOURCE_DIR}/qtsimpleclient/skypekit.icns)
	# find_file(uikit_icon skypekit.icns)
	SET(CPACK_GENERATOR "DragNDrop")
	SET(CPACK_PACKAGE_FILE_NAME "SkypeKit-UiKit")
	SET(CPACK_PACKAGE_ICON ${uikit_icon})
	SET(CPACK_BUNDLE_NAME "SkypeKit-UiKit Bundle")
	# SET(CPACK_BUNDLE_PLIST Info.plist)
	SET(CPACK_BUNDLE_ICON ${uikit_icon})

ELSEIF(UNIX)
	# Debian packaging parameters
    SET(CPACK_GENERATOR "DEB")

	# SET(CPACK_RPM_PACKAGE_SUMMARY "CONFIDENTIAL - Pre alpha, internal distribution only.  Blah blah blah.")
	SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt4-dev, libpocofoundation9, libpocodata9, libpocoxml9")

ELSEIF(WIN32)
    
ENDIF(APPLE)

# fugly path variables cut, pasted, and mangled from the Qt Cpack example
SET(plugin_dest_dir bin)
SET(qtconf_dest_dir bin)
SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/qtsimpleclient")
IF(APPLE)
  SET(plugin_dest_dir qtsimpleclient.app/Contents/MacOS)
  SET(qtconf_dest_dir qtsimpleclient.app/Contents/Resources)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/qtsimpleclient.app")
ENDIF(APPLE)
IF(WIN32)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/qtsimpleclient.exe")
ENDIF(WIN32)

#--------------------------------------------------------------------------------
# Install the qtsimpleclient application, on Apple, the bundle is at the root of the
# install tree, and on other platforms it'll go into the bin directory.
INSTALL(TARGETS qtsimpleclient 
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)

IF(APPLE)
	INSTALL(PROGRAMS ${skyperuntime}
	  	DESTINATION ${plugin_dest_dir}
		RENAME skypekit-uikit-runtime
	)
ELSEIF(UNIX)
	INSTALL(PROGRAMS ${skyperuntime}
	   	DESTINATION bin	
		RENAME skypekit-uikit-runtime
	)
ELSEIF(WIN32)
	# COMING SOON
ENDIF(APPLE)

# Now the work of copying dependencies into the bundle/package
# The quotes are escaped and variables to use at install time have their $ escaped
# An alternative is the do a configure_file() on a script and use install(SCRIPT  ...).
# Note that the image plugins depend on QtSvg and QtXml, and it got those copied
# over.
IF(APPLE)
	# directories to look for dependencies
	SET(LIB_DIRS ${QT_LIBRARY_DIRS} ${Poco_LIBRARY_DIR})

	INSTALL(CODE "
			FILE(GLOB_RECURSE QTPLUGINS
				\"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
			# bundle utilities should handle copying and fixing all external dependencies
			INCLUDE(BundleUtilities)
			FIXUP_BUNDLE(\"${APPS}\" \"\${QTPLUGINS}\" \"${LIB_DIRS}\")

			# but fixup_bundle does not complete the job, leaving libraries unstripped and
			# poco libraries not rebased correctly.
			# SO instead we fall back on the qt utility that does the same thing -- correctly
			EXECUTE_PROCESS(COMMAND macdeployqt ${APPS} -verbose=2)
		" COMPONENT Runtime)
ENDIF(APPLE)

INCLUDE(CPack)
