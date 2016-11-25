import java.math.BigInteger;

public class Fib {
    public static void main(String[] args) {
        System.out.println("Fib");
        BigInteger n0 = new BigInteger("0");
        BigInteger n1 = new BigInteger("1");
        BigInteger tmp = new BigInteger("0");
        long nLoop = 10;
        boolean verbose = false;
        boolean quiet = false;

        for (int i = 0; i < args.length; i++) {
            String arg = args[i];
            if (arg.equals("-v"))
                verbose = true;
            else if (arg.equals("-q"))
                quiet = true;
            else
                nLoop = Long.parseLong(arg);
        }
        System.out.println("nLoop: " + nLoop);

        for (long i = 0; i < nLoop; i++) {
            tmp = n0.add(n1);
            n0 = n1;
            n1 = tmp;
            if (verbose)
                System.out.printf("%d, %d%n", n0, n1);
        }
        if (quiet) {
            System.out.printf("Finished.");
        } else if (!verbose) {
            System.out.printf("%d, %d%n", n0, n1);
        }
    }
}
