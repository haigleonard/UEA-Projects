package com.example.dan.framework;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.content.Context;
import android.content.SharedPreferences;

public interface FileInOut {


    /**
     * reads the file that is passed in.
     *@param  file path sotred as a sting
     *@return a new external file path with the file
     *@throws IOException if can not be completed
     */
    public String read(String file, Context context) throws IOException;
    /**
     * writes to file.
     *@param  file path sotred as a sting
     *@return a new external file path with the file
     *@throws IOException if can not be completed
     */
    public String write(String file,Context context) throws IOException;
    /**
     * returns a open file from passed in file location.
     *@param  file path sotred as a sting
     *@return a new external file path with the file
     *@throws IOException if can not be completed
     */
    public String iniRead(Context context) throws IOException;
    /**
     * gets the defult prefrances with context.
     *@return a contect object of the prefered prefrances
     */
    public void write2(String username, Context context);


}

