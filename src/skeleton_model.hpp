#pragma once

// glm
#include <glm/glm.hpp>

// project
#include "opengl.hpp"
#include "skeleton.hpp"

class skeleton_model {
private:
	// recursive helper method
	void drawBone(const glm::mat4 &view, int boneid);
	void drawJoint(const glm::mat4 &parentTransform);
	void drawBonePart(const glm::mat4 &parentTransform, skeleton_bone &bone);

    void drawAxis(const glm::mat4 &view, skeleton_bone &bone);
    void drawXAxis(const glm::mat4 &view, skeleton_bone &bone);
    void drawYAxis(const glm::mat4 &view, skeleton_bone &bone);
    void drawZAxis(const glm::mat4 &view, skeleton_bone &bone);

public:
	GLuint shader = 0;
	skeleton_data skel;
	skeleton_pose pose;

	skeleton_model() { }
	void draw(const glm::mat4 &view, const glm::mat4 &proj);
};