package Model;

import android.content.Context;
import android.util.Log;

import Controller.ModelController;

/**
 * Created by Haig on 1/2/2017.
 */

public class Profile {
    static int user_name_id;
    static String user_name;
    public static int degree_percentage;
    public static String degree_classification;
    static String[] userlocations = new String[12];
    static String temp[] = new String[12];
    static int hint;
    static int Lang;

    public void setusername(String user) {
        String[] parts = user.split("#");
        String part1 = parts[0];
        String part2 = parts[1];
        String part3 = parts[2];
        String part4 = parts[3];
        String part5 = parts[4];
        String part6 = parts[5];
        String part7 = parts[6];
        String part8 = parts[7];
        String part9 = parts[8];
        String part10 = parts[9];
        String part11 = parts[10];
        String part12 = parts[11];
        String part13 = parts[12];
        String part14 = parts[13];
        String part15 = parts[14];
        String part16 = parts[15];
        user_name = part1;
        degree_percentage = Integer.parseInt(part2);
        degree_classification = part3;
        hint = Integer.parseInt(part4);
        userlocations[0] = temp[0] = part5;
        userlocations[1] = temp[1] = part6;
        userlocations[2] = temp[2] = part7;
        userlocations[3] = temp[3] = part8;
        userlocations[4] = temp[4] = part9;
        userlocations[5] = temp[5] = part10;
        userlocations[6] = temp[6] = part11;
        userlocations[7] = temp[7] = part12;
        userlocations[8] = temp[8] = part13;
        userlocations[9] = temp[9] = part14;
        userlocations[10] = temp[10] = part15;
        userlocations[11] = temp[11] = part16;
    }
    public int getHint(){return hint;}
    public void updateHint(Context context){hint--;degreeCalc(context);}
    public void setLang(int i){Lang = i;}
    public int getLang(){return Lang;}
    public String getusername() {
        return user_name;
    }
    public String[] getLoc() {
        return temp;
    }
    public String[]  userLocs(){
        return temp;
    }
    public void setPercent(int percent, Context context) {degree_percentage += percent;degreeCalc(context);}
    public int getPercent() {
        return degree_percentage;
    }
    public String getdegClass() {
        return degree_classification;
    }
    public void degreeCalc(Context context) {
        setQPercent(0);
        String Q1 = temp[0];
        String Q2 = temp[1];
        String Q3 = temp[2];
        String Q4 = temp[3];
        String Q5 = temp[4];
        String Q6 = temp[5];
        String Q7 = temp[6];
        String Q8 = temp[7];
        String Q9 = temp[8];
        String Q10 = temp[9];
        String Q11 = temp[10];
        String Q12 = temp[11];
        String temp = user_name + "#" + String.valueOf(degree_percentage) + "#" + degree_classification + "#" + String.valueOf(hint) + "#" + Q1 + "#" + Q2 + "#" + Q3 + "#" + Q4 + "#" + Q5 + "#" + Q6 + "#" +Q7 + "#" +Q8+ "#" + Q9 + "#" + Q10 + "#" +Q11 + "#" +Q12;
        Log.d("FINAL", temp);
        ModelController model = new ModelController();
        model.updateDegree(temp, context);
    }
    public void checkLocation(String userLoc, Context context) {
        for (int i = 0; i < 12; i++) {
            ModelController model = new ModelController();
            if (model.getLoc(i).equals(userLoc)) {
                temp[i] = userLoc;
                if (model.getLoc(i).equals(userlocations[i])) {
                } else {
                    setPercent(5, context);
                }
                userlocations[i] = userLoc;
            }
        }
    }
    public void setQPercent(int per){
        degree_percentage += per;
        if (degree_percentage >= 40 && degree_percentage < 50) {degree_classification = "Third";}
        if (degree_percentage >= 50 && degree_percentage < 60) {degree_classification = "Two:Two";}
        if (degree_percentage >= 60 && degree_percentage < 70) {degree_classification = "Two:One";}
        if (degree_percentage >= 70) {degree_classification = "First";}
    }
    public void setID(int id){
        user_name_id = id;
        Log.d("USER ID", String.valueOf(user_name_id));
    }
    public int getUser_name_id(){
        return user_name_id;
    }
}
