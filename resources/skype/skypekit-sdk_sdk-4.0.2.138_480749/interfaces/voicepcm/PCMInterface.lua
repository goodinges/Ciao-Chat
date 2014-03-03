require 'xsid'

-- Decorators
RETURN = "RETURN"

pcmif = SID "SkypePCMInterface" { 
	ENUMERATED "DeviceType" {
		ENUM(0)  "INPUT_DEVICE" {},
		ENUM(1)  "OUTPUT_DEVICE" {},
		ENUM(2)  "NOTIFICATION_DEVICE" {}
	};
	ENUMERATED "DeviceMask" {
		ENUM(1)  "DEVICEMASK_INPUT" {},
		ENUM(2)  "DEVICEMASK_OUTPUT" {},
		ENUM(4)  "DEVICEMASK_NOTIFICATION" {},
		ENUM(7)  "DEVICEMASK_ALL" {}
	};

	ENUMERATED "ReturnType" {
		ENUM(0)  "PCMIF_OK" {},
		ENUM(1)  "PCMIF_ERROR" {},
		ENUM(2)  "PCMIF_ERROR_PROP_NOT_SUPPORTED" {},
		ENUM(3)  "PCMIF_ERROR_NOT_INITIALIZED" {},
		ENUM(4)  "PCMIF_ERROR_UNKNOWN_DEVICE" {},
		ENUM(5)  "PCMIF_ERROR_ALREADY_RUNNING" {},
		ENUM(6)  "PCMIF_ERROR_SIZE" {}
	};


	ACTION(0) "Init"
	{
	}
	-{
		FIELD(1,RETURN) "rv" {type = "int"}
	};

	ACTION(1) "Start"
	{
		FIELD(1) "deviceType" {type = "int"}
	}
	-{
		FIELD(1,RETURN) "rv" {type = "int"}
	};

	ACTION(3) "Stop"
	{
		FIELD(1) "deviceType" {type = "int"}
	}
	-{
		FIELD(1,RETURN) "rv" {type = "int"}
	};

	ACTION(20) "GetDefaultDevice"
	{
		FIELD(1) "deviceType" {type = "int"}
	}
	- {
		FIELD(1,RETURN) "rv" {type = "int"},
		FIELD(2) "guid" {type = "string"},
		FIELD(3) "name" {type = "string"},
		FIELD(4) "productID" {type = "string"},
	};
	
	ACTION(21) "UseDefaultDevice"
	{
		FIELD(1) "deviceType" {type = "int"},
	}
	-{
		FIELD(1,RETURN) "rv" {type = "int"}
	};
	

	ACTION(22) "GetCurrentDevice"
	{
		FIELD(1) "deviceType" {type = "int"},
	}
	- {
		FIELD(1,RETURN) "rv" {type = "int"},
		FIELD(2) "guid" {type = "string"},
		FIELD(3) "name" {type = "string"},
		FIELD(4) "productID" {type = "string"},
	};

	ACTION(23) "UseDevice"
	{
		FIELD(1) "deviceType" {type = "int" },
		FIELD(2) "guid" {type = "string"},
	}
	-{
		FIELD(1,RETURN) "rv" {type = "int"}
	};


	ACTION(24) "GetDeviceCount"
	{
		FIELD(1) "deviceType" {type = "int"},
	}
	-{
		FIELD(1,RETURN) "rv" {type = "int"},
		FIELD(2) "count" {type = "uint"}
	};

	ACTION(25) "GetDevices"
	{
		FIELD(1) "deviceType" {type = "int"},
	}
	- {
		FIELD(1,RETURN) "rv" {type = "int"},
		FIELD(2) "guid" {type = LIST "string"},
		FIELD(3) "name" {type = LIST "string"},
		FIELD(4) "productID" {type = LIST "string"},
	};

	ACTION(30) "GetVolumeParameters"
	{
		FIELD(1) "deviceType" {type = "int"},
	}
	- {
		FIELD(1,RETURN) "rv" {type = "int"},
		FIELD(2) "range_min" { type = "uint" },
		FIELD(3) "range_max" { type = "uint" },
		FIELD(4) "volume" {type = "uint"},
		FIELD(5) "boost" { type = "int" },
	};

	ACTION(31) "SetVolume"
	{
		FIELD(1) "deviceType" {type = "int"},
		FIELD(2) "volume" {type = "uint"},
	}
	-{
		FIELD(1,RETURN) "rv" {type = "int"}
	};

	ACTION(34) "SetInputBoost"
	{
		FIELD(1) "boost" { type = "int" },
	}
	-{
		FIELD(1,RETURN) "rv" {type = "int"}
	};

	ACTION(35) "GetMute"
	{
		FIELD(1) "deviceType" {type = "int"},
	}
	- {
		FIELD(1,RETURN) "rv" {type = "int"},
		FIELD(2) "muted" {type = "int"}
	};

	ACTION(36) "SetMute"
	{
		FIELD(1) "deviceType" {type = "int"},
		FIELD(2) "mute" {type = "int"},
	}
	-{
		FIELD(1,RETURN) "rv" {type = "int"}
	};

	ACTION(40) "GetSampleRateCount"
	{
		FIELD(1) "deviceType" {type = "int"},
	}
	-{
		FIELD(1,RETURN) "rv" {type = "int"},
		FIELD(2) "count" {type = "uint"}
	};

	ACTION(41) "GetSupportedSampleRates"
	{
		FIELD(1) "deviceType" {type = "int"},
	}
	-{
		FIELD(1,RETURN) "rv" {type = "int"},
		FIELD(2) "sampleRates" {type = LIST "uint"},
	};

	ACTION(42) "GetCurrentSampleRate"
	{
		FIELD(1) "deviceType" {type = "int"},
	}
	- {
		FIELD(1,RETURN) "rv" {type = "int"},
		FIELD(2) "sampleRate" {type = "uint"},
	};
	
	ACTION(43) "SetSampleRate"
	{
		FIELD(1) "deviceType" {type = "int"},
		FIELD(2) "sampleRate" {type = "uint" },
	}
	-{
		FIELD(1,RETURN) "rv" {type = "int"}
	};

	ACTION(50) "SetNumberOfChannels"
	{
		FIELD(1) "deviceType" {type = "int"},
		FIELD(2) "numberOfChannels" {type = "int"},
	}
	-{
		FIELD(1,RETURN) "rv" {type = "int"}
	};
	ACTION(60) "CustomCommand"
	{
		FIELD(1) "command" {type = "string"},
	}
	-{
		FIELD(1) "response" {type = "string"},
		FIELD(2,RETURN) "rv" {type = "int"}
	};

}


pcmif_cb = SID "SkypePCMCallbackInterface" { 
	ACTION(1) "InputDeviceReady"
	{
		FIELD(1) "noOfSamples" {type = "int"},
		FIELD(2) "fs" {type = "int"},
		FIELD(3) "nChannels" {type = "int"},
		FIELD(4) "data" {type = "binary"}
	};
	ACTION(2) "OutputDeviceReady"
	{
		FIELD(1) "noOfSamples" {type = "int"},
		FIELD(2) "fs" {type = "int"},
		FIELD(3) "nChannels" {type = "int"},
	}
	-{
		FIELD(1) "data" {type = "binary"}
	};
	ACTION(3) "NotificationDeviceReady"
	{
		FIELD(1) "noOfSamples" {type = "int"},
		FIELD(2) "fs" {type = "int"},
		FIELD(3) "nChannels" {type = "int"},
	}
	-{
		FIELD(1) "data" {type = "binary"}
	};

	ACTION(4) "InputAndOutputDeviceReady"
	{
		FIELD(1) "length_in_ms" {type = "int"},
		FIELD(2) "input_fs" {type = "int"},
		FIELD(3) "output_fs" {type = "int"},
		FIELD(4) "input_channels" {type = "int"},
		FIELD(5) "output_channels" {type = "int"},
		FIELD(6) "input_data" {type = "binary"}
	}	
	-{
		FIELD(1) "output_data" {type = "binary"}
	};



	ACTION(10) "DeviceStopped"
	{
		FIELD(1) "deviceType" {type = "int"}
	};

	ACTION(20) "DeviceListChanged"
	{
	};
	ACTION(21) "CurrentDeviceChanged"
	{
		FIELD(1) "deviceType" {type = "int"},
		FIELD(2) "guid" {type = "string"},
		FIELD(3) "name" {type = "string"},
		FIELD(4) "productID" {type = "string"}
	};

	ACTION(31) "VolumeChanged"
	{
		FIELD(1) "deviceType" {type = "int"},
		FIELD(2) "range_min" {type = "uint"},
		FIELD(3) "range_max" {type = "uint"},
		FIELD(4) "volume" {type = "uint"},
		FIELD(5) "boost" {type = "int"}
	};
}
