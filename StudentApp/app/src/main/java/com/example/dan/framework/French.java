package com.example.dan.framework;

/**
 * Created by dan on 23/01/2017.
 */

public class French implements Language {
    String[] French = new String[10];
    public French(){
        French[0] = "\n Pour le meilleur guide personnel de l'UEA, cette application n'est pas l'habituel, ennuyeux Campus Tour! Cela ne tient qu'à toi! À pied autour de la visite du campus où vous voulez, trouver votre propre chemin autour. Mais n'oubliez pas d'être à l'affût des codes QR plus vous numérisez le mieux vous faites. \n\n \n" +
                "Son tout sur les choses que les étudiants vraiment! Alors allez et recherchez ce campus, et obtenez-vous l'expérience d'étudiant, ainsi vous avez ce qu'il prend pour passer la journée comme un étudiant vrai d'UEA.\n" +
                "        \n\t\t\t\t\t\t\t\t\t\t\t\t Êtes-vous prêt";
        French[1] = "Nous saluons le retour ";
    }

    @Override
    public String getLangString(int i) {
        return French[i];
    }
}
