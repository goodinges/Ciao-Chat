sid_path = "../../../"
dofile("../../../buildfiles/premake-common.lua")

solution "skypekit-cppwrapper_2"
  location (build_dir .. "/" .. _ACTION)
  configurations { "Debug", "Release", "Debugbe", "Releasebe" }
  if os.is("windows") then
   defines {"_WIN32", "WIN32", "_CRT_SECURE_NO_DEPRECATE" }
   if _ACTION == "vs2005" or _ACTION == "vs2008" then
     buildoptions { "/wd4100", "/W2" } --turn off excess "unused parameter" warnings
   end
  end
  if _ACTION == "gmake" then
        buildoptions { "-Wshadow -Wconversion" }
  end
  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }
    flags   { "Symbols" }

  configuration { "Release*" }
    defines { "NDEBUG" }
    flags   { "Optimize" } -- "OptimizeSize" }

  configuration { "*be" }
	defines { "BIG_ENDIAN_ORDER" }
--------------------------------------------------------
if _OPTIONS["ssl_transport"] == "cyassl" then
project "skypekit-cyassl_lib"
  uuid "D6F6595F-DAD5-4063-922F-7A22BD54C16C"
  kind "StaticLib"
  targetdir ( "build/" )
  location ( "build/" .. _ACTION )
  language "C"

  includedirs { sidipcpath .. "ssl/cyassl/include/",
                sidipcpath .. "ssl/cyassl/ctaocrypt/include/",
                sidipcpath .. "ssl/cyassl/include/openssl/" }

  files  { sidipcpath .. "ssl/cyassl/include/*.h", sidipcpath .. "ssl/cyassl/src/*.c" }
  files  { sidipcpath .. "ssl/cyassl/ctaocrypt/include/*.h", sidipcpath .. "ssl/cyassl/ctaocrypt/src/*.c" }

  defines { "NO_RABBIT", "NO_HC128", "NO_DES", "NO_DSA", "NO_MD4", "NO_FILESYSTEM", "HAVE_CONFIG_H" }
end
--------------------------------------------------------

project "skypekit-cppwrapper_2_lib"
  uuid "4F859021-BC05-C007-6E54-32A520D3AB04"
  flags { "ExtraWarnings", "FatalWarnings" }
  location (build_dir .. "/" .. _ACTION)
  language "C++"
  kind     "StaticLib"
  targetdir (build_dir)
  includedirs { "src/api/",
                "src/ipc/",
                "src/types/",
                sidipcpath,
                sidplatformpath
        }

    --threading
  if os.is("windows") then
    files  { "src/platform/threading/win/*.h", "src/platform/threading/win/*.cpp" }
    includedirs { "src/platform/threading/win/", "src/platform/threading/" }
  else
    files  { "src/platform/threading/pthread/*.h", "src/platform/threading/pthread/*.cpp" }
    includedirs { "src/platform/threading/pthread/", "src/platform/threading/" }
  end

  if _OPTIONS["framework"] == "se" then
    --se types
    files  { "src/types/*.h", "src/types/*.cpp" }
  elseif _OPTIONS["framework"] == "qt" then
    includedirs { "/usr/include/qt4/" }
  elseif _OPTIONS["framework"] == "poco" then
  end

  --ssl
  if _OPTIONS["ssl_transport"] == "openssl" then
    defines { "SSL_LIB_OPENSSL" }
  else
    defines { "SSL_LIB_CYASSL", "NO_FILESYSTEM" }
    includedirs { sidipcpath .. "ssl/cyassl/ctaocrypt/include/" }
    includedirs { sidipcsslpath .. "include/" }
  end

  --source
  files  { "src/ipc/*.h", "src/ipc/*.cpp" }
  --platform
  files  { sidplatformpath.."*.h", sidplatformpath.."SidPlatform.cpp" }
  --generated api
  files  { "src/api/sidg_Skypeevent_dispatch.cpp" }
  files  { "src/api/sidg_msgs_Skype.cpp" }
  files  { "src/api/sidg_Skypeaction_call.cpp" }
  files  { "src/api/skype-embedded_2.cpp" }
  files  { "src/api/sidg_Skypeproperty_get_call.cpp" }
  files  { "src/api/sidg_Skyperefs.cpp" }

  --common ipc
  files  {      sidipcpath .. "/SidProtocolClientInterface.hpp",
                sidipcpath .. "/SidSession.cpp",
                sidipcpath .. "/SidSession.hpp",
                sidipcpath .. "/SidClientSession.cpp",
                sidipcpath .. "/SidClientSession.hpp",
                sidipcpath .. "/SidCommandProcessor.cpp",
                sidipcpath .. "/SidCommandProcessor.hpp",
                sidipcpath .. "/SidField.hpp",
                sidipcpath .. "/SidProtocolBinCommon.cpp",
                sidipcpath .. "/SidProtocolBinCommon.hpp",
                sidipcpath .. "/SidProtocolBinClient.cpp",
                sidipcpath .. "/SidProtocolBinClient.hpp",
                sidipcpath .. "/SidConnection.cpp",
                sidipcpath .. "/SidConnection.hpp",
                sidipcpath .. "/SidTransportLog.cpp",
                sidipcpath .. "/SidTransportLog.hpp",
                sidipcpath .. "/SidClientConnection.hpp",
                sidipcpath .. "/SidClientConnection.cpp",
                sidipcpath .. "/SidTLSEncryption.cpp",
                sidipcpath .. "/SidTLSEncryption.hpp",
		sidipcpath .. "/SidDebugging.cpp"
        }

--------------------------------------------------------

project "skypekitclient"
  uuid "27C85894-B46D-76FE-4146-71A68E9872CB"
  flags { "ExtraWarnings", "FatalWarnings" }
  if _ACTION == "gmake" then
        buildoptions { "-fno-use-cxa-atexit" }
  end
  location (build_dir .. "/".. _ACTION)
  language "C++"
  targetdir (build_dir)
  kind     "ConsoleApp"

  links { "skypekit-cppwrapper_2_lib" }

  --threading
  if os.is("windows") then
    includedirs { "src/platform/threading/win/", "src/platform/threading/" }
  else
    if _OPTIONS["target-os"] ~= "android" then
      links { "pthread" }
    end
    includedirs { "src/platform/threading/pthread/", "src/platform/threading/" }
  end

  --framework
  if _OPTIONS["framework"] == "qt" then
    links{ "QtCore" }
    includedirs { "/usr/include/qt4/" }
  elseif _OPTIONS["framework"] == "poco" then
    links{ "PocoFoundation" }
  end

  if os.is("windows") then
    links { "Ws2_32" }
  end

  --ssl transport
  if _OPTIONS["ssl_transport"] == "openssl" then
    defines { "SSL_LIB_OPENSSL" }
    if os.is("windows") then
      links { "ssleay32", "libeay32" }
    else
      links { "ssl", "crypto" }
    end
  else
    defines { "SSL_LIB_CYASSL", "NO_FILESYSTEM" }
    links { "skypekit-cyassl_lib" }
    includedirs { sidipcpath .. "ssl/cyassl/ctaocrypt/include/" }
	includedirs { sidipcsslpath .. "include/" }
  end

  files  { "src/client/*.h", "src/client/*.cpp" }
  includedirs { "src/api/",
                "src/ipc/",
                "src/types/",
                sidipcpath,
                sidplatformpath
        }

  -- surface rendering

  if _OPTIONS["enable-surfacerendering"] then
    defines { "SKYPEKIT_SURFACE_RENDERING" }
    defines { "GL_WINDOW" }

    if os.is("windows") then
      files { sidipcpath .. "/VideoBuffers/WinShm.hpp", sidipcpath .. "/VideoBuffers/WinShm.cpp" }
    else
      files { sidipcpath .. "/VideoBuffers/SysVShm.hpp", sidipcpath .. "/VideoBuffers/SysVShm.cpp" }
    end
    includedirs { sidipcpath .. "/VideoBuffers" }

    if os.is("macosx") then
      linkoptions { "-framework OpenGL", "-framework glut" }
    elseif os.is("windows") then
      links { "opengl32" }
      links { "gdi32" }
    else
      links { "GL", "glut" }
    end

    if not _OPTIONS["surface-transport"] then
      if not os.is("windows") then
        _OPTIONS["surface-transport"] = "posix"
      end
    end

    configuration "posix"
      defines { "VIDEO_TRANSPORT_POSIX" }
      files { sidipcpath .. "/VideoBuffers/PosixShm.hpp", sidipcpath .. "/VideoBuffers/PosixShm.cpp" }
      if os.is("linux") then
        links { "rt" }
      end

    configuration "sysv"
      defines { "VIDEO_TRANSPORT_SYSV" }

  end

  -- there's no support for postflags so this has to be given here..
  if _OPTIONS["target-os"] == "android" then
    links { "gcc" }
  end

  --no target property in premake
  if os.is("windows") then
    ext = ".exe"
  else
    ext = ""
  end

  define_install_action {
        { build_dir .. "/skypekitclient"..ext , install_dir }
  }
