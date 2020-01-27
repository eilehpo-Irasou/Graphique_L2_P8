/*!\file animations.h
 *
 * \brief Votre espace de liberté : c'est ici que vous pouvez ajouter
 * vos fonctions de transition et d'animation avant de les faire
 * référencées dans le tableau _animations du fichier \ref window.c
 *
 * Des squelettes d'animations et de transitions sont fournis pour
 * comprendre le fonctionnement de la bibliothèque. En bonus des
 * exemples dont un fondu en GLSL.
 *
 * \author Farès BELHADJ, amsi@ai.univ-paris8.fr
 * \date May 05, 2014
 */
#ifndef _ANIMATIONS_H

#define _ANIMATIONS_H

#ifdef __cplusplus
extern "C" {
#endif

  
  extern void fondu(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state);
  extern void fondui(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state);

  extern void animationsInit(void);

 
  extern void identite(int state);
   extern void canal(int state);
extern void circulaire(int state);
extern void rect_cir(int state);



//2D
extern void  cercles_descendants(int state);
extern void  cercles_montants(int state);
extern void  grossissement(int state);
extern void effet_bulles(int state);
//3D
extern void tourner_sphere(int state);
extern void transformations(int state);



 
  
  
  

#ifdef __cplusplus
}
#endif

#endif
