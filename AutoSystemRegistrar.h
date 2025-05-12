#pragma once
#include "SystemRegistry.h"

namespace engine::system {
    template<typename SystemType>
    struct AutoSystemRegistrar {
        AutoSystemRegistrar(bool fixedUpdate = false) {
            if (fixedUpdate) {
                SystemRegistry::instance().registerFixedUpdateSystem<SystemType>();
            }
            else {
                SystemRegistry::instance().registerSystem<SystemType>();
            }
        }
    };
}