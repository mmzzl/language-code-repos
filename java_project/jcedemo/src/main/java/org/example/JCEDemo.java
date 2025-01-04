import com.fmjce.crypto.dev.FMException;
import fisher.man.jce.provider.FishermanJCE;
import org.example.FMSYS;
import java.io.File;
import java.security.Security;
import java.util.Base64;
import javax.crypto.spec.IvParameterSpec;

import fisher.man.util.GetConfig;
import org.apache.log4j.Logger;
import java.nio.charset.StandardCharsets;
import java.util.Objects;

//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
public class JCEDemo {
    private static final IvParameterSpec ivSpec = new IvParameterSpec(new byte[]{0x0A, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F});
    private static byte[] ivBytes = ivSpec.getIV();
    private static final Logger logger = Logger.getLogger(JCEDemo.class.getName());

    public static void sm4Encrypt(String s) {
        try {
            FMSYS sm4 = new FMSYS();
            byte[] indata = s.getBytes();
            byte[] encdata = sm4.InternalSM4Enc(10, "CBC", true, indata, ivBytes);
            if (encdata != null) {
                String encdataBase64 = Base64.getEncoder().encodeToString(encdata);
                System.out.println(encdataBase64);
            }
        } catch (Exception e) {
            logger.error("SM4 encryption failed: " + e.getMessage());
        }
    }

    public static void sm4Decrypt(String mimeBase64EncodedString) {
        try {
            FMSYS sm4 = new FMSYS();
            Base64.Decoder mimeDecoder = Base64.getMimeDecoder();
            byte[] decodedBytes = mimeDecoder.decode(mimeBase64EncodedString);
            byte [] decdata=sm4.InternalSM4Dec(10, "CBC", true, decodedBytes, ivBytes); //解密方法
            String res=new String(decdata, StandardCharsets.UTF_8); //明文转字符串输出
            System.out.println(res);
        } catch (Exception e) {
            logger.error("SM4 decryption failed: " + e.getMessage());
        }
    }


    public static void main(String[] args) {
        Security.addProvider(new FishermanJCE());
        System.out.println("hah");
        for (String arg : args) {
            if (arg.startsWith("--")) {
                String[] keyValue = arg.substring(2).split("=");
                System.out.println("Setting " + keyValue[0] + " to " + keyValue[1]);
                if (keyValue.length == 2) {
                    String key = keyValue[0];
                    String value = keyValue[1];
                    if (Objects.equals(key, "sm4encode")) {
                        sm4Encrypt(value);
                    }
                    if (Objects.equals(key, "sm4decode")) {
                        sm4Decrypt(value);
                    }
                }
            }
        }
        System.exit(0);
    }
}