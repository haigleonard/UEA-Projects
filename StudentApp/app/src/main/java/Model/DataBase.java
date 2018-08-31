package Model;

import android.app.Activity;
import android.app.ProgressDialog;
import android.os.AsyncTask;
import android.util.Log;

import org.apache.http.NameValuePair;
import org.apache.http.message.BasicNameValuePair;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by Haig on 1/14/2017.
 */

public class DataBase extends Activity {
    // Progress Dialog
    static String Percentage;
    String PerCentage;
    String username;
    JSONParser jsonParser = new JSONParser();

    //url to update data
    private static final String url_update_leader_board = "http://tarakanda.com/android-data" +
            "/leader-board/update-data-leader-board.php";

    //JSON Node name
    private static final String TAG_NAME = "user_name";
    private static final String TAG_PERCENTAGE = "percentage";

    // url to create new product
    private static String url_create_new_data = "http://www.tarakanda.com/android-data/" +
            "leader-board/add-data-to-leader-board.php";

    // JSON Node names
    private static final String TAG_SUCCESS = "success";

    public void add(String user, String percent){
        username = user;
        Percentage = percent;
        Log.d("In Add new", user);
        new CreateNewData().execute();
    }
    public void setID(String user, String Percentage) {
        username = user;
        PerCentage = Percentage;
        new UpdateDataDetails().execute();
    }

    private class CreateNewData extends AsyncTask<String, String, String> {
        /**
         * Before starting background thread Show Progress Dialog
         * */
        @Override
        protected void onPreExecute() {
            super.onPreExecute();
        }
        /**
         * Creating product
         * */
        protected String doInBackground(String... args) {;

            // Building Parameters
            List<NameValuePair> params = new ArrayList<NameValuePair>();

            params.add(new BasicNameValuePair("user_name", username));
            params.add(new BasicNameValuePair("percentage", Percentage));

            // getting JSON Object
            // Note that create product url accepts POST method
            JSONObject json = jsonParser.makeHttpRequest(url_create_new_data,
                    "POST", params);

            // check log cat fro response
            Log.d("Add new data Response", json.toString());

            // check for success tag
            try {

                int success = json.getInt(TAG_SUCCESS);

                if (success == 1) {
                    finish();
                } else {
                    // failed to create product
                }
            } catch (JSONException e) {
                e.printStackTrace();
            }

            return null;
        }
        protected void onPostExecute(String file_url) {
        }

    }
    class UpdateDataDetails extends AsyncTask<String, String, String> {
        @Override
        protected void onPreExecute() {
            super.onPreExecute();
        }
        protected String doInBackground(String... args) {

            List<NameValuePair> params = new ArrayList<NameValuePair>();
            params.add(new BasicNameValuePair(TAG_SUCCESS, "success"));
            params.add(new BasicNameValuePair(TAG_NAME, username));
            params.add(new BasicNameValuePair(TAG_PERCENTAGE, PerCentage));
            Log.d("PERCENTAGE   ", PerCentage +" "+username);
            JSONObject json = jsonParser.makeHttpRequest(url_update_leader_board,
                    "POST", params);

            try {
                json.getInt(TAG_SUCCESS);
            } catch (JSONException e) {
                e.printStackTrace();
            }
            return null;
        }

        protected void onPostExecute(String file_url) {
        }
    }

}
