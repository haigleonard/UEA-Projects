package uk.ac.leonarduea.haig.studentexperience.View;

import android.app.Activity;

import android.os.Bundle;

import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Random;

import Controller.GUI_Controller;
import Controller.ModelController;
import Model.Locations;
import Model.Notifiy;
import Model.Profile;

/**
 * Created by dan on 9/1/2017.
 */

public class Hints_GUI extends Activity {
    Button btn;
    TextView tw;
    ImageView iv;

    Notifiy notifiyClass = new Notifiy();
    GUI_Controller gui = new GUI_Controller();
    Random rand = new Random();

    static String temp[][] = new String[6][2];
    static String temp2[] = new String[6];
    String clue;
    String notiTitle = "Heres your Hint";
    int count;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_hints);
        btn = (Button) findViewById(R.id.btnn);
        iv = (ImageView) findViewById(R.id.qmark);
        count = gui.getHint();
        tw = (TextView) findViewById(R.id.hintsleft);
        if (count > 0) {
            tw.setText(" You have " + count + " hints left");
        } else{
            tw.setText(" You have no hints left");
    }
        temp2 = gui.userLocs();
        clue = "found";
        for (int i = 0; i < 6; i++) {
            temp[i][0] = temp2[i];
            temp[i][1] = gui.getLoc(i);
        }
    }
    public void onClick(View v) {
    boolean newHint = false;
        if ( count > 0){
            gui.updateHint(getApplicationContext());
            while(newHint == false){
                int n = rand.nextInt(6);
                if (temp[n][0].equals("?")) {
                    clue = temp[n][1];
                    temp[n][0]=temp[n][1];
                    newHint = true;
                }
            }
            switch (clue) {
                case "Lake":
                    Toast.makeText(this, "Hint sent as a push notification", Toast.LENGTH_LONG).show();
                    notifiyClass.getnotification(v, getApplicationContext(), notiTitle, "Its big and blue and filled with water");
                    break;
                case "LCR":
                    Toast.makeText(this, "Hint sent as a push notification", Toast.LENGTH_LONG).show();
                    notifiyClass.getnotification(v, getApplicationContext(), notiTitle, "Really cheap drinks and loud music");
                    break;
                case "Library":
                    Toast.makeText(this, "Hint sent as a push notification", Toast.LENGTH_LONG).show();
                    notifiyClass.getnotification(v, getApplicationContext(), notiTitle, "If you want a book you should go here");
                    break;
                case "Congregation Hall":
                    Toast.makeText(this, "Hint sent as a push notification", Toast.LENGTH_LONG).show();
                    notifiyClass.getnotification(v, getApplicationContext(), notiTitle, "The last part it is name rhythms with ball  ");
                    break;
                case "Sports Park":
                    Toast.makeText(this, "Hint sent as a push notification", Toast.LENGTH_LONG).show();
                    notifiyClass.getnotification(v, getApplicationContext(), notiTitle, "Go here to exercise ");
                    break;
                case "Square":
                    Toast.makeText(this, "Hint sent as a push notification", Toast.LENGTH_LONG).show();
                    notifiyClass.getnotification(v, getApplicationContext(), notiTitle, "Its named after a shape");
                    break;
                case "found":
                    Toast.makeText(this, "No hints left, all codes found", Toast.LENGTH_LONG).show();
                    break;
            }
        }
        else{
            Toast.makeText(this, "No have no hints left", Toast.LENGTH_LONG).show();
        }
        count--;
        if (count > 0) {
            tw.setText(" You have " + count + " hints left");
        } else{
            tw.setText(" You have no hints left");
        }
    }
}
