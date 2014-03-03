/**
 *
 */
package com.skype.util;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.security.KeyFactory;
import java.security.NoSuchAlgorithmException;
import java.security.PrivateKey;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.PKCS8EncodedKeySpec;


/**
 * Minimal utilities for reading and decoding PEM certificate files
 *
 * @author kcjones
 *
 */
public class PemReader
{

    private static final String TAG          = "PemReader";
    private static final String certStart    = "-----BEGIN CERTIFICATE-----";
    private static final String certEnd      = "-----END CERTIFICATE-----";
    private InputStream         certInStream    = null;
    private InputStream         keyInStream  = null;

    public PemReader(InputStream cert, InputStream key)
    {
        certInStream = cert;
        keyInStream = key;
    }

    public PemReader(String pemFilePath) throws FileNotFoundException
    {
        File file = new File(pemFilePath);

        certInStream = new FileInputStream(file);

        assert (pemFilePath.endsWith("pem"));
        String derPath = pemFilePath.substring(0, pemFilePath.length() - 3);
        derPath += "der";

        file = new File(derPath);
        Log.d(TAG, "file length:" + file.length());

        keyInStream = new FileInputStream(file);
    }

    public X509Certificate getCertificate() throws IOException, InvalidKeySpecException
    {
        if (certInStream == null)
            throw new IOException("No certification source specified");

        StringBuffer contents = new StringBuffer();

        try {
            BufferedReader reader = new BufferedReader(new InputStreamReader(certInStream));
            String text;
            while ((text = reader.readLine()) != null) {
                contents.append(text).append(System.getProperty("line.separator"));
            }

            // find and parse certificate
            int ci = contents.indexOf(certStart);
            int cf = contents.indexOf(certEnd) + certEnd.length();
            if ( ! (ci >= 0 && cf > ci))
                throw new InvalidKeySpecException("Missing or malformed certificate data.");

            String certString = contents.substring(ci, cf);
            certInStream = new ByteArrayInputStream(certString.getBytes());

            CertificateFactory cfac = CertificateFactory.getInstance("X.509");
            return (X509Certificate)cfac.generateCertificate(certInStream);
        }
        catch (CertificateException e) {
            throw new IOException("Invalid certificate in PEM file: " + e.getMessage());
        }
    }

    public PrivateKey getKey() throws IOException
    {

        if (keyInStream == null)
            throw new IOException("No private key source specified");

        try {
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            int bite;
            while ((bite = keyInStream.read()) != - 1) {
                baos.write(bite);
            }

            byte[] encodedKey = baos.toByteArray();

            PKCS8EncodedKeySpec spec = new PKCS8EncodedKeySpec(encodedKey);

            KeyFactory factory = KeyFactory.getInstance("RSA");
            return factory.generatePrivate(spec);
        }
        catch (IndexOutOfBoundsException e) {
            throw new IOException("Invalid private key source");
        }
        catch (InvalidKeySpecException e) {
            throw new IOException("Invalid private key in DER file: " + e.getMessage());
        }
        catch (NoSuchAlgorithmException e) {
            // I'll be damned. No RSA? I don't believe it.
            e.printStackTrace();
            System.exit(1);
        }
        return null;
    }
}
