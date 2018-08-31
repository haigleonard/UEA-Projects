package uk.ac.leonarduea.haig.studentexperience.View;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;


public class Help_GUI extends Activity {
    private static Help_GUI instance = new Help_GUI();
    public Help_GUI(){}
    Button back;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_help);
        ImageView myimage = (ImageView) findViewById(R.id.qr1);
        myimage.setImageResource(R.drawable.qr2);

        back = (Button) findViewById(R.id.back);
        back.setOnClickListener(new View.OnClickListener() {


            @Override
            public void onClick(View v){
                Intent intent = new Intent("uk.ac.leonarduea.haig.studentexperience.View.Home");
                startActivity(intent);
            }
        });
    }
    public static Help_GUI getInstance(){
        return instance;
    }
}
