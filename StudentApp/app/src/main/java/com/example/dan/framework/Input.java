package com.example.dan.framework;

import android.view.MotionEvent;

import java.util.List;

public interface Input {
    /**
     * setting values for touch down, touch up, touch dragged and touch hold
     * making variables of type in for type, x, y and pointer
     */
    public static class TouchEvent {

        public static final int TOUCH_DOWN = 0;
        public static final int TOUCH_UP = 1;
        public static final int TOUCH_DRAGGED = 2;
        public static final int TOUCH_HOLD = 3;

        public int type;
        public int x, y;
        public int pointer;


    }

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
    public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY, int Y1, int Y2);
    public boolean onTouchEvent(MotionEvent event);
}
