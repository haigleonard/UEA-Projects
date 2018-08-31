package Model;

import android.app.Activity;
import android.content.Context;
import android.os.Environment;
import android.util.Log;

import com.example.dan.framework.FileInOut;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;

import Controller.ModelController;

/**
 * Created by Haig on 1/3/2017.
 */

public class FileIO extends Activity implements FileInOut{
    public String path = Environment.getExternalStorageDirectory().getAbsolutePath() + "/user";
    ModelController add = new ModelController();

    public FileIO() {
        File dir = new File(path);
        dir.mkdirs();
    }
    public String write(String username, Context context){
        String Message = username+"#40"+"#Third"+"#3"+"#?"+"#?"+"#?"+"#?"+"#?"+"#?"+"#?"+"#?"+"#?"+"#?"+"#?"+"#?";
        String file_name = "hello_file";
        String[] parts = Message.split("#");
        String part1 = parts[0];
        String part2 = parts[1];
        Log.d("name ", part1);
        Log.d("percentage ", part2);


        add.addData(part1, part2);


    try {
        FileOutputStream fileOutputStream = context.openFileOutput(file_name, MODE_PRIVATE);
        Log.d("WTF ", username);
        fileOutputStream.write(Message.getBytes());
        fileOutputStream.close();
        return Message;
    } catch (FileNotFoundException e) {
        e.printStackTrace();
        return "FILE NOT FOUND";
    } catch (IOException e) {
        e.printStackTrace();
        return "ERROR";
    }
    }
    public String read(String username, Context context){
        String user;
        try {
            String Message;
            FileInputStream fileInputStream = context.openFileInput("hello_file");
            InputStreamReader inputStreamReader = new InputStreamReader(fileInputStream);
            BufferedReader bufferedReader = new BufferedReader(inputStreamReader);
            StringBuffer stringBuffer = new StringBuffer();
            while((Message = bufferedReader.readLine()) != null){
                stringBuffer.append(Message + "\n");
            }
            user=stringBuffer.toString();
            return user;
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return write(username, context);
        } catch (IOException e) {
            e.printStackTrace();
            return "ERROR";
        }
    }
    public String iniRead(Context context){
        try {
            String Message;
            FileInputStream fileInputStream = context.openFileInput("hello_file");
            InputStreamReader inputStreamReader = new InputStreamReader(fileInputStream);
            BufferedReader bufferedReader = new BufferedReader(inputStreamReader);
            StringBuffer stringBuffer = new StringBuffer();
            while((Message = bufferedReader.readLine()) != null){
                stringBuffer.append(Message + "\n");
            }
            return stringBuffer.toString();

        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return null;
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }
    public void write2(String username, Context context){
        String Message = username;
        String[] parts = Message.split("#");
        String part1 = parts[0];
        String part2 = parts[1];
        Log.d("name ", part1);
        Log.d("percentage ", part2);

        add.setID(part1, part2);

        String file_name = "hello_file";
        try {
            FileOutputStream fileOutputStream = context.openFileOutput(file_name, MODE_PRIVATE);
            fileOutputStream.write(Message.getBytes());
            fileOutputStream.close();

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
