#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace engine
{
    glm::vec3 rotateVector(const glm::vec3 &v, const glm::vec3 &axis, float angleDegrees)
    {
        const glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angleDegrees), glm::normalize(axis));
        return glm::normalize(glm::vec3(rotation * glm::vec4(v, 1.0f)));
    }

    // Angle between two unit vectors, in degrees (0..180).
    float angleBetween(const glm::vec3 &a, const glm::vec3 &b)
    {
        const float d = glm::clamp(glm::dot(glm::normalize(a), glm::normalize(b)), -1.0f, 1.0f);
        return glm::degrees(glm::acos(d));
    }
}