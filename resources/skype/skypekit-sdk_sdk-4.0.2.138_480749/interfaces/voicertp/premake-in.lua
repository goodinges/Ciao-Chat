voicertphostif_path = "../../interfaces/voicertp"
se_path = "../../interfaces/skype/cpp_embedded/src/types"
se_path1 = "../../ipc/cpp/platform/se"
se_path2 = "../../interfaces/skype/cpp_embedded/src/platform/threading"

if _OPTIONS["target-os"] == "windows" then
se_path3 = "../../interfaces/skype/cpp_embedded/src/platform/threading/win"
else
se_path3 = "../../interfaces/skype/cpp_embedded/src/platform/threading/pthread"
end

sidlibipc_path = "../../ipc/cpp/"
sidlibipc_path1 = "../../ipc/cpp/AVTransport"


files {
    voicertphostif_path .. "/*.cpp",
    voicertphostif_path .. "/*.hpp",
	sidlibipc_path .. "/SidApi.cpp",
	sidlibipc_path .. "/SidAVTransportWrapper.cpp",
	sidlibipc_path .. "/SidField.cpp",
	sidlibipc_path .. "/SidProtocolBinClient.cpp",
	sidlibipc_path .. "/SidProtocolBinCommon.cpp",
	sidlibipc_path .. "/SidProtocolBinServer.cpp",
	sidlibipc_path .. "/SidProtocolFactory.cpp",
	sidlibipc_path .. "/SidAVServer.cpp",
	sidlibipc_path .. "/SidAVDummyHelpers.cpp",
	sidlibipc_path1 .. "/SocketTransport.cpp",

	se_path .. "/skype-string.cpp",
	se_path .. "/skype-int-list.cpp",
	se_path .. "/skype-string-list.cpp",
	se_path .. "/skype-binary.cpp",
}
if _OPTIONS["target-os"] == "windows" then
files {
	sidlibipc_path1 .. "/NamedPipes.cpp",
	se_path3 .. "/skype-thread-win.cpp",
	se_path1 .. "/SidPlatform.cpp",
}
else
files {
	sidlibipc_path1 .. "/UnixSocket.cpp",
	se_path3 .. "/skype-thread-pthread.cpp",
}
end

includedirs {
	voicertphostif_path,
	se_path,
	se_path1,
	se_path2,
	se_path3,
	sidlibipc_path,
	sidlibipc_path1,
}

if _OPTIONS["target-os"] == "linux" then
linkoptions {
	"-lpthread"
}
end

