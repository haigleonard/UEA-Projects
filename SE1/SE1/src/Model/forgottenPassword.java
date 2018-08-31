/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Model;

/**
 *
 * @author paj13suu
 */
public class forgottenPassword {
    
    String USER_NAME;
    String EMAIL;
    String email;

    public forgottenPassword(String PUSERNAME, String PEMAIL, String Pemail) {
        USER_NAME = PUSERNAME;
        EMAIL = PEMAIL;
        email = Pemail;
    }

    
    
    public boolean check(){
        if(EMAIL == null ? email == null : EMAIL.equals(email)){
            return true;
        }else{
            return false;
        }
    }

    public boolean compareA(String ANSWER, String awnser) {
        if(ANSWER == null ? awnser == null : ANSWER.equals(awnser)){
            return true;
        }else{
            return false;
        }
    }
    
}
