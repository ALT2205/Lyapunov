#ifndef DEF_MENU
#define DEF_MENU

#include <gtkmm.h>
#include <gdk/gdk.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
class Menu : public Gtk::Window {

    Gtk::Entry sequence;
    unsigned int precision = 100;
    Gdk::RGBA m_color[4];
    Gtk::Button m_color_button_neg_e;
    Gtk::Button m_color_button_neg_s;
    Gtk::Button m_color_button_pos_s;
    Gtk::Button m_color_button_pos_e;
    Gtk::SpinButton m_select_precision;
    Gtk::Grid grid;
    Gtk::Label tagSpin;

    Glib::RefPtr<Gtk::Adjustment> ajustement;
    /*Gtk::ColorSelectionDialog colorNegStart("Couleur de la valeur négative du plus petit exposant de Lyapunov calculée");
    Gtk::ColorSelectionDialog colorNegEnd("Couleur de la valeur négative du plus grand exposant de Lyapunov calculée");
    Gtk::ColorSelectionDialog colorPosStart("Couleur de la valeur positive du plus petit exposant de Lyapunov calculée");
    Gtk::ColorSelectionDialog colorPosend("Couleur de la valeur positive du plus grand exposant de Lyapunov calculée");*/


    public :

    Menu();
    void setColorMinNegExpo();
    void setColorMaxNegExpo();
    void setColorMinPosExpo();
    void setColorMaxPosExpo();
    std::string colorToString(Gdk::RGBA color);
    void getSequence(std::ofstream &file);
    void getColor(std::ofstream &file);
    void getPrecision(std::ofstream &file);
    void textArea();
    void newPrecision();






};

#endif
