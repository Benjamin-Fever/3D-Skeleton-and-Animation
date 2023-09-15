#pragma once

// glm
#include <glm/glm.hpp>
#include <vector>

// project
#include "opengl.hpp"

class spline_model {
public:
	GLuint shader = 0;
    std::vector<glm::vec3> points = std::vector<glm::vec3>();

	spline_model(){};
	void draw(const glm::mat4 &view, const glm::mat4 &proj);

private:
    glm::mat4 basisMatrix = glm::mat4(
        glm::vec4(-0.5, 1.5, -1.5, 0.5),
        glm::vec4(1, -2.5, 2, -0.5),
        glm::vec4(-0.5, 0, 0.5, 0),
        glm::vec4(0, 1, 0, 0)
    );

    glm::vec3 get_spline_point(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float t);
};