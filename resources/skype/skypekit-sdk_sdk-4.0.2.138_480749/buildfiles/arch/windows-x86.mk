
ifeq ($(TOOLCHAIN),MSVC10)
	MSBUILD = /cygdrive/c/Windows/Microsoft.NET/Framework/v4.0.30319/MSBuild.exe
else 
	MSBUILD = /cygdrive/c/Windows/Microsoft.NET/Framework/v2.0.50727/MSBuild.exe
endif