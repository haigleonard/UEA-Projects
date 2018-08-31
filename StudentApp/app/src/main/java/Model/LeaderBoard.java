package Model;

/**
 * Created by Haig on 1/14/2017.
 */

public class LeaderBoard {
    static String[] topNine = new String[10];

    LeaderBoard(){
        topNine[0] = "Haig";
        topNine[1] = "Fran";
        topNine[2] = "Harun";
        topNine[3] = "Dan";
        topNine[4] = "Haig";
        topNine[5] = "Haig";
        topNine[6] = "Haig";
        topNine[7] = "Haig";
        topNine[8] = "Haig";
        topNine[9] = "Haig";
    }

    public void setLeaderboard(String[] dataBase) {
        for(int i = 0; i <9; i++){
            topNine[i] = dataBase[i];
        }
    }

    public String getLeaderBoard(int i){
        return topNine[i];
    }
}
