package uk.ac.leonarduea.haig.studentexperience.View;

import android.app.Activity;
import android.content.Intent;
import android.os.Environment;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;

import com.example.dan.framework.French;
import com.example.dan.framework.Language;
import com.example.dan.framework.Spanish;

import java.io.File;
import Controller.GUI_Controller;
import Model.FileIO;

public class MainActivity_GUI extends Activity  {
    Button start;
    EditText username;
    TextView test;
    TextView text;
    TextView language;
    String user;
    String temp;
    public static GUI_Controller gui = new GUI_Controller();
    French fren = new French();
    Spanish span = new Spanish();
    public String path = Environment.getExternalStorageDirectory().getAbsolutePath() + "/user";
    FileIO file = new FileIO();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        File dir = new File(path);
        dir.mkdirs();
        start = (Button) findViewById(R.id.start_button);
        username = (EditText) findViewById(R.id.user_name);
        test = (TextView) findViewById(R.id.test);
        text = (TextView) findViewById(R.id.textView3);
        ImageView myimage = (ImageView) findViewById(R.id.uea1);
        myimage.setImageResource(R.drawable.uea1);
        user = gui.iniRead(getApplicationContext());
        int lang = gui.getLang();
        if  (lang == 1){
            language = (TextView) findViewById(R.id.text1);
            temp = gui.getLang(0, lang);
            language.setText(temp);
        }
        if  (lang == 2){
            language = (TextView) findViewById(R.id.text1);
            temp = gui.getLang(0, lang);
            language.setText(temp);
        }
        if (user != null) {
            String[] parts = user.split("#");
            user = parts[0];
            if(lang == 0){
            test.setText("Welcome Back " + user + "!");}
            if(lang == 1){
                temp = gui.getLang(1, lang);
                test.setText(temp + user + "!");}
            if(lang == 2){
                temp = gui.getLang(1, lang);
                test.setText(temp + user + "!");}
            username.setVisibility(View.GONE);
            text.setVisibility(View.GONE);
        }
        start.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v){
                String temp = username.getText().toString();
                String[] parts = temp.split("#");
                if(parts.length >= 1) {
                    temp = parts[0];
                }
                gui.setUser(temp, getApplicationContext());
                Intent intent = new Intent("uk.ac.leonarduea.haig.studentexperience.View.Home");
                startActivity(intent);
            }
        });
    }








}

