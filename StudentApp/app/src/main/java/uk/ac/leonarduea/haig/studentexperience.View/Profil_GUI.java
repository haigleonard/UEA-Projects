package uk.ac.leonarduea.haig.studentexperience.View;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;
import java.text.SimpleDateFormat;
import java.util.Date;
import Controller.GUI_Controller;
import Model.Profile;


public class Profil_GUI extends Activity implements View.OnClickListener {
    EditText user_name;
    Button back;
    Button Grad;
    Button leader;

    String userNamee;
    static ProgressBar mProgess;
    Profile profile = new Profile();
    GUI_Controller gui = new GUI_Controller();
    SimpleDateFormat sdf = new SimpleDateFormat("HH");
    String currentDateandTime = sdf.format(new Date());
    int Hour = Integer.parseInt(currentDateandTime);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_profil__gui);

        TextView userName = (TextView) findViewById(R.id.userName);
        TextView percent = (TextView) findViewById(R.id.degreePercent);
        TextView degreeclass = (TextView) findViewById(R.id.degreeClass);
        TextView location1 = (TextView) findViewById(R.id.location1);
        TextView location2 = (TextView) findViewById(R.id.location2);
        TextView location3 = (TextView) findViewById(R.id.location3);
        TextView location4 = (TextView) findViewById(R.id.location4);
        TextView location5 = (TextView) findViewById(R.id.location5);
        TextView location6 = (TextView) findViewById(R.id.location6);
        TextView location7 = (TextView) findViewById(R.id.location7);
        TextView location8 = (TextView) findViewById(R.id.location8);
        TextView location9 = (TextView) findViewById(R.id.location9);
        TextView location10 = (TextView) findViewById(R.id.location10);
        TextView location11 = (TextView) findViewById(R.id.location11);
        TextView location12 = (TextView) findViewById(R.id.location12);

        back = (Button) findViewById(R.id.back);
        Grad = (Button) findViewById(R.id.grad);
        leader = (Button) findViewById(R.id.leader);
        //findGPS getter = new findGPS();
        //System.out.println(getter.longitude);
        //System.out.println(getter.latitude);
        back.setOnClickListener(this);
        Grad.setOnClickListener(this);
        leader.setOnClickListener(this);

        String username = gui.getusername();
        userName.setText(username);

        String percentage = String.valueOf(gui.getPercent());
        percent.setText(percentage);

        String classification = String.valueOf(gui.getclass());
        degreeclass.setText(classification);
        mProgess = (ProgressBar) findViewById(R.id.progressBar2);
        mProgess.setProgress(gui.getPercent());

        String Loc [] = gui.getLoc();
        location1.setText(Loc[0]);
        location2.setText(Loc[1]);
        location3.setText(Loc[2]);
        location4.setText(Loc[3]);
        location5.setText(Loc[4]);
        location6.setText(Loc[5]);
        location7.setText(Loc[6]);
        location8.setText(Loc[7]);
        location9.setText(Loc[8]);
        location10.setText(Loc[9]);
        location11.setText(Loc[10]);
        location12.setText(Loc[11]);
    }

   @Override
    public void onClick(View v) {
       switch (v.getId()) {
           case R.id.back:
               Intent intent = new Intent("uk.ac.leonarduea.haig.studentexperience.View.Home");
               startActivity(intent);
               break;
           case R.id.grad:
               if (70 <= profile.getPercent() || Hour > 10) {
                   intent = new Intent("uk.ac.leonarduea.haig.studentexperience.View.Quiz_GUI");
                   startActivity(intent);
               } else {
                   Toast.makeText(this," its not 5pm yet or you don't have over 70%", Toast.LENGTH_LONG).show();
               }
               break;
           case R.id.leader:
               intent = new Intent("uk.ac.leonarduea.haig.studentexperience.View.LeaderBoardData");
               startActivity(intent);
               break;
       }
   }
}

