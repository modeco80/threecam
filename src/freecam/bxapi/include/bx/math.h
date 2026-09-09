#ifndef BX_MATH_H
#define BX_MATH_H

#include <ml/types.h>

struct t2Vector {
	float v[2];

	t2Vector() {}
	t2Vector(float x, float y) {
		v[0] = x;
		v[1] = y;
	}

	float x() const { return v[0]; }
	float y() const { return v[1]; }
};

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

struct tARGB {
	float a;
	float r;
	float g;
	float b;

	tARGB() {
		a = 1.f;
		r = 0.f;
		g = 0.f;
		b = 0.f;
	}

	tARGB(float a, float r, float g, float b)
	{
		this->a = a;
		this->r = r;
		this->g = g;
		this->b = b;
	}
};

ML_STATIC_ASSERT(sizeof(tARGB) == 0x10);

#endif
