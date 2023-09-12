#define GLM_ENABLE_EXPERIMENTAL

// glm
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
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
		drawAxis(parentTransform, bone);
	}

	mat4 rotation_matrix = mat4(1) * orientation(bone.direction, vec3(0,0,1));
	mat4 translate_matrix = translate(mat4(1), bone.direction * (bone.length * 50));
	mat4 childTransform = parentTransform * translate_matrix;
	for (int i = 0; i < (int)bone.children.size(); i++){
		drawBone(childTransform, bone.children[i]);
	}
}

void skeleton_model::drawJoint(const mat4 &view){
	mat4 scale_matrix = scale(mat4(1), vec3(1.0f));
	mat4 final_matrix = view * scale_matrix;
	glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(final_matrix));
	glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(vec3{0.2, 0.65, 0.65}));
	drawSphere();
}

void skeleton_model::drawBonePart(const mat4 &parentTransform, skeleton_bone &bone){
	mat4 rotation_matrix = mat4(1) * orientation(bone.direction, vec3(0,0,1));
    mat4 scale_matrix = scale(mat4(1), vec3(0.5,0.5, bone.length * 50));
	mat4 final_matrix = parentTransform  * rotation_matrix * scale_matrix;
	glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(final_matrix));
	glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(vec3{0.8, 0.8, 0.8}));
	drawCylinder();
}

void skeleton_model::drawAxis(const mat4 &view, skeleton_bone &bone){
	drawXAxis(view, bone);
	drawYAxis(view, bone);
	drawZAxis(view, bone);
}

void skeleton_model::drawXAxis(const mat4 &view, skeleton_bone &bone){
	mat4 rotate_matrix = rotate(mat4(1), pi<float>() / 2.0f, vec3(0, 1, 0)) * rotate(mat4(1), bone.basis.x, vec3(1)) * rotate(mat4(1), bone.basis.y, vec3(1)) * rotate(mat4(1), bone.basis.z, vec3(1));
	mat4 scale_matrix = scale(mat4(1), vec3(0.3,0.3,3));
	glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(view * rotate_matrix * scale_matrix));
	glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(vec3{1, 0, 0}));
	drawCylinder();

}

void skeleton_model::drawYAxis(const mat4 &view, skeleton_bone &bone){
	mat4 rotate_matrix = rotate(mat4(1), pi<float>() / 2.0f, vec3(-1, 0, 0)) * rotate(mat4(1), bone.basis.x, vec3(1)) * rotate(mat4(1), bone.basis.y, vec3(1)) * rotate(mat4(1), bone.basis.z, vec3(1));
	mat4 scale_matrix = scale(mat4(1), vec3(0.3,0.3,3));
	glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(view * rotate_matrix * scale_matrix));
	glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(vec3{0, 1, 0}));
	drawCylinder();
}

void skeleton_model::drawZAxis(const mat4 &view, skeleton_bone &bone){
	mat4 rotate_matrix = rotate(mat4(1), pi<float>() / 2.0f, vec3(0, 0, 1)) * rotate(mat4(1), bone.basis.x, vec3(1)) * rotate(mat4(1), bone.basis.y, vec3(1)) * rotate(mat4(1), bone.basis.z, vec3(1));
	mat4 scale_matrix = scale(mat4(1), vec3(0.3,0.3,3));
	glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(view * rotate_matrix * scale_matrix));
	glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(vec3{0, 0, 1}));
	drawCylinder();
	glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(view * translate(mat4(1), vec3(0,0,1)) * rotate_matrix));
	drawCone();
}
