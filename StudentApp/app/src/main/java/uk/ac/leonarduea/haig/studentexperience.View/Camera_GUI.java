package uk.ac.leonarduea.haig.studentexperience.View;
import android.app.Activity;
import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.hardware.Camera;
import android.net.Uri;
import android.provider.MediaStore;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import java.io.File;

import Controller.GUI_Controller;

// Rferences: https://www.youtube.com/watch?v=je9bdkdNQqg
public class Camera_GUI extends Activity implements View.OnClickListener {
    private static Camera_GUI instance = new Camera_GUI();
    public Camera_GUI(){}
    Button button;
    Button button2;
    ImageView imageView;
    TextView fin;
    static final int CAM_REQUEST = 1;
    GUI_Controller gui = new GUI_Controller();
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_camera);
        button = (Button) findViewById(R.id.cameraButton);
        button2 = (Button) findViewById(R.id.post);
        imageView = (ImageView) findViewById(R.id.image);
        fin = (TextView) findViewById(R.id.finalScore);
        fin.setText("Well done on completing the quiz your scored " + gui.getclass());
        button.setOnClickListener(this);
        button2.setOnClickListener(this);
        }
        @Override
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.cameraButton:
                    Intent camera_intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
                    File file = getFile();
                    camera_intent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(file));
                    startActivityForResult(camera_intent, CAM_REQUEST);
                    break;
                case R.id.post:
                    Intent intent = new Intent("uk.ac.leonarduea.haig.studentexperience.View.Graduate");
                    startActivity(intent);
                    break;
            }
        }
    private File getFile(){
        File folder = new File("sdcard/StudentExpFolder");
        if(!folder.exists()) {folder.mkdir();}
        File image_file = new File(folder, "SE_image.jpg");
        return image_file;
    }
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        String path = "sdcard/StudentExpFolder/SE_image.jpg";
        imageView.setImageDrawable(Drawable.createFromPath(path));
        GUI_Controller gui = new GUI_Controller();
        gui.setPhotostatus();
    }
    public static Camera_GUI getInstance(){
        return instance;
    }
}
