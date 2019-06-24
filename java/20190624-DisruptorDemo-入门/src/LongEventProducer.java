import com.lmax.disruptor.RingBuffer;
import com.lmax.disruptor.dsl.Disruptor;

import java.nio.ByteBuffer;
import java.util.Random;

/**
 * @author : arbboter@gamil.com
 * @version : $
 * @date : created in 2019-06-24 10:30
 * @description: 生产者
 * @modified By:
 */
public class LongEventProducer {
    public void produce(Disruptor<LongEvent> disruptor){
        // 发布事件；
        RingBuffer<LongEvent> ringBuffer = disruptor.getRingBuffer();
        long sequence = ringBuffer.next();              // 请求下一个事件序号

        LongEvent event = ringBuffer.get(sequence); // 获取该序号对应的事件对象
        try {
            Random random = new Random();
            event.set(random.nextInt());                // 设置业务数据
        } finally{
            ringBuffer.publish(sequence);               // 发布事件
            System.out.println(sequence + "-生产者完成:" + event);
        }
    }
}
