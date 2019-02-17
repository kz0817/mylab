package anbutter;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import java.util.concurrent.BlockingQueue;

class An extends Thread {

    private static final Logger logger = LogManager.getLogger();

    private BlockingQueue<Integer> q;
    private int numLoops = 0;

    An(BlockingQueue<Integer> _q, int n) {
        logger.info("New instanc: An");
        q = _q;
        numLoops = n;
    }

    @Override
    public void run() {
        for (int i = 0; i < numLoops; i++) {
            try {
                int v = i;
                q.put(v);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        q.add(-1);
    }
}
