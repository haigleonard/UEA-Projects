package uk.ac.leonarduea.haig.studentexperience.View;


import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;

import Controller.ModelController;
import Model.Profile;


import static uk.ac.leonarduea.haig.studentexperience.View.R.id.hint;


public class Home_GUI extends Activity implements View.OnClickListener {
    Button profile;
    Button QR;
    Button Help;
    Button Hint;
    TextView Fact;
    TextView Percentage2;
    static ProgressBar mProgess1;
    ModelController model = new ModelController();

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_home);
        Profile pro = new Profile();

        profile = (Button) findViewById(R.id.profile);
        QR = (Button) findViewById(R.id.qr);
        Hint = (Button) findViewById(hint);
        Help = (Button) findViewById(R.id.help);
        Fact = (TextView) findViewById(R.id.fact);
        mProgess1 = (ProgressBar) findViewById(R.id.progressBar);
        mProgess1.setProgress(pro.getPercent());

        //findGPS gps = new findGPS();
        //gps.getLoc(this);
        String ranFact = model.returnFact();
        Fact.setText("Fun Fact : "+ranFact);

        Percentage2 = (TextView) findViewById(R.id.perentage2);
        String percentage = String.valueOf(pro.getPercent());
        Percentage2.setText(percentage+"%");

        profile.setOnClickListener(this);
        QR.setOnClickListener(this);
        Help.setOnClickListener(this);
        Hint.setOnClickListener(this);

        ImageView myimage2 = (ImageView) findViewById(R.id.map);
        myimage2.setImageResource(R.drawable.map1);





    }
    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.profile:
                Intent intent = new Intent("uk.ac.leonarduea.haig.studentexperience.View.Profil_GUI");
                startActivity(intent);
                break;
            case R.id.qr:
                intent = new Intent("uk.ac.leonarduea.haig.studentexperience.View.qr");
                startActivity(intent);
                break;
            case R.id.help:
                Help_GUI help = new Help_GUI();
                help.getInstance();
                intent = new Intent("uk.ac.leonarduea.haig.studentexperience.View.Help");
                startActivity(intent);
                break;
            case hint:
                intent = new Intent("uk.ac.leonarduea.haig.studentexperience.View.Hints");
                startActivity(intent);
                break;
        }
    }







}

