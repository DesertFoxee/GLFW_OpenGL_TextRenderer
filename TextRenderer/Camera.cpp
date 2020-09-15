#include "Camera.h"
glm::vec3   Camera::target  ;
glm::vec3   Camera::up      ;
glm::vec3   Camera::position;

glm::mat4   Camera::mat4Proj;

float       Camera::pitch   ;
float       Camera::yaw     ;