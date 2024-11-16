#pragma once

#include "../LumEngine/Engine/GUI/Rendering/Meshes/Mesh.h"

#pragma make_public(MeshCore)

#include "../Entities/Properties/Properties.h"

using namespace LumScripting::Script::Properties;

namespace LumScripting
{
	namespace Script
	{
		namespace Rendering
		{
			public ref class Mesh : public Property
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

				const System::Type^ GetNativeType() override
				{
					return MeshCore::typeid;
				}

			private:
				MeshCore* nativeMesh;

			internal:
				const type_info& GetNativeTypeInfo() override
				{
					return typeid(MeshCore);
				}

				virtual Property^ CreateFromNative(IProperty* prop) override
				{
					return gcnew Mesh(static_cast<MeshCore*>(prop));
				}
			};
		}
	}
}