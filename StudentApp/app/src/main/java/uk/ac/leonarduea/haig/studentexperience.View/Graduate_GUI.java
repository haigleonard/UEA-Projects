package uk.ac.leonarduea.haig.studentexperience.View;


import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.FacebookSdk;
import com.facebook.login.LoginManager;
import com.facebook.login.LoginResult;
import com.facebook.share.ShareApi;
import com.facebook.share.model.SharePhoto;
import com.facebook.share.model.SharePhotoContent;
import java.util.Arrays;
import java.util.List;

// Reference: http://simpledeveloper.com/how-to-share-an-image-on-facebook-in-android/

public class Graduate_GUI extends AppCompatActivity implements View.OnClickListener {

    Button Post;
    private CallbackManager callbackManager;
    private LoginManager loginManager;
     TextView message;
    String topost;
    static boolean photoB = false;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_graduate);
        Post = (Button) findViewById(R.id.post);
        Post.setOnClickListener(this);
        message = (TextView) findViewById(R.id.postMess);
        topost ="I completed my UEA open day, I graduated with a "+ Model.Profile.degree_classification +"!!  #UEA #openday #illbehereinSeptember #ohUEAisWonderful #ilovesoftwareengeerning2";
        message.setText(topost);
    }
    public void onClick(View v) {

            FacebookSdk.sdkInitialize(getApplicationContext());

            callbackManager = CallbackManager.Factory.create();

            List<String> permissionNeeds = Arrays.asList("publish_actions");

            loginManager = LoginManager.getInstance();

            loginManager.logInWithPublishPermissions(this, permissionNeeds);

            loginManager.registerCallback(callbackManager, new FacebookCallback<LoginResult>() {
                @Override
                public void onSuccess(LoginResult loginResult) {
                    sharePhotoToFacebook();
                }

                @Override
                public void onCancel() {
                    System.out.println("onCancel");
                }

                @Override
                public void onError(FacebookException exception) {
                    System.out.println("onError");
                }
            });

        }
    private void sharePhotoToFacebook() {
        Bitmap image;
       if(photoB == true) {
            image = BitmapFactory.decodeFile("sdcard/StudentExpFolder/SE_image.jpg");
       }else {
            image = BitmapFactory.decodeResource(getResources(), R.drawable.uealake);
       }
                SharePhoto photo = new SharePhoto.Builder()
                .setBitmap(image)
                .setCaption(topost)
                .build();
        SharePhotoContent content = new SharePhotoContent.Builder()
                .addPhoto(photo)
                .build();
        ShareApi.share(content, null);
    }
    @Override
    protected void onActivityResult(int requestCode, int responseCode, Intent data) {
        super.onActivityResult(requestCode, responseCode, data);
        callbackManager.onActivityResult(requestCode, responseCode, data);
        Intent intent = new Intent("uk.ac.leonarduea.haig.studentexperience.View.Finish");
        startActivity(intent);
    }

    public void setPhotostatus() {
        photoB = true;
    }


}


