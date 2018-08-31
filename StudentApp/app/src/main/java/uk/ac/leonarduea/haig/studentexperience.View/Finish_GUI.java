package uk.ac.leonarduea.haig.studentexperience.View;

import android.content.Context;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.example.dan.framework.FileInOut;

import java.io.IOException;

import Controller.GUI_Controller;

public class Finish_GUI extends AppCompatActivity {
Button done;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_finish);
        GUI_Controller gui = new GUI_Controller();
        gui.clear(getApplicationContext());
        done = (Button) findViewById(R.id.homebutton);
        done.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v){
                Intent intent = new Intent("uk.ac.leonarduea.haig.studentexperience.View.Home");
                startActivity(intent);
            }
        });
        }
    }

