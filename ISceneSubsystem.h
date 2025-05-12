#pragma once
#include <glm/glm.hpp>

namespace engine::system {
	struct SystemContext;
}

namespace engine::scene {
	class ISceneSubsystem {
	public:
		virtual ~ISceneSubsystem() = default;

		virtual void update(engine::system::SystemContext& ctx) = 0;
		virtual void render(const glm::mat4& view, const glm::mat4& proj) {}
		virtual bool isActive() const = 0;
		virtual const char* getSubsystemName() const = 0;
	};
}