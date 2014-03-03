sid_path = "../../"

videohost_name = "videortphost-imx27"

common_path = sid_path .. "/reference/common"
common_video_path = sid_path .. "/reference/common/video"
videohost_path = sid_path .. "/reference/" .. videohost_name .. "/src"
vpulib_path = sid_path .. "/" .. videohost_name .. "/src/lib/vpu"

dofile(sid_path .. "/buildfiles/premake-common.lua")

define_default_solution()

newoption {
   trigger     = "minimal-mode",
   description = "build without capturing, rendering, encoding and decoding"
}

project "loopback"
	location ("build/")
	language "C++"
	targetdir ("build/")
	targetname ("loopback")
	kind "ConsoleApp"
	defines { "ENABLE_DEBUG", "IMX27PDK" }

	files {
		"../../interfaces/skype/cpp_embedded/src/types/skype-string.cpp",
		common_path .. "/*.hpp",
		common_path .. "/*.cpp",
		common_video_path .. "/*.hpp",
		common_video_path .. "/*.cpp",
		videohost_path .. "/*.hpp",
		videohost_path .. "/*.cpp"
	}
	excludes {
		common_video_path .. "/V4l2Capture.cpp"
	}
	includedirs {
		common_path,
		common_video_path,
		vpulib_path
	}

	dont_add_videohost_files = true

	dofile(sid_path .. "/interfaces/videortp/videohost_ipc/premake-in.lua")

	buildoptions {
	}

	linkoptions {
		vpulib_path .. "/libvpu.a",
		"-lpthread"
	}

define_default_project(videohost_name)
	location ("build/")
	language "C++"
	targetdir ("build/")
	kind "ConsoleApp"
	defines { "ENABLE_DEBUG", "IMX27PDK" }

	define_install_action {
		{build_dir .. "/" .. videohost_name, install_dir},
	}

	if _OPTIONS["minimal-mode"] then
		defines { "MINIMAL_MODE" }
	end

	files {
		common_path .. "/*.hpp",
		common_path .. "/*.cpp",
		common_video_path .. "/*.hpp",
		common_video_path .. "/*.cpp",
		videohost_path .. "/iMX27RtpInterface.cpp",
		videohost_path .. "/SendVideoWrapper.cpp",
		videohost_path .. "/ReceiveVideoWrapper.cpp",
		videohost_path .. "/H264AnxbFileReader.cpp",
		videohost_path .. "/PcapFileReader.cpp",
		videohost_path .. "/Configuration.cpp"
	}
	excludes {
		common_video_path .. "/V4l2Capture.cpp"
	}
	includedirs {
		common_path,
		common_video_path,
		videohost_path,
		vpulib_path
	}

	if not _OPTIONS["minimal-mode"] then
		files {
			videohost_path .. "/Capturer.cpp",
			videohost_path .. "/Encoder.cpp",
			videohost_path .. "/Renderer.cpp",
			videohost_path .. "/Decoder.cpp",
			videohost_path .. "/DummyCapturer.cpp"
		}

		buildoptions {
		}

		linkoptions {
			vpulib_path .. "/libvpu.a"
		}
	end

	dont_add_videohost_files = false

	dofile( sid_path .. "/interfaces/videortp/videohost_ipc/premake-in.lua")
