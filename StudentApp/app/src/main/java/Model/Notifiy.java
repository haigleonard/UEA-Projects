package Model;

/**
 * Created by dan on 10/1/2017.
 */
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.media.RingtoneManager;
import android.support.v4.app.NotificationCompat;
import android.support.v4.app.TaskStackBuilder;
import android.view.View;
import com.example.dan.framework.Notification;
import uk.ac.leonarduea.haig.studentexperience.View.Hints_GUI;
import uk.ac.leonarduea.haig.studentexperience.View.Home_GUI;
import uk.ac.leonarduea.haig.studentexperience.View.R;
import static android.content.Context.NOTIFICATION_SERVICE;

public class Notifiy  implements Notification{


    @Override
    public void getnotification(View view, Context context,String title, String text) {

        NotificationCompat.Builder builder = new NotificationCompat.Builder(context);
        builder.setSmallIcon(R.drawable.gradlogo);
        builder.setContentTitle(title);
        builder.setContentText(text);


        builder.setSound(RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION));
        // builder.setColor(color);

        Intent intentNoti = new Intent(context, Home_GUI.class);

        TaskStackBuilder stackB = TaskStackBuilder.create(context);

        stackB.addParentStack(Hints_GUI.class);

        stackB.addNextIntent(intentNoti);

        PendingIntent pendingIntentNoti = stackB.getPendingIntent(0, PendingIntent.FLAG_UPDATE_CURRENT);
        builder.setContentIntent(pendingIntentNoti);

        NotificationManager notifimgr = (NotificationManager) context.getSystemService(NOTIFICATION_SERVICE);

        notifimgr.notify(0, builder.build());

    }
}
