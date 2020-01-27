/*!\file dep3d.vs
 *
 * \brief Transformations matricielles standards pour les sommets, les
 * normales et les coordonnées de texture
 * \author Farès BELHADJ, amsi@ai.univ-paris8.fr 
 * \date April 15 2016
 */
#version 330

uniform float freq;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
layout (location = 0) in vec3 vsiPosition;
layout (location = 1) in vec3 vsiNormal;
layout (location = 2) in vec2 vsiTexCoord; /* ne sont pas utilisées dans cet exemple */
 
out vec3 vsoNormal;

void main(void) {
  vec3 pos = vec3(vsiPosition.x * (freq / 100), vsiPosition.y * (freq/100), vsiPosition.z * (freq/100));



// 1024 spheres très petites
//vec3 pos = vec3(vsiPosition.x * (freq /1024), vsiPosition.y * (freq/1024), vsiPosition.z * (freq/1024));
  vsoNormal = (transpose(inverse(modelViewMatrix))  *vec4(vsiNormal, 0.0)).xyz;
  gl_Position = projectionMatrix * modelViewMatrix * vec4(pos, 1.0);
}
