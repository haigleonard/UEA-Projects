/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Model;

import java.sql.SQLException;
import java.text.ParseException;

/**
 *
 * @author paj13suu
 */
public class register {

    String USER_NAME;
    String PASS_WORD;
    String CONFIRM_PASSWORD;
    String FULL_NAME;
    String EMAIL;
    String DOB;
    String MODULE1;
    String MODULE2;
    String MODULE3;
    String MODULE4;
    String MODULE5;
    String MODULE6;
    String GENDER;
    boolean check;
    
    

    public register(String PPASS_WORD, String PCONFIRM_PASSWORD) {
        CONFIRM_PASSWORD = PCONFIRM_PASSWORD;
        PASS_WORD = PPASS_WORD;
        }

    public boolean signup() throws SQLException, ParseException {
        if(CONFIRM_PASSWORD == null ? PASS_WORD == null : CONFIRM_PASSWORD.equals(PASS_WORD)){
        return true;
        }else{
        return false;
        }
    }
}
