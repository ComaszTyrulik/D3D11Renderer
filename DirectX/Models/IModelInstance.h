#pragma once
#include "../IGeometry.h"
#include "../Texture.h"
#include "ModelReference.h"
#include "IDrawable.h"
#include "../Material.h"

#include <memory>

namespace d3dt
{
	class IModelInstance
		: public IGeometry, public IDrawable
	{
	public:
		virtual ~IModelInstance() = default;
		virtual const std::shared_ptr<Texture>& GetTexture() const = 0;
		virtual void Update() = 0;

		virtual const ModelReference& Reference() const = 0;
		virtual void SetMaterial(Material material) = 0;
	};
}