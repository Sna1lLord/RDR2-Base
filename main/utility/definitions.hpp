#ifndef _DEFINITIONS_HPP
#define _DEFINITIONS_HPP

#include <common.hpp>

typedef unsigned long Void;
typedef unsigned long Any;
typedef unsigned long uint;
typedef unsigned long Hash;
typedef int Entity;
typedef int Player;
typedef int FireId;
typedef int Ped;
typedef int Vehicle;
typedef int Cam;
typedef int CarGenerator;
typedef int Group;
typedef int Train;
typedef int Pickup;
typedef int Object;
typedef int Weapon;
typedef int Interior;
typedef int Blip;
typedef int Texture;
typedef int TextureDict;
typedef int CoverPoint;
typedef int Camera;
typedef int TaskSequence;
typedef int ColourIndex;
typedef int Sphere;
typedef int INT, ScrHandle;
typedef int AnimScene;
typedef int Prompt;
typedef int Volume;
typedef int PersChar;
typedef int PopZone;
typedef int ItemSet;
typedef int PropSet;

struct Vector4 {
    float x, y, z, w;
};

class Vector3 {
public:
    float x;
    float y;
    float z;
public:
    Vector3 operator+(const Vector3& other) {
        Vector3 result;
        result.x = x + other.x;
        result.y = y + other.y;
        result.z = z + other.z;

		return result;
	}

    Vector3 operator-(const Vector3& other) {
        Vector3 result;
        result.x = x - other.x;
        result.y = y - other.y;
        result.z = z - other.z;

		return result;
	}

    Vector3 operator*(const Vector3& other) {
        Vector3 result;
        result.x = x * other.x;
        result.y = y * other.y;
        result.z = z * other.z;
        
		return result;
	}

    Vector3 operator*(float other) {
        Vector3 result;
        result.x = x * other;
        result.y = y * other;
        result.z = z * other;
        
		return result;
	}
};

struct Vector2 {
    float x, y;
};

class NativeVector {
public:
    float x;
    unsigned long paddingX;
    float y;
    unsigned long paddingY;
    float z;
    unsigned long paddingZ;
public:
    NativeVector operator+(const NativeVector& other) {
        NativeVector result;
        result.x = x + other.x;
        result.y = y + other.y;
        result.z = z + other.z;

		return result;
	}

    NativeVector operator-(const NativeVector& other) {
        NativeVector result;
        result.x = x - other.x;
        result.y = y - other.y;
        result.z = z - other.z;

		return result;
	}

    NativeVector operator*(const NativeVector& other) {
        NativeVector result;
        result.x = x * other.x;
        result.y = y * other.y;
        result.z = z * other.z;
        
		return result;
	}

    NativeVector operator*(float other) {
        NativeVector result;
        result.x = x * other;
        result.y = y * other;
        result.z = z * other;
        
		return result;
	}
};

#endif