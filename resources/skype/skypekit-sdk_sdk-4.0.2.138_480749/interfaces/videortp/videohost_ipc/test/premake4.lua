sid_path = "../../../.."
videohost_name = "videortphost-ipctest"

dofile(sid_path .. "/buildfiles/premake-common.lua")

define_default_solution()

define_default_project(videohost_name)
	defines { "SKYPEKIT" }

	define_install_action {
		{build_dir .. "/" .. videohost_name, install_dir}, 
	}

	files { 
		"VideoRTPTest.cpp",
	}

	dofile(sid_path .. "/interfaces/videortp/videohost_ipc/premake-in.lua")

	linkoptions {
		"-lpthread -lrt -static"
	}
