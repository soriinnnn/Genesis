#ifndef GENESIS_CAMERA_H
#define GENESIS_CAMERA_H
#include <core/Base.h>

namespace genesis
{
	struct CameraDesc
	{
		BaseDesc base;
	};

	class Camera: public Base
	{
	public:
		explicit Camera(const CameraDesc& desc);
		virtual ~Camera() override;

	private:

	};

}

#endif