package uk.ac.leonarduea.haig.studentexperience.View;

import android.os.AsyncTask;
import android.os.Bundle;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import org.apache.http.NameValuePair;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.ListActivity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.TextView;
import android.widget.Toast;

import Model.DataBase;
import Model.JSONParser;

/**
 * View all the data in Leaderboard in list
 */

public class LeaderBoardData extends ListActivity {
    private ProgressDialog pDialog;
    JSONParser jParser = new JSONParser();
    ArrayList<HashMap<String, String>> itemLists;

    //url to get all data in list from leader board
    private static String url_all_items = "http://www.tarakanda.com/android-data/leader-board/get-board-all-data.php";

    //JSON node names
    private static final String TAG_SUCCESS = "success";
    private static final String TAG_TABLE_NAME = "LeaderBoard";
    private static final String TAG_ID = "user_id";
    private static final String TAG_NAME = "user_name";
    private static final String TAG_PERCENTAGE = "percentage";

    //Items JSON array
    JSONArray items;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.leader_board_data);

        itemLists = new ArrayList<HashMap<String, String>>();
        new LoadAllItems().execute();
        ListView lv = getListView();
        //Update or delete on item click
        lv.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                String user_id = ((TextView) view.findViewById(R.id.user_id)).getText().toString();
                Toast.makeText(getApplication(), user_id, Toast.LENGTH_LONG).show();
                Intent in = new Intent(getApplicationContext(),
                        DataBase.class);
                in.putExtra(TAG_ID, user_id);
                startActivityForResult(in, 100);
            }
        });

    }

    public class LoadAllItems extends AsyncTask<String, String, String> {
        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            pDialog = new ProgressDialog(LeaderBoardData.this);
            pDialog.setMessage("Loading Items. Please wait..........");
            pDialog.setIndeterminate(false);
            pDialog.setCancelable(false);
            pDialog.show();
        }
        @Override
        protected String doInBackground(String... args) {
            List<NameValuePair> params = new ArrayList<NameValuePair>();
            JSONObject json = jParser.makeHttpRequest(url_all_items, "GET", params);
            Log.d("All Items: ", json.toString());
            try {
                int success = json.getInt(TAG_SUCCESS);
                if (success == 1) {
                    items = json.getJSONArray(TAG_TABLE_NAME);
                    for (int i = 0; i < items.length(); i++) {
                        JSONObject c = items.getJSONObject(i);

                        String id = c.getString(TAG_ID);
                        String name = c.getString(TAG_NAME);
                        String percentage = c.getString(TAG_PERCENTAGE);
                        HashMap<String, String> map = new HashMap<String, String>();
                        map.put(TAG_ID, id);
                        map.put(TAG_NAME, name);
                        map.put(TAG_PERCENTAGE, percentage);
                        itemLists.add(map);
                    }
                } else {
                        //nothing found
                }
            } catch (JSONException e) {
                e.printStackTrace();
            }
            return null;
        }

        protected void onPostExecute(String file_url) {
            pDialog.dismiss();
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    /**
                     * Updating parsed JSON data into ListView
                     */
                    ListAdapter adapter = new SimpleAdapter(LeaderBoardData.this, itemLists,
                            R.layout.display_list_from_database, new String[]{TAG_ID,
                            TAG_NAME, TAG_PERCENTAGE},
                            new int[]{R.id.user_id, R.id.name, R.id.percentage});
                    setListAdapter(adapter);

                }
            });

        }
    }


}
