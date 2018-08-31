package com.example.dan.framework;


import android.app.Activity;
import android.app.NotificationManager;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;


/**
 * Created by Dan Ling on 3/1/2016.
 */

public interface Notification {
    /**
     *
     * @param view
     * @param context
     * @param title
     * @param text
     */
    public void getnotification(View view, Context context,String title,String text );

}
