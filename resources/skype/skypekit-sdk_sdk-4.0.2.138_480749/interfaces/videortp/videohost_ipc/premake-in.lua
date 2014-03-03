videortphostipc_path1 = "../../../interfaces/videortp/"
videortphostipc_path2 = "../../../interfaces/videortp/videohost_ipc"
se_path = "../../../interfaces/skype/cpp_embedded/src/types"
se_path1 = "../../../ipc/cpp/platform/se"
se_path2 = "../../../interfaces/skype/cpp_embedded/src/platform/threading"
se_path3 = "../../../interfaces/skype/cpp_embedded/src/platform/threading/pthread"
sidlibipc_path = "../../../ipc/cpp/"
sidlibipc_path1 = "../../../ipc/cpp/AVTransport"


if dont_add_videohost_files then
files {
	se_path .. "/skype-binary.cpp",
	se_path3 .. "/skype-thread-pthread.cpp",
}
includedirs {
	videortphostipc_path1,
	videortphostipc_path2,	
	se_path,
	se_path1,
	se_path2,
	se_path3,
	sidlibipc_path
}

else

files {
    videortphostipc_path2 .. "/*.cpp",
    videortphostipc_path2 .. "/*.hpp",
	sidlibipc_path .. "/SidApi.cpp",
	sidlibipc_path .. "/SidAVDummyHelpers.cpp",
	sidlibipc_path .. "/SidAVTransportWrapper.cpp",
	sidlibipc_path .. "/SidAVServer.cpp",
	sidlibipc_path .. "/SidProtocolBinClient.cpp",
	sidlibipc_path .. "/SidProtocolBinCommon.cpp",
	sidlibipc_path .. "/SidProtocolBinServer.cpp",
	sidlibipc_path .. "/SidProtocolFactory.cpp",
	sidlibipc_path1 .. "/SocketTransport.cpp",
	sidlibipc_path1 .. "/UnixSocket.cpp",
--	sidlibipc_path1 .. "/ShmTransport.cpp",
--	sidlibipc_path1 .. "/ShmIpcSysV.cpp",

	se_path .. "/skype-string.cpp",
	se_path .. "/skype-int-list.cpp",
	se_path .. "/skype-string-list.cpp",
	se_path .. "/skype-binary.cpp",
	se_path3 .. "/skype-thread-pthread.cpp",
}
includedirs {
	videortphostipc_path1,
	videortphostipc_path2,
	se_path,
	se_path1,
	se_path2,
	se_path3,
	sidlibipc_path,
	sidlibipc_path1,
}

if _OPTIONS["target-os"] ~= "android" then
linkoptions {
	"-lpthread"
}
else
	linkoptions {
		"-llog"
	}
end
end

