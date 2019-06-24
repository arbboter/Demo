/**
 * @author : arbboter@gamil.com
 * @version : 1.0$
 * @date : created in 2019-06-24 10:14
 * @description: 事件实体
 * @modified By:
 */
public class LongEvent {
    private long value;

    public void set(long value)
    {
        this.value = value;
    }

    @Override
    public String toString() {
        return "LongEvent{" +
                "value=" + value +
                '}';
    }
}
