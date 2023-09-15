// glm
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "cgra/cgra_geometry.hpp"
#include "spline_model.hpp"

using namespace std;
using namespace glm;

void spline_model::draw(const glm::mat4 &view, const glm::mat4 &proj){
    glUseProgram(shader); // load shader and variables
	glUniformMatrix4fv(glGetUniformLocation(shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
    
    for (int i = 0; i < (int)points.size(); i++){
        glm::mat4 modelview = view * glm::translate(glm::mat4(1.0f), points[i]);
        glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(modelview * scale(mat4(1.0f), vec3(1, 1, 1))));
        if (i == 1) glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(vec3(0,1,0)));
        else if (i == points.size()-2) glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(vec3(1,0,0)));
        else glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(vec3(0.7f)));
        cgra::drawSphere();
    }

    if (points.size() < 4) return;
    glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(vec3(1.0f)));
    glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(view));
    glLineWidth(10);
    glBegin(GL_LINES);
    for (int i =0; i < (int)points.size(); i++){
        if (points.size() - i < 4) break;
        for (float t = 0; t <= 1.0f; t += 0.01){
            vec3 point = get_spline_point(points[i], points[i+1], points[i+2], points[i+3], t);
            glVertex3f(point.x, point.y, point.z);
        }
    }
    glEnd();
}

vec3 spline_model::get_spline_point(vec3 p0, vec3 p1, vec3 p2, vec3 p3, float t){
    float t2 = t * t;
    float t3 = t2 * t;

    vec4 tVec = vec4(
        basisMatrix[0][0] * t3 + basisMatrix[1][0] * t2 + basisMatrix[2][0] * t + basisMatrix[3][0],
        basisMatrix[0][1] * t3 + basisMatrix[1][1] * t2 + basisMatrix[2][1] * t + basisMatrix[3][1],
        basisMatrix[0][2] * t3 + basisMatrix[1][2] * t2 + basisMatrix[2][2] * t + basisMatrix[3][2],
        basisMatrix[0][3] * t3 + basisMatrix[1][3] * t2 + basisMatrix[2][3] * t + basisMatrix[3][3]
    );
    vec3 point = p0 * tVec.x + p1 * tVec.y + p2 * tVec.z + p3 * tVec.w;
    return point;
}   