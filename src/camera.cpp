#include "vionis/camera.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <cassert>
#include <limits>

namespace vionis
{

void Camera::setOrthoProjection(float left, float right, float top, float bottom, float near, float far)
{
    projectionMatrix = glm::ortho(left, right, bottom, top, near, far);
}

void Camera::setPerspectiveProjection(float fov, float aspect, float near, float far)
{
    assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
    projectionMatrix = glm::perspective(glm::radians(fov), aspect, near, far);
    projectionMatrix[1][1] *= -1;
}

void Camera::setView(glm::vec3 position, glm::vec3 rotation, glm::vec3 forward, glm::vec3 up)
{
    glm::mat4 rotMat = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);

    glm::vec3 worldForward = rotMat * glm::vec4(forward, 0.0f);
    glm::vec3 worldUp = rotMat * glm::vec4(up, 0.0f);

    viewMatrix = glm::lookAt(position, position + worldForward, worldUp);
}

void Camera::setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{
    viewMatrix = glm::lookAt(position, target, up);
}

} // namespace vionis
