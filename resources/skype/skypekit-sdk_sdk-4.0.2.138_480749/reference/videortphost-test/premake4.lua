sid_path = "../../"

videohost_name = "videortphost-test"

common_path = sid_path .. "reference/common"
common_video_path = sid_path .. "reference/common/video"
videohost_path = sid_path .. "reference/" .. videohost_name .. "/src"

dofile(sid_path .. "buildfiles/premake-common.lua")

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
	defines { "SKYPEKIT" }
	defines { "ENABLE_DEBUG" }

	files { 
		"../../interfaces/skype/cpp_embedded/src/types/skype-string.cpp",
		common_path .. "/*.hpp",
		common_path .. "/*.cpp",
		common_video_path .. "/*.hpp",
		common_video_path .. "/*.cpp",
		videohost_path .. "/SkypekitTestEngine.cpp",
		videohost_path .. "/SendVideoWrapper.cpp",
		videohost_path .. "/ReceiveVideoWrapper.cpp",
		videohost_path .. "/H264AnxbFileReader.cpp",
		videohost_path .. "/PcapFileReader.cpp",
		videohost_path .. "/Configuration.cpp",
		videohost_path .. "/V4l2Capturer.cpp",
		videohost_path .. "/H264Encoder.cpp",
		videohost_path .. "/X11Renderer.cpp",
		videohost_path .. "/H264Decoder.cpp",
		videohost_path .. "/ColorConvert.cpp",
		videohost_path .. "/DummyCapturer.cpp",
		videohost_path .. "/main.cpp"
	}
	excludes { videohost_path .. "/SkypekitTestEngine.cpp" }
	includedirs {
		common_path,
		common_video_path,
		videohost_path
	}

	dont_add_videohost_files = true

	dofile(sid_path .. "interfaces/videortp/videohost_ipc/premake-in.lua")

	buildoptions {
		"`pkg-config libavcodec --cflags`"
	}

	linkoptions {
		"`pkg-config libavcodec libavutil --libs`",
		"-lx264 -lXv"
	}

define_default_project(videohost_name)
	defines { "SKYPEKIT" }
	defines { "ENABLE_DEBUG" }

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
		videohost_path .. "/SkypekitTestEngine.cpp",
		videohost_path .. "/SendVideoWrapper.cpp",
		videohost_path .. "/ReceiveVideoWrapper.cpp",
		videohost_path .. "/H264AnxbFileReader.cpp",
		videohost_path .. "/PcapFileReader.cpp",
		videohost_path .. "/Configuration.cpp",
	}

	includedirs {
		common_path,
		common_video_path,
		videohost_path
	}

	if not _OPTIONS["minimal-mode"] then
		files {
			videohost_path .. "/V4l2Capturer.cpp",
			videohost_path .. "/H264Encoder.cpp",
			videohost_path .. "/X11Renderer.cpp",
			videohost_path .. "/H264Decoder.cpp",
			videohost_path .. "/ColorConvert.cpp",
			videohost_path .. "/DummyCapturer.cpp"
		}

		buildoptions {
			"`pkg-config libavcodec --cflags`"
		}
	
		linkoptions {
			"`pkg-config libavcodec libavutil --libs`",
			"-lx264 -lX11 -lXv"
		}
	end

	dont_add_videohost_files = false

	dofile( sid_path .. "interfaces/videortp/videohost_ipc/premake-in.lua")
