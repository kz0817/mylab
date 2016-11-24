public class Fib {
    public static void main(String[] args) {
        System.out.println("Fib");
        long n0 = 0;
        long n1 = 1;
        long nLoop = 10;
        boolean verbose = false;

        for (int i = 0; i < args.length; i++) {
            String arg = args[i];
            if (arg == "-v")
                verbose = true;
            else
                nLoop = Long.parseLong(arg);
        }
        System.out.println("nLoop: " + nLoop);

        for (long i = 0; i < nLoop; i++) {
            long a = n0 + n1;
            n0 = n1;
            n1 = a;
            if (verbose)
                System.out.printf("%d, %d%n", n0, n1);
        }
        System.out.printf("%d, %d%n", n0, n1);
    }
}
