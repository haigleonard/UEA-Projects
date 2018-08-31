package Model;

import java.util.Random;

/**
 * Created by Haig on 1/5/2017.
 */

public class Facts {
    Random random = new Random();
    String Facts[] = new String[10];

    public Facts() {
        Facts[0] = "UEA was established in 1963";
        Facts[1] = "UEA has a student population of 15,058";
        Facts[2] = "UEA has a staff population of 2,820";
        Facts[3] = "The Times put UEA at 15th in the UK";
        Facts[4] = "UEA is consistantly in the top 5 for student experience";
        Facts[5] = "Norwich Cathedral has the second tallest spire in England";
        Facts[6] = "Norwich has one of the lowst crime rates in the UK";
        Facts[7] = "Norwich Market is the largest open air market in the UK";
        Facts[8] = "26% of Norwich is aged between 16 and 29";
        Facts[9] = "Alumni include Booker Prize winner's Anne Enright, Kazuo Ishiguro and Ian McEwan";
    }
    public String returnFact(){
        int i = random.nextInt(9)+1;
        return Facts[i];
    }
}
