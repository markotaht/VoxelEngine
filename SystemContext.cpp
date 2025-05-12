#include "SystemContext.h"

#include "CameraComponent.h"

namespace engine::system {
    entity::Entity SystemContext::getActiveCamera() const
    {
        entity::Entity activeCamera = entity::INVALID_ENTITY;

        globalScene->forEach<component::CameraComponent>([&](entity::Entity e, const component::CameraComponent& camera) {
            if (activeCamera == entity::INVALID_ENTITY) {
                activeCamera = e;
            }
            // Optionally: check for camera.isPrimary or some flag
            });

        return activeCamera;
    }
    component::CameraComponent* SystemContext::getCameraComponent() const
    {
        entity::Entity camEntity = getActiveCamera();
        if (camEntity == entity::INVALID_ENTITY) return nullptr;

        return globalScene->template tryGetComponent<component::CameraComponent>(camEntity);
    }
    component::TransformComponent* SystemContext::getCameraTransform() const
    {
        entity::Entity camEntity = getActiveCamera();
        if (camEntity == entity::INVALID_ENTITY) return nullptr;

        return globalScene->template tryGetComponent<component::TransformComponent>(camEntity);
    }
}
