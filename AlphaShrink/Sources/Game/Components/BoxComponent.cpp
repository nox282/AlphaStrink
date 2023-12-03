#include "BoxComponent.h"

void BoxComponent::getBounds(const BoxComponent& boxComponent, const Mani::Transform& boxTransform, glm::vec3& outMin, glm::vec3& outMax)
{
    outMin = boxTransform.position + glm::rotate(boxTransform.rotation, -boxComponent.extent);
    outMax = boxTransform.position + glm::rotate(boxTransform.rotation, boxComponent.extent);
}

bool BoxComponent::isIn(const BoxComponent& boxComponent, const Mani::Transform& boxTransform, const glm::vec3& position)
{
    glm::vec3 min;
    glm::vec3 max;
    BoxComponent::getBounds(boxComponent, boxTransform, min, max);

    return position.x >= min.x && position.x <= max.x &&
        position.y >= min.y && position.y <= max.y &&
        position.z >= min.z && position.z <= max.z;
}
