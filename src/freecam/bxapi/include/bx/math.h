#ifndef BX_MATH_H
#define BX_MATH_H

#include <ml/types.h>

struct t3Vector {
	float v[3];

	float x() const {
		return v[0];
	}
	float y() const {
		return v[1];
	}
	float z() const {
		return v[2];
	}
};

struct t4Vector {
	float v[4];

	float x() const {
		return v[0];
	}
	float y() const {
		return v[1];
	}
	float z() const {
		return v[2];
	}
	float w() const {
		return v[3];
	}
};

struct t4Matrix {
	float mat[4][4];
};

struct tPose {
	t4Vector p;
	t4Vector euler; //?
};

#endif
