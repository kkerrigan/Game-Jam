#pragma once
namespace physics
{
	namespace interfaces
	{
		/*!	\class	iShape
			\brief  
			\author Daniel Maclam
			\date	2019-02-01 5:45 PM
			*/
		class iShape
		{
			

		public:
			enum shape_types
			{
				plane,
				sphere,
				box,
				capsule,
				cone
			};

			virtual ~iShape() = default;
			virtual shape_types get_shape_type() = 0;
		};
	}
}
