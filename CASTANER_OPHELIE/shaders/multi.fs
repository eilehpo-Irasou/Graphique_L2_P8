



#version 330
uniform sampler2D tex;
uniform float temps;
in  vec2 vsoTexCoord;
out vec4 fragColor;

void main(void) {
  vec2 vecteur = vsoTexCoord * vec2(8.5);
  float distance = length(vecteur);
  float angle = atan(vecteur.y+2, vecteur.x+2);
  /* c'est la ligne qui suit qui change tout ! */
  angle +=  1 - temps+(distance);
  vec2 tc = vec2(8.5)+ vec2(distance *angle, distance/angle);
  fragColor = texture(tex, tc);
}
