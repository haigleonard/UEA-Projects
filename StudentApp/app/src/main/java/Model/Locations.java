package Model;

/**
 * Created by Haig on 1/5/2017.
 */

public class Locations {
    static String[] locations = new String[12];
    public Locations() {
        locations[0] = "Lake";
        locations[1] = "Library";
        locations[2] = "LCR";
        locations[3] = "Congregation Hall";
        locations[4] = "Sports Park";
        locations[5] = "Square";
        locations[6] = "Joost";
        locations[7] = "SE2";
        locations[8] = "The Village";
        locations[9] = "The Ziggurats";
        locations[10] = "Union Shop";
        locations[11] = "Sainsbury Centre";

       /* locations[0][1] = "52.618958";
        locations[0][2] = "1.240643";*/
    }
    public String getLoc(int i){
        return locations[i];
    }
 /*
    public String lat(int i){
        return locations[i][1];
    }
    public String lon(int i){
        return locations[i][2];
    }*/
}
