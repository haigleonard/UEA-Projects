package com.example.dan.framework;

import android.app.Activity;
import android.content.Context;

/**
 * Created by dan on 23/01/2017.
 */

public interface Permissions {
    public void showPermissionDialog(Context ctext, Activity activity);

    public  boolean checkPermission(final Context context);

}
