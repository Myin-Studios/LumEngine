#pragma once

#include "../LumTypes/Entities/Properties/Property.h"

namespace LumScripting
{
	namespace Script
	{
		namespace Properties
		{
			public ref class Property
			{
			private:
				IProperty* native;

			public:
				Property(IProperty* prop) : native(prop) {}
				Property() : native(new IProperty()) {}
			};
		}
	}
}