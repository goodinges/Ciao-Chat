sid_path = "../../../.."
module_name = "shm"
module_test_name = "common-shm-test"
module_path = sid_path .. "/reference/media-engine/shm"
common_path = sid_path .. "/reference/common"
shm_common_path = module_path

dofile(sid_path .. "/buildfiles/premake-common.lua")

define_default_solution()

define_default_project(module_test_name)
	defines { "ENABLE_DEBUG" }

	define_install_action {
		{build_dir .. "/.." .. module_path .. "/tests/", install_dir},
	}

	includedirs {
		common_path,
		shm_common_path,
	}

	files {
                module_path .. "/tests/main.c",

		shm_common_path .. "/shmalloc.c",
		shm_common_path .. "/shmalloc.h",
		shm_common_path .. "/shmpipe.c",
		shm_common_path .. "/shmpipe.h",

		shm_common_path .. "/shm-reader.c",
		shm_common_path .. "/shm-reader.h",
		shm_common_path .. "/shm-writer.c",
		shm_common_path .. "/shm-writer.h",
	}

	buildoptions {
		"`pkg-config --cflags gobject-2.0`",
	}

	linkoptions {
		"-lrt",
		"`pkg-config --libs gobject-2.0`",
	}

	excludes {
		sid_path .. "/interfaces/voicertp/SidMultiRTPHostMain.cpp"
	}

	dofile(sid_path .. "/interfaces/voicertp/premake-in.lua")
