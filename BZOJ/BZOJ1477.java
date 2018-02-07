// 自行将文件名改为 Main.java
import java.util.Scanner;

public class Main {

    public static long x0, y0;

    public static void main(String[] args) {
        Scanner scan = new Scanner(System.in);
        long x = scan.nextInt();
        long y = scan.nextInt();
        long m = scan.nextInt();
        long n = scan.nextInt();
        long L = scan.nextInt();
        x = x % L;
        y = y % L;
        if (x > y) {
            long t = y;
            y = x;
            x = t;
            t = n;
            n = m;
            m = t;
        }
        long a = Math.abs(m - n);
        long b = -L;
        long c;
        if (m > n) {
            c = y - x;
        } else {
            c = x - y + L;
        }
        long d = gcd(a, b);
        if (c % d != 0) {
            System.out.println("Impossible");
        } else {
            long add1 = x0 * c / d;
            long add2 = Math.abs(b / d);
            while (add1 < 0) {
                add1 += add2;
            }
            while (add1 - add2 >= 0) {
                add1 -= add2;
            }
            System.out.println(add1);
        }
    }
    public static long gcd(long a, long b) {
        long t, d;
        if (b == 0) {
            x0 = 1;
            y0 = 0;
            return a;
        }
        d = gcd(b, a % b);
        t = x0;
        x0 = y0;
        y0 = t - a / b * y0;
        return d;
    }
}