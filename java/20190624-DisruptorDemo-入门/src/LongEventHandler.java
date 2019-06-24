import java.util.Random;
import com.lmax.disruptor.EventHandler;

/**
 * @author : arbboter@gamil.com
 * @version : 1.0$
 * @date : created in 2019-06-24 10:21
 * @description: 事件处理类
 * @modified By:
 */
public class LongEventHandler implements EventHandler<LongEvent> {
    public void onEvent(LongEvent event, long sequence, boolean endOfBatch) {
        try {
            Random random = new Random();
            Thread.sleep(random.nextInt(1000));
            System.out.println(sequence + "-消费者完成-Event: " + event.toString());
        }catch (InterruptedException e){
            System.out.println(sequence + "-异常中止-Event: " + event.toString());
        }
    }
}
