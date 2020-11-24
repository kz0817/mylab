import java.util.concurrent.ExecutionException;
import java.util.concurrent.Executors;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Future;

public class App {

    private static class Cat implements Runnable {
        @Override
        public void run() {
            System.out.println("Cat started");
            while (true) {
                System.out.println("cat!");
                try {
                    Thread.sleep(1000);
                } catch (final InterruptedException e) {
                    e.printStackTrace();
                    break;
                }
            }
            System.out.println("Cat exited");
        }
    }

    private static class BusyLoop implements Runnable {
        @Override
        public void run() {
            System.out.println("BusyLoop started");
            long cnt = 0;
            while (true) {
                if (cnt % (100 * 1000 * 1000) == 0) {
                    System.out.format("cnt: %d\n", cnt);
                }
                cnt++;
            }
        }
    }

    private static boolean has(final String[] args, final String target) {
        for (final var arg: args) {
            if (arg.equals(target)) {
                return true;
            }
        }
        return false;
    }

    public static void main(final String[] args) {
        final ExecutorService executor = Executors.newSingleThreadExecutor();
        final boolean doInterrupt = has(args, "-int");
        final boolean doBusyLoop = has(args, "-busyloop");
        System.out.format("Interrupt: %s\n", doInterrupt);
        System.out.format("Mode: %s\n", doBusyLoop ? "BusyLoop" : "Cat");

        final Future<?> future = executor.submit(doBusyLoop ? new BusyLoop() : new Cat());

        try {
            Thread.sleep(500);
            future.cancel(doInterrupt);
            System.out.println("main thread 1");
            Thread.sleep(100 * 1000);
        } catch(final InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("main thread: exited");
        executor.shutdown();
    }
}
