sid_path = "../../"

videohost_name = "videortphost-loopback"

videohost_path = sid_path .. "reference/" .. videohost_name

dofile(sid_path .. "buildfiles/premake-common.lua")

define_default_solution()

define_default_project(videohost_name)
	define_install_action {
		{build_dir .. "/" .. videohost_name, install_dir}, 
	}

	files { 
		videohost_path .. "/*.cpp"
	}

	dofile (sid_path .. "interfaces/videortp/videohost_ipc/premake-in.lua")

	