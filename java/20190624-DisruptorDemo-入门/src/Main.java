import com.lmax.disruptor.EventFactory;
import com.lmax.disruptor.EventHandler;
import com.lmax.disruptor.YieldingWaitStrategy;
import com.lmax.disruptor.dsl.Disruptor;
import com.lmax.disruptor.dsl.ProducerType;

import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadFactory;

public class Main {

    public static void main(String[] args) throws Exception  {
        // 创建disruptor时，需要预先定义其事件、事件工厂、时间处理对象(消费者)、线程池
        EventFactory<LongEvent> eventFactory = new LongEventFactory();
        int ringBufferSize = 1024 * 1024; // RingBuffer 大小，必须是 2 的 N 次方；

        Disruptor<LongEvent> disruptor = null;

        if(false){
            // 1.线程池-ExecutorService(已被废弃，不建议使用)
            ExecutorService executor = Executors.newSingleThreadExecutor();
            disruptor = new Disruptor<LongEvent>(eventFactory,
                    ringBufferSize, executor, ProducerType.SINGLE,
                    new YieldingWaitStrategy());
        }else {
            // 2.线程池-ThreadFactory
            ThreadFactory threadFactory = Executors.defaultThreadFactory();
            disruptor = new Disruptor<LongEvent>(eventFactory,
                    ringBufferSize, threadFactory, ProducerType.SINGLE,
                    new YieldingWaitStrategy());
        }

        EventHandler<LongEvent> eventHandler = new LongEventHandler();
        disruptor.handleEventsWith(eventHandler);
        disruptor.start();  // 异步调用，非阻塞
        System.out.println("disruptor 启动完成");

        // 定义生产者
        LongEventProducer longEventProducer = new LongEventProducer();
        Random random = new Random();
        int num = 0;
        while (true){
            longEventProducer.produce(disruptor);
            Thread.sleep(random.nextInt(10));
            if(++num%1000 == 0){
                Thread.sleep(10000);
            }
        }
    }
}
