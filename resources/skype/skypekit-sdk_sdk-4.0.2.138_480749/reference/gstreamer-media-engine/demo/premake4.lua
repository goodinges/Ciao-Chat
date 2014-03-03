sid_path = "../../../"
voicehost_name = "gstreamer-media-engine"
voicehost_test_name = voicehost_name .. "-demo"
voicehost_path = sid_path .. "/reference/" .. voicehost_name
common_path = sid_path .. "/reference/common"
shm_common_path = common_path .. "/shm"

dofile(sid_path .. "/buildfiles/premake-common.lua")

define_default_solution()

define_default_project(voicehost_test_name)
    for _, block in pairs(premake.solution.list["default_solution"].blocks) do
        block["buildoptions"] = nil
    end

	defines { "ENABLE_DEBUG" }

	define_install_action {
		{build_dir .. "/" .. voicehost_test_name, install_dir},
	}

	includedirs {
		common_path,
		shm_common_path,
	}

	files {
		voicehost_path .. "/demo/main.c",
	}

	includedirs {
		voicehost_path
	}

	buildoptions {
		"`pkg-config --cflags gobject-2.0`",
		"`pkg-config --cflags farsight2-0.10`",
	}

	libdirs {
		"../build/"
	}

	linkoptions {
		"`pkg-config --libs gobject-2.0`",
		"`pkg-config --libs farsight2-0.10`",
		"-lgstreamer-media-engine"
	}
