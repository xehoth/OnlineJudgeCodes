// 自行将文件名改为 Main.java
import java.math.BigInteger;
import java.util.Scanner;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        BigInteger b1 = sc.nextBigInteger();
        BigInteger b2 = sc.nextBigInteger();
        System.out.println(b1.gcd(b2));
    }
}