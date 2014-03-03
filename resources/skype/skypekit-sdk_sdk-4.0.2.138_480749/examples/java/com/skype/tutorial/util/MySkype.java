/**
 * Copyright (C) 2010, Skype Limited
 *
 * All intellectual property rights, including but not limited to copyrights,
 * trademarks and patents, as well as know how and trade secrets contained in,
 * relating to, or arising from the internet telephony software of
 * Skype Limited (including its affiliates, "Skype"), including without
 * limitation this source code, Skype API and related material of such
 * software proprietary to Skype and/or its licensors ("IP Rights") are and
 * shall remain the exclusive property of Skype and/or its licensors.
 * The recipient hereby acknowledges and agrees that any unauthorized use of
 * the IP Rights is a violation of intellectual property laws.
 *
 * Skype reserves all rights and may take legal action against infringers of
 * IP Rights.
 *
 * The recipient agrees not to remove, obscure, make illegible or alter any
 * notices or indications of the IP Rights and/or Skype's rights and
 * ownership thereof.
 */

package com.skype.tutorial.util;

import java.util.Locale;

import com.skype.api.Skype;

/**
 * Tutorials-specific extension of Skype that overrides <em>nothing</em>.
 * However, this sub-class:
 * <ul>
 *   <li>provides a template/basis for tutorial-specific overrides</li>
 *   <li>provides storage for session-wide members that shouldn't be garbage-collected</li>
 *   <li>includes common add-on methods, such as {@link #SetupAudioDevices}</li>
 *   <li>commonizes parameterization for methods that pass a MySkype instance</li>
 * </ul>
 * 
 * @author Andrea Drane (ported/refactored from existing C++ tutorial code)
 * 
 * @since 1.0
 */

public class MySkype extends Skype {
	/**
	 * Info/Debug console output message prefix/identifier tag <em>prefix</em>.
	 * Corresponds to class name.
	 * 
	 * @since 1.0
	 */
    public static final String MY_CLASS_TAG_PFX = "MySkype";
    
	/**
	 * Info/Debug console output message prefix/identifier tag.
	 * Corresponds to class name.
	 * 
	 * @since 1.0
	 */
    public static String myTutorialTag;
    
	/**
	 * Datagram stream ID, used by Tutorial 11.
	 * 
	 * @since 1.0
	 */
	public String streamName = new String("");


    /**
     * Default constructor.
     * <br/><br />
     * Forces the info/debug console output message prefix/identifier tag to:<br />
     * &nbsp;&nbsp;<code>MY_CLASS_TAG_PFX + "0"</code> (zero).
     * 
	 * @since 1.0
     */
    public MySkype() {
    	
    	this((MY_CLASS_TAG_PFX + "0"));
    }


    /**
     * Tutorial constructor.
     * <br/><br />
     * Sets the info/debug console output message prefix/identifier tag to
     * <code>MY_CLASS_TAG_PFX</code> concatenated with the portion of the
     * invoker's prefix/identifier tag that starts with an underscore. These
     * characters should be the digits corresponding to the tutorial step number.
     * 
	 * @since 1.0
     */
    public MySkype(String tutorialTag) {

     	myTutorialTag = MY_CLASS_TAG_PFX + tutorialTag.substring(tutorialTag.indexOf('_'));
    }

    
	/**
	 * Assigns active input and output devices from among those available.
	 * Notifies user regarding the name of the selected devices or whether the request failed.
	 * <em>Both</em> devices must exist for the request to succeed.
	 * 
	 * @param micIdx
	 * 	Index into the array of available recording devices of the requested input device.
	 * @param spkrIdx
	 * 	Index into the array of available playback devices of the requested output device.
	 * 
	 * @return
	 * <ul>
	 *   <li>true: success</li>
	 *   <li>false: failure</li>
	 * </ul>
	 * 
	 * @see com.skype.api.Skype#GetAvailableRecordingDevices()
	 * @see com.skype.api.Skype#GetAvailableOutputDevices()
	 * 
	 * @since 1.0
	 */
	public boolean SetupAudioDevices(int micIdx, int spkrIdx) {
		boolean	passFail = true;	// Ever the optimist, assume success!
		
		Skype.GetAvailableRecordingDevicesResult inputDevices = this.GetAvailableRecordingDevices();
		Skype.GetAvailableOutputDevicesResult outputDevices = this.GetAvailableOutputDevices();

		if (micIdx > (inputDevices.handleList.length + 1)) {
			MySession.myConsole.printf("%s: Invalid mic device no. (%d) passed!%n", myTutorialTag, micIdx);
			passFail = false;
		}

		if (spkrIdx > (outputDevices.handleList.length + 1)) {
			MySession.myConsole.printf("%s: Invalid speaker device no. (%d) passed!%n", myTutorialTag, spkrIdx);
			passFail = false;
		}
		
		if (passFail) {
			MySession.myConsole.printf("%s: Setting mic to %s (%s)%n",
					myTutorialTag, inputDevices.nameList[micIdx], inputDevices.productIdList[micIdx]);
			MySession.myConsole.printf("%s: Setting speakers to %s  (%s)%n",
					myTutorialTag, outputDevices.nameList[spkrIdx], outputDevices.productIdList[spkrIdx]);
			this.SelectSoundDevices(inputDevices.handleList[micIdx],
					outputDevices.handleList[spkrIdx], outputDevices.handleList[spkrIdx]);
			this.SetSpeakerVolume(100);
		}

		return (passFail);
	}
	
	
	/**
	 * Normalizes a phone number and indicates that operation's success/failure.
	 * <br /><br />
	 * Determines the country code dialing prefix through {@link com.skype.api.Skype#GetISOCountryInfo()}
	 * by matching the default Locale country with an entry in the
	 * {@link com.skype.api.Skype.GetISOCountryInfoResult#countryCodeList}.
	 * Writes a message to the console indicating success/failure reason.
	 * 
	 * @param pstn
	 * 	Phone number to normalize.
	 * 
	 * @return
	 *   The normalization result, which includes:
	 *   <ul>
	 *     <li>an Enum instance detailing success/failure reason.</li>
	 *     <li>the normalized string (success) or error message string (failure)</li>
	 *   </ul>
	 * 
	 * @see com.skype.api.Skype#NormalizePSTNWithCountry(String, int)
	 * @see com.skype.api.Skype#GetISOCountryInfo()
	 * 
	 * @since 1.0
	 */
	public Skype.NormalizeIdentityResult GetNormalizationStr(String pstn) {
		Skype.NormalizeIdentityResult nrmlResultReturn = new NormalizeIdentityResult();
		
		Skype.GetISOCountryInfoResult isoInfo = this.GetISOCountryInfo();
		int availCountryCodes = isoInfo.countryCodeList.length;
		int isoInfoIdx;
		String ourCountryCode = Locale.getDefault().getCountry();
		for (isoInfoIdx = 0; isoInfoIdx < availCountryCodes; isoInfoIdx++) {
			if (ourCountryCode.equalsIgnoreCase(isoInfo.countryCodeList[isoInfoIdx])) {
				break;
			}
		}
		if (isoInfoIdx >= availCountryCodes) {
			nrmlResultReturn.result = Skype.NORMALIZERESULT.IDENTITY_EMPTY; // Anything but IDENTITY_OK...
			nrmlResultReturn.normalized = "Couldn't match Locale!";
			MySession.myConsole.printf("%s: Error! Couldn't match Locale %s in Skype.GetISOCountryInfo results%n",
					myTutorialTag, ourCountryCode);
			return (nrmlResultReturn);
		}
		MySession.myConsole.printf("%n%s ISOInfo match (%d of %d):%n\tCode: %s%n\tDialExample: %s%n\tName: %s%n\tPrefix: %s%nLocale: %s%n%n",
				myTutorialTag, (isoInfoIdx + 1),
				this.GetISOCountryInfo().countryCodeList.length,
				this.GetISOCountryInfo().countryCodeList[isoInfoIdx],
				this.GetISOCountryInfo().countryDialExampleList[isoInfoIdx],
				this.GetISOCountryInfo().countryNameList[isoInfoIdx],
				this.GetISOCountryInfo().countryPrefixList[isoInfoIdx],
				Locale.getDefault().getCountry());
		
		Skype.NormalizePSTNWithCountryResult nrmlResult =
			this.NormalizePSTNWithCountry(pstn, isoInfo.countryPrefixList[isoInfoIdx]);

		switch (nrmlResult.result) {
		case IDENTITY_OK:
			nrmlResultReturn.normalized = nrmlResult.normalized;
			break;
		case IDENTITY_EMPTY:
			nrmlResultReturn.normalized = "Identity input was empty";
			break;
		case IDENTITY_TOO_LONG:
			nrmlResultReturn.normalized = "Identity string too long";
			break;
		case IDENTITY_CONTAINS_INVALID_CHAR:
			nrmlResultReturn.normalized = "Invalid character(s) found in identity string";
			break;
		case PSTN_NUMBER_TOO_SHORT:
			nrmlResultReturn.normalized = "PSTN number too short";
			break;
		case PSTN_NUMBER_HAS_INVALID_PREFIX:
			nrmlResultReturn.normalized = "Invalid character(s) found in PSTN prefix";
			break;
		case SKYPENAME_STARTS_WITH_NONALPHA :
			nrmlResultReturn.normalized = "Skype Name string starts with non-alphanumeric character";
			break;
		case SKYPENAME_SHORTER_THAN_6_CHARS:
			nrmlResultReturn.normalized = "Skype Name too short";
			break;
		default:
			nrmlResultReturn.normalized = "Cannot determine Skype.NORMALIZATION ?!?";
			break;
		}

		if (nrmlResult.result != Skype.NORMALIZERESULT.IDENTITY_OK) {
			MySession.myConsole.printf("%s: Error! Raw PSTN: %s - Normalized PSTN: %s.%n", myTutorialTag, pstn, nrmlResultReturn.normalized);
		}
		else {
			MySession.myConsole.printf("%s: Raw PSTN: %s / Normalized PSTN: %s.%n",
									myTutorialTag, pstn, nrmlResultReturn.normalized);
		}
		
		nrmlResultReturn.result = nrmlResult.result;
		return nrmlResultReturn;
	}

/*
 	 **
	 * Translates an APP2APP_STREAMS type to a displayable string.
	 * Used by Tutorial 11.
	 * 
	 * @param listType
	 * 	APP2APP_STREAMS enum to translate.
	 * 
	 * @return
	 *   A string representation of the enum value, or "unknown stream type" if not recognized.
	 * 
	 * @see com.skype.api.Skype.APP2APP_STREAMS
	 * 
	 * @since 1.0
	 *
	public String StreamListType(Skype.APP2APP_STREAMS listType) {
		String listTypeAsText;
		
	    switch (listType) {
	        case ALL_STREAMS:
	        	listTypeAsText = "all streams";
	        	break;
	        case SENDING_STREAMS:
	        	listTypeAsText = "sending stream";
	        	break;
	        case RECEIVED_STREAMS:
	        	listTypeAsText = "receiving stream";
	        	break;
	        default:
	        	listTypeAsText = "unknown stream type";
	        	break;
	    }
	    return (listTypeAsText);
	}
*/
}
