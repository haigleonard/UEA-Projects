package Model;


/**
 * Created by Haig on 1/14/2017.
 */

public class Quiz {
    static String[][] Question = new String[10][3];
    String temp;
    static int token = 0;

    public Quiz() {
        Question[0][0] = "What Year was the University of East Anglia Established?";
        Question[1][0] = "What population of students does UEA consist of?";
        Question[2][0] = "How many Staff does UEA have employeed?";
        Question[3][0] = "UEA is ranked what in 'the Times' top Univcersity list?";
        Question[4][0] = "UEA is consistantly ranked what for student experience?";
        Question[5][0] = "The Norwich Catherdal Spire is __ tallest in the England";
        Question[6][0] = "Norwich has one of the lowest ____ rates";
        Question[7][0] = "Norwich market is the largest open air market in...";
        Question[8][0] = "What percentage of Norwich is aged between 16 and 29?";
        Question[9][0] = "Alumni include Booker Prize winner's Anne Enright, Kazuo Ishiguro and ___";

        Question[0][1] = "#1";
        Question[1][1] = "#3";
        Question[2][1] = "#1";
        Question[3][1] = "#1";
        Question[4][1] = "#3";
        Question[5][1] = "#3";
        Question[6][1] = "#2";
        Question[7][1] = "#2";
        Question[8][1] = "#3";
        Question[9][1] = "#2";

        Question[0][2] = "#1963#1965#1967";
        Question[1][2] = "#14,678#16,129#15,058";
        Question[2][2] = "#2,820#2,152#3,638";
        Question[3][2] = "#15#21#18";
        Question[4][2] = "#7#9#5";
        Question[5][2] = "#3rd#1st#2nd";
        Question[6][2] = "#Pregnancy#Crime#Satisfaction";
        Question[7][2] = "#Norfolk#Europe#World";
        Question[8][2] = "#15%#22%#26%";
        Question[9][2] = "#Kim Kardashian#Ian McEwan#Jeremy Corbyn";
    }
    public String getQuestion() {temp = Question[token][0] + Question[token][2] + Question[token][1];token ++;return temp;}
    public void reset(){token=0;}
}
