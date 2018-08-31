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
public class login {
    
    String USER_NAME;
    String PASS_WORD;

    public login(String PUSER_NAME, String PPASS_WORD) {
        USER_NAME = PUSER_NAME;
        PASS_WORD = PPASS_WORD;
    }
    
    public boolean checkDetails(String DATA_PASS_WORD) throws SQLException, ParseException{
    if(DATA_PASS_WORD == null ? PASS_WORD == null : DATA_PASS_WORD.equals(PASS_WORD)){
        return true;
    }else{
        return false;
    }
    }
}
