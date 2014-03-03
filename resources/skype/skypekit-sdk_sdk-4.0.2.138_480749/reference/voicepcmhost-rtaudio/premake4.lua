sid_path = "../../"
voicehost_name = "voicepcmhost-rtaudio"
voicehost_path = "../../reference/" .. voicehost_name

dofile(sid_path .. "buildfiles/premake-common.lua")

define_default_solution()

define_default_project(voicehost_name)
	if os.is("windows") then
		ext = ".exe"
	else 
		ext = ""
	end

	define_install_action {
		{build_dir .. "/" .. voicehost_name .. ext, install_dir}, 
	}

	files { 
		voicehost_path .. "/RtAudioPCMInterface.cpp",
		voicehost_path .. "/rtaudio-4.0.6/RtAudio.cpp"
	}

	if _ACTION == "vs2005" or _ACTION == "vs2008" then
		buildoptions { "/wd4100", "/W2", "/EHsc" } --turn off excess "unused parameter" warnings
	end
	
	includedirs {
		voicehost_path .. "/rtaudio-4.0.6"
	}

	dofile "../../interfaces/voicepcm/premake-in.lua"

if _OPTIONS["target-os"] == "windows" then
	defines {
		"__WINDOWS_DS__"
	}
	linkoptions {
		"dsound.lib"
	}	
else
	defines {
		"__LINUX_ALSA__"
	}
	linkoptions {
		"-lasound"
	}	
end