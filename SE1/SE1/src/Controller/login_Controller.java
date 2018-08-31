/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Controller;

import Model.ErrorMessage;
import java.sql.SQLException;
import java.text.ParseException;
import java.util.ArrayList;

/**
 *
 * @author paj13suu
 */
public class login_Controller {
    
    String USER_NAME;
    String PASS_WORD;
    String check;
    boolean bool = false;
    String CONFIRM_PASSWORD;
    String FULL_NAME;
    String EMAIL;
    String DOB;
    String QUESTION;
    String AWNSER;
    String GENDER;
    String MODULE1;
    String MODULE2;
    String MODULE3;
    String MODULE4;
    String MODULE5;
    String MODULE6;
    String Error = "Incorrect Password";
    ArrayList rs = new ArrayList();

    public login_Controller(String USERNAME, String PASSWORD) {
        USER_NAME = USERNAME;
        PASS_WORD = PASSWORD;
    }
    public boolean check() throws SQLException, ParseException{
        Model.login login = new Model.login(USER_NAME, PASS_WORD);
        Data.DataManager data = new Data.DataManager();
        check = data.sqlLogin(USER_NAME);
        bool = login.checkDetails(check);
        if(bool == true){
            GUI.DashboardGUI dashboard = new GUI.DashboardGUI();
            dashboard.setVisible(true);
            setProfilePage(USER_NAME);
            return true;
        }else{
        ErrorMessage.infoBox(Error, "Password Incorrect");
        return false;
        }
    }
    
    

    public boolean signup(String CONFIRM_PASSWORD, String FULL_NAME, String EMAIL, String DOB, String QUESTION, String AWNSER, String GENDER, String MODULE1, String MODULE2, String MODULE3, String MODULE4, String MODULE5, String MODULE6) throws SQLException, ParseException {
        Model.register register = new Model.register(PASS_WORD, CONFIRM_PASSWORD);
        bool = register.signup();
        if(bool == false){
            return false;
        }
        Data.DataManager data = new Data.DataManager();
        data.setUsername(USER_NAME);
        bool = data.sqlcheckAval();
        if(bool == true){
        data.sqlsignup(USER_NAME, PASS_WORD, FULL_NAME, EMAIL, QUESTION, AWNSER, GENDER,  MODULE1, MODULE2, MODULE3, MODULE4, MODULE5, MODULE6, DOB);
        }
        if(bool == false){
        return false;
        }
        return false;
    }

    public boolean forgot(String USERNAME, String EMAIL) throws SQLException, ParseException {
        Data.DataManager data = new Data.DataManager();
        String email = data.checkUser(USERNAME);
        Model.forgottenPassword forgotP = new Model.forgottenPassword(USERNAME,EMAIL,email);
        boolean emailcheck = forgotP.check();
        if(emailcheck == true){
            String question = data.getQuestion(USERNAME);
            GUI.passwordQ passwordQ = new GUI.passwordQ();
            passwordQ.setquestion(question, USERNAME);
            passwordQ.setVisible(true);
            return true;
        }else{
            ErrorMessage.infoBox(Error, "incorrect");
            return false;
        }
        
    }

    public void getAnswer(String ANSWER, String USERNAME) throws SQLException, ParseException {
        Data.DataManager data = new Data.DataManager();
        String awnser = data.getAnswer(USERNAME);
        Model.forgottenPassword compareA = new Model.forgottenPassword("BLANK", "BLANK", "BLANK");
        boolean checkA  = compareA.compareA(ANSWER, awnser);
        String password = data.sqlLogin(USERNAME);
        if(checkA == true){
            ErrorMessage.infoBox("Password is: " + password, "Password:");
        }else{
            ErrorMessage.infoBox("Incorrect Awsner", "incorrect");
        }
        
    }
    
    public void setProfilePage(String USER_NAME) throws SQLException, ParseException{
        Data.DataManager data = new Data.DataManager();
        rs = data.getProfile(USER_NAME);
        FULL_NAME = (String) rs.get(0);
        PASS_WORD = (String) rs.get(1);
        EMAIL = (String) rs.get(2);
        QUESTION =(String) rs.get(3);
        AWNSER =(String) rs.get(4);
        GENDER =(String) rs.get(5);
        MODULE1 = (String) rs.get(6);
        MODULE2 = (String) rs.get(7);
        MODULE3 = (String) rs.get(8);
        MODULE4 = (String) rs.get(9);
        MODULE5 = (String) rs.get(10);
        MODULE6 = (String) rs.get(11);
        DOB = (String) rs.get(12);
        GUI.Profile profile = new GUI.Profile();
        profile.setProfile(USER_NAME, PASS_WORD, FULL_NAME,  EMAIL, QUESTION, AWNSER,  GENDER,  DOB,  MODULE1,  MODULE2,  MODULE3,  MODULE4,  MODULE5,  MODULE6);
        profile.setVisible(true);
    }
    
}
