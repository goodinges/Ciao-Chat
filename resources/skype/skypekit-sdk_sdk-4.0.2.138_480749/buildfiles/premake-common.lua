newoption {
   trigger     = "target-arch",
   description = "target architecture",
   value       = "ARCH",
}

newoption {
   trigger     = "target-os",
   description = "target os",
   value       = "OS",
}

newoption {
   trigger     = "enable-exceptions",
   description = "enable exceptions",
   value       = false,
}

newoption {
   trigger     = "framework",
   description = "target framework",
   value       = "se",
   allowed = {
      { "se",   "Skype embedded" },
      { "qt",   "Qt" },
      { "poco", "POCO C++ Libraries" }
   }   
}

newoption {
   trigger     = "internal",
   description = "internal",
   value       = "INTERNAL_BUILD",
   allowed = {
      { "yes",  "" },
      { "no",   "" },
      { "1",   "" },
      { "0",   "" },
   }   
}

newoption {
   trigger     = "ssl_transport",
   description = "ssl_transport",
   value       = "cyassl",
   allowed = {
      { "openssl",   "" },
      { "cyassl",   "" },            
   }   
}

newoption {
   trigger     = "enable-surfacerendering",
   description = "enable surface rendering(client side video rendering).",
   value       = false,
}

newoption {
        trigger         = "surface-transport",
        value           = "BUF",
        description     = "Buffers to use on video surface transport for mac and linux. Windows will use always windows shared memory.",
        allowed = {
                { "sysv", "SysV shared memory" },
                { "posix", "POSIX shared memory" },
                { "iosurface", "IOSurface (Mac only)" }
        }
}

if not _OPTIONS["target-arch"] then
   _OPTIONS["target-arch"] = "x86"
end

if not _OPTIONS["framework"] then
   _OPTIONS["framework"] = "se"
end

if os.is("macosx") then
  platform = "mac"
elseif os.is("windows") then
  platform = "windows"
else 
  platform = "linux"
end

if not _OPTIONS["target-os"] then
   _OPTIONS["target-os"] = platform
end

if not _OPTIONS["ssl_transport"] then
   _OPTIONS["ssl_transport"] = "cyassl"
end

build_dir = "build"
sidipcpath = sid_path .. "/ipc/cpp/"
sidipcsslpath = sidipcpath .. "ssl/" .. _OPTIONS["ssl_transport"] .. "/"
sidplatformpath = sid_path .. "/ipc/cpp/platform/" .. _OPTIONS["framework"] .. "/"

install_dir = sid_path .. "bin/" .. _OPTIONS["target-os"] .. "-" .. _OPTIONS["target-arch"].."/"

function define_default_solution()
	solution "default_solution"
		flags { "ExtraWarnings", "FatalWarnings", "NoRTTI" }
		if not _OPTIONS["enable-exceptions"]  then
			flags {"NoExceptions"}
		end
		if _ACTION == "gmake" then
		buildoptions {
			"-fno-use-cxa-atexit"
		}
		end
		location (build_dir .. "/"  .. _ACTION)
		configurations {
			"Debug-" .. _OPTIONS["target-arch"],
			"Release-" .. _OPTIONS["target-arch"]
		}

		configuration { "Debug*" }
			defines { "_DEBUG", "DEBUG" }
			flags { "Symbols" }

		configuration { "Release*" }
			defines { "NDEBUG" }
			flags { "Optimize" }

end

function define_default_project(binary_name)
	project(binary_name)
	language "C++"
	kind "ConsoleApp"
	targetdir (build_dir)	
	targetname (binary_name)
end


function define_install_action(install_actions)
	newaction {
		trigger     = "install",
		description = "Install the software",
		execute     = function ()
			for k,install_action in pairs(install_actions) do
				print("mkdir ", install_action[2])
				os.mkdir(install_action[2])
				print("copy ", install_action[1], install_action[2]..path.getname(install_action[1]))
				res, errormsg = os.copyfile(install_action[1], install_action[2]..path.getname(install_action[1]))
				
				if not res then 
				    printf("error while copying: "..errormsg) 
				end    
			end 
		end
	}
end

