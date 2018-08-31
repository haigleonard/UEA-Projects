package Controller;

import android.content.Context;

import uk.ac.leonarduea.haig.studentexperience.View.Graduate_GUI;
import uk.ac.leonarduea.haig.studentexperience.View.Quiz_GUI;

/**
 * Created by Haig on 1/2/2017.
 */

public class GUI_Controller {

    ModelController model = new ModelController();

    public void setUser(String user, Context context){model.setuserName(user, context);}
    public void checkLocation(String qrLoc, Context context){model.checkLocation(qrLoc, context);}
    public String iniRead(Context context){return model.iniRead(context);}
    public String getLB(int i){return model.getLB(i);}
    public void setLang(int i){model.setLang(i);}
    public int getLang(){return model.getLang();}
    public String getQuestion() {return model.getQuestion();}
    public void setPercent(){model.setPercent();}
    public int getHint() {
        return model.getHint();
    }
    public void updateHint(Context context){model.updateHint(context);}
    public String getclass(){return model.getdegree();}
    public String getQR(int i) {
        return model.getQR(i);
    }
    public String getusername() {return model.getusername();    }
    public int getPercent() {return model.getPercent();}
    public String[] getLoc() {return model.getLoc2();}
    public void setPhotostatus() {Graduate_GUI grad = new Graduate_GUI();grad.setPhotostatus();}
    public String[] userLocs() {return model.userLocs();}
    public String getLoc(int i) {return model.getLoc(i);}
    public void clear(Context context) {Quiz_GUI quiz = new Quiz_GUI(); quiz.reset();model.clear(context);model.reset();}
    public String getLang(int i, int lang){return model.getlan(i, lang);}

}
