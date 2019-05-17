#include <Gamebuino-Meta.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const char* entries[] = {
  "IA",
  "2_Joueurs",
  "Scores",
};

typedef enum directions{
  UP, // 0
  DOWN,  // 1
  RIGHT,  // 2
  LEFT  // 3
} Sens;

bool menu_choice = false;
bool congrats = false;
Image myImg("/pong/licorne.BMP");

// Caractéristiques de la balle
int balle_posX = 20;
int balle_posY = 20;
Sens directionX = RIGHT;
Sens directionY = UP;
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
int score1 = 0;  // Score du joueur 1
int score2 = 0;  // Score du joueur 2

int level = 3;  // Niveau de difficulté. 3 = FACILE et 2 = DIFFICILE

////////////////////////////////////////////////////////////////////////////////////
//Création d'une méthode pour générer un random borné
// le random semblait un peu lazy
int rand_born(int a, int b){
  // permet d'avoir plus de variabilité avec le rand()
    //srand(time(NULL));
    // retourne un rand entre a < b
    return rand()%(b-a) +a;
}

//Création d'une méthode pour random une direction Y
Sens rand_Y(){
  if (random(0, 2) == 1) { 
    // 50% of the time, this is true 
    return UP;
    } 
  else { // Other 50% of the time 
    return DOWN; 
    }
}

//Création d'une méthode pour random une direction X
Sens rand_X(){
  if (random(0, 2) == 1) { 
    // 50% of the time, this is true 
    return RIGHT;
    } 
  else { // Other 50% of the time 
    return LEFT; 
    }
}


////////////////////////////////////////////////////////////////////////////////////
//Création d'une méthode pour marquer un point
int increment(int scorePlayer) {
  scorePlayer = scorePlayer + 1;
  return scorePlayer;
 }

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

////////////////////////////////////////////////////////////////////////////////
  // déplacement balle
    void moveBall(Sens sensY, Sens sensX) {
    if(sensY == UP) {
      balle_posY = balle_posY - balle_speedY;
      directionY = UP;
    }if(sensY == DOWN) {
      balle_posY = balle_posY + balle_speedY;
      directionY = DOWN;
    }
    if(sensX == RIGHT) {
      balle_posX = balle_posX + balle_speedX;
      directionX = RIGHT;
    }
    if(sensX == LEFT) {
      balle_posX = balle_posX - balle_speedX;
      directionX = LEFT;
    }
  }

  void moveBallY(Sens sensY) {
    if(sensY == UP) {
      balle_posY = balle_posY - balle_speedY;
    }if(sensY == DOWN) {
      balle_posY = balle_posY + balle_speedY;
    }
  }

    void moveBallX(Sens sensX) {
    if(sensX == RIGHT) {
      balle_posX = balle_posX + balle_speedX;
    }
    if(sensX == LEFT) {
      balle_posX = balle_posX - balle_speedX;
    }
  }
////////////////////////////////////////////////////////////////////////////////////
/////// Méthode de Draw initial du level ////////

void drawLevel(int level, const char* mode){
  // désactiver l'affichage de l'écran des félicitations
  congrats = false;
  
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
  moveBall(directionY, directionX);
}

////////////////////////////////////////////////////////////////////
////// Méthode pour dessiner un écran de félicitations entre les menus 
void drawCongrats(int winner){
    /*pong_ia;*/
  // char playerName[13];
  // gb.getDefaultName(playerName);
  // gb.gui.keyboard("Nouveau score!", playerName);
  // gb.display.print("Bravo ");
  // gb.display.print(playerName);
    
   // gb.display.clear();
    gb.display.print("Bravo joueur");
    gb.display.print(winner);
    gb.display.print("\nTu as gagné le niveau : ");
    if (level == 2) {
      gb.display.print("EASY");
    }
    else {
      gb.display.print("HARD");
    }
}
////////////////////////////////////////////////////////////////////
////// Méthode pour gérer le déplacement des raquettes 
void setPaddlesBehaviors(const char* mode){
   if ( (gb.buttons.repeat(BUTTON_UP, 0)) 
      && (raquette1_posY > 0)) {
      raquette1_posY = raquette1_posY - raquette_speed;
      }
    if ((gb.buttons.repeat(BUTTON_DOWN, 0)) 
      && (raquette1_posY + raquette_hauteur < gb.display.height()-1)) {
      raquette1_posY = raquette1_posY + raquette_speed;
      }
  // en mode 2 joueurs
  if (mode == "2_Joueurs"){
    if ((gb.buttons.repeat(BUTTON_B, 0))
      && (raquette2_posY > 0)) {
      raquette2_posY = raquette2_posY - raquette_speed;
      }
    if ((gb.buttons.repeat(BUTTON_A, 0))
    && (raquette2_posY + raquette_hauteur < gb.display.height()-1)){
      raquette2_posY = raquette2_posY + raquette_speed;
      }
  }
  // en mode IA
  
  // TODO : INSTAURER DES LIMITES DE DÉPLACEMENT HAUT ET BAS : BORDS ÉCRAN POUR L'IA 
  // => OK à tester quand le rebond de la balle sera ok sur les bords haut et bas de l'écran
  
  if (mode == "IA"){
    if (balle_posY > raquette2_posY + raquette_hauteur / 2 && random(0, level) == 1) {
        raquette_IA_speed = raquette_IA_speed;
      } 
    else if (balle_posY < raquette2_posY + raquette_hauteur / 2 && random(0, level) == 1) {
        raquette_IA_speed = -raquette_IA_speed;
      }
      if( (raquette2_posY > 0) && (raquette2_posY + raquette_hauteur < gb.display.height()-1)) {
      raquette2_posY = raquette2_posY + raquette_IA_speed;
    }
  }
}

////////////////////////////////////////////////////////////////////
////// Méthode pour switcher de level 
int switchLevel(int level) {
      if (level == 3) { // Facile
      level = 2;  // Changer de difficulté
    }
    else {  // Difficile
      level = 3;  // Changer de difficulté
    }
    return level;
}

void setup() {
  gb.begin();
  gb.display.drawImage(0, 0, myImg); 
  delay(1000);
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
else if(congrats){
    level = switchLevel(level);
    int winner = (score1 > score2 ? 1 : 2);
    drawCongrats(winner);
    delay(1000);
    score1 = 0 ;
    score2 = 0 ;
    delay(1000);
    drawLevel(level, entries[entry]);

}
 else {
  drawLevel(level, entries[entry]);


////////////////////////////////////////////////////////////////////////////////
///// Changement de level et enregistrement nom player vainqueur
  if(score1 == 6 || score2 == 6){
    congrats = true;
  }


  
////////////////////////////////////////////////////////////////////////////////
 // Changement de difficulté quand on appuie sur le bouton GAUCHE
  if (gb.buttons.pressed(BUTTON_LEFT)) {
    level = switchLevel(level);
  }


////////////////////////////////////////////////////////////////////////////////
  // Collisions avec les murs (haut et bas)
 // haut
 if (balle_posY == 0) {
    moveBall(DOWN, directionX);
 }
 // bas
 if (balle_posY + balle_taille > gb.display.height()) {
    moveBall(UP, directionX);
 }

////////////////////////////////////////////////////////////////////////////////
  // replacer la balle + SCORER sur l'écran après la sortie à droite ou à gauche
  // gauche
  if (balle_posX + balle_taille < 0) {
    // Replacer la balle sur l'écran
    balle_posX = raquette1_posX + raquette_largeur + 10;
    balle_posY = rand_born(20, gb.display.height() - 20);
    // Position aléatoire en Y au centre de l'écran
    moveBall(rand_Y(), RIGHT);
    score2 = increment(score2);
  }
  // replacer la balle + SCORER sur l'écran après la sortie à droite ou à gauche
  // droite
  if (balle_posX > gb.display.width()) { 
    // Replacer la balle sur l'écran
    balle_posX = raquette2_posX - 10;
    balle_posY = rand_born(20, gb.display.height() - 20);
    moveBall(rand_Y(), LEFT);
    score1 = increment(score1);
 } 
  
//////////////////////////////////////////////////////////////////////////////// SEVERINE
// Collision avec la raquette gauche
  if ( (balle_posX == raquette1_posX + raquette_largeur)
       && (balle_posY + balle_taille >= raquette1_posY)
       && (balle_posY <= raquette1_posY + raquette_hauteur) ) {
    moveBall(directionY, RIGHT);
    //On change la couleur de la variable color de la balle après impact avec la raquette gauche avec notre random
     balle_color = createColor();
     gb.lights.fill(balle_color = createColor());
     gb.sound.fx(my_sfx);

  }
  // Collision avec la raquette droite
  if ( (balle_posX + balle_taille == raquette2_posX)
       && (balle_posY + balle_taille >= raquette2_posY)
       && (balle_posY <= raquette2_posY + raquette_hauteur) ) {
    moveBall(directionY, LEFT);
  //On change la couleur de la variable color de la balle après impact avec la raquette gauche avec notre random
    balle_color = createColor();
    gb.lights.fill(balle_color = createColor());
    gb.sound.fx(my_sfx);
  }






 }

  
}
