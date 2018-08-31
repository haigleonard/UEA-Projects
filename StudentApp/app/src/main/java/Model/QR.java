package Model;

/**
 * Created by dan on 18/01/2017.
 */

public class QR {
    private static QR instance = new QR();
    public String[] qrResult = new String[12];
    public QR() {
        qrResult[0] = "Lovely lake of UEA, Many students come here to relax and chill out after busy days or just to enjoy the sun. You can also have a BBQ with all your friends on one of the provided BBQ.";
        qrResult[1] = "LCR, Many a night has been spent dancing away all of the uni stress, cheap drinks, cheesy music and good friends what more do you need? ";
        qrResult[2] = "state of the art equipment for any of your sporting needs. stay away from the gym at the begining of the term when all the freshers join it will be busy! for a week or two.";
        qrResult[3] = "a place you will love and hate, all of your exams will take place here so make sure you know how to get here!";
        qrResult[4] = "more books than you could ever read, a place you will spend many a night normally just before the deadline of submission.";
        qrResult[5] = "Normally filled with students, a great place to meet people and socialise and the biggest event of the year takes part here ... Pimp My Barrow, you wont want to miss that.";
        qrResult[6] = "Joost Noppen is lecturer in Software Engineering at the University of East Anglia in Norwich (UK). Joost holds an M.Sc. degree and a Ph. D. degree in Computer Science from the University of Twente in the Netherlands with a special focus on computational intelligence in Software Engineering. Joost has held positions in leading Software Engineering research groups across Europe, such as the École des Mines and the University of Lancaster, where he has worked in international research projects. In 2008 Joost was awarded a prestigious Marie Curie Intra-European fellowship from the EU for his work on Software Product Line development. Within the School of Computing Sciences Joost is responsible for the Software Engineering aspect of the teaching curriculum. He has been responsible for the complete redesign of the Software Engineering modules and bringing them up to date with state of the art in industrial software design and academic research. His modules focus on providing students with the fundamental skills for software development, such as object-orientation, and agile and test-driven development, combined with research-led topics and novel developments in industry such as mobile applications and software product lines.\n" +
                "\n" +
                "The long-standing research interest of Joost is in decision support for modelling and analysing design information during the development of software systems. This includes questions such as: is it possible to reconstruct software designs from partial documentation? How can you optimise the quality and extensibility of software systems when faced with uncertain change scenarios? Can historical data of software projects be used to steer the design of new systems? Most of Joost's research has resulted in advanced modelling techniques for capturing and reasoning with imprecise and uncertain design information based on probability and fuzzy set theory that support software development activities.\n" +
                "\n" +
                "Joost has over 14 years experience in Software Engineering both from an academic and industrial perspective. He has founded a successful web development company as a student and he has developed software in collaboration with multi-national companies. In his spare time Joost is exploring the application of Software Engineering principles to mobile games development practice.\n" +
                "\n" +
                "Currently Joost is involved in a variety of Software Engineering activities both related to research and teaching and he is happy to be approached for projects in these areas. He is also available for consultancy, advice and assessment in the field of industrial software engineering.\n" +
                "\n" +
                "\n"+"Career History\n" +
                "Lecturer, University of East Anglia, Norwich, United Kingdom (from 2011)\n" +
                "Senior Research Associate, Lancaster University, Lancaster, United Kingdom. (2008-2011)\n" +
                "Visiting Professor, University of Alberta, Edmonton, Canada. (2009)\n" +
                "Post–Doctoral Researcher, École des Mines de Nantes, Nantes, France. (2007–2008)\n" +
                "Post–Doctoral Researcher, University of Twente, Enschede, the Netherlands. (2007)\n" +
                "Ph.D. Associate, University of Twente, Enschede, the Netherlands. (2002–2007)";
        qrResult[7] = "us";
        qrResult[8] = "village";
        qrResult[9] = "zigg";
        qrResult[10] = "shop";
        qrResult[11] = "sains";
    }
    public String getQR(int i){
        return qrResult[i];
    }
    public static QR getInstance(){
        return instance;
    }
}
