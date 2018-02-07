// 自行将文件名改为 Main.java
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.math.BigDecimal;
import java.util.Scanner;

public class Main {
    public static final int CEIL_LEN = 300;

    public static final BigDecimal solve(BigDecimal a, BigDecimal b) {
        BigDecimal c = a;
        c = c.setScale(0, BigDecimal.ROUND_DOWN);
        a = a.subtract(c);
        b = b.subtract(c);
        if (b.compareTo(BigDecimal.ONE) >= 0)
            return BigDecimal.ONE;
        b = BigDecimal.ONE.divide(b, CEIL_LEN, BigDecimal.ROUND_HALF_UP);
        a = BigDecimal.ONE.divide(a, CEIL_LEN, BigDecimal.ROUND_HALF_UP);
        BigDecimal t = solve(b, a);
        b = b.multiply(t);
        return b.setScale(0, BigDecimal.ROUND_UP);
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(new BufferedReader(new InputStreamReader(System.in)));
        BigDecimal a = in.nextBigDecimal();
        BigDecimal b = in.nextBigDecimal();
        a.setScale(CEIL_LEN, BigDecimal.ROUND_HALF_UP);
        b.setScale(CEIL_LEN, BigDecimal.ROUND_HALF_UP);
        System.out.println(solve(a, b));
    }
}