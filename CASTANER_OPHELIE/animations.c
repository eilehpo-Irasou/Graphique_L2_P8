

#include <GL4D/gl4dh.h>
#include "audioHelper.h"
#include "animations.h"
#include <assert.h>
#include <stdlib.h>
#include <GL4D/gl4dg.h>
#include <SDL_image.h>

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

/*!\brief identifiant de la géométrie QUAD GL4Dummies */
static GLuint _quadId = 0;



void fondu(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state) {
  /* INITIALISEZ VOS VARIABLES */
  int vp[4], i;
  GLint tId;
  static GLuint tex[2], pId;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE TRANSITION (SES VARIABLES <STATIC>s) */
    glGetIntegerv(GL_VIEWPORT, vp);
    glGenTextures(2, tex);
    for(i = 0; i < 2; i++) {
      glBindTexture(GL_TEXTURE_2D, tex[i]);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, vp[2], vp[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    }
    pId = gl4duCreateProgram("<vs>shaders/basic.vs", "<fs>shaders/mix.fs", NULL);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    if(tex[0]) {
      glDeleteTextures(2, tex);
      tex[0] = tex[1] = 0;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR LES DEUX ANIMATIONS EN FONCTION DU SON */
    if(a0) a0(state);
    if(a1) a1(state);
    return;
  default: /* GL4DH_DRAW */
    /* RECUPERER L'ID DE LA DERNIERE TEXTURE ATTACHEE AU FRAMEBUFFER */
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &tId);
    /* JOUER LES DEUX ANIMATIONS */
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tex[0],  0);
    if(a0) a0(state);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tex[1],  0);
    if(a1) a1(state);
    /* MIXER LES DEUX ANIMATIONS */
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tId,  0);
    glDisable(GL_DEPTH_TEST);
    glUseProgram(pId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex[1]);
    if(et / (GLfloat)t > 1) {
      fprintf(stderr, "%d-%d -- %f\n", et, t, et / (GLfloat)t);
      exit(0);
    }
    glUniform1f(glGetUniformLocation(pId, "dt"), et / (GLfloat)t);
    glUniform1i(glGetUniformLocation(pId, "tex0"), 0);
    glUniform1i(glGetUniformLocation(pId, "tex1"), 1);
    gl4dgDraw(_quadId);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    return;
  }
}

void fondui(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state) {
  /* INITIALISEZ VOS VARIABLES */
  int vp[4], i;
  GLint tId;
  static GLuint tex[3], pId;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE TRANSITION (SES VARIABLES <STATIC>s) */
    glGetIntegerv(GL_VIEWPORT, vp);
    glGenTextures(3, tex);
    for(i = 0; i < 3; i++) {
      glBindTexture(GL_TEXTURE_2D, tex[i]);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, vp[2], vp[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    }
    loadTexture(tex[2], "images/multicolor.jpg");
    pId = gl4duCreateProgram("<vs>shaders/basic.vs", "<fs>shaders/mixi.fs", NULL);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    if(tex[0]) {
      glDeleteTextures(3, tex);
      tex[0] = tex[1] = 0;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR LES DEUX ANIMATIONS EN FONCTION DU SON */
    if(a0) a0(state);
    if(a1) a1(state);
    return;
  default: /* GL4DH_DRAW */
    /* RECUPERER L'ID DE LA DERNIERE TEXTURE ATTACHEE AU FRAMEBUFFER */
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &tId);
    /* JOUER LES DEUX ANIMATIONS */
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tex[0],  0);
    if(a0) a0(state);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tex[1],  0);
    if(a1) a1(state);
    /* MIXER LES DEUX ANIMATIONS */
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tId,  0);
    glDisable(GL_DEPTH_TEST);
    glUseProgram(pId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex[1]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, tex[2]);
    if(et / (GLfloat)t > 1) {
      fprintf(stderr, "%d-%d -- %f\n", et, t, et / (GLfloat)t);
      exit(0);
    }
    glUniform1f(glGetUniformLocation(pId, "dt"), et / (GLfloat)t);
    glUniform1i(glGetUniformLocation(pId, "tex0"), 0);
    glUniform1i(glGetUniformLocation(pId, "tex1"), 1);
    glUniform1i(glGetUniformLocation(pId, "tex2"), 2);
    gl4dgDraw(_quadId);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    return;
  }
}







void rect_cir(int state) {
  /* INITIALISEZ VOS VARIABLES */
  int l, i;
int grossissement = 250;
  Sint16 * s;
  GLint random_color_rect[4], tr, mr;
  static int r = 0, or = 0;
  static GLuint screen_id;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    screen_id = gl4dpInitScreen();
    gl4dpUpdateScreen(NULL);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(screen_id);
    gl4dpDeleteScreen();
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    s = (Sint16 *)ahGetAudioStream();
    l = ahGetAudioStreamLength();
     for(i = 0 ,tr = 0; i < l >> 3; i++)
      tr += abs(s[i + 1]);
    tr /= l >> 4;
    r =  grossissement+ (tr >> 9);
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    tr = r;
    mr = MAX(tr, or) +1;
    gl4dpSetScreen(screen_id);
    random_color_rect[0] = MAX((gl4dpGetWidth()  >> 1) - mr, 0);
    random_color_rect[1] = MAX((gl4dpGetHeight() >> 1) - mr, 0);
    random_color_rect[2] = MIN(mr << 1,  gl4dpGetWidth() - random_color_rect[0]);
    random_color_rect[3] = MIN(mr << 1, gl4dpGetHeight() - random_color_rect[1]);

    gl4dpSetColor(RGB(rand()%256, rand()%256, rand()%256));	
    gl4dpRect(random_color_rect);
 
 gl4dpSetColor(RGB(1, 0, 100));
    gl4dpFilledCircle(gl4dpGetWidth() >> 1, gl4dpGetHeight() >> 1, (tr));
    gl4dpUpdateScreen(random_color_rect);
    or = tr;
    return;
  }
}


/*
rendu d'un effet de bulles style gazeux*/
void effet_bulles(int state) {
 /* INITIALISEZ VOS VARIABLES */ 
static GLuint screen = 0;
 int l, i;
 Sint16 * s;
 static Sint16 t[1000];
 static int w, h; 
switch(state) { 
case GL4DH_INIT: /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */ 
screen = gl4dpInitScreen(); 
w = gl4dpGetWidth(); 
h = gl4dpGetHeight(); 
return; 
case GL4DH_FREE: /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */ 
if(screen) { 
gl4dpSetScreen(screen); 
gl4dpDeleteScreen(); 
screen = 0; }
 return; 
case GL4DH_UPDATE_WITH_AUDIO: /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
 s = (Sint16 *)ahGetAudioStream();
 l = ahGetAudioStreamLength();
if(l >=  4)
 for(i = 0; i < 1000; i++) t[i] +=   s[i] *h ; 
return; default: /* GL4DH_DRAW */ /* JOUER L'ANIMATION */ 
gl4dpSetScreen(screen); 
gl4dpClearScreen(); 
for(i = 0; i < 1000 -1; i++) { 
  int  x1, y1;

  x1 = ((i+0.01)*(w - 1)) / (999); 
  y1 = t[i+1]; 
  gl4dpSetColor(rand());
  

gl4dpCircle(x1,y1,rand()%20);	
 } 
gl4dpUpdateScreen(NULL); 



return; 
} 
}



void circulaire(int state) {
  /* INITIALISEZ VOS VARIABLES */
  int l, i;
  Sint16 * s;
  GLint tr;
  static GLint r = 0;
  static GLuint s1, s2;
  static GLfloat r1[] = {0.0, 0.0, 1.0, 1.0}, r2[] = {0, 0, 1, 1}, rf = 0.0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    s1 = gl4dpInitScreen();
    s2 = gl4dpInitScreen();
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    s = (Sint16 *)ahGetAudioStream();
    l = ahGetAudioStreamLength();
     for(i = 0 ,tr = 0; i < l >> 3; i++)
      tr += abs(s[i + 1]);
    tr /= l >> 4;
    r = 5 + (tr >> 9);
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    tr = r;
    gl4dpSetScreen(s2);
    gl4dpClearScreen();
    for(i = 0; i < tr; i++) 
    {
      
      gl4dpSetColor(RGB(56, 78, 34));
      gl4dpFilledCircle(gl4dpGetWidth() >> 2, gl4dpGetHeight() >> 1, i);

gl4dpSetColor(RGB(255, 8, 34));
gl4dpFilledCircle(gl4dpGetWidth() >> 3, gl4dpGetHeight() >> 2, i);
gl4dpSetColor(RGB(5, 7, 3));
gl4dpFilledCircle(gl4dpGetWidth() >> 2, gl4dpGetHeight() >> 3, i);
}
    gl4dpSetColor(RGB(255, 255, 255));
    gl4dpFilledCircle(gl4dpGetWidth() * 3 / 4, gl4dpGetHeight() >> 1, i);
    gl4dpUpdateScreen(NULL);
    rf += 3;
    gl4dpSetScreen(s1);
    gl4dpClearScreen();
    gl4dpMap(s1, s2, r1, r2, rf);
    gl4dpUpdateScreen(NULL);
    return;
  }
}


void  cercles_descendants(int state) {
  int l, i;
int v = 100;
  Sint16 * s;
static int descente = 0;
  GLint tr;
  static GLint r = 0;
  static GLuint s1, s2;
  static GLfloat r1[] = {0.0, 0.0, 1.0, 1.0}, r2[] = {0, 0, 1, 1}, rf = 0.0;
  switch(state) {
    case GL4DH_INIT:
    s1 = gl4dpInitScreen();
    s2 = gl4dpInitScreen();

    return;
  case GL4DH_FREE:
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();

    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    s = (Sint16 *)ahGetAudioStream();
    l = ahGetAudioStreamLength();
    for(i = 0 ,tr = 0; i < l >> 3; i++)
      tr += abs(s[i + 1]);
    tr /= l >> 4;
    r = 5 + (tr >> 9);
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */

    tr = r;
    gl4dpSetScreen(s1);
    gl4dpClearScreen();
    for(i = 0; i < tr; i += 9) {
      gl4dpSetColor(100);
      gl4dpFilledCircle(cos(tr)+v, sin(tr)+descente, i); 

    }
    
    gl4dpUpdateScreen(NULL);
    rf += 0.01;
    gl4dpSetScreen(s2);
    gl4dpClearScreen();
    gl4dpMap(s1, s1, r1, r1, rf);
    if(descente > 0)
      descente--;
    else 
      descente = 900;
    gl4dpUpdateScreen(NULL);
    return;
  }
}





void  cercles_montants(int state) {
  int l, i;

  Sint16 * s;
static int descente = 100;
  GLint tr;
  static GLint r = 0;
  static GLuint s1, s2;
  static GLfloat r1[] = {0.0, 0.0, 1.0, 1.0}, r2[] = {0, 0, 1, 1}, rf = 0.0;
  switch(state) {
    case GL4DH_INIT:
    s1 = gl4dpInitScreen();
    s2 = gl4dpInitScreen();

    return;
  case GL4DH_FREE:
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();

    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    s = (Sint16 *)ahGetAudioStream();
    l = ahGetAudioStreamLength();
    for(i = 0 ,tr = 0; i < l >> 3; i++)
      tr += abs(s[i + 1]);
    tr /= l >> 4;
    r = 5 + (tr >> 9);
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    tr = r;
    gl4dpSetScreen(s1);
    gl4dpClearScreen();
    for(i = 0; i < tr; i += 9) {
for(int d = 100 ; d < 1200 ; d+=300){
      gl4dpFilledCircle(cos(tr)+d, sin(tr)+descente, i); }
    }
    
    gl4dpUpdateScreen(NULL);
    rf += 0.01;
    gl4dpSetScreen(s2);

    gl4dpClearScreen();

    gl4dpMap(s1, s2, r1, r2, rf);
    if(descente < 300){
      descente++;
gl4dpSetColor(rand());
}
    else{
      descente = 100;

}


    gl4dpUpdateScreen(NULL);
    return;
  }
}




void  grossissement(int state) {
  int l, i;
int v = 100;
  Sint16 * s;
static int descente = 0;
  GLint tr;
  static GLint r = 0;
  static GLuint s1, s2;
  static GLfloat r1[] = {0.0, 0.0, 1.0, 1.0}, r2[] = {0, 0, 1, 1}, rf = 0.0;
  switch(state) {
    case GL4DH_INIT:
    s1 = gl4dpInitScreen();
    s2 = gl4dpInitScreen();

    return;
  case GL4DH_FREE:
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();

    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    s = (Sint16 *)ahGetAudioStream();
    l = ahGetAudioStreamLength();
    for(i = 0 ,tr = 0; i < l >> 3; i++)
      tr += abs(s[i + 1]);
    tr /= l >> 4;
    r =  (tr >> 9);
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */

    tr = r;
    gl4dpSetScreen(s1);
    gl4dpClearScreen();
    for(i = 0; i < tr; i += 8) {
v+=50;

for (int w =500 ; w > 0; w-=50){
for(int p = 1200; p > 0; p-=50){
 gl4dpSetColor(RGB(rand(),rand(),rand()));
      gl4dpFilledCircle(cos(tr)+500, sin(tr)+v, i); }} 
    }
    
    gl4dpUpdateScreen(NULL);
    rf += 0.01;
    gl4dpSetScreen(s2);

    gl4dpClearScreen();
    gl4dpMap(s1, s2, r1, r2, rf);
    if(descente > 0)
      descente--;
    else 
      descente = 900;
    gl4dpUpdateScreen(NULL);
    return;
  }
}





void animationsInit(void) {
  if(!_quadId)
    _quadId = gl4dgGenQuadf();
}
