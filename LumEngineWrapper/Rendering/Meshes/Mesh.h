#pragma once

#include "../LumEngine/Engine/GUI/Rendering/Meshes/Mesh.h"

namespace LumScripting
{
	namespace Script
	{
		namespace Rendering
		{
			public ref class Mesh
			{
			public:
				Mesh(MeshCore* meshCore)
				{
					this->nativeMesh = meshCore;
				}

				Mesh()
				{
					this->nativeMesh = new MeshCore();
				}

				!Mesh()
				{
					if (this->nativeMesh != nullptr)
					{
						delete this->nativeMesh;
						this->nativeMesh = nullptr;
					}
				}

				~Mesh()
				{
					this->!Mesh();
				}

			private:
				MeshCore* nativeMesh;
			};
		}
	}
}