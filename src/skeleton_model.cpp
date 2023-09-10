
// glm
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "cgra/cgra_geometry.hpp"
#include "skeleton_model.hpp"


using namespace std;
using namespace glm;
using namespace cgra;


void skeleton_model::draw(const mat4 &view, const mat4 &proj) {
	// set up the shader for every draw call
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "uProjectionMatrix"), 1, false, value_ptr(proj));

	// if the skeleton is not empty, then draw
	if (!skel.bones.empty()) {
		drawBone(view, 0);
	}
}


void skeleton_model::drawBone(const mat4 &parentTransform, int boneid) {
	skeleton_bone &bone = skel.bones[boneid];
	mat4 boneTransform = parentTransform;
	boneTransform *= mat4(vec4(1,0,0,bone.basis.x), vec4(0,1,0, bone.basis.y), vec4(0, 0, 1, bone.basis.z), vec4(0,0,0,1)); // Translate
	boneTransform *= mat4(vec4(0.5,0,0,0), vec4(0,0.5,0,0), vec4(0,0,1.5,0), vec4(0,0,0,1)); // Scale
	
	boneTransform *= mat4(); // Rotation
	if (bone.length > 0){
		// Draw the bone
		glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(boneTransform));
		glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(vec3{1,0,0}));
		drawCylinder();
	}

	for (int i = 0; i < bone.children.size(); i++){
		drawBone(boneTransform, bone.children[i]);
	}
}
