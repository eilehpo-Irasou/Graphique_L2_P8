/*!\file window.c
 * \brief géométries lumière diffuse, transformations de base et chargement de textures en GL4Dummies
 * \author Farès BELHADJ, amsi@ai.univ-paris8.fr
 * \date May 13 2018 */
#include <stdio.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4df.h>
#include <GL4D/gl4duw_SDL2.h>
#include <SDL_image.h>
#include <GL4D/gl4dh.h>
#include "audioHelper.h"
/* Prototypes des fonctions statiques contenues dans ce fichier C */
static void init(void);
static void loadTexture(GLuint id, const char * filename);
static void draw(void);
static void quit(void);
/*!\brief identifiant du programme GLSL */
static GLuint _pId = 0;
/*!\brief quelques objets géométriques */
static GLuint _quad = 0, _tex = 0;
/*!\brief initialise les paramètres OpenGL et les données */
static void init(void) {
  GLint vp[4];
  glGetIntegerv(GL_VIEWPORT, vp);

  _pId  = gl4duCreateProgram("<vs>shaders/canal.vs", "<fs>shaders/canal.fs", NULL);
  gl4duGenMatrix(GL_FLOAT, "modelViewMatrix");
  gl4duGenMatrix(GL_FLOAT, "projectionMatrix");
 
  _quad =gl4dgGenCubef();
  glGenTextures(1, &_tex);

 loadTexture(_tex, "images/canal.png");
  glEnable(GL_TEXTURE_2D);
}
static void loadTexture(GLuint id, const char * filename) {
  SDL_Surface * t;
  glBindTexture(GL_TEXTURE_2D, id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  if( (t = IMG_Load(filename)) != NULL ) {
#ifdef __APPLE__
    int mode = t->format->BytesPerPixel == 4 ? GL_BGRA : GL_BGR;
#else
    int mode = t->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
#endif       
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->w, t->h, 0, mode, GL_UNSIGNED_BYTE, t->pixels);
    SDL_FreeSurface(t);
  } else {
    fprintf(stderr, "can't open file %s : %s\n", filename, SDL_GetError());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  }
}

/*!\brief dessine dans le contexte OpenGL actif. */
static void draw(void) {
  
  GLint vp[4];

  glGetIntegerv(GL_VIEWPORT, vp);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.1f, 0.7f, 0.5f, 1.0f);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  gl4duBindMatrix("projectionMatrix");
  gl4duLoadIdentityf();
  gl4duBindMatrix("modelViewMatrix");
double temps = gl4dGetElapsedTime();
  glUseProgram(_pId);
  glActiveTexture(GL_TEXTURE0);
  glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
  glUniform1f(glGetUniformLocation(_pId, "temps"), (GLfloat)temps);
  glBindTexture(GL_TEXTURE_2D, _tex);
  gl4dgDraw(_quad);

}
/*!\brief appelée au moment de sortir du programme (atexit), libère les éléments utilisés */
static void quit(void) {
  /* suppression de plusieurs identifiants de texture */
glDeleteTextures(1, &_tex);
}


void canal(int state) {
  switch(state) {
  case GL4DH_INIT:
    init();
    return;
  case GL4DH_FREE:
    quit();
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    return;
  default: /* GL4DH_DRAW */
    draw();
    return;
  }
}
