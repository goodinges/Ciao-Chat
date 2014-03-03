sid_path = "../../"
enginehost_name = "gstreamer-media-engine"
enginehost_path = sid_path .. "/reference/" .. enginehost_name
common_path = sid_path .. "/reference/common"
shm_common_path = enginehost_path .. "/shm"

dofile(sid_path .. "/buildfiles/premake-common.lua")

define_default_solution()

define_default_project(enginehost_name)
	kind "SharedLib"

    for _, block in pairs(premake.solution.list["default_solution"].blocks) do
        block["buildoptions"] = nil
    end

	defines { "ENABLE_DEBUG" }

	define_install_action {
		{build_dir .. "/lib" .. enginehost_name .. ".so", install_dir},
	}

	includedirs {
		common_path,
		shm_common_path,
	}

	files {
		enginehost_path .. "/skype-audio-manager.hpp",
		enginehost_path .. "/skype-audio-manager.cpp",
		enginehost_path .. "/skype-audio-stream.h",
		enginehost_path .. "/skype-audio-stream.c",
		enginehost_path .. "/skype-base-stream.h",
		enginehost_path .. "/skype-base-stream.c",
		enginehost_path .. "/skype-video-manager.hpp",
		enginehost_path .. "/skype-video-manager.cpp",
		enginehost_path .. "/skype-video-stream.h",
		enginehost_path .. "/skype-video-stream.c",
		enginehost_path .. "/skype-utils.h",
		enginehost_path .. "/skype-utils.c",

		enginehost_path .. "/skype-media-engine.h",
		enginehost_path .. "/skype-media-engine.cpp",
		enginehost_path .. "/skype-media-engine-audio-server.hpp",
		enginehost_path .. "/skype-media-engine-audio-server.cpp",
		enginehost_path .. "/skype-media-engine-video-server.hpp",
		enginehost_path .. "/skype-media-engine-video-server.cpp",
		enginehost_path .. "/skype-audio-call.h",
		enginehost_path .. "/skype-audio-call.c",
		enginehost_path .. "/skype-base-call.h",
		enginehost_path .. "/skype-base-call.c",
		enginehost_path .. "/skype-base-call-priv.h",
		enginehost_path .. "/skype-video-call.h",
		enginehost_path .. "/skype-video-call.c",

		shm_common_path .. "/shmalloc.c",
		shm_common_path .. "/shmalloc.h",
		shm_common_path .. "/shmpipe.c",
		shm_common_path .. "/shmpipe.h",
		shm_common_path .. "/shm-reader.c",
		shm_common_path .. "/shm-reader.h",
		shm_common_path .. "/shm-writer.c",
		shm_common_path .. "/shm-writer.h",
	}

	dofile(sid_path .. "interfaces/videortp/videohost_ipc/premake-in.lua")

	buildoptions {
		"`pkg-config --cflags gobject-2.0`",
		"`pkg-config --cflags farsight2-0.10`"
	}

	linkoptions {
		"-lrt",
		"`pkg-config --libs gobject-2.0`",
		"`pkg-config --libs farsight2-0.10`"
	}

	excludes {
		sid_path .. "/interfaces/voicertp/SidMultiRTPHostMain.cpp",
		sid_path .. "/interfaces/voicertp/SidVideoRTPHostMain.cpp",
	}

	dofile(sid_path .. "/interfaces/voicertp/premake-in.lua")


define_install_action(
{
	{build_dir .. "/lib" .. enginehost_name .. ".so", install_dir},
	{enginehost_path .. "/element-preferences", install_dir},
	{enginehost_path .. "/codec-preferences", install_dir}
})
