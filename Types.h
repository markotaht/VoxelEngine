#pragma once
#include <cstdint>
#include <string>
#include <variant>
#include <unordered_map>
#include <typeindex>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace engine {
    namespace descriptor {
        struct FileDescriptor;
        struct ShaderDescriptor;
        struct MaterialDescriptor;
    }

    namespace resource {
        class Resource;
        class ResourceManager;
    }

    namespace asset {
        class Texture;
        class ShaderProgram;
        class Material;
        class Mesh;
        class Font;
        class TextureMaterial;
    }
}

namespace engine {
    using Hash = std::size_t;

    template<typename T>
    Hash MakeHashKey(T desc) {
        return std::hash<T>{}(desc);
    }

    using UniformValue = std::variant<
        int,
        float,
        glm::vec2,
        glm::vec3,
        glm::vec4,
        glm::mat4// or ResourceID if you're post-loading
    >;

    using UniformMap = std::unordered_map<std::string, UniformValue>;
}