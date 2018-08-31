package Controller;

import android.content.Context;

import com.example.dan.framework.French;
import com.example.dan.framework.Spanish;

import Model.DataBase;
import Model.FileIO;
import Model.Locations;
import Model.Profile;
import Model.QR;
import Model.Quiz;
import Model.Facts;

/**
 * Created by Haig on 1/2/2017.
 */

public class ModelController {









    public void setuserName(String userTemp, Context context) {Profile profile = new Profile();FileIO file = new FileIO();String user = file.read(userTemp, context);profile.setusername(user);}

    public int getPercent() {
        Profile profile = new Profile();return profile.getPercent();
    }
    public String getdegree() {
        Profile profile = new Profile();return profile.getdegClass();
    }
    public void checkLocation(String qrLoc, Context context) {Profile profile = new Profile();profile.checkLocation(qrLoc, context);}
    public String iniRead(Context context) {FileIO file = new FileIO();
        return file.iniRead(context);
    }
    public String getLoc(int i) {Locations loc = new Locations();return loc.getLoc(i);}
    public void setLang(int i){Profile profile = new Profile();profile.setLang(i);}
    public int getLang(){ Profile profile = new Profile();return profile.getLang();}
    public String getQuestion() { Quiz quiz = new Quiz();return quiz.getQuestion();}
    public void setPercent(){Profile profile = new Profile();profile.setQPercent(2);}
    public int getHint(){Profile profile = new Profile();return profile.getHint();}
    public void updateHint(Context context){Profile profile = new Profile();profile.updateHint(context);}
    public String getQR(int i){ QR qr = new QR();qr.getInstance();return qr.getQR(i);}
    public String getusername() {Profile profile = new Profile();return profile.getusername();}
    public String returnFact() { Facts fac = new Facts();Profile profile = new Profile();return fac.returnFact();}
    public String[] getLoc2() {Profile profile = new Profile();return profile.getLoc();}
    public String[] userLocs() {Profile profile = new Profile();return profile.userLocs();}
    public void updateDegree(String temp, Context M) {FileIO file = new FileIO();file.write2(temp, M); }
    public String getLB(int i){
        //LeaderBoard leader = new LeaderBoard();
        //return leader.getLeaderBoard(i);
        return null;
    }
    public void reset(){Quiz quiz = new Quiz();quiz.reset();}
    public void clear(Context context) {Profile profile = new Profile();
        String temp= profile.getusername();FileIO file = new FileIO();
        file.write(temp, context);
        setuserName(temp, context);
    }
    public String getlan(int i, int lang) {
        if(lang == 1) {
            French fren = new French();
            return fren.getLangString(i);
        }
        if(lang == 2) {
            Spanish span = new Spanish();
            return span.getLangString(i);
        }
        else{return "";}
    }
//DataBase
    public void addData(String part1, String part2) { DataBase data = new DataBase();data.add(part1,part2);}
    public void setID(String part1, String part2) {DataBase data = new DataBase();data.setID(part1,part2);
    }
}
