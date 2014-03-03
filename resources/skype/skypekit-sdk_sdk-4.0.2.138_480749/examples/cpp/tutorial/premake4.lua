require "functions"

solution "skypekit-tutorials"
  location ("build/" .. _ACTION)
  configurations { "Debug","Release" }
  if isMacOS() then
    platforms { "x32", "x64","native" }
  end
  if isWindows() then
   defines { "WIN32", "_CRT_SECURE_NO_DEPRECATE" }
   buildoptions { "/wd4100" } --turn off excess "unused parameter" warnings
  end
  flags { "ExtraWarnings", "NoExceptions" ,"NoRTTI" }  
  
  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }
    flags   { "Symbols" }
  
  configuration { "Release*" }
    defines { "NDEBUG" }
    flags   { "Optimize" }
    
include "step1"
include "step2"
include "step3"
include "step4"
include "step5"
include "step6"
include "step7"
include "step8"
include "step9"
include "step10"
include "step11"
include "step12"
include "step13"
include "step14"
include "step15"
include "step16"
