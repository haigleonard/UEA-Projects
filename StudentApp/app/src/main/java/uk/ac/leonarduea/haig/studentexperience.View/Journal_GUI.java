package uk.ac.leonarduea.haig.studentexperience.View;

import android.content.Intent;
import android.support.v4.view.GestureDetectorCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.util.JsonWriter;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.ImageSwitcher;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.ViewSwitcher;

import com.example.dan.framework.Gestures;

import Controller.GUI_Controller;

public class Journal_GUI extends AppCompatActivity implements GestureDetector.OnContextClickListener, GestureDetector.OnGestureListener {

    ImageSwitcher imageSwitcher;
    TextView filler;
    Button back;
    GUI_Controller gui = new GUI_Controller();
    int i = 0;
    int picQ = R.drawable.qm;
    int pic0 = R.drawable.qr2;
    int pic1 = R.drawable.uealake;
    int pic2 = R.drawable.uealcr;
    int pic3 = R.drawable.sp;
    int pic4 = R.drawable.chall;
    int pic5 = R.drawable.lib;
    int pic6 = R.drawable.ueasq;
    int pic7 = R.drawable.joost;
    int pic8 = R.drawable.joost;
    int pic9 = R.drawable.village;
    int pic10 = R.drawable.zigg;
    int pic11 = R.drawable.shop;
    int pic12 = R.drawable.sains;
    Animation in, out, in2, out2;
    GestureDetectorCompat gestureDetector;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_journal);

        imageSwitcher = (ImageSwitcher) findViewById(R.id.switch2);
        imageSwitcher.setFactory(new ViewSwitcher.ViewFactory() {
            @Override
            public View makeView(){
                ImageView imageView = new ImageView(getApplicationContext());
                imageView.setScaleType(ImageView.ScaleType.CENTER_INSIDE);
                imageView.setLayoutParams(new ImageSwitcher.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));
                return imageView;
            }
        });

        in = AnimationUtils.loadAnimation(getApplicationContext(),R.anim.in);
        out = AnimationUtils.loadAnimation(getApplicationContext(),R.anim.out);
        in2 = AnimationUtils.loadAnimation(getApplicationContext(),R.anim.in2);
        out2 = AnimationUtils.loadAnimation(getApplicationContext(),R.anim.out2);

        imageSwitcher.setInAnimation(in);
        imageSwitcher.setOutAnimation(out);
        imageSwitcher.setImageResource(pic0);

        filler = (TextView) findViewById(R.id.filler);
        this.gestureDetector = new GestureDetectorCompat(this, this);

        back = (Button) findViewById(R.id.backBUT);
        back.setOnClickListener(new View.OnClickListener() {


            @Override
            public void onClick(View v){
                i = 0;
                Intent intent = new Intent("uk.ac.leonarduea.haig.studentexperience.View.qr");
                startActivity(intent);
            }
        });
    }

    @Override
    public boolean onContextClick(MotionEvent e) {
        return false;
    }

    @Override
    public boolean onDown(MotionEvent e) {
        return false;
    }

    @Override
    public void onShowPress(MotionEvent e) {

    }

    @Override
    public boolean onSingleTapUp(MotionEvent e) {
        return false;
    }

    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
        return false;
    }

    @Override
    public void onLongPress(MotionEvent e) {

    }

    @Override
    public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
        Gestures ges = new Gestures();
        boolean temp1 = ges.onFling(e1, e2, velocityX, velocityY, 0, 2000);

        if (!temp1) {
            imageSwitcher.setInAnimation(in);
            imageSwitcher.setOutAnimation(out);
            i--;
            if(i == -1){i = 11;}
            if(i==0) {
                imageSwitcher.setImageResource(pic1);
                filler.setText(gui.getQR(0));
            }if(i == 1){if(gui.userLocs().equals(null)){question();}else{
                imageSwitcher.setImageResource(pic2);
                filler.setText(gui.getQR(1));}
            }if(i == 2){imageSwitcher.setImageResource(pic3);
                filler.setText(gui.getQR(2));}
            if(i==3) {
                imageSwitcher.setImageResource(pic4);
                filler.setText(gui.getQR(3));
            }if(i == 4){
                imageSwitcher.setImageResource(pic5);
                filler.setText(gui.getQR(4));
            }if(i == 5){imageSwitcher.setImageResource(pic6);
                filler.setText(gui.getQR(5));}
            if(i==6) {
                imageSwitcher.setImageResource(pic7);
                filler.setText(gui.getQR(6));
            }if(i == 7){
                imageSwitcher.setImageResource(pic8);
                filler.setText(gui.getQR(7));
            }if(i == 8){imageSwitcher.setImageResource(pic9);
                filler.setText(gui.getQR(8));}
            if(i==9) {
                imageSwitcher.setImageResource(pic10);
                filler.setText(gui.getQR(9));
            }if(i == 10){
                imageSwitcher.setImageResource(pic11);
                filler.setText(gui.getQR(10));
            }if(i == 11){imageSwitcher.setImageResource(pic12);
                filler.setText(gui.getQR(11));}

        } else {
            imageSwitcher.setInAnimation(in2);
            imageSwitcher.setOutAnimation(out2);
            i++;
            if(i == 12){i = 0;}
            if(i==0) {
                imageSwitcher.setImageResource(pic1);
                filler.setText(gui.getQR(0));
            }if(i == 1){
                imageSwitcher.setImageResource(pic2);
                filler.setText(gui.getQR(1));
            }if(i == 2){imageSwitcher.setImageResource(pic3);filler.setText(gui.getQR(2));
            }
            if(i==3) {
                imageSwitcher.setImageResource(pic4);
                filler.setText(gui.getQR(3));
            }if(i == 4){
                imageSwitcher.setImageResource(pic5);
                filler.setText(gui.getQR(4));
            }if(i == 5){imageSwitcher.setImageResource(pic6);filler.setText(gui.getQR(5));
            }
            if(i==6) {
                imageSwitcher.setImageResource(pic7);
                filler.setText(gui.getQR(6));
            }if(i == 7){
                imageSwitcher.setImageResource(pic8);
                filler.setText(gui.getQR(7));
            }if(i == 8){imageSwitcher.setImageResource(pic9);filler.setText(gui.getQR(8));
            }if(i == 9){
                imageSwitcher.setImageResource(pic10);
                filler.setText(gui.getQR(9));
            }if(i == 10){
                imageSwitcher.setImageResource(pic11);
                filler.setText(gui.getQR(10));
            }if(i == 11){
                imageSwitcher.setImageResource(pic12);
                filler.setText(gui.getQR(11));
            }
        }
    return false;
    }
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        this.gestureDetector.onTouchEvent(event);
        return super.onTouchEvent(event);
    }
    public void question() {
        imageSwitcher.setImageResource(picQ);
        filler.setText("Locked");
    }


}
