#include <Gamebuino-Meta.h>

const char* entries[] = {
  "IA",
  "2_Joueurs",
  "Scores",
};

bool menu_choice = false;
Image myImg("/pong/licorne.BMP");

// Caractéristiques de la balle
int balle_posX = 20;
int balle_posY = 20;
int balle_speedX = 1;
int balle_speedY = 1;
int balle_taille = 3;
//On crée une variable pour la couleur de la balle 
Color balle_color= ORANGE;

// Caractéristiques des raquettes
//raquette gauche
int raquette1_posX = 10;
int raquette1_posY = 30;
// raquette droite
int raquette2_posX = gb.display.width() - 13;
int raquette2_posY = 30; 
// les deux raquettes
int raquette_hauteur = 10;
int raquette_largeur = 3;

// vitesse
int raquette_IA_speed = 1; // Pour l'IA
int raquette_speed = 1;

// Scores
int score1;  // Score du joueur 1
int score2;  // Score du joueur 2

int difficulte = 3;  // Niveau de difficulté. 3 = FACILE et 2 = DIFFICILE

////////////////////////////////////////////////////////////////////////////////////
//Création d'une méthode random pour la couleur 

Color createColor() { 
  // On crée les trois couleurs primaires
  int rand_color_rouge = rand() % 255; 
  int rand_color_vert = rand() % 255; 
  int rand_color_bleu = rand() % 255; 
  //On crée notre couleur random
  Color randColor= gb.createColor(rand_color_rouge,rand_color_vert,rand_color_bleu); 
  return randColor; 
  }
  
////////////////////////////////////////////////////////////////////////////////////
//création du son émis au contact de la raquette
const Gamebuino_Meta::Sound_FX my_sfx[] = { 
  {Gamebuino_Meta::Sound_FX_Wave::NOISE,1,100,2,5,96,3}, 
  {Gamebuino_Meta::Sound_FX_Wave::SQUARE,1,100,10,0,126,10}, 
  {Gamebuino_Meta::Sound_FX_Wave::SQUARE,0,120,-6,0,84,10},
  };

////////////////////////////////////////////////////////////////////////////////////
/////// Méthode de Draw initial du level ////////

void drawLevel(int level, const char* mode){

  // caractéristiques des éléments en fonction du niveau
  // difficulté EASY
  if (level == 3) {
    balle_speedX = 1;
    balle_speedY = 1;
    raquette_speed = 1;
    raquette_IA_speed = 1;
  }
  // difficulté HARD
  if (level == 2){
    balle_speedX = 2;
    balle_speedY = 2;
    raquette_speed = 2;  // Vitesse verticale de la raquette
    raquette_IA_speed = 2;
  }
  // Afficher la difficulté
  gb.display.setCursor(33, gb.display.height() - 5);
  if (level == 3) {
    gb.display.print("EASY");
  }
  else {
    gb.display.print("HARD");
  }
  // définir les comportements des raquettes 
  setPaddlesBehaviors(mode);
  
  // Afficher la balle
  gb.display.setColor(balle_color);
  gb.display.fillRect(balle_posX, balle_posY, balle_taille, balle_taille);
  gb.display.setColor(WHITE);
  // Afficher la raquette1
  gb.display.fillRect(raquette1_posX, raquette1_posY, raquette_largeur, raquette_hauteur);
  // Afficher la raquette2
  gb.display.fillRect(raquette2_posX, raquette2_posY, raquette_largeur, raquette_hauteur);

  // Afficher les scores
  gb.display.setCursor(35, 5);
  gb.display.print(score1);
  gb.display.setCursor(42, 5);
  gb.display.print(score2);

}

////////////////////////////////////////////////////////////////////
////// Méthode pour gérer le déplacement des raquettes 
void setPaddlesBehaviors(const char* mode){
  // en mode 2 joueurs
  if (mode == "2_Joueurs"){
    if (gb.buttons.repeat(BUTTON_UP, 0)) {
      raquette1_posY = raquette1_posY - raquette_speed;
      }
      if (gb.buttons.repeat(BUTTON_DOWN, 0)) {
      raquette1_posY = raquette1_posY + raquette_speed;
      }

    if (gb.buttons.repeat(BUTTON_B, 0)) {
      raquette2_posY = raquette2_posY - raquette_speed;
      }
    if (gb.buttons.repeat(BUTTON_A, 0)) {
      raquette2_posY = raquette2_posY + raquette_speed;
      }
  }
  // en mode IA
  if (mode == "IA"){
      if (balle_posY > raquette2_posY + raquette_hauteur / 2 && random(0, difficulte) == 1) {
        raquette_IA_speed = 2;
      } else if (balle_posY < raquette2_posY + raquette_hauteur / 2 && random(0, difficulte) == 1) {
        raquette_IA_speed = -2;
      }
      raquette2_posY = raquette2_posY + raquette_IA_speed;
    if (gb.buttons.repeat(BUTTON_UP, 0)) {
      raquette1_posY = raquette1_posY - raquette_speed;
      }
      if (gb.buttons.repeat(BUTTON_DOWN, 0)) {
      raquette1_posY = raquette1_posY + raquette_speed;
      }
  }
}



void setup() {
  gb.begin();
  gb.display.drawImage(0, 0, myImg); 
  delay(4000);
}
uint8_t entry;
void loop() {
  while(!gb.update());
  gb.display.clear();
  
if (!menu_choice || gb.buttons.pressed(BUTTON_MENU)){
  // display the menu
  entry = gb.gui.menu("Choix mode de jeu :", entries);
  gb.display.clear();
  menu_choice = true;
 }
 
  drawLevel(difficulte, entries[entry]);


////////////////////////////////////////////////////////////////////////////////
///// Changement de level et enregistrement nom player vainqueur
  if(score1 == 6 || score2 == 6){
    /*pong_ia;*/
    char playerName[13];
    gb.getDefaultName(playerName);
    gb.gui.keyboard("Nouveau score!", playerName);
  }
  
////////////////////////////////////////////////////////////////////////////////
 // Changement de difficulté
 // if (gb.buttons.pressed(BUTTON_MENU)) {
 //   if (difficulte == 3) { // Facile
 //     difficulte = 2;  // Changer de difficulté
 //   }
 //   else {  // Difficile
 //     difficulte = 3;  // Changer de difficulté
 //   }
 // }

  
////////////////////////////////////////////////////////////////////////////////
  // déplacement balle
  balle_posX = balle_posX + balle_speedX;
  balle_posY = balle_posY + balle_speedY;
  
////////////////////////////////////////////////////////////////////////////////
  // Collisions avec les murs (haut et bas)
  if ((balle_posY < 0) || (balle_posY > gb.display.height() - balle_taille)) {
    balle_speedY = -balle_speedY;
  }
  
////////////////////////////////////////////////////////////////////////////////
// Collision avec la raquette gauche
  if ( (balle_posX == raquette1_posX + raquette_largeur)
       && (balle_posY + balle_taille >= raquette1_posY)
       && (balle_posY <= raquette1_posY + raquette_hauteur) ) {
    balle_speedX = 1;
    //On change la couleur de la variable color de la balle après impact avec la raquette gauche avec notre random
     balle_color = createColor();
     gb.lights.fill(balle_color = createColor());
     gb.sound.fx(my_sfx);

  }
  // Collision avec la raquette droite
  if ( (balle_posX + balle_taille == raquette2_posX)
       && (balle_posY + balle_taille >= raquette2_posY)
       && (balle_posY <= raquette2_posY + raquette_hauteur) ) {
    balle_speedX = -1;
  //On change la couleur de la variable color de la balle après impact avec la raquette gauche avec notre random
    balle_color = createColor();
    gb.lights.fill(balle_color = createColor());
    gb.sound.fx(my_sfx);
  }


////////////////////////////////////////////////////////////////////////////////
  // Vérifier si la balle est sortie de l'écran
  if (balle_posX < 0) {
    // Replacer la balle sur l'écran
    balle_posX = 20;
    balle_posY = random(20, gb.display.height() - 20);  // Position aléatoire au centre de l'écran
    balle_speedX = 1;
    if (random(0, 2) == 1) {  // 50% du temps
      balle_speedY = 1;
    } 
    else {  // 50% du temps
      balle_speedY = -1;
    }

////////////////////////////////////////////////////////////////////////////////
    // incrémenter le score du joueur 2
    score2 = score2 + 1;
  }
  if (balle_posX > gb.display.width()) {
    // Replacer la balle sur l'écran
    balle_posX = 20;
    balle_posY = random(20, gb.display.height() - 20);  // Position aléatoire au centre de l'écran
    balle_speedX = 1;
    if (random(0, 2) == 1) {  // 50% du temps
      balle_speedY = 1;
    } 
    else {  // 50% du temps
      balle_speedY = -1;
    }
////////////////////////////////////////////////////////////////////////////////
    ////////// incrémenter le score du joueur 1  //////////
    score1 = score1 + 1;
  }













  
}
