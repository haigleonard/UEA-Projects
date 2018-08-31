package uk.ac.leonarduea.haig.studentexperience.View;

import android.app.Activity;
import android.content.Intent;
import android.support.v4.view.GestureDetectorCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.ImageSwitcher;
import android.widget.ImageView;
import android.widget.ViewSwitcher;


import com.example.dan.framework.Gestures;

import Controller.GUI_Controller;
import com.example.dan.framework.setPermissions;

public class Language_GUI extends AppCompatActivity implements GestureDetector.OnContextClickListener, GestureDetector.OnGestureListener  {

    Button Confirm;
    ImageSwitcher imageSwitcher;
    int i = 0;
    int i1 = R.drawable.uk;
    int i2 = R.drawable.fr;
    int i3 = R.drawable.span;
    Animation in, out, in2, out2;
    GestureDetectorCompat gestureDetector;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_language);

        setPermissions per = new setPermissions();
         Activity activity =  this;
        per.showPermissionDialog(this, activity);

        this.gestureDetector = new GestureDetectorCompat(this, this);


        imageSwitcher = (ImageSwitcher) findViewById(R.id.switchpic);
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
        imageSwitcher.setImageResource(i1);
        Confirm = (Button) findViewById(R.id.confirm);


        Confirm.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v){
                    GUI_Controller gui = new GUI_Controller();
                    gui.setLang(i);
                    Intent intent = new Intent("uk.ac.leonarduea.haig.studentexperience.View.MainActivity");
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
        boolean temp =  ges.onFling(e1, e2, velocityX, velocityY, 1200, 2000);

                    if (!temp) {
                        imageSwitcher.setInAnimation(in);
                        imageSwitcher.setOutAnimation(out);
                        i--;
                        if(i == -1){i = 2;}
                        if(i==0) {
                            imageSwitcher.setImageResource(i1);
                        }if(i == 1){
                            imageSwitcher.setImageResource(i2);
                        }if(i == 2){imageSwitcher.setImageResource(i3);}

                    } else {
                        imageSwitcher.setInAnimation(in2);
                        imageSwitcher.setOutAnimation(out2);
                        i++;
                        if(i == 3){i = 0;}
                        if(i==0) {
                            imageSwitcher.setImageResource(i1);
                        }if(i == 1){
                            imageSwitcher.setImageResource(i2);
                        }if(i == 2){imageSwitcher.setImageResource(i3);
                        }
                    }
        return false;
    }


    @Override
    public boolean onTouchEvent(MotionEvent event) {
        this.gestureDetector.onTouchEvent(event);
        return super.onTouchEvent(event);
    }
}
