#include "Vec3.h"

Vec3::Vec3(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
};

Vec3 Vec3::copy() {
    return Vec3(this->x, this->y, this->z);
}

float Vec3::getElement(int axis) {
    switch (axis) {
        case 0:
            return this->x;
        case 1:
            return this->y;
        case 2:
            return this->z;
        default:
            throw std::invalid_argument("No such axis!");
    }
}

void Vec3::setElement(int axis, float value) {
    switch (axis) {
        case 0:
            this->x = value;
            break;
        case 1:
            this->y = value;
            break;
        case 2:
            this->z = value;
            break;
        default:
            throw std::invalid_argument("No such axis!");
    }
}

void Vec3::add(Vec3& that) {
    this->x += that.x;
    this->y += that.y;
    this->z += that.z;
}

float Vec3::getX() { return this->x; }

float Vec3::getY() { return this->y; }

float Vec3::getZ() { return this->z; }

void Vec3::set(Vec3 &vector) {
    this->x = vector.getX();
    this->y = vector.getY();
    this->z = vector.getZ();
}


