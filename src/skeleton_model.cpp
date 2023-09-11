
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

	if (boneid != 0){
		drawJoint(parentTransform);
		drawBonePart(parentTransform, bone);
		
	}
	mat4 childTransform = translate(parentTransform, bone.direction * (bone.length * 20));
	for (int i = 0; i < (int)bone.children.size(); i++){
		drawBone(childTransform, bone.children[i]);
	}
}

void skeleton_model::drawJoint(const mat4 &view){
	mat4 model_matrix = scale(view, vec3(2.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(model_matrix));
	glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(vec3{0.2, 0.65, 0.65}));
	drawSphere();
}

void skeleton_model::drawBonePart(const glm::mat4 &parentTransform, skeleton_bone &bone){
	mat4 modelTransformations(parentTransform);
    modelTransformations = rotate(modelTransformations, bone.direction.x, bone.basis);
    modelTransformations = rotate(modelTransformations, bone.direction.y, bone.basis);
    modelTransformations = rotate(modelTransformations, bone.direction.z, bone.basis);

    modelTransformations = scale(modelTransformations, vec3(1,1, bone.length * 20));

	glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(modelTransformations));
	glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(vec3{0.8, 0.8, 0.8}));
	drawCylinder();
}