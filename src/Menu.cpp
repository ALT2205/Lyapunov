#include "Menu.h"


// Constructeur de la classe Menu : crée les différents boutons nécessaires à la séléction des couleurs.
Menu::Menu() : m_color_button_neg_s((Gtk::Stock::SELECT_COLOR)), m_color_button_neg_e((Gtk::Stock::SELECT_COLOR)), m_color_button_pos_s((Gtk::Stock::SELECT_COLOR)),m_color_button_pos_e(Gtk::Stock::SELECT_COLOR){

  // Créer une fenêtre centrée de taille 450 * 300
  Menu::set_position(Gtk::WIN_POS_CENTER);
  Menu::resize(450, 300);


  // Chaque colonne et chaque ligne fait la même taille que les autres colonnes/lignes
  grid.set_row_homogeneous(true);
  grid.set_column_homogeneous(true);
  // Espacement entre les différentes colonnes/lignes
  grid.set_column_spacing(0);
  grid.set_row_spacing(0);

  // Nom de la fenêtre
  Menu::set_title("Paramètres de la génération des fractales de Lyapunov");

  //Positionnement des labels et du texte au sein du bouton
   m_color_button_neg_s.set_can_focus(false);
   m_color_button_neg_s.set_alignment(0.1,0.5);
   m_color_button_neg_s.set_label("Couleur Exposant Lyapunov Négatif loin de 0 : ████");
   // Evenement produit en cas de clic sur le Bouton
   m_color_button_neg_s.signal_clicked().connect(sigc::mem_fun(*this, &Menu::setColorMaxNegExpo));
   // Position du bouton dans la grille : Position 0 0 de largeur 10 et de hauteur 1
   grid.attach(m_color_button_neg_s,0,0,10,1);


   //Positionnement des labels et du texte au sein du bouton
   m_color_button_neg_e.set_can_focus(false);
   m_color_button_neg_e.set_alignment(0.1,0.5);
   m_color_button_neg_e.set_label("Couleur Exposant Lyapunov Négatif proche de 0 : ████");
   // Evenement produit en cas de clic sur le Bouton
   m_color_button_neg_e.signal_clicked().connect(sigc::mem_fun(*this, &Menu::setColorMinNegExpo));
   // Position du bouton dans la grille : Position 0 1 de largeur 10 et de hauteur 1
   grid.attach(m_color_button_neg_e,0,1,10,1);

   //Positionnement des labels et du texte au sein du bouton
   m_color_button_pos_s.set_can_focus(false);
   m_color_button_pos_s.set_alignment(0.1,0.5);
   m_color_button_pos_s.set_label("Couleur Exposant Lyapunov Positif proche de 0 : ████");
   // Evenement produit en cas de clic sur le Bouton
   m_color_button_pos_s.signal_clicked().connect(sigc::mem_fun(*this, &Menu::setColorMinPosExpo));
   // Position du bouton dans la grille : Position 0 2 de largeur 10 et de hauteur 1
   grid.attach(m_color_button_pos_s,0,2,10,1);

   //Positionnement des labels et du texte au sein du bouton
   m_color_button_pos_e.set_can_focus(false);
   m_color_button_pos_e.set_alignment(0.1,0.5);
   m_color_button_pos_e.set_label("Couleur Exposant Lyapunov Positif loin de 0 : ████");
   // Evenement produit en cas de clic sur le Bouton
   m_color_button_pos_e.signal_clicked().connect(sigc::mem_fun(*this, &Menu::setColorMaxPosExpo));
   // Position du bouton dans la grille : Position 0 3 de largeur 10 et de hauteur 1
   grid.attach(m_color_button_pos_e,0,3,10,1);


   // Création des limites pour le Spin Button : Valeur initiale, valeur minimale/maximale, pas
   Glib::RefPtr<Gtk::Adjustment> limits = Gtk::Adjustment::create(100, MINSPIN, MAXSPIN, 1);
   // Définie la limite du bouton
   m_select_precision.set_adjustment(limits);
   // Seulement des nombres peuvent être rentrés
   m_select_precision.set_numeric();
   // Evenement en cas de clic sur le bouton
   m_select_precision.signal_value_changed().connect(sigc::mem_fun(*this, &Menu::newPrecision));
   // Position du bouton dans la grille : Position 0 4 de largeur 10 et de hauteur 1
   grid.attach(m_select_precision,0,4,10,1);

   // Recupère la valeur du SpinButton pour l'affecter dans l'attribut Précision
   precision = m_select_precision.get_value_as_int();

   //Le champ de saisie de la séquence n'est pas activé par defaut pour afficher le place holder
   sequence.set_activates_default(false);

   Menu::textArea();
   // Affiche les différents éléments présents dans la grille
   grid.show_all();

   //Ajout de la grille dans le Menu
   add(grid);

}
void Menu::setColorMaxNegExpo(){
  // Titre de la boite de dialogue de sélection de couleur
  Gtk::ColorSelectionDialog color("Couleur de la valeur négative du plus petit exposant de Lyapunov calculée ");

  // Ouverture de la boite de dialogue
  Gtk::ColorSelection* selectColor = color.get_color_selection();
  // Selection de la couleur
  selectColor->set_current_rgba(m_color[0]);
  int end = color.run();
  // Fermeture de la boîte si l'utilisateur est satisfait de son choix
  if(end == Gtk::RESPONSE_OK) {
    m_color[0] = selectColor->get_current_rgba();
    // Remplace la couleur du label à l'intérieur du bouton par la couleur saisie par l'utilisateur
    m_color_button_neg_s.override_color(m_color[0],Gtk::StateFlags::STATE_FLAG_NORMAL);
  }
}

// Comportement identique à la méthode Menu::setColorMaxNegExpo()
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

// Comportement identique à la méthode Menu::setColorMaxNegExpo()
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

// Comportement identique à la méthode Menu::setColorMaxNegExpo()
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
// Champ de saisie de la séquence
void Menu::textArea(){
  sequence.set_activates_default(false);
  sequence.set_placeholder_text("Entrez la séquence voulue de A et B");
  sequence.set_max_length(precision);
  // Alignement du texte au centre du champ de saisie
  sequence.set_alignment(Gtk::ALIGN_CENTER);
  // Position du bouton dans la grille : Position 0 5 de largeur 10 et de hauteur 1
  grid.attach(sequence,0,5,10,1);
}

// Ecriture de la séquence rentrée dans un fichier de config
void Menu::getSequence(std::ofstream& file){
  std::string seq = sequence.get_text();
  file << "sequence= "+seq;
}

// Remplace l'ancienne valeur de la précision par la nouvelle
void Menu::newPrecision(){
  m_select_precision.set_text(m_select_precision.get_text());
}

// Convertit une Gdk::RGBA en une chaine de caractères afin d'être exploitée plus tard
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

// Ecriture dans le fichier de config des différentes couleurs de l'utilisateur
void Menu::getColor(std::ofstream& file){
  file << "color_neg_+= "+ colorToString(m_color[0]) << std::endl;
  file << "color_neg_-= "+ colorToString(m_color[1]) << std::endl;
  file << "color_pos_-= "+ colorToString(m_color[2]) << std::endl;
  file << "color_pos_+= "+ colorToString(m_color[3]) << std::endl;
}

// Ecriture dans le fichier de config de la précision souhaitée
void Menu::getPrecision(std::ofstream& file){
  std::stringstream precis;
  precis << m_select_precision.get_value();
  file << "precision= "+ precis.str()<< std::endl;
}


int Menu::writeFile(){
  std::ofstream file("config.txt");
  if (!file.is_open()){
    std::cout << "Error : Cannot open file config " <<std::endl;
    return -1;
  }
  getColor(file);
  getPrecision(file);
  getSequence(file);
  return 0;
}
