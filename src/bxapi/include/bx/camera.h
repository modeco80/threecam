#ifndef BX_CAMERA_H
#define BX_CAMERA_H

#include <bx/math.h>
#include <ml/types.h>

class cSimpleCamera {
   public:
	float fov;
	float near;
	float far;
	float unk;
	float maxNear;
	float maxFar;
	u8 __pad[0x8];
	tPose pose;
	t4Matrix mat;
	t4Vector vec;
	void* vtable;

	void setPosition(const t4Vector& vec) {
		pose.p = vec;
	}

	t4Vector getPosition() const {
		return pose.p;
	}
};

ML_STATIC_ASSERT(sizeof(cSimpleCamera) == 0x94);

enum eCamControllerType {
	eCamController_Invalid = 0,
	eCamController_Manual,
	eCamController_Chase,
	eCamController_Script
};

class cCameraController {
public:
};

class cManualCameraAlgorithm {
public:
	u8 pad[0x20];
	t4Vector position;
};

ML_STATIC_ASSERT(ml_offsetof(cManualCameraAlgorithm, position) == 0x20);

class cManualCamController : public cCameraController {
public:
	u8 pad[0x14];
	cManualCameraAlgorithm* pManualAlgo;
};

ML_STATIC_ASSERT(ml_offsetof(cManualCamController, pManualAlgo) == 0x18);

class cCamera : public cSimpleCamera {
public:
	u8 pad[12];
	cCameraController* pActiveController;

	// sub controllers.
	cManualCamController* pManualCamController;
	cCameraController* pChaseController;
	cCameraController* pScriptController;


	void update();

	void setCurrentController(eCamControllerType controller);
};

ML_STATIC_ASSERT(ml_offsetof(cCamera,pActiveController) == 0xa0);

#endif
