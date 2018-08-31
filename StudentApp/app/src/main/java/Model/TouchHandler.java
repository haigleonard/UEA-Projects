package Model;

import java.util.List;

import android.view.MotionEvent;
import android.view.View.OnTouchListener;

import com.example.dan.framework.Input.TouchEvent;

public interface TouchHandler extends OnTouchListener {

    /**
     * find out if touch is down
     * using a touch handler object
     * @param pointer
     * @return a point relating to a coordinate on the screen.
     */
    public boolean isTouchDown(int pointer);
    /**
     * find out where touch happened for x coordinate
     * using a touch handler object
     * @param pointer
     * @return  a point relating to a coordinate on the screen.
     */
    public int getTouchX(int pointer);
    /**
     * find out where touch happened for y coordinate
     * using a touch handler object
     * @param pointer
     * @return  a point relating to a coordinate on the screen.
     */
    public int getTouchY(int pointer);
    /**
     * creates an array list of touch events
     * using a touch handler object
     * an array of touches
     * @return  a point relating to a coordinate on the screen.
     */
    public List<TouchEvent> getTouchEvents();
    public boolean onContextClick(MotionEvent e);
    public boolean onDown(MotionEvent e);
    public void onShowPress(MotionEvent e);
    public boolean onSingleTapUp(MotionEvent e);
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY);
    public void onLongPress(MotionEvent e);
    public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY);
    public boolean onTouchEvent(MotionEvent event);
}
