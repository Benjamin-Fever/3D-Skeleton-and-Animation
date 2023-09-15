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
	const mat4 &childMatrix = parentTransform * translate(mat4(1), bone.direction * (bone.length * 50)); 
	if (boneid != 0){
		drawJoint(parentTransform);
		drawBonePart(parentTransform, bone);
		drawAxis(parentTransform, bone);
	}	
	
	for (int i = 0; i < (int)bone.children.size(); i++){
		drawBone(childMatrix, bone.children[i]);
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
	vec3 axis = cross(vec3(0,0,1), bone.direction);
	float angle = acos(dot(vec3(0,0,1), bone.direction));
	mat4 rotationMatrix = rotate(mat4(1), angle, axis);
    mat4 scale_matrix = scale(mat4(1), vec3(0.5,0.5, bone.length * 50));
	glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr((parentTransform * rotationMatrix) * scale_matrix));
	glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(vec3{0.8, 0.8, 0.8}));
	drawCylinder();
}

void skeleton_model::drawAxis(const glm::mat4 &parentTransform, skeleton_bone &bone){
	mat4 rot = rotate(rotate(rotate(parentTransform, bone.basis.z, vec3(0,0,1)), bone.basis.y, vec3(0,1,0)), bone.basis.x, vec3(1,0,0));

	drawXAxis(rot);
	drawYAxis(rot);
	drawZAxis(rot);
}

void skeleton_model::drawXAxis(const glm::mat4 &parentTransform){
	glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(vec3{1,0,0}));
	mat4 rotationMatrix = rotate(mat4(1), pi<float>()/2.0f, vec3(0,1,0));
	mat4 scaleMatrix = scale(mat4(1), vec3(0.3,0.3,4));
	glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(parentTransform * rotationMatrix * scaleMatrix));
	drawCylinder();
}

void skeleton_model::drawYAxis(const glm::mat4 &parentTransform){
	glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(vec3{0, 1, 0}));
	mat4 rotationMatrix = rotate(mat4(1), -(pi<float>()/2.0f), vec3(1,0,0));
	mat4 scaleMatrix = scale(mat4(1), vec3(0.3,0.3,4));
	glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(parentTransform * rotationMatrix * scaleMatrix));
	drawCylinder();
}

void skeleton_model::drawZAxis(const glm::mat4 &parentTransform){
	glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(vec3{0, 0, 1}));
	mat4 rotationMatrix = rotate(mat4(1), pi<float>()/2.0f, vec3(0,0,1));
	mat4 scaleMatrix = scale(mat4(1), vec3(0.3,0.3,4));
	glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(parentTransform * rotationMatrix * scaleMatrix));
	drawCylinder();
}