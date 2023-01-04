#pragma once
#include<iostream>

class Vec3 {
	float x;
	float y;
	float z;
	
public:
	Vec3() = default; 
	Vec3(float x, float y, float z);
    Vec3 copy();
    float getElement(int axis);
    void setElement(int axis, float value);
	void add(Vec3& that);
	float getX();
	float getY();
	float getZ();
	void set(Vec3& that);
};
