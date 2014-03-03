sid_path = "../../.."
voicehost_name = "media-engine"
voicehost_test_name = voicehost_name .. "-test"
voicehost_path = sid_path .. "/reference/" .. voicehost_name
common_path = sid_path .. "/reference/common"
shm_common_path = voicehost_path .. "/shm"

dofile(sid_path .. "/buildfiles/premake-common.lua")

define_default_solution()

define_default_project(voicehost_test_name)
	defines { "ENABLE_DEBUG" }

	define_install_action {
		{build_dir .. "/../" .. voicehost_name .. "/tests/", install_dir},
	}

	includedirs {
		common_path,
		shm_common_path,
	}

	files {
		voicehost_path .. "/tests/main.c",
		voicehost_path .. "/tests/test-media-engine.cpp",
		voicehost_path .. "/tests/test-audio-stream.c",
		voicehost_path .. "/tests/test-video-stream.c",

		voicehost_path .. "/tests/sidg_multirtpif_client.cpp",
		voicehost_path .. "/tests/sidg_multirtpif_client.hpp",
		voicehost_path .. "/tests/sidg_videortpif_client.cpp",
		voicehost_path .. "/tests/sidg_videortpif_client.hpp",
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
		"-lmedia-engine"
	}

	excludes {
		sid_path .. "/interfaces/voicertp/SidMultiRTPHostMain.cpp",
		sid_path .. "/interfaces/videortp/SidVideoRTPHostMain.cpp"
	}

	dofile(sid_path .. "/interfaces/voicertp/premake-in.lua")
	dofile(sid_path .. "/interfaces/videortp/premake-in.lua")
