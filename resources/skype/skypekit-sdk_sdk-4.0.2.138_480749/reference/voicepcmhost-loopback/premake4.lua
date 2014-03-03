sid_path = "../../"
voicehost_name = "voicepcmhost-loopback"
voicehost_path = sid_path .. "reference/" .. voicehost_name

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
		voicehost_path .. "/PCMLoopback.cpp"
	}
	if _ACTION == "vs2005" or _ACTION == "vs2008" then
		buildoptions { "/wd4100", "/W2", "/EHsc" } --turn off excess "unused parameter" warnings
	end
	
	dofile(sid_path .. "interfaces/voicepcm/premake-in.lua")

			