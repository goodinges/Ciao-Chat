	package com.skype.ipc;

//import sun.misc.BASE64Decoder;

import javax.net.ssl.X509ExtendedKeyManager;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.net.Socket;
import java.security.*;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.security.spec.PKCS8EncodedKeySpec;
import com.skype.util.Base64;


public class SkypeKeyManager extends X509ExtendedKeyManager {
    private X509Certificate cert;
    private PrivateKey key;

    // key and cert must be Base64 encoded
    public SkypeKeyManager(String key, String cert) throws IOException, GeneralSecurityException {
        this(b64Decode(key), b64Decode(cert));
    }

    public SkypeKeyManager(byte[] key, byte[] cert) throws GeneralSecurityException {
        CertificateFactory crt_fctry = CertificateFactory.getInstance("X.509", "BC");
        this.cert = (X509Certificate) crt_fctry.generateCertificate(new ByteArrayInputStream(cert));

        KeyFactory keyFactory = KeyFactory.getInstance("RSA");
        PKCS8EncodedKeySpec privSpec = new PKCS8EncodedKeySpec(key);
        this.key = keyFactory.generatePrivate(privSpec);
    }

    public String[] getClientAliases(String s, Principal[] principals) {
        return new String[0];
    }

    public String chooseClientAlias(String[] strings, Principal[] principals, Socket socket) {
        return "";
    }

    public String[] getServerAliases(String s, Principal[] principals) {
        return new String[0];
    }

    public String chooseServerAlias(String s, Principal[] principals, Socket socket) {
        return "";
    }

    public X509Certificate[] getCertificateChain(String s) {
        return new X509Certificate[]{cert};
    }

    public PrivateKey getPrivateKey(String s) {
        return key;
    }

    private static byte[] b64Decode(String s) throws IOException {
//        BASE64Decoder decoder = new BASE64Decoder();
 //       Base64 decoder = new Base64();
  //      return decoder.decodeBuffer(s);
        return Base64.decode(s);
    }



    // bellow this point only test code
    /*
    public static void main(String[] args) throws Exception {
        Security.addProvider(new org.bouncycastle.jce.provider.BouncyCastleProvider());

        SkypeKeyManager skm = new SkypeKeyManager(KEY_STR, CERT_STR);

        SSLContext sslContext = SSLContext.getInstance("TLS");
        TrustManagerFactory tmf = TrustManagerFactory.getInstance("X509");
        tmf.init(KeyStore.getInstance("BKS", "BC"));
        sslContext.init(new KeyManager[]{skm}, tmf.getTrustManagers(), new java.security.SecureRandom());


        ServerSocket ss = sslContext.getServerSocketFactory().createServerSocket(5555);
        Socket s = ss.accept();
        while(true) {
            System.out.print((char) s.getInputStream().read());
        }
    }


    private static final String CERT_STR =
            "MIID4zCCAsugAwIBAgIJAPmYgI9o7pDLMA0GCSqGSIb3DQEBBQUAMFQxJDAiBgkq\n" +
            "hkiG9w0BCQEWFXNreXBlbW9iaWxlQHNreXBlLm5ldDEUMBIGA1UEAxMLU2t5cGVN\n" +
            "b2JpbGUxFjAUBgNVBAsTDU1vYmlsZUFuZHJvaWQwHhcNMTAxMTE1MTQxMzQ4WhcN\n" +
            "MzUxMTA5MTQxMzQ4WjBUMSQwIgYJKoZIhvcNAQkBFhVza3lwZW1vYmlsZUBza3lw\n" +
            "ZS5uZXQxFDASBgNVBAMTC1NreXBlTW9iaWxlMRYwFAYDVQQLEw1Nb2JpbGVBbmRy\n" +
            "b2lkMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuM5GkYfBAeQehp7F\n" +
            "931RyHd6RtwoStWF9hytyaff6JUmn3BAYU7EM/rAofHOCZr0RNVmLU37oICZwozG\n" +
            "5uIDSdE/JzXa5xTfIR6DjvXVYvFRx1KhpGbTsaJtaAdMMi2Sg9vOBuPX8uuwixHM\n" +
            "Hy2VxVFRq7o1Wh/wbo8Wx5xsBz0C5GI65OVkbZHb+NNZTgg8R8aY/R5eCbdsEV7u\n" +
            "zVg/pIxyw0brpbTcTT1o6Nl7wdowA5JAjNB4P0WbEXK+qiHDOlaszNIwCSa8A8c1\n" +
            "Ap1Lef1IPnCXQP4prg2LvudeESX88ogx9Uot9LsAIaufl79Rj3xVJ0jH+wsf7j1g\n" +
            "7ZuGbQIDAQABo4G3MIG0MB0GA1UdDgQWBBRY8qck63v1OZq+Y5YveEeh7vMGLzCB\n" +
            "hAYDVR0jBH0we4AUWPKnJOt79TmavmOWL3hHoe7zBi+hWKRWMFQxJDAiBgkqhkiG\n" +
            "9w0BCQEWFXNreXBlbW9iaWxlQHNreXBlLm5ldDEUMBIGA1UEAxMLU2t5cGVNb2Jp\n" +
            "bGUxFjAUBgNVBAsTDU1vYmlsZUFuZHJvaWSCCQD5mICPaO6QyzAMBgNVHRMEBTAD\n" +
            "AQH/MA0GCSqGSIb3DQEBBQUAA4IBAQCv16CltWRfqmvTe8oekqrCgwECTSmgPWnj\n" +
            "BnIY+1z4unyPn5c9kCRsmWZKL3oDVIIWeCLTlFgGUPK28eW3bV1W+mnCUBGo+YyT\n" +
            "Tc8nMFWtAjsSlm0jaIaF+pk1wlcvkhpk4MAYiENxQomV//kgDx9uMQm1DegQYc+V\n" +
            "w+4NPBIqmYXZxX4VW2/TXUHl0SUv1eipqUILKg/HUOo7h3uh3K01MpG/UJbYV/md\n" +
            "SqUoJ6lrU8BdQeLeBawPIDg9mAZkRjM96sPMB+xxFIuYRp0NPk77mmG24f7/Ajmd\n" +
            "BPeyOAzXMdgvWRpiSsSVLgqZ2qJywwcqcDRP3+xYNOxsg6+kegms";

    private static final String KEY_STR =
            "MIIEpAIBAAKCAQEAuM5GkYfBAeQehp7F931RyHd6RtwoStWF9hytyaff6JUmn3BA\n" +
            "YU7EM/rAofHOCZr0RNVmLU37oICZwozG5uIDSdE/JzXa5xTfIR6DjvXVYvFRx1Kh\n" +
            "pGbTsaJtaAdMMi2Sg9vOBuPX8uuwixHMHy2VxVFRq7o1Wh/wbo8Wx5xsBz0C5GI6\n" +
            "5OVkbZHb+NNZTgg8R8aY/R5eCbdsEV7uzVg/pIxyw0brpbTcTT1o6Nl7wdowA5JA\n" +
            "jNB4P0WbEXK+qiHDOlaszNIwCSa8A8c1Ap1Lef1IPnCXQP4prg2LvudeESX88ogx\n" +
            "9Uot9LsAIaufl79Rj3xVJ0jH+wsf7j1g7ZuGbQIDAQABAoIBAG51vPBXeCGtPCGy\n" +
            "3RkkqrFr2IZqKRsxrTSBx850rPzTyZYa/iAreFKpWmJxqXlMSRyrgLsrlW9F7qG+\n" +
            "i1SNWvHQRDMnmYPtkcC2AGR+w+Cz5PGCjmM5oubV8xbu5bhITP7LbsXngbgM8CTv\n" +
            "qlYoZRDfnjVKw4Du/kLW/AKrmFZqOThgSiQInm3pg0xx+KIlmUQt3AVtvvV0Tbbo\n" +
            "qUwQCGjomVV0Pk7FNukxuFqUlUftZEEkyQg7UsnyJkjw74rt4g5IOE/sa16QorsP\n" +
            "BT25c45c2NFlJnBKvI5Ip4/ijlxGRnWnrcsAPqa5vCS5Ry/qzJ5g/jCEm5jcIkut\n" +
            "d8uXxQECgYEA3MHQqwqoXLutYJ3fOTHLLiio8TVPiFBRHiGtV4EqWWfJ/17yA11T\n" +
            "ufw+9fvO4uGfD4MObPcahuOr7MqdQUr/t9F4LGNn6NFPQiI5fDGn2H1DIngEv5iw\n" +
            "lwcbhgV+nfQn1neMtdhK7GGVyxH469Y6l05sHHQs8sqCaNh06340Q30CgYEA1k8n\n" +
            "zfziLG7J42+6Pe/CA0Loo8/Lq1Qbi9f322P1godpelUArqXjq/xlq4uvjG3srR/q\n" +
            "rXbjVIdoHZ4fzkd9+fjxssjnZOvMEfIq/CahvKdzwT9xGQ2XmVZsPwWmJCm4JxUE\n" +
            "nJbT2eEzQKAyZvIMrxw/Aqf0ToA9dd09yJVI4bECgYB5SKqCLjrNp/bbTCEl5SuB\n" +
            "0qjuzKFDfQz0BwCbQMWplRIvkVRZcN7dIa7dqljB4b9n4FL+nsweK9t7Ht5Czxer\n" +
            "QBRMHtkp2kU3SmJWz1kt4bQOre297f+nGiqO+VnVIhz5AXl3NrwDHqITjW4689oA\n" +
            "4v8WquU/Bu+a9sFV2GKSaQKBgQCOrcRib6pkyp2+vfPQqH8SjMk1xnJibmyoffCW\n" +
            "L8aUUHL47v61R9x79F38B9OoOscHu+gR9MUE0eGPVmb2fsJwWj0QOpFTr2j9yay0\n" +
            "k0O6xgFa91HkzBfSDqeijbA/A0eHPYcEeaYxuKzWqdDkmOG486U/r3IHkXlZryIj\n" +
            "GMCdAQKBgQCqNTOyM7Msl74RASFzY64kg1egD8qYviEBg14jhzArmpnR2NquLgAc\n" +
            "ULbTS2C76vWci5JTdCxodcFS03UMcmnCzLlY7gTt68zstm2PERtRObeDa4mrdB/q\n" +
            "WTLBY0SR67MP/4i6h7DppaMTboTzHoP2s5NXGHFD4q2cbGTTppIYPw==";
*/
    }
