/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Data;

import java.io.File;
import java.io.FileNotFoundException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Time;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Scanner;

/**
 *
 * @author paj13suu
 */
public class DataManager {
    
    String check;
    Connection con;
    PreparedStatement stmt;
    PreparedStatement stmt2;
    ResultSet rs;
    String USER_NAME;
    String PASS_WORD;
    String Error = "not found";
    int mark = 0;
    private Scanner x;
    
    public DataManager() throws SQLException, ParseException {
        DoConnect();
    }
    
    public void DoConnect() {
        try {
            String host = "jdbc:derby://localhost:1527/software1";
            String uName = "software";
            String uPass = "engineers";
            con = DriverManager.getConnection(host, uName, uPass);
        } catch (SQLException err) {
            System.out.println(err.getMessage());
        }
    }
    
    public String sqlLogin(String USER_NAME) throws SQLException {
        stmt = con.prepareStatement("select * from SOFTWARE.USERPROFILE WHERE USERNAME =   ?  ");
        stmt.setString(1, USER_NAME);
        rs = stmt.executeQuery();
        while (rs.next()) {
            PASS_WORD = rs.getString("PASSWORD");
            return PASS_WORD;
        }   
        return Error;
    }
    
    public boolean sqlcheckAval() throws SQLException {
       stmt = con.prepareStatement("select * from SOFTWARE.USERPROFILE WHERE USERNAME = ?");
        stmt.setString(1, USER_NAME);
        rs = stmt.executeQuery();
        while (rs.next()){
            System.out.println("no username");
            return false;
        }
        return true;
    }
    
    public void setUsername(String PUSER_NAME) {
        USER_NAME = PUSER_NAME;
    }

    public void sqlsignup(String USER_NAME, String PASS_WORD, String FULL_NAME, String EMAIL, String QUESTION, String ANSWER, String GENDER, String MODULE1, String MODULE2, String MODULE3, String MODULE4, String MODULE5, String MODULE6, String DOB) throws SQLException, ParseException {
        stmt = con.prepareStatement("INSERT INTO SOFTWARE.USERPROFILE (USERNAME, PASSWORD, FULL_NAME, EMAIL, QUESTION, AWNSER, GENDER, MODULE1, MODULE2 , MODULE3 , MODULE4 ,MODULE5, MODULE6, DOB) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
        stmt.setString(1, USER_NAME);
        stmt.setString(2, PASS_WORD);
        stmt.setString(3, FULL_NAME);
        stmt.setString(4, EMAIL);
        stmt.setString(5, QUESTION);
        stmt.setString(6, ANSWER);
        stmt.setString(7, GENDER);
        stmt.setString(8, MODULE1);
        stmt.setString(9, MODULE2);
        stmt.setString(10, MODULE3);
        stmt.setString(11, MODULE4);
        stmt.setString(12, MODULE5);
        stmt.setString(13, MODULE6);
        SimpleDateFormat sdf1 = new SimpleDateFormat("dd-MM-yyyy");
        java.util.Date date = sdf1.parse(DOB);
        java.sql.Date sqlDOB = new java.sql.Date(date.getTime());  
        stmt.setDate(14, sqlDOB);
        stmt.executeUpdate();
    }

    public String checkUser(String USERNAME) throws SQLException {
        String EMAIL;
        stmt = con.prepareStatement("select * from SOFTWARE.USERPROFILE WHERE USERNAME = ?");
        stmt.setString(1, USERNAME);
        rs = stmt.executeQuery();
        while (rs.next()){
            EMAIL = rs.getString("EMAIL");
            return EMAIL;
        }
        return null;
    }

    public String getQuestion(String USERNAME) throws SQLException {
        String QUESTION;
        stmt = con.prepareStatement("select * from SOFTWARE.USERPROFILE WHERE USERNAME = ?");
        stmt.setString(1, USERNAME);
        rs = stmt.executeQuery();
        while (rs.next()){
            QUESTION = rs.getString("QUESTION");
            return QUESTION;
        }
        return null;
        }

    public String getAnswer(String USERNAME) throws SQLException {
        String awnser;
        stmt = con.prepareStatement("select * from SOFTWARE.USERPROFILE WHERE USERNAME = ?");
        stmt.setString(1, USERNAME);
        rs = stmt.executeQuery();
        while (rs.next()){
            awnser = rs.getString("AWNSER");
            return awnser;
        }
        return null;
        }

    public ArrayList getProfile(String USER_NAME) throws SQLException {
        String temp;
        ArrayList results = new ArrayList();
        stmt = con.prepareStatement("select * from SOFTWARE.USERPROFILE WHERE USERNAME = ?");
        stmt.setString(1, USER_NAME);
        rs = stmt.executeQuery();
        while (rs.next()){
            temp = rs.getString("FULL_NAME");
            results.add(temp);
            temp = rs.getString("PASSWORD");
            results.add(temp);
            temp = rs.getString("EMAIL");
            results.add(temp);
            temp = rs.getString("QUESTION");
            results.add(temp);
            temp = rs.getString("AWNSER");
            results.add(temp);
            temp = rs.getString("GENDER");
            results.add(temp);
            temp = rs.getString("MODULE1");
            results.add(temp);
            temp = rs.getString("MODULE2");
            results.add(temp);
            temp = rs.getString("MODULE3");
            results.add(temp);
            temp = rs.getString("MODULE4");
            results.add(temp);
            temp = rs.getString("MODULE5");
            results.add(temp);
            temp = rs.getString("MODULE6");
            results.add(temp);
            temp = rs.getString("DOB");
            results.add(temp);
            return results;
        }
        return null;
    }
    public void CSVUpload() throws SQLException, ParseException{
    
        openFile();
        ResultSet readFile = readFile();
        
    }
    public void openFile(){
    try {
        x = new Scanner (new File("Hub.txt"));
        x.useDelimiter(",");
    }
    catch(FileNotFoundException e){
            System.out.println("Could not find file");
    }
    }

    public ResultSet readFile() throws SQLException, ParseException{
    int mark = 0;
    while(x.hasNext()){
        int rows;
        String modCode = x.next();
        String modName = x.next();
        String Organiser = x.next();
        String eMail = x.next();
        String tempid = x.next();
        int id = Integer.valueOf(tempid);
        String name = x.next();
        
        String starttime = x.next();
        Time start = java.sql.Time.valueOf(starttime);
        
        String endtime = x.next();
        Time end = java.sql.Time.valueOf(endtime);
        
        String datetemp = x.next();
        Date utilDate = new SimpleDateFormat("yyyy-MM-dd").parse(datetemp);
        java.sql.Date sqlDate = new java.sql.Date(utilDate.getTime());
        
        String room = x.next();
        
        System.out.println(modCode + " | " + modName + " | " + Organiser + " | " + eMail
        + " | " + name + " | " + start + " | " + end + " | " + sqlDate + " | " + room);
        
        stmt2 = con.prepareStatement("select * from SOFTWARE.MODULES WHERE MODULE_CODE =   ?  ");
        stmt2.setString(1, modCode);
        rs = stmt2.executeQuery();
        while (rs.next()) {
            stmt2 = con.prepareStatement ("UPDATE SOFTWARE.MODULES SET MODULE_NAME=?, ORGANIZER=?,MODULE_EMAIL=? WHERE MODULE_CODE=?");
            stmt2.setString(1, modName);
            stmt2.setString(2, Organiser);
            stmt2.setString(3, eMail);
            stmt2.setString(4, modCode);
            rows = stmt2.executeUpdate();
            mark++;
        }
        if(mark == 0){
            stmt2 = con.prepareStatement ("INSERT INTO SOFTWARE.MODULES (MODULE_CODE, MODULE_NAME, ORGANIZER, MODULE_EMAIL) VALUES (?, ?, ?, ?)");
            stmt2.setString(1, modCode);
            stmt2.setString(2, modName);
            stmt2.setString(3, Organiser);
            stmt2.setString(4, eMail);
            rows = stmt2.executeUpdate();
        }
        mark = 0;
        stmt2 = con.prepareStatement("select * from SOFTWARE.TIMETABLE WHERE TIMETABLE_ID =   ?  ");
        stmt2.setInt(1, id);
        rs = stmt2.executeQuery();
        while (rs.next()) {
            stmt2 = con.prepareStatement ("UPDATE SOFTWARE.TIMETABLE SET MODULE_CODE=?, TT_NAME=?,STARTTIME=?,ENDTIME=?,TT_DATE=?,ROOM=? WHERE TIMETABLE_ID=?");
            stmt2.setString(1, modCode);
            stmt2.setString(2, name);
            stmt2.setTime(3, (Time) start);
            stmt2.setTime(4, (Time) end);
            stmt2.setDate(5,  sqlDate);
            stmt2.setString(6, room);
            stmt2.setInt(7, id);
            rows = stmt2.executeUpdate();
            mark++;
        }
        
        if(mark == 0){
            stmt2 = con.prepareStatement ("INSERT INTO SOFTWARE.TIMETABLE (MODULE_CODE, TT_NAME, STARTTIME, ENDTIME, TT_DATE, ROOM) VALUES (?, ?, ?, ?, ?, ?)");
            stmt2.setString(1, modCode);
            stmt2.setString(2, name);
            stmt2.setTime(3, (Time) start);
            stmt2.setTime(4, (Time) end);
            stmt2.setDate(5, sqlDate);
            stmt2.setString(6, room);
            rows = stmt2.executeUpdate();
        }
        mark = 0;
    }
        closeFile();
        File file = new File("Hub.txt");
        boolean delete = file.delete();
        System.out.println(delete);
        return rs;
    }
    public void closeFile(){
        x.close();
    }
    

    
    
}


