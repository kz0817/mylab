import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class CompletableFutureStudy {

    private final ExecutorService executor = Executors.newWorkStealingPool();
    boolean doThrow1 = false;
    boolean doThrow2 = false;

    static void print(final String msg) {
        System.out.format("[%d] %s\n", Thread.currentThread().getId(), msg);
    }


    public void run() {
        print("run:1");
        CompletableFuture.supplyAsync(() -> {
            print("CF:1");
            if (this.doThrow1) {
                throw new RuntimeException("Error in CF1");
            }
            return "Stage1";
        }, this.executor)
        .thenApply((String msg) -> {
            print("CF:2, received: " + msg);
            if (this.doThrow2) {
                throw new RuntimeException("Error in CF2");
            }
            return "Stage2";
        })
        .whenComplete((String msg, Throwable e) -> {
            if (e != null) {
                print("Thrown: " + e.toString());
            } else {
                print("Complete: " + msg);
            }
        });
    }

    static void sleep() {
        try {
            Thread.sleep(500);
        } catch (InterruptedException e) {
            print(e.toString());
        }
    }

    public static void main(String[] args) {
        print("main:1");
        var s = new CompletableFutureStudy();
        s.run();
        sleep();
        print("---");

        print("main:2");
        var s2 = new CompletableFutureStudy();
        s2.doThrow1 = true;
        s2.run();
        sleep();
        print("---");

        print("main:3");
        var s3 = new CompletableFutureStudy();
        s3.doThrow2 = true;
        s3.run();
        sleep();
        print("---");
    }
}
