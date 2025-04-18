#include "PhysicsSystem.h"

#include <glm/glm.hpp>
#include <array>
#include <vector>

#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"
#include "Entity.h"

namespace engine::system {
	void PhysicsSystem::update(SystemContext& ctx)
	{
		entity::Registry& ecs = ctx.ecs;

		std::vector<entity::Entity> entities;

		ecs.forEach<component::TransformComponent, component::RigidBodyComponent, component::ColliderComponent>([&](entity::Entity e, component::TransformComponent& transform, component::RigidBodyComponent& rb, component::ColliderComponent& collider) {
			entities.push_back(e);

			if (!rb.isStatic) {
				rb.velocity += rb.acceleration * ctx.dt;
				transform.position += rb.velocity * ctx.dt;
			}
			});

		for (size_t i = 0; i < entities.size(); ++i) {
			entity::Entity a = entities[i];

			auto& tA = ecs.getComponent<TransformComponent>(a);
			auto& rbA = ecs.getComponent<RigidBodyComponent>(a);
			auto& cA = ecs.getComponent<ColliderComponent>(a);

			for (size_t j = i + 1; j < entities.size(); ++j) {
				entity::Entity b = entities[j];

				auto& tB = ecs.getComponent<TransformComponent>(b);
				auto& rbB = ecs.getComponent<RigidBodyComponent>(b);
				auto& cB = ecs.getComponent<ColliderComponent>(b);

				if (rbA.isStatic && rbB.isStatic)
					continue;

				CollisionInfo collision = checkCollision(tA, cA, tB, cB);
				if (collision.collided) {
					resolveCollision(tA, rbA, tB, rbB, collision);
				}
			}
		}

	}

	CollisionInfo PhysicsSystem::checkCollision(const TransformComponent& a, const ColliderComponent& ca, const TransformComponent& b, const ColliderComponent& cb)
	{
		if (ca.type == ColliderType::AABB && cb.type == ColliderType::AABB) {
			float dx = (ca.size.x + cb.size.x) - abs(a.position.x - b.position.x);
			float dy = (ca.size.y + cb.size.y) - abs(a.position.y - b.position.y);
			float dz = (ca.size.z + cb.size.z) - abs(a.position.z - b.position.z);

			if (dx <= 0 || dy <= 0 || dz <= 0)
				return { {}, 0.0f, false }; // no collision

			// Resolve along the axis with the least penetration
			if (dx < dy && dx < dz) {
				float sign = (a.position.x < b.position.x) ? -1.0f : 1.0f;
				return { glm::vec3(sign, 0, 0), dx, true };
			}
			else if (dy < dz) {
				float sign = (a.position.y < b.position.y) ? -1.0f : 1.0f;
				return { glm::vec3(0, sign, 0), dy, true };
			}
			else {
				float sign = (a.position.z < b.position.z) ? -1.0f : 1.0f;
				return { glm::vec3(0, 0, sign), dz, true };
			}
		}
		else if (ca.type == ColliderType::Sphere && cb.type == ColliderType::Sphere) {
			float dist = glm::distance(a.position, b.position);
			float totalRadius = ca.radius + cb.radius;
			if (dist < totalRadius) {
				float penetration = totalRadius - dist;
				glm::vec3 normal = (dist > 0.0f) ? glm::normalize(a.position - b.position) : glm::vec3(1, 0, 0);

				return { normal , penetration, true };
			}
			return { {}, 0.0f, false };
		}
		else {
			if (ca.type == ColliderType::Sphere) {
				return checkSphereToAABB(a, ca, b, cb);
			}
			return checkSphereToAABB(b, cb, a, ca);
		}
	}

	glm::vec3 PhysicsSystem::closestPointToAABB(const TransformComponent& aabb, const ColliderComponent& caabb, const TransformComponent& sphere)
	{
		glm::vec3 minPoint = aabb.position - caabb.size;
		glm::vec3 maxPoint = aabb.position + caabb.size;
		glm::vec3 closestPointOnAABB = { glm::clamp(sphere.position.x, minPoint.x, maxPoint.x),
			glm::clamp(sphere.position.y, minPoint.y, maxPoint.y),
			glm::clamp(sphere.position.z, minPoint.z, maxPoint.z)
		};

		return closestPointOnAABB;
	}

	void PhysicsSystem::resolveCollision(TransformComponent& a, RigidBodyComponent& rbA, TransformComponent& b, RigidBodyComponent& rbB, const CollisionInfo& collision)
	{
		if (rbA.isStatic && rbB.isStatic) return;

		glm::vec3 correction = collision.normal * collision.penetration;

		if (!rbA.isStatic) a.position += correction * 0.5f;
		if (!rbB.isStatic) b.position -= correction * 0.5f;

		// Simple velocity response
		float restitution = 0.5f; // bounciness
		glm::vec3 relativeVelocity = rbA.velocity - rbB.velocity;
		float velAlongNormal = glm::dot(relativeVelocity, collision.normal);

		if (velAlongNormal < 0) {
			float j = -(1 + restitution) * velAlongNormal;
			j /= rbA.mass + rbB.mass;
			glm::vec3 impulse = j * collision.normal;

			if (!rbA.isStatic) rbA.velocity += impulse / rbA.mass;
			if (!rbB.isStatic) rbB.velocity -= impulse / rbB.mass;
		}
	}

	CollisionInfo PhysicsSystem::checkSphereToAABB(
		const TransformComponent& sphere, const ColliderComponent& csphere,
		const TransformComponent& aabb, const ColliderComponent& caabb)
	{
		glm::vec3 closestPoint = closestPointToAABB(aabb, caabb, sphere);
		glm::vec3 delta = sphere.position - closestPoint;
		float dist2 = glm::dot(delta, delta);
		float radius = csphere.radius;

		if (dist2 < radius * radius) {
			float dist = glm::sqrt(dist2);
			glm::vec3 normal = dist > 0.0f ? delta / dist : glm::vec3(1, 0, 0); // fallback
			float penetration = radius - dist;
			return { normal, penetration, true };
		}

		return { {}, 0.0f, false };
	}
}
