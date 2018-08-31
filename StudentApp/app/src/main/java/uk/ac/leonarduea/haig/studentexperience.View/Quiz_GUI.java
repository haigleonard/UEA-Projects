package uk.ac.leonarduea.haig.studentexperience.View;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;
import android.widget.Toast;
import Controller.GUI_Controller;

public class Quiz_GUI extends AppCompatActivity {
    TextView Question, answer1,answer2,answer3;
    RadioButton A1,A2,A3;
    GUI_Controller gui = new GUI_Controller();
    String answer;
    Button Submit;
    RadioGroup rg;
    static int token = 0;
    static int token2 = -1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_quiz__gui);

        if(token2 == 1){Toast.makeText(this, "Correct", Toast.LENGTH_LONG).show();token2 = 0;} else if(token2 == 0){Toast.makeText(this, "Wrong", Toast.LENGTH_LONG).show();}

        rg = (RadioGroup) findViewById(R.id.answers);
        Submit = (Button) findViewById(R.id.sub);
        Question = (TextView) findViewById(R.id.questionid);
        answer1 = (RadioButton) findViewById(R.id.a1);
        answer2 = (RadioButton) findViewById(R.id.a2);
        answer3 = (RadioButton) findViewById(R.id.a3);
        A1 = (RadioButton) findViewById(R.id.a1);
        A2 = (RadioButton) findViewById(R.id.a2);
        A3 = (RadioButton) findViewById(R.id.a3);

        String temp = gui.getQuestion();
        String[] QA = temp.split("#");
        Question.setText(QA[0]);
        answer1.setText(QA[1]);
        answer2.setText(QA[2]);
        answer3.setText(QA[3]);
        answer = QA[4];


        Submit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(A1.isChecked()){if(1 == Integer.parseInt(answer)){gui.setPercent(); token2 = 1;}}
                if(A2.isChecked()){if(2 == Integer.parseInt(answer)){gui.setPercent();token2 = 1;}}
                if(A3.isChecked()){if(3 == Integer.parseInt(answer)){gui.setPercent();token2 = 1;}}
                if(token != 9) {
                    Intent intent = new Intent("uk.ac.leonarduea.haig.studentexperience.View.Quiz_GUI");
                    startActivity(intent);
                    token++;
                }else{
                    Camera_GUI cam = new Camera_GUI();
                    cam.getInstance();
                    Intent intent = new Intent("uk.ac.leonarduea.haig.studentexperience.View.Camera");
                    startActivity(intent);
                }
            }
        });
    }
    public void rbclick(View v){
        gui.setPercent();
    }
    @Override
    public void onBackPressed() {
        //super.onBackPressed();
    }
    public void reset() {token = 0;}
}

