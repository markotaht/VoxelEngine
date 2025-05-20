#pragma once
#include "ResourceId.h"
#include "Mesh.h"
#include "Material.h"
#include "ResourceReflection.h"

namespace engine::component {
	struct MeshRendererComponent {
		core::ResourceId<asset::Mesh> meshId;
		core::ResourceId<asset::Material> materialId;

		REFLECT_RESOURCE_IDS(meshId, materialId)
	};
}