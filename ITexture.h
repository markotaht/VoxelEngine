#pragma once
#include "NewResource.h"
#include <gl/glew.h>

namespace engine::asset {
	class ITexture: public resource::Resource {
	public:
		virtual void bind(GLuint slot) const = 0;
		virtual GLuint getID() const = 0;
		virtual ~ITexture() = default;
	};
}