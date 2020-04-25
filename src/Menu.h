#ifndef DEF_MENU
#define DEF_MENU
#define MINSPIN 10
#define MAXSPIN 500

#include <gtkmm.h>
#include <gdk/gdk.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class Menu : public Gtk::Window {

    Gtk::Entry sequence;                  // Champ de saisie de la séquence
    unsigned int precision = 100;         // Précision (lien avec m_precision)
    Gdk::RGBA m_color[4];                 // Tableau des couleurs selon la vlauer des exposants
    Gtk::Button menu_quit;
    Gtk::AccelLabel label_expoNegE;
    Gtk::AccelLabel label_expoNegS;
    Gtk::AccelLabel label_expoPosS;
    Gtk::AccelLabel label_expoPosE;
    Gtk::AccelLabel label_precision;
    Gtk::AccelLabel label_sequence;
    Gtk::ColorButton m_color_button_neg_e;     // Bouton qui permet la séléction des couleurs de l'exposant
    Gtk::ColorButton m_color_button_neg_s;     // Bouton qui permet la séléction des couleurs de l'exposant
    Gtk::ColorButton m_color_button_pos_s;     // Bouton qui permet la séléction des couleurs de l'exposant
    Gtk::ColorButton m_color_button_pos_e;     // Bouton qui permet la séléction des couleurs de l'exposant
    Gtk::SpinButton m_select_precision;        // Bouton qui permet de régler la précison : 10 à 500 pour le moment (MINSPIN - MAXSPIN)
    Gtk::Grid grid;                            // Grille d'affichage des différents boutons

public :

    Menu();

    void setColorMaxNegExpo();

    void setColorMinNegExpo();

    void setColorMinPosExpo();

    void setColorMaxPosExpo();

    std::string colorToString(Gdk::RGBA color);

    void getSequence(std::ofstream& file);

    void getColor(std::ofstream& file);

    void getPrecision(std::ofstream& file);

    void textArea();

    void newPrecision();

    void writeFile();

    void closeButton();
};

#endif
