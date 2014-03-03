sid_path = "../../../.."
skvidipctest_name = "skvidipctest"

dofile(sid_path .. "/buildfiles/premake-common.lua")

define_default_solution()

define_default_project(skvidipctest_name)
	defines { "SKYPEKIT" }

	define_install_action {
		{build_dir .. "/" .. skvidipctest_name, install_dir}, 
	}

	includedirs {
		sid_path .. "/../VideoEngine/VideoEnginePlatforms/RtpIf/SID",
		se_path,
	}	
	
	files { 
		"SidIpcTestClient.cpp",
	}

	dofile(sid_path .. "/interfaces/videortp/skypekit_ipc/premake-in.lua")

	linkoptions {
		"-lpthread -lrt -static"
	}
