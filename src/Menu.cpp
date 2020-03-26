#include "Menu.h"

Menu::Menu() : m_color_button_neg_s((Gtk::Stock::SELECT_COLOR)), m_color_button_neg_e((Gtk::Stock::SELECT_COLOR)), m_color_button_pos_s((Gtk::Stock::SELECT_COLOR)),m_color_button_pos_e(Gtk::Stock::SELECT_COLOR){
  Menu::set_position(Gtk::WIN_POS_CENTER);
  Menu::resize(450, 300);
  Menu::unmaximize();

  grid.set_row_homogeneous(true);
  grid.set_column_homogeneous(true);
  grid.set_column_spacing(0);
  grid.set_row_spacing(0);


  Menu::set_title("Paramètres de la génération des fractales de Lyapunov");
   m_color_button_neg_s.set_can_focus(false);
   m_color_button_neg_s.set_alignment(0.1,0.5);
   m_color_button_neg_s.set_label("Couleur Exposant Lyapunov Négatif loin de 0 : ████");
   m_color_button_neg_s.signal_clicked().connect(sigc::mem_fun(*this, &Menu::setColorMaxNegExpo));
   grid.attach(m_color_button_neg_s,0,0,10,1);

   m_color_button_neg_e.set_can_focus(false);
   m_color_button_neg_e.set_alignment(0.1,0.5);
   m_color_button_neg_e.set_label("Couleur Exposant Lyapunov Négatif proche de 0 : ████");
   m_color_button_neg_e.signal_clicked().connect(sigc::mem_fun(*this, &Menu::setColorMinNegExpo));
   grid.attach(m_color_button_neg_e,0,1,10,1);


   m_color_button_pos_s.set_can_focus(false);
   m_color_button_pos_s.set_alignment(0.1,0.5);
   m_color_button_pos_s.set_label("Couleur Exposant Lyapunov Positif proche de 0 : ████");
   m_color_button_pos_s.signal_clicked().connect(sigc::mem_fun(*this, &Menu::setColorMinPosExpo));
   grid.attach(m_color_button_pos_s,0,2,10,1);


   m_color_button_pos_e.set_can_focus(false);
   m_color_button_pos_e.set_alignment(0.1,0.5);
   m_color_button_pos_e.set_label("Couleur Exposant Lyapunov Positif loin de 0 : ████");
   m_color_button_pos_e.signal_clicked().connect(sigc::mem_fun(*this, &Menu::setColorMaxPosExpo));
   grid.attach(m_color_button_pos_e,0,3,10,1);

   Gtk::Label tagSpin("Précision :");
   tagSpin.set_alignment(Gtk::ALIGN_END);
   tagSpin.override_color(Gdk::RGBA("red"));
   Glib::RefPtr<Gtk::Adjustment> limits = Gtk::Adjustment::create(100, 10, 500, 1);
   m_select_precision.set_placeholder_text("Précision du calcul de l'exposant de Lyapunov");
   m_select_precision.set_adjustment(limits);
   m_select_precision.set_numeric();
   m_select_precision.signal_value_changed().connect(sigc::mem_fun(*this, &Menu::newPrecision));
   grid.attach(tagSpin,0,4,3,1);
   grid.attach(m_select_precision,0,4,10,1);

   precision = m_select_precision.get_value_as_int();
   sequence.set_activates_default(false);

   Menu::textArea();
   grid.show_all();
   add(grid);

}
void Menu::setColorMaxNegExpo(){
  Gtk::ColorSelectionDialog color("Couleur de la valeur négative du plus petit exposant de Lyapunov calculée ");

  Gtk::ColorSelection* selectColor = color.get_color_selection();
  selectColor->set_current_rgba(m_color[0]);
  int end = color.run();
  if(end == Gtk::RESPONSE_OK) {
    m_color[0] = selectColor->get_current_rgba();
    m_color_button_neg_s.override_color(m_color[0],Gtk::StateFlags::STATE_FLAG_NORMAL);
  }
}
void Menu::textArea(){
  sequence.set_activates_default(false);
  sequence.set_placeholder_text("Entrez la séquence voulue de A et B");
  sequence.set_max_length(precision);
  sequence.set_alignment(Gtk::ALIGN_CENTER);
  grid.attach(sequence,0,5,10,1);


}

void Menu::setColorMinNegExpo(){
  Gtk::ColorSelectionDialog color("Couleur de la valeur négative du plus grand exposant de Lyapunov calculée ");

  Gtk::ColorSelection* selectColor = color.get_color_selection();
  selectColor->set_current_rgba(m_color[1]);
  int end = color.run();
  if(end == Gtk::RESPONSE_OK) {
    m_color[1] = selectColor->get_current_rgba();
    m_color_button_neg_e.override_color(m_color[1],Gtk::StateFlags::STATE_FLAG_NORMAL);
  }
}

void Menu::setColorMinPosExpo(){
  Gtk::ColorSelectionDialog color("Couleur de la valeur négative du plus grand exposant de Lyapunov calculée ");

  Gtk::ColorSelection* selectColor = color.get_color_selection();
  selectColor->set_current_rgba(m_color[2]);
  int end = color.run();
  if(end == Gtk::RESPONSE_OK) {
    m_color[2] = selectColor->get_current_rgba();
    m_color_button_pos_s.override_color(m_color[2],Gtk::StateFlags::STATE_FLAG_NORMAL);
  }
}

void Menu::setColorMaxPosExpo(){
  Gtk::ColorSelectionDialog color("Couleur de la valeur négative du plus grand exposant de Lyapunov calculée ");

  Gtk::ColorSelection* selectColor = color.get_color_selection();
  selectColor->set_current_rgba(m_color[3]);
  int end = color.run();
  if(end == Gtk::RESPONSE_OK) {
    m_color[3] = selectColor->get_current_rgba();
    m_color_button_pos_e.override_color(m_color[3],Gtk::StateFlags::STATE_FLAG_NORMAL);
  }
}

void Menu::getSequence(std::ofstream& file){
  std::string seq = sequence.get_text();
  file << "sequence= "+seq << std::endl;
}

void Menu::newPrecision(){
  m_select_precision.set_text(m_select_precision.get_text());
}

// ECRIRE LES COULEURS  DANS UN FICHIER

std::string Menu::colorToString(Gdk::RGBA color){
  std::stringstream convertRed;
  std::stringstream convertGreen;
  std::stringstream convertBlue;
  convertRed << (int)(color.get_red()*255);
  convertGreen << (int)(color.get_green()*255);
  convertBlue << (int)(color.get_blue()*255);
  std::string str = convertRed.str() + " " + convertGreen.str() + " " + convertBlue.str();
  return str;
}


void Menu::getColor(std::ofstream& file){

  file << "color_neg_+= "+ colorToString(m_color[0]) << std::endl;
  file << "color_neg_-= "+ colorToString(m_color[1]) << std::endl;
  file << "color_pos_-= "+ colorToString(m_color[2]) << std::endl;
  file << "color_pos_+= "+ colorToString(m_color[3]) << std::endl;
}

void Menu::getPrecision(std::ofstream& file){
  std::stringstream precis;
  precis << m_select_precision.get_value();
  file << "precision= "+ precis.str()<< std::endl;
}

int main(int argc, char* argv[]) {
    Gtk::Main app(argc, argv);
    Menu m=Menu();
    Gtk::Main::run(m);
    std::ofstream file("../config.txt");

    if (!file.is_open()){
      std::cout << "Error : Cannot open file config " <<std::endl;
      return -1;
    }
    m.getSequence(file);
    m.getColor(file);
    m.getPrecision(file);
    //std::cout << m.getSequence() << std::endl;
    return 0;
}