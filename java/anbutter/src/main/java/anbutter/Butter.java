package anbutter;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import java.util.concurrent.BlockingQueue;

class Butter extends Thread {

    int count = 0;
    int sum = 0;
    private static final Logger logger = LogManager.getLogger();
    private BlockingQueue<Integer> q;

    Butter(BlockingQueue<Integer> _q) {
        logger.info("New instanc: Butter");
        q = _q;
    }

    @Override
    public void run() {
        count = 0;
        sum = 0;
        while (true) {
            try {
                final var v = q.take();
                if (v < 0)
                    break;
                count++;
                sum += v;
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        logger.info(String.format("count: %d, sum: %d", count, sum));
    }
}

