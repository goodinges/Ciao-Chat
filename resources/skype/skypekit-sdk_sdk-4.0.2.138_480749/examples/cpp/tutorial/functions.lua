sid_path = "../../../";
dofile(sid_path .. "buildfiles/premake-common.lua")

function isWindows()
	if (_ACTION == vs2005) or (_ACTION == vs2008) then return true end
	if os.is("windows") then return true end
	return false
end

function isMacOS()
	if (_ACTION == xcode3) then return true end
	if os.is("macosx") then return true end
	return false
end

function createTutorialProject(m_name,m_files,m_uuid)
  project(m_name)
  uuid(m_uuid)
  language "C++"
  kind     "ConsoleApp"

  defines { "SSL_LIB_CYASSL", "NO_FILESYSTEM" }

  local cppemb = "../../../../interfaces/skype/cpp_embedded/"  
  
  if os.is("windows") then
    links { "Ws2_32" }
    includedirs { cppemb .. "/src/platform/threading/win/" }
  else
    links { "pthread" } 
    includedirs { cppemb .. "src/platform/threading/pthread/" }    
  end

  files(m_files)
  
  includedirs { 
		cppemb .."/src/api/",
		cppemb .."/src/ipc/",
		cppemb .."/src/types/",
		cppemb .."/src/platform/threading/",
		cppemb .."/src/platform/transport/",
		cppemb .."/src/platform/transport/tcp/",
		"../../../../ipc/cpp",
		"../../../../ipc/cpp/platform/se",
		"../../../../ipc/cpp/ssl/cyassl/include",
		"../../../../ipc/cpp/ssl/cyassl/ctaocrypt/include",
		"../../../../ipc/cpp/ssl/cyassl/include/openssl",
    "../keypair/",
    "../common/"
	}
        
  libdirs { cppemb .."/build/" }
  links { "skypekit-cppwrapper_2_lib" }
     
	local ssl_lib_dir = sid_path .. "../bin/" .. _OPTIONS["target-os"] .. "-" .. _OPTIONS["target-arch"].."/";       
	libdirs { ssl_lib_dir }	
	links { "skypekit-cyassl_lib" }			

end
