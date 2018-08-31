package uk.ac.leonarduea.haig.studentexperience.View;

import android.app.Activity;
import android.content.Intent;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;
import com.google.zxing.integration.android.IntentIntegrator;
import com.google.zxing.integration.android.IntentResult;
import Controller.GUI_Controller;
// Reference: https://www.youtube.com/watch?v=Fe7F4Jx7rwo
public class QR_GUI extends Activity implements View.OnClickListener {
    Button scan_btn;
    Button back, journal;
    ImageView myimage;
    TextView header;
    TextView filler;
    final Activity activity = this;
    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_qr);

        scan_btn = (Button) findViewById(R.id.scan_btn);
        back = (Button) findViewById(R.id.back);
        header = (TextView) findViewById(R.id.header) ;
        filler = (TextView) findViewById(R.id.filler) ;
        journal = (Button) findViewById(R.id.jour);

        ImageView myimage = (ImageView) findViewById(R.id.qrid);
        myimage.setImageResource(R.drawable.qr2);

        scan_btn.setOnClickListener(this);
        back.setOnClickListener(this);
        journal.setOnClickListener(this);

        header.setText("QR Codes");
        filler.setText("Think you have seen me click the scan button and see what happens.");

        }
    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.scan_btn:
                IntentIntegrator integrator = new IntentIntegrator(activity);
                integrator.setDesiredBarcodeFormats(IntentIntegrator.QR_CODE_TYPES);
                integrator.setPrompt("Scan");
                integrator.setCameraId(0);
                integrator.setBeepEnabled(false);
                integrator.setBarcodeImageEnabled(false);
                integrator.initiateScan();
                break;
            case R.id.back:
                Intent intent = new Intent("uk.ac.leonarduea.haig.studentexperience.View.Home");
                startActivity(intent);
                break;
            case R.id.jour:

                intent = new Intent("uk.ac.leonarduea.haig.studentexperience.View.Journal");
                startActivity(intent);
                break;

        }
    }
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        IntentResult result = IntentIntegrator.parseActivityResult(requestCode, resultCode,data);
        ImageView myimage = (ImageView) findViewById(R.id.qrid);
        final MediaPlayer correct = MediaPlayer.create(this, R.raw.correct);
        final MediaPlayer wrong = MediaPlayer.create(this, R.raw.wrong);
        if(result != null){
            if(result.getContents()==null){Toast.makeText(this, "You Cancelled the Scan", Toast.LENGTH_LONG).show();
            }else{
                String temp = result.getContents();
                GUI_Controller gui = new GUI_Controller();
                gui.checkLocation(temp, getApplicationContext());

                switch (result.getContents()) {
                    case "Lake":
                        myimage.setImageResource(R.drawable.uealake);
                        header.setText("Welcome to the "+result.getContents());
                        filler.setText(gui.getQR(0));
                        correct.start();
                        break;
                    case "LCR":
                        myimage.setImageResource(R.drawable.uealcr);
                        header.setText("Welcome to the "+result.getContents());
                        filler.setText(gui.getQR(1));
                        correct.start();
                        break;
                    case "Sports Park":
                        myimage.setImageResource(R.drawable.sp);
                        header.setText("Welcome to the "+result.getContents());
                        filler.setText(gui.getQR(2));
                        correct.start();
                        break;
                    case "Congregation Hall":
                        myimage.setImageResource(R.drawable.chall);
                        header.setText("Welcome to the "+result.getContents());
                        filler.setText(gui.getQR(3));
                        correct.start();
                        break;
                    case "Library":
                        myimage.setImageResource(R.drawable.lib);
                        header.setText("Welcome to the"+result.getContents());
                        filler.setText(gui.getQR(4));
                        correct.start();
                        break;
                    case "Square":
                        myimage.setImageResource(R.drawable.ueasq);
                        header.setText("Welcome to the "+result.getContents());
                        filler.setText(gui.getQR(5));
                        correct.start();
                        break;
                    case "Joost":
                        myimage.setImageResource(R.drawable.joost);
                        header.setText("Welcome to the "+result.getContents());
                        filler.setMovementMethod(new ScrollingMovementMethod());
                        filler.setText(gui.getQR(6));
                        correct.start();
                        break;
                    case "SE2":
                        myimage.setImageResource(R.drawable.ueasq);
                        header.setText("Welcome to the "+result.getContents());
                        filler.setText(gui.getQR(7));
                        correct.start();
                        break;
                    case "The Village":
                        myimage.setImageResource(R.drawable.lib);
                        header.setText("Welcome to the"+result.getContents());
                        filler.setText(gui.getQR(8));
                        correct.start();
                        break;
                    case "The Ziggurats":
                        myimage.setImageResource(R.drawable.ueasq);
                        header.setText("Welcome to the "+result.getContents());
                        filler.setText(gui.getQR(9));
                        correct.start();
                        break;
                    case "Union Shop":
                        myimage.setImageResource(R.drawable.joost);
                        header.setText("Welcome to the "+result.getContents());
                        filler.setMovementMethod(new ScrollingMovementMethod());
                        filler.setText(gui.getQR(10));
                        correct.start();
                        break;
                    case "Sainsbury Centre":
                        myimage.setImageResource(R.drawable.ueasq);
                        header.setText("Welcome to the "+result.getContents());
                        filler.setText(gui.getQR(11));
                        correct.start();
                        break;
                    default: wrong.start(); Toast.makeText(this, "Not a UEA QR Code", Toast.LENGTH_LONG).show();
                }
            }
        }else{super.onActivityResult(requestCode, resultCode, data);}
    }
}
