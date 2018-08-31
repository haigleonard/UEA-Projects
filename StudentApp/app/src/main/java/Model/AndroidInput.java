package Model;

import java.util.List;

import android.content.Context;
import android.os.Build.VERSION;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

import com.example.dan.framework.Input;

public class AndroidInput implements Input {    
    TouchHandler touchHandler;
    private static final int SWIPE_THRESHOLD = 100;
    private static final int SWIPE_VELOCITY_THRESHOLD = 100;
    /**
     * setting the touch handler basied on te sdk version
     * @param context
     * @param view
     * @param scaleX
     * @param scaleY
     */
    public AndroidInput(Context context, View view, float scaleX, float scaleY) {

    }

    /**
     * find out if touch is down
     * using a touch handler object
     * @param pointer
     * @return a point relating to a coordinate on the screen.
     */
    @Override
    public boolean isTouchDown(int pointer) {
        return touchHandler.isTouchDown(pointer);
    }

    /**
     * find out where touch happened for x coordinate
     * using a touch handler object
     * @param pointer
     * @return  a point relating to a coordinate on the screen.
     */
    @Override
    public int getTouchX(int pointer) {
        return touchHandler.getTouchX(pointer);
    }
    /**
     * find out where touch happened for y coordinate
     * using a touch handler object
     * @param pointer
     * @return  a point relating to a coordinate on the screen.
     */
    @Override
    public int getTouchY(int pointer) {
        return touchHandler.getTouchY(pointer);
    }

    /**
     * creates an array list of touch events
     * using a touch handler object
     * an array of touches
     * @return  a point relating to a coordinate on the screen.
     */

    @Override
    public List<TouchEvent> getTouchEvents() {
        return touchHandler.getTouchEvents();
    }

    @Override
    public boolean onContextClick(MotionEvent e) {
        return false;
    }
    @Override
    public boolean onDown(MotionEvent e) {
        return false;
    }
    @Override
    public void onShowPress(MotionEvent e) {

    }
    @Override
    public boolean onSingleTapUp(MotionEvent e) {
        return false;
    }
    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
        return false;
    }
    @Override
    public void onLongPress(MotionEvent e) {

    }
    @Override
    public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY, int Y1, int Y2) {
        boolean result = false;
        try {
            float diffY = e2.getY() - e1.getY();
            float diffX = e2.getX() - e1.getX();
            if (Math.abs(diffX) > Math.abs(diffY) && (e2.getY()) < Y2 && (e2.getY()) > Y1) {
                if (Math.abs(diffX) > SWIPE_THRESHOLD && Math.abs(velocityX) > SWIPE_VELOCITY_THRESHOLD) {
                    if (diffX > 0) {

                    } else {

                    }
                }
                result = true;
            }
            result = true;
        } catch (Exception exception) {
            exception.printStackTrace();
        }
        return result;
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        return false;
    }
    /*
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        this.gestureDetector.onTouchEvent(event);
        return super.onTouchEvent(event);
    }*/
    
}
