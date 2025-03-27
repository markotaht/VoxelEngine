#pragma once
#include "ResourceId.h"
#include "NewMesh.h"
#include "NewMaterial.h"

namespace engine::component {
	struct MeshRendererComponent {
		core::ResourceId<asset::Mesh> meshId;
		core::ResourceId<asset::Material> materialId;
	};
}