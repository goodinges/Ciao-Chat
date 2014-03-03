/**
 * Copyright (C) 2011, Skype Limited
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

package com.skype.tutorial.appkeypair;

import java.io.*;
import java.security.PrivateKey;
import java.security.cert.X509Certificate;
import java.security.spec.InvalidKeySpecException;

import com.skype.util.PemReader;
import com.skype.tutorial.util.MySession;


	/**
	 * Encapsulates the AppKeyPair required to access the SkypeKit runtime.
	 * 
	 * @author Andrea Drane
	 * 
	 * @since 1.0
	 */
	public class AppKeyPairMgr {
		/**
		 * Info/Debug console output message prefix/identifier tag.
		 * Corresponds to class name.
		 * 
		 * @since 1.0
		 */
	    public static final String MY_CLASS_TAG = "AppKeyPairMgr";

	    /**
	     * Expected suffixes (case insensitive) for the file containing the AppKeyPair data.
	     * Used by {@link #readAppKeyPairFromFile(String)}.
	     * 
	     * @since 1.0
	     */
	    public static final String[] KEY_PAIR_FILE_SUFFIXES = {
	    	"crt",
	    	"pem",
	    	"CRT",
	    	"PEM"
	    };

	    /**
	     * Default name of the PEM file containing the AppKeyPair data string.
	     * Used by {@link #readAppKeyPairFromFile(String)}.
	     * 
	     * @since 1.0
	     */
	    public static final String KEY_PAIR_DEFAULT_PATHNAME = "skypekit-cert.pem";

		private X509Certificate x509CertificateData = null;; 
		private PrivateKey privateKeyData = null;

		
		/**
		 * Retrieves the AppKeyPair certificate data.
		 * 
		 * @return
		 * 	The actual X.509 certificate or
		 *  null if the certificate file hasn't been read/set.
		 * 
		 * @see #x509CertificateData
		 * 
		 * @since 1.0
		 */
		public X509Certificate getAppKeyPairCertificate() {

			return (this.x509CertificateData);
		}

		
		/**
		 * Retrieves the AppKeyPair private key data.
		 * 
		 * @return
		 * 	The actual private key associated with the X.509 certificate or
		 *  null if the certificate file hasn't been read/set.
		 * 
		 * @see #privateKeyData
		 * 
		 * @since 1.0
		 */
		public PrivateKey getAppKeyPairPrivateKey() {

			return (this.privateKeyData);
		}

		
		/**
		 * Sets the AppKeyPair X.509 certificate and associated private key data from the <i>configured default</i> pathname of the PEM file containing the actual AppKeyPair data.
		 * 
		 * @return
		 * 	<ul>
		 *	  <li>true: AppKeyPair successfully set from the file data</li>
		 *	  <li>false:
		 *	    <ul>
		 *	      <li>Could not open the file</li>
		 *	      <li>Could not read the file</li>
		 *	      <li>Not a valid PEM file</li>
		 *	    </ul>
		 *	    AppKeyPair is unchanged.
		 *	  </li>
		 *	</ul>
		 * 
		 * @see #KEY_PAIR_DEFAULT_PATHNAME
		 * @see #readAppKeyPairFromFile
		 * 
		 * @since 1.0
		 */
		public boolean setAppKeyPairFromFile() {
	
			// readAppKeyPairFromFile will issue the appropriate error messages
			return (this.readAppKeyPairFromFile(KEY_PAIR_DEFAULT_PATHNAME));
		}


		/**
		 * Sets the AppKeyPair X.509 certificate and associated private key data from the <i>specified</i> pathname.
		 * 
		 * @param pathName
		 * 	The pathname of the PEM file containing the actual AppKeyPair data.
		 * 
		 * @return
		 * 	<ul>
		 *	  <li>true: AppKeyPair successfully set from the file data</li>
		 *	  <li>false:
		 *	    <ul>
		 *	      <li>Could not open the file</li>
		 *	      <li>Could not read the file</li>
		 *	      <li>Not a valid PEM file</li>
		 *	    </ul>
		 *	    AppKeyPair is unchanged.
		 *	  </li>
		 *	</ul>
		 * 
		 * @see #readAppKeyPairFromFile
		 * 
		 * @since 1.0
		 */
		public boolean setAppKeyPairFromFile(String pathName) {
	
			// readAppKeyPairFromFile will issue the appropriate error messages
			return (this.readAppKeyPairFromFile(pathName));
		}

		
		/**
		 * Common method to read the AppKeyPair data from a file and extract the certificate and private key data.
		 * <br /><br />
		 * Initially assumes the name of the path of the file containing the AppKeyPair data is
		 * fully qualified. If it doesn't end in one of the recognized suffixes, assumes its a
		 * path-only component and appends the default file pathname {@link #KEY_PAIR_DEFAULT_PATHNAME},
		 * which it assumes is configured as a filename. Writes diagnostic messages to the console
		 * if the specified file cannot be opened or the AppKeyPair data is not valid.
		 * 
		 * @param pathName
		 * 	The path of the file containing the actual certificate data.
		 * 
		 * @return
		 * 	<ul>
		 *	  <li>true: AppKeyPair successfully read (and set) from the file data</li>
		 *	  <li>false:
		 *	    <ul>
		 *	      <li>Could not open the file</li>
		 *	      <li>Could not read the file</li>
		 *	      <li>Not a valid certificate file</li>
		 *	    </ul>
		 *	    AppKeyPair is unchanged.
		 *	  </li>
		 *	</ul>
		 * 
		 * @see #KEY_PAIR_FILE_SUFFIXES
		 * @see #KEY_PAIR_DEFAULT_PATHNAME
		 * @see #x509CertificateData
		 * @see #privateKeyData
		 * @see #setAppKeyPairFromFile()
		 * @see #setAppKeyPairFromFile(String)
		 * 
		 * @since 1.0
		 */
		public boolean readAppKeyPairFromFile(String pathName) {
			int	i;
			int	j;
			String fullPathName = pathName;

			j = KEY_PAIR_FILE_SUFFIXES.length;
			for (i = 0; i < j; i++) {
				if (pathName.endsWith(KEY_PAIR_FILE_SUFFIXES[i])) {
					break;
				}
			}
			
			// Check for no suffix match - either an unrecognized suffix or just a path name.
			if (i >= j) {
				if (pathName.charAt((pathName.length() - 3)) == '.') {
					// Unrecognized suffix (if you mean a path that includes ".xyz" as it's last
					// four characters, pass it as ".xyz/"
					MySession.myConsole.printf("%s/readAppKeyPairFromFile: Unrecognized Certificate file suffix.%n\tPathname: %s (%c)%n\tRecognized suffixes:%n", 
							MY_CLASS_TAG, pathName);
					j = KEY_PAIR_FILE_SUFFIXES.length;
					for (i = 0; i < j; i++) {
						MySession.myConsole.printf("\t\t%s%n", KEY_PAIR_FILE_SUFFIXES[i]);
					}
					return (false);
				}
				else {
					// Just a path name, so append the default portion of the path.
					fullPathName = pathName + "/" + KEY_PAIR_DEFAULT_PATHNAME;
				}
			}

		    try {
		        PemReader myPemReader = new PemReader(fullPathName);
		        this.x509CertificateData = myPemReader.getCertificate();		
		        this.privateKeyData = myPemReader.getKey();		
		    }  
	          catch (FileNotFoundException e1) {
	               MySession.myConsole.printf("%s/readAppKeyPairFromFile: Could not find certificate file:%n\t%s%n", 
	            		   MY_CLASS_TAG, fullPathName);
	               return (false);
	           }
		    catch (IOException e2) {
		    	MySession.myConsole.printf("%s/readAppKeyPairFromFile: Unable to read certificate file:%n\t%s%n",
	            		   MY_CLASS_TAG, fullPathName);
	               return (false);
		    }
		    catch (InvalidKeySpecException e3) {
		    	MySession.myConsole.printf("%s/readAppKeyPairFromFile: Certificate file %s contains invalid certficate data.%n",
			    		MY_CLASS_TAG, fullPathName);
			    return(false);
		    }		

	    	MySession.myConsole.printf("%s/readAppKeyPairFromFile: Extracted certificate and key data from:%n\t%s%n",
         		   MY_CLASS_TAG, fullPathName);
		    return (true);
		}
	}
