#pragma once

#include <glm/glm.hpp>

namespace vionis
{

struct TransformComponent
{
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    glm::vec3 scale{1.0f, 1.0f, 1.0f};
    glm::vec3 rotation{0.0f};

    glm::mat4 toMatrix() const;
    glm::mat3 computeNormalMatrix() const;
};

} // namespace vionis