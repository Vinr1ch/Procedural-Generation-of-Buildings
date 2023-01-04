#include "Geometry.h"
#include "Vec3.h"
#include <string>
#include "math.h"


const float PI = 3.14159265358979323846;

Geometry::Geometry(std::string name, Vec3 &position, Vec3 &size, GeometryShape shape) {
    this->name = name;
    this->position = position;
    this->size = size;
    this->shape = shape;
}

Geometry &Geometry::move(Vec3 &translation) {
    this->position.add(translation);
    return *this;
}

Vec3& Geometry::getPosition() { return this->position; }

Vec3& Geometry::getSize() { return this->size; }

GeometryShape Geometry::getType() {return this->shape; }

void Geometry::setType(GeometryShape shape) {this->shape = shape; }

Vec3 Geometry::axis_rotation(Vec3 &size, float degrees, int axis) {
    float angle = PI * degrees / 180.0;
    float cos_angle = cos(angle);
    float sin_angle = sin(angle);
    float posX = size.getX();
    float posY = size.getY();
    float posZ = size.getZ();
    float x, y, z; 
    switch (axis) {
        case 0: {
            x = cos_angle * posX - sin_angle * posY;
            y = sin_angle * posX + cos_angle * posY;
            z = posZ;
        }
            break;
        case 1: {
            x = cos_angle * posX + sin_angle * posZ;
            y = posY;
            z = -sin_angle * posX + cos_angle * posZ;
        }
            break;
        case 2: {
            x = posX;
            y = cos_angle * posY - sin_angle * posZ;
            z = sin_angle * posY + cos_angle * posZ;
        }
        default:
            break;
    }

    return Vec3(x, y, z);

}

Geometry &Geometry::rotate(Vec3 &angles) {
    Vec3 rotatedX = axis_rotation(this->size, angles.getX(), 0);
    Vec3 rotatedY = axis_rotation(rotatedX, angles.getY(), 1);
    Vec3 rotatedZ = axis_rotation(rotatedY, angles.getZ(), 2);
    this->size.set(rotatedZ);
    return *this;
}

Geometry &Geometry::setSize(Vec3 &newSize) {
	if (newSize.getX() > -1000)
		this->size.setElement(0, newSize.getX());
	if (newSize.getY() > -1000)
		this->size.setElement(1, newSize.getY());
	if (newSize.getZ() > -1000)
		this->size.setElement(2, newSize.getZ());
    return *this;
}

std::vector<Geometry> Geometry::split(int axis, std::vector<float> ratios, std::vector<std::string> newShapeNames) {
    std::vector<Geometry> successors;
    if (ratios.size() != newShapeNames.size()) {
        throw "Illegal split, num of ratios and names should be the same" ;
        successors.push_back(*this);
        return successors;
    }
    Vec3 newPosition = this->position.copy();;
    Vec3 newSize = this->size.copy();
    float positionChange = this->position.getElement(axis);

    for (int i = 0; i < ratios.size(); i++) {
        newSize.setElement(axis, ratios[i]);
        if (i != 0)
            positionChange += ratios[i - 1];
        newPosition.setElement(axis, positionChange);
        Geometry newShape = Geometry(newShapeNames[i], newPosition, newSize, this->shape);
        successors.push_back(newShape);
    }
    return successors;
}

std::string Geometry::getName() { return this->name; }

void Geometry::setName(std::string name){
	this->name = name;
}

std::vector<Geometry> Geometry::repeat(int axis, int times, std::string newShapesName) {
    std::vector<Geometry> successors;
    float ratio = this->size.getElement(axis)/times;

    Vec3 newPosition = this->position.copy();
    Vec3 newSize = this->size.copy();
	
	float initPosition = this->position.getElement(axis);
    for(int i=0; i<times; i++){
        newPosition.setElement(axis, initPosition + i*ratio);
        newSize.setElement(axis,ratio);
        Geometry newShape(newShapesName,newPosition,newSize, this->shape);
        successors.push_back(newShape);
    }
    return successors;
}

std::vector<Geometry> Geometry::realRepeat(int axis, int times, std::string newShapesName) {
	std::vector<Geometry> successors;

	Vec3 newPosition = this->position.copy();
	Vec3 newSize = this->size.copy();

	float initPosition = this->position.getElement(axis);
	for (int i = 0; i<times; i++) {
		
		Geometry newShape(newShapesName, newPosition, newSize, this->shape);
		//std::cout << newShape << std::endl;
		successors.push_back(newShape);
		newPosition.setElement(axis,newPosition.getElement(axis)+this->size.getElement(axis));
		//std::cout << newPosition << std::endl;
		
	}

	return successors;
}

std::vector<Geometry> Geometry::componentSplit(std::string shape, std::vector<std::string> newShapeNames) {
	std::vector<Geometry> newShapes;
	auto position = this->getPosition();
	auto size = this->getSize();
	
	//front
	auto fp = position.copy();
	auto fs = size.copy();
	fs.setElement(0,-3);
	newShapes.push_back(Geometry(newShapeNames[0],fp,fs,SCOPE));
	//back
	auto bp = position.copy();
	auto bs = size.copy();
	bp.setElement(0,bp.getX()+ bs.getX());
	bs.setElement(0, -1);
	newShapes.push_back(Geometry(newShapeNames[1], bp, bs, SCOPE));
	//left
	auto lp = position.copy();
	auto ls = size.copy();
	ls.setElement(2, -3);
	//std::cout << "l " << ls << std::endl;
	//std::cout << lp << std::endl;
	newShapes.push_back(Geometry(newShapeNames[2], lp, ls, SCOPE));
	//right
	auto rp = position.copy();
	auto rs = size.copy();
	rp.setElement(2, rp.getZ()+rs.getZ());
	rs.setElement(2, -1);
	newShapes.push_back(Geometry(newShapeNames[3], rp, rs, SCOPE));

	if (shape.compare("sides") != 0) {
	//top
		auto tp = position.copy();
		auto ts = size.copy();
		tp.setElement(1, tp.getY()+ ts.getY());
		ts.setElement(1, -1);
		newShapes.push_back(Geometry(newShapeNames[4], tp, ts, SCOPE));
	//bottom
		auto bbp = position.copy();
		auto bbs = size.copy();
		bbs.setElement(1, -3);
		newShapes.push_back(Geometry(newShapeNames[5], bbp, bbs, SCOPE));
	}
	return newShapes;
}



