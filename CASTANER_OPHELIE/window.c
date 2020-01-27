#include <stdlib.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4dh.h>
#include <GL4D/gl4duw_SDL2.h>
#include "animations.h"
#include "audioHelper.h"
#include <SDL_ttf.h>

/* Prototypes des fonctions statiques contenues dans ce fichier C. */
static void init(void);
static void quit(void);
static void resize(int w, int h);
static void keydown(int keycode);

/*!\brief tableau contenant les animations sous la forme de timeline,
 * ce tableau se termine toujours par l'élémént {0, NULL, NULL,
 * NULL} */
static GL4DHanime _animations[] = {

{ 4000,effet_bulles,NULL,NULL}, //1
{ 15000,effet_bulles,grossissement,fondui},//2
{ 10000,effet_bulles,cercles_descendants,fondu},//3
{10000,effet_bulles,rect_cir,fondui},//4
{15000,effet_bulles,circulaire,fondui},//5
{ 1000,effet_bulles,grossissement,fondu},//6
{ 10000,effet_bulles,cercles_descendants,fondui},//7
{ 13000,effet_bulles,cercles_montants,fondu},//8
{ 4000, effet_bulles, NULL, NULL},//9
{ 3000, canal,NULL, NULL},//10
{ 10000, transformations, NULL,NULL },//11
{ 15000, canal,transformations,fondui },//12
{ 10000, tourner_sphere,NULL,NULL },//13
{ 15000, canal,tourner_sphere,fondui },//14
{15000,transformations,tourner_sphere,fondu},//15
{ 25000,identite, NULL, NULL },//16
  {    0, NULL, NULL, NULL } /* Toujours laisser à la fin */
};

/*!\brief dimensions initiales de la fenêtre */
//static GLfloat _dim[] = {1024, 768};
static GLfloat _dim[] = {1280, 900};

/*!\brief fonction principale : initialise la fenêtre, OpenGL, audio
 * et lance la boucle principale (infinie).
 */
int main(int argc, char ** argv) {
  if(!gl4duwCreateWindow(argc, argv, "GL4Dummies DemoHelper", 
			 0, 0, 
			 _dim[0], _dim[1],
			 GL4DW_RESIZABLE | /* GL4DW_FULLSCREEN | */GL4DW_SHOWN))
    return 1;
  if(TTF_Init() == -1) {
    fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
    exit(2);
  }
  init();
  atexit(quit);
  gl4duwResizeFunc(resize);
  gl4duwKeyDownFunc(keydown);
  gl4duwDisplayFunc(gl4dhDraw);

  ahInitAudio("audio/takeonme.mod");
  gl4duwMainLoop();
  return 0;
}

/*!\brief Cette fonction initialise les paramètres et éléments OpenGL
 * ainsi que les divers données et fonctionnalités liées à la gestion
 * des animations.
 */
static void init(void) {
  glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
  gl4dhInit(_animations, _dim[0], _dim[1], animationsInit);
  resize(_dim[0], _dim[1]);
}

/*!\brief paramétre la vue (viewPort) OpenGL en fonction des
 * dimensions de la fenêtre.
 * \param w largeur de la fenêtre.
 * \param w hauteur de la fenêtre.
 */
static void resize(int w, int h) {
  _dim[0] = w; _dim[1] = h;
  glViewport(0, 0, _dim[0], _dim[1]);
}

/*!\brief permet de gérer les évènements clavier-down.
 * \param keycode code de la touche pressée.
 */
static void keydown(int keycode) {
  switch(keycode) {
  case SDLK_ESCAPE:
  case 'q':
    exit(0);
  default: break;
  }
}

/*!\brief appelée à la sortie du programme (atexit).
 */
static void quit(void) {
  ahClean();
  gl4duClean(GL4DU_ALL);
}
