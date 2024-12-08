#pragma once

#include <iostream>
#include <string>
#include <cmath>

class Vec4Core;

class Mat4Core
{
public:
	Mat4Core(float m00 = 0.0f, float m01 = 0.0f, float m02 = 0.0f, float m03 = 0.0f,
		float m10 = 0.0f, float m11 = 0.0f, float m12 = 0.0f, float m13 = 0.0f,
		float m20 = 0.0f, float m21 = 0.0f, float m22 = 0.0f, float m23 = 0.0f,
		float m30 = 0.0f, float m31 = 0.0f, float m32 = 0.0f, float m33 = 0.0f);
	Mat4Core(const Mat4Core& other);
	
	const float m00() const;
	const float m01() const;
	const float m02() const;
	const float m03() const;
	const float m10() const;
	const float m11() const;
	const float m12() const;
	const float m13() const;
	const float m20() const;
	const float m21() const;
	const float m22() const;
	const float m23() const;
	const float m30() const;
	const float m31() const;
	const float m32() const;
	const float m33() const;

	Mat4Core Inverse() const;
	float Determinant() const;
	float Det3x3(float a00, float a01, float a02, float a10, float a11, float a12, float a20, float a21, float a22) const;
	float Cofactor(int i0, int i1, int i2, int j0, int j1, int j2) const;

	Mat4Core& operator=(const Mat4Core& other);
	Mat4Core operator*(const Mat4Core& other);
	Vec4Core operator*(const Vec4Core& v) const;

	std::string ToString() const;

private:
	float _m00 = 0.0f;
	float _m01 = 0.0f;
	float _m02 = 0.0f;
	float _m03 = 0.0f;
	float _m10 = 0.0f;
	float _m11 = 0.0f;
	float _m12 = 0.0f;
	float _m13 = 0.0f;
	float _m20 = 0.0f;
	float _m21 = 0.0f;
	float _m22 = 0.0f;
	float _m23 = 0.0f;
	float _m30 = 0.0f;
	float _m31 = 0.0f;
	float _m32 = 0.0f;
	float _m33 = 0.0f;
};

class Vec4Core
{
public:
	Vec4Core(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f);

	Vec4Core operator/(float scalar) const;
	Vec4Core operator-(const Vec4Core& other) const;
	std::string ToString() const;

	float x() const;
	float y() const;
	float z() const;
	float w() const;

private:
	float _x = 0.0f;
	float _y = 0.0f;
	float _z = 0.0f;
	float _w = 0.0f;
};

class Vec3Core
{
public:
	Vec3Core();
	Vec3Core(const Vec3Core& other);
	Vec3Core(float x, float y, float z);
	Vec3Core(const Vec4Core& v);

	float x() const;
	float y() const;
	float z() const;

	void setX(float x);
	void setY(float y);
	void setZ(float z);

	static Vec3Core Cross(const Vec3Core& v1, const Vec3Core& v2);
	static Vec3Core Lerp(const Vec3Core& v1, const Vec3Core& v2, float t);

	float length() const;
	Vec3Core& Normalize();

	Vec3Core& operator=(const Vec3Core& v);
	Vec3Core& operator=(const Vec3Core* v);
	Vec3Core operator*(const float scalar) const;
	float operator*(const Vec3Core& v) const;
	Vec3Core operator+(const Vec3Core& v) const;
	Vec3Core operator+(const float s) const;
	Vec3Core& operator+=(const Vec3Core& v);
	Vec3Core& operator+=(const float s);
	Vec3Core operator-(const Vec3Core& v) const;
	Vec3Core operator-(const float s) const;
	Vec3Core& operator-=(const Vec3Core& v);
	Vec3Core& operator/=(const float s);
	Vec3Core operator^(const Vec3Core& v) const;
	bool operator==(const Vec3Core& v) const;

	std::string ToString() const;

private:
	float _x = 0.0f;
	float _y = 0.0f;
	float _z = 0.0f;
};