/*!\file window.c
 * \brief géométries lumière diffuse et transformations de base en GL4Dummies
 * \author Farès BELHADJ, amsi@ai.univ-paris8.fr
 * \date April 15 2016 */
#include <stdio.h>
#include <GL4D/gl4dp.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4dh.h>
#include <GL4D/gl4df.h>
#include <GL4D/gl4duw_SDL2.h>
#include <audioHelper.h>
#include <SDL_ttf.h>
#include <assert.h>
/* Prototypes des fonctions statiques contenues dans ce fichier C */
static void init(void);
static void draw(void);
/*!\brief identifiant du programme GLSL */
static GLuint _pId = 0, _pId2 = 0, _creditsPId = 0;
/*!\brief quelques objets géométriques */
static GLuint _quad = 0, _cube = 0, _screenId = 0;
static GLuint _sphere = 0, _torus = 0;

static float _rayon = 0;

void transformations(int state) {
  int l, i;
  Uint16 * s;
  double m = 0;
  /* INITIALISEZ VOS VARIABLES */
  switch(state) {
  case GL4DH_INIT:
    init();
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    s = (Uint16 *)ahGetAudioStream();
    l = MIN(ahGetAudioStreamLength()>>1, 1024);
    for(i = 0; i < l /2 ; ++i)
      m += s[i] / ((1 << 16) - 1.0);
    _rayon = 255.0 * m / (l / 2);
    return;
  default: /* GL4DH_DRAW */
    draw();
    return;
  }
}




/*!\brief fonction appelée à chaque draw par la gl4duwMainLoop. */



/*!\brief initialise les paramètres OpenGL et les données */
static void init(void) {

  _screenId = gl4dpInitScreenWithDimensions(300, 300);
  glDisable(GL_CULL_FACE);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  _pId  = gl4duCreateProgram("<vs>shaders/rouge.vs", "<fs>shaders/rouge.fs", NULL);
  _pId2  = gl4duCreateProgram("<vs>shaders/rouge_freq.vs", "<fs>shaders/rouge.fs", NULL);
  _creditsPId  = gl4duCreateProgram("<vs>shaders/credits.vs", "<fs>shaders/credits.fs", NULL);
  gl4duGenMatrix(GL_FLOAT, "modelViewMatrix");
  gl4duGenMatrix(GL_FLOAT, "projectionMatrix");
_sphere = gl4dgGenSpheref(20, 20);
  _cube = gl4dgGenFanConef(12,12);
  _quad = gl4dgGenQuadf();
  _torus = gl4dgGenTorusf(300, 30, 0.1f);
  
}

/*!\brief dessine dans le contexte OpenGL actif. */
static void draw(void) {
static GLfloat aa = 0;
  GLfloat rouge[] = {1, 0, 0, 1}, vert[] = {aa, 1, 1, 1}, jaune[] = {1, 1, 0, 1};

  static GLfloat a = 0;
  GLint vp[4];
  glGetIntegerv(GL_VIEWPORT, vp);
  glEnable(GL_DEPTH_TEST);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  gl4duBindMatrix("projectionMatrix");
  gl4duLoadIdentityf();
  gl4duFrustumf(-0.1, 0.1, -0.1f * vp[3] / vp[2], 0.1f * vp[3] / vp[2], 0.1, 100.0);
  gl4duBindMatrix("modelViewMatrix");
  gl4duLoadIdentityf();
  glUseProgram(_pId);
   gl4duTranslatef(0, 0, -10.0);
  gl4duPushMatrix(); {
    gl4duTranslatef(0, -2.0, 0.0);

    gl4duRotatef(-90, 1, 0, 0);
    gl4duScalef(6.0f, 6, 6);
    gl4duSendMatrices();

  }

gl4duPopMatrix();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, vert);
  gl4dgDraw(_quad);




//milieu
  glUseProgram(_pId2);
  gl4duTranslatef(0,1, 0);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId2, "couleur"), 1, rouge);
  glUniform1f(glGetUniformLocation(_pId2, "freq"), _rayon);
  gl4dgDraw(_sphere);
  gl4duPopMatrix();
  gl4dgDraw(_sphere);



//droite
  gl4duTranslatef(-1, -1, 0);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId2, "couleur"), 1, rouge);
  glUniform1f(glGetUniformLocation(_pId2, "freq"), _rayon);

  gl4dgDraw(_sphere);
  gl4duPopMatrix();
  gl4dgDraw(_sphere);

  //gauche

  gl4duTranslatef(-1, 1, 0);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId2, "couleur"), 1, rouge);
  glUniform1f(glGetUniformLocation(_pId2, "freq"), _rayon);
  gl4dgDraw(_sphere);
  gl4duPopMatrix();
  gl4dgDraw(_sphere);



  gl4duRotatef(a, 0, 1, 0);
  gl4duTranslatef(4, 2, 1);
  gl4duPushMatrix(); {
    gl4duScalef(0.4f, 0.4f, 0.4f);
    gl4duSendMatrices();
  } gl4duPopMatrix();

  glUseProgram(_pId);
  gl4duRotatef(1, 1, -1, 0);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, jaune);
  gl4dgDraw(_torus);
  gl4dgDraw(_cube);


  a++;
}







