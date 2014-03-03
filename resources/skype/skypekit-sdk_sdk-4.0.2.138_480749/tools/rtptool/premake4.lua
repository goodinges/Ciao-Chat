sid_path = "../.."

tool_name = "rtptool"

common_path = sid_path .. "/reference/common"
common_video_path = common_path .. "/video"
tool_path = sid_path .. "/tools/" .. tool_name .. "/src"

dofile(sid_path .. "/buildfiles/premake-common.lua")

define_default_solution()

define_default_project(tool_name)
	define_install_action {
		{build_dir .. "/" .. tool_name, install_dir}
	}

	files {
		tool_path .. "/*.cpp"
	}

	includedirs {
		common_path,
		common_video_path
	}
