import com.lmax.disruptor.EventFactory;

/**
 * @author : arbboter@gamil.com
 * @version : 1.0
 * @date : created in 2019-06-24 10:15
 * @description: 事件工厂类
 * @modified By:
 */
public class LongEventFactory implements EventFactory<LongEvent> {
    public LongEvent newInstance()
    {
        return new LongEvent();
    }
}
