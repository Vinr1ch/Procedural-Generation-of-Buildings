#pragma once

#include <iostream>
#include <vector>
#include "Vec3.h"
#include "GeometryShape.h"
#include <stack>

class Geometry {
    std::string name;
    Vec3 position;
    Vec3 size;
    //size of the scope
    GeometryShape shape; //scope/non-scope
    

    Vec3 axis_rotation(Vec3 &position, float degrees, int axe);

public:
    Geometry() = default;
    Geometry(std::string name, Vec3 &position, Vec3 &size, GeometryShape shape);

    /* Adds translation vector to the scope position*/
    Geometry &move(Vec3 &translation);

    /* Rotates the respective axis of the coordinate system */
    Geometry &rotate(Vec3 &angles);

    /* Sets the size of the scope */
    Geometry &setSize(Vec3 &newSize);

    void setType(GeometryShape shape);

    GeometryShape getType();

    Vec3& getPosition();

    Vec3& getSize();

    std::string getName();
	void setName(std::string name);

    /* Split 
    splits the current scope along one axis.
    Example: fac -> Subdiv(1,3.5,0.3,3,3,3){ floor | ledge | floor | floor | floor } 
	Axes x=0, y=1, z=2 */
    std::vector<Geometry> split(int axis, std::vector<float> ratios, std::vector<std::string> newShapeNames);

    /* To allow for larger scale changes in the split rules, we often want to tile a specified element.
    Example :floor -> Repeat(0, 2) { B }.
	Axes x=0, y=1, z=2 
	*/
    std::vector<Geometry> repeat(int axis, int times, std::string newShapesName);

	std::vector<Geometry> Geometry::realRepeat(int axis, int times, std::string newShapesName);

    /* Splits the shape into shapes of lesser dimensions :
    a -> Comp(shape, param) { A | B | ... | Z }
    Where shape identifies the shape of the component split with associated parameters param(if any).*/
	std::vector<Geometry> Geometry::componentSplit(std::string shape, std::vector<std::string> newShapeNames);
};

