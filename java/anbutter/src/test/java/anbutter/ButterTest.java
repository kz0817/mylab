package anbutter;

import static org.junit.Assert.*;
import org.junit.Test;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.stream.IntStream;
import java.util.stream.Collectors;
import java.util.ArrayList;

public class ButterTest {

    private void pushRangeValue(BlockingQueue q, int max)
            throws InterruptedException {
        var v_list = new ArrayList<Integer>(max);
        IntStream.rangeClosed(1, max).forEach(v -> v_list.add(v));
        for (int v : v_list) {
            q.put(v);
        }
    }

    @Test
    public void testInitialValue() {
        var q = new LinkedBlockingQueue<Integer>();
        var butter = new Butter(q);
        assertEquals(0, butter.count);
        assertEquals(0, butter.sum);
    }

    @Test
    public void testSum() throws InterruptedException {
        var q = new LinkedBlockingQueue<Integer>();
        var butter = new Butter(q);
        butter.start();
        try {
            pushRangeValue(q, 10);
            q.put(-1);
            butter.join();
        } catch (InterruptedException e) {
            throw e;
        }
        assertEquals(10, butter.count);
        assertEquals(55, butter.sum);
    }
}
