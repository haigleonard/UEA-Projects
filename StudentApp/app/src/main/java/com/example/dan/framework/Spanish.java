package com.example.dan.framework;

/**
 * Created by dan on 23/01/2017.
*/

public class Spanish implements Language {
    String[] Spanish = new String[10];
    public Spanish(){
        Spanish[0] = "\nPara el mejor guía turístico personal de UEA, esta aplicación no es la habitual, aburrida visita del campus!\n\n¡Todo depende de usted! Camine por la visita del campus donde quiera, encuentre su propio camino alrededor. Pero recuerde estar en la mirada hacia fuera para los códigos de QR el más que usted escanea el mejor usted hace.\n\n Su todo acerca de las cosas que los estudiantes realmente! Así que vaya y busque en este campus y obtenga experiencia de estudiante, así que tiene lo que se necesita para pasar el día como un verdadero estudiante de UEA." +
                "\n\t\t\t\t\t\t\t\t\t\t\t\tEstás listo?";
        Spanish[1] = "Dar una buena acogida ";
    }

    //@Override
    public String getLangString(int i) {
        return Spanish[i];
    }
}
