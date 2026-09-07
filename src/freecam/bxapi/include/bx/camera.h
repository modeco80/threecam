#ifndef BX_CAMERA_H
#define BX_CAMERA_H

#include <bx/math.h>
#include <ml/types.h>

class cSimpleCamera {
   public:
	u8 __pad[0x20];
	tPose pose;
	t4Matrix mat;
	t4Vector vec;
	void* vtable;
};

ML_STATIC_ASSERT(sizeof(cSimpleCamera) == 0x94);

enum eCamControllerType {
	eCamController_Invalid = 0,
	eCamController_Manual,
	eCamController_Chase,
	eCamController_Unk
};

class cCamera : public cSimpleCamera {
   public:
	virtual ~cCamera();

	void update();

	void setCurrentController(eCamControllerType controller);
}; //

#endif
