#include "Menu.h"


// Constructeur de la classe Menu : crée les différents boutons nécessaires à la séléction des couleurs.
Menu::Menu() : m_color_button_neg_s(), m_color_button_neg_e(), m_color_button_pos_s(),m_color_button_pos_e(),menu_write(){

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

    //Configuration des labels et des boutons de couleurs
  label_expoNegE = Gtk::AccelLabel("Borne Minimale Négative");
  label_expoNegE.set_xalign(0.4);

   m_color_button_neg_e.set_can_focus(false);
   m_color_button_neg_e.set_alignment(0.1,0.5);
   m_color_button_neg_e.set_title("Couleur Exposant Lyapunov Négatif loin de 0");

   // Position du label dans la grille : Position 0 0 de largeur 4 et de hauteur 1
   // Position du bouton dans la grille : Position 4 0 de largeur 6 et de hauteur 1

   grid.attach(m_color_button_neg_e,4,0,6,1);
   grid.attach(label_expoNegE,0,0,4,1);



    //Configuration des labels et des boutons de couleurs
   label_expoNegS = Gtk::AccelLabel("Borne Maximale Négative");
   label_expoNegS.set_xalign(0.409);

   m_color_button_neg_s.set_can_focus(false);
   m_color_button_neg_s.set_alignment(0.1,0.5);
   m_color_button_neg_s.set_title("Couleur Exposant Lyapunov Négatif proche de 0");

    // Position du label dans la grille : Position 0 1 de largeur 4 et de hauteur 1
    // Position du bouton dans la grille : Position 4 1 de largeur 6 et de hauteur 1
   grid.attach(m_color_button_neg_s,4,1,6,1);
   grid.attach(label_expoNegS,0,1,4,1);

    //Configuration des labels et des boutons de couleurs
   label_expoPosS = Gtk::AccelLabel("Borne Minimale Positive");
   label_expoPosS.set_xalign(0.395);

   m_color_button_pos_s.set_can_focus(false);
   m_color_button_pos_s.set_alignment(0.1,0.5);
   m_color_button_pos_s.set_title("Couleur Exposant Lyapunov Positif proche de 0");

    // Position du label dans la grille : Position 0 2 de largeur 4 et de hauteur 1
    // Position du bouton dans la grille : Position 4 2 de largeur 6 et de hauteur 1
   grid.attach(m_color_button_pos_s,4,2,6,1);
   grid.attach(label_expoPosS,0,2,4,1);


   //Configuration des labels et des boutons de couleurs
   label_expoPosE = Gtk::AccelLabel("Borne Maximale Positive");
   label_expoPosE.set_xalign(0.4);

   m_color_button_pos_e.set_can_focus(false);
   m_color_button_pos_e.set_alignment(0.1,0.5);
   m_color_button_pos_e.set_title("Couleur Exposant Lyapunov Positif loin de 0");

    // Position du label dans la grille : Position 0 3 de largeur 4 et de hauteur 1
    // Position du bouton dans la grille : Position 4 3 de largeur 6 et de hauteur 1
   grid.attach(m_color_button_pos_e,4,3,6,1);
   grid.attach(label_expoPosE,0,3,4,1);

   // Permet d'appeler les fonctions en cas de changement de couleurs sur chacun des boutons de couleurs
   m_color_button_neg_e.signal_color_set().connect(sigc::mem_fun(*this, &Menu::setColorMinNegExpo));
   m_color_button_neg_s.signal_color_set().connect(sigc::mem_fun(*this, &Menu::setColorMaxNegExpo));
   m_color_button_pos_s.signal_color_set().connect(sigc::mem_fun(*this, &Menu::setColorMinPosExpo));
   m_color_button_pos_e.signal_color_set().connect(sigc::mem_fun(*this, &Menu::setColorMaxPosExpo));


    //Configuration du label et du bouton de précision
    label_precision = Gtk::AccelLabel("Précision");
    label_precision.set_xalign(0.1);

   // Création des limites pour le Spin Button : Valeur initiale, valeur minimale/maximale, pas
   Glib::RefPtr<Gtk::Adjustment> limits = Gtk::Adjustment::create(100, MINSPIN, MAXSPIN, 1);
   // Définie la limite du bouton
   m_select_precision.set_adjustment(limits);
   // Seulement des nombres peuvent être rentrés
   m_select_precision.set_numeric();
   // Evenement en cas de clic sur le bouton : Appel de la fonction newPrecision
   m_select_precision.signal_value_changed().connect(sigc::mem_fun(*this, &Menu::newPrecision));

    // Position du label dans la grille : Position 0 4 de largeur 4 et de hauteur 1
    // Position du bouton dans la grille : Position 4 4 de largeur 6 et de hauteur 1
   grid.attach(m_select_precision,4,4,6,1);
   grid.attach(label_precision,0,4,4,1);

   // Recupère la valeur du SpinButton pour l'affecter dans l'attribut Précision
   precision = m_select_precision.get_value_as_int();

   // Le champ de saisie de la séquence n'est pas activé par defaut pour afficher le place holder
   sequence.set_activates_default(false);

    // Configuration du label et de la saisie de la séquence
   label_sequence = Gtk::AccelLabel("Séquence de A-B");
   label_sequence.set_xalign(0.195);
    // Permet de réaliser la mise en forme du bouton de saisie de la séquence
    Menu::textArea();

    // Position du label dans la grille : Position 0 5 de largeur 4 et de hauteur 1
   grid.attach(label_sequence,0,5,4,1);


   // Evenement produit en cas de clic sur le bouton valider : Ecrit la configuration voulue par l'utilisateur dans le fichier de config
   menu_write.signal_clicked().connect(sigc::mem_fun(*this, &Menu::writeFile));
   menu_write.set_label("Valider");
    // Position du label dans la grille : Position 8 6 de largeur 2 et de hauteur 1
   grid.attach(menu_write,8,6,2,1);




   // Affiche les différents éléments présents dans la grille
   grid.show_all();

   //Ajout de la grille dans le Menu
   add(grid);
}

// Fonction appelée lors d'un changement de couleur sur le bouton de la borne minimale négative
void Menu::setColorMinNegExpo(){
    m_color[0] = m_color_button_neg_e.get_rgba();
    // std::cout << m_color[0].to_string() << std::endl;
}
// Fonction appelée lors d'un changement de couleur sur le bouton de la borne maximale négative
void Menu::setColorMaxNegExpo(){
    m_color[1] = m_color_button_neg_s.get_rgba();
    // std::cout << m_color[1].to_string() << std::endl;
}
// Fonction appelée lors d'un changement de couleur sur le bouton de la borne minimale positive
void Menu::setColorMinPosExpo(){
    m_color[2] = m_color_button_pos_s.get_rgba();
    // std::cout << m_color[2].to_string() << std::endl;
}
// Fonction appelée lors d'un changement de couleur sur le bouton de la borne maximale positive
void Menu::setColorMaxPosExpo(){
    m_color[3] = m_color_button_pos_e.get_rgba();
    // std::cout << m_color[3].to_string() << std::endl;
}

// Champ de saisie de la séquence
void Menu::textArea(){
  sequence.set_activates_default(false);
  sequence.set_placeholder_text("Entrez la séquence voulue de A et B");
  sequence.set_max_length(precision);
  // Alignement du texte au centre du champ de saisie
  sequence.set_alignment(Gtk::ALIGN_CENTER);
  // Position du bouton dans la grille : Position 4 5 de largeur 6 et de hauteur 1
  grid.attach(sequence,4,5,6,1);
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


void Menu::writeFile(){
  std::ofstream file("config.txt");
  if (!file.is_open()){
    std::cout << "Error : Cannot open file config " <<std::endl;
    return;
  }
  getColor(file);
  getPrecision(file);
  getSequence(file);
  return;
}
