sid_path = "../../../../"
dofile(sid_path .. "buildfiles/premake-common.lua")

solution "skypekit-cyassl_lib"
  location ("build/" .. _ACTION)
  configurations { "ReleaseStatic", "ReleaseShared", "DebugStatic", "DebugShared" }
  if os.is("windows") then
   defines {"_WIN32", "WIN32", "_CRT_SECURE_NO_DEPRECATE" }
  end
  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }
    flags   { "Symbols" }

  configuration { "Release*" }
    defines { "NDEBUG" }
    flags   { "Optimize" }

--------------------------------------------------------

project "skypekit-cyassl_lib"
  uuid "D6F6595F-DAD5-4063-922F-7A22BD54C16C"
  targetdir ( install_dir )
  location ( "build/" .. _ACTION )
  language "C"
  os.mkdir( install_dir )
    
  includedirs { "include/", 
                "ctaocrypt/include/",
                "include/openssl/" }  
        
  files  { "include/*.h", "src/*.c" }      
  files  { "ctaocrypt/include/*.h", "ctaocrypt/src/*.c" }
  
  defines { "NO_FILESYSTEM", "NO_RABBIT", "NO_HC128", "NO_DES", "NO_DSA", "HAVE_CONFIG_H" }
   
  configuration { "*Static" }
    kind "StaticLib" 
           
  configuration { "*Shared" }
    kind "SharedLib" 
        
