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

    private static boolean shouldInterrupt(final String[] args) {
        for (final var arg: args) {
            if (arg.equals("-int")) {
                return true;
            }
        }
        return false;
    }

    public static void main(final String[] args) {
        final ExecutorService executor = Executors.newSingleThreadExecutor();
        final Future<?> future = executor.submit(new Cat());
        final boolean doInterrupt = shouldInterrupt(args);
        System.out.format("Interrupt: %s\n", doInterrupt);
        try {
            Thread.sleep(500);
            future.cancel(doInterrupt);
            System.out.println("main thread 1");
            Thread.sleep(5000);
        } catch( final InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("main thread: exited");
        executor.shutdown();
    }
}
