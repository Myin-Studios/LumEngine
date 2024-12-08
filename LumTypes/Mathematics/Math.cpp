/////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                   ///
///                                      THIS CODE IS PART OF:                                        ///
///                                          LumEngine (C)                                            ///
///                                                                                                   ///
///                                     WHICH IS LICENSED UNDER                                       ///
///                                          MIT License                                              ///
///                                      All Right Reserved.                                          ///
///                                                                                                   ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                   ///
///   Copyright (c) 2024 Myin Studios                                                                 ///
///                                                                                                   ///
///   Permission is hereby granted, free of charge,  to any person obtaining a copy of this software  ///
///   and associated documentation files (the "Software"), to deal in the Software without            ///
///   restriction, including without limitation the rights to use, copy, modify, merge, publish,      ///
///   distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the   ///
///   Software is furnished to do so, subject to the following conditions:                            ///
///                                                                                                   ///
///   The above copyright notice and this permission notice shall be included in all copies or        ///
///   substantial portions of the Software.                                                           ///
///                                                                                                   ///
///   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING   ///
///   BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND      ///
///   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,    ///
///   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  ///
///   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.         ///
///                                                                                                   ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Math.h"

Mat4Core::Mat4Core(const Mat4Core& other)
{
	this->_m00 = other._m00;
	this->_m01 = other._m01;
	this->_m02 = other._m02;
	this->_m03 = other._m03;
	this->_m10 = other._m10;
	this->_m11 = other._m11;
	this->_m12 = other._m12;
	this->_m13 = other._m13;
	this->_m20 = other._m20;
	this->_m21 = other._m21;
	this->_m22 = other._m22;
	this->_m23 = other._m23;
	this->_m30 = other._m30;
	this->_m31 = other._m31;
	this->_m32 = other._m32;
	this->_m33 = other._m33;
}

Mat4Core::Mat4Core(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33) :
	_m00(m00), _m01(m01), _m02(m02), _m03(m03),
	_m10(m10), _m11(m11), _m12(m12), _m13(m13),
	_m20(m20), _m21(m21), _m22(m22), _m23(m23),
	_m30(m30), _m31(m31), _m32(m32), _m33(m33)
{

}

const float Mat4Core::m00() const { return _m00; }
const float Mat4Core::m01() const { return _m01; }
const float Mat4Core::m02() const { return _m02; }
const float Mat4Core::m03() const { return _m03; }
const float Mat4Core::m10() const { return _m10; }
const float Mat4Core::m11() const { return _m11; }
const float Mat4Core::m12() const { return _m12; }
const float Mat4Core::m13() const { return _m13; }
const float Mat4Core::m20() const { return _m20; }
const float Mat4Core::m21() const { return _m21; }
const float Mat4Core::m22() const { return _m22; }
const float Mat4Core::m23() const { return _m23; }
const float Mat4Core::m30() const { return _m30; }
const float Mat4Core::m31() const { return _m31; }
const float Mat4Core::m32() const { return _m32; }
const float Mat4Core::m33() const { return _m33; }

Mat4Core Mat4Core::Inverse() const
{
	// Primo passo: calcolo del determinante
	float det = Determinant();

	// Se il determinante è zero, la matrice non è invertibile
	if (std::abs(det) < 1e-7f) {
		std::cerr << "Matrix is not invertible" << std::endl;
		return *this;
	}

	// Calcolo della matrice dei cofattori
	Mat4Core adj;

	// Calcolo dei cofattori
	adj._m00 = Cofactor(1, 2, 3, 1, 2, 3);
	adj._m01 = -Cofactor(0, 2, 3, 1, 2, 3);
	adj._m02 = Cofactor(0, 1, 3, 1, 2, 3);
	adj._m03 = -Cofactor(0, 1, 2, 1, 2, 3);

	adj._m10 = -Cofactor(1, 2, 3, 0, 2, 3);
	adj._m11 = Cofactor(0, 2, 3, 0, 2, 3);
	adj._m12 = -Cofactor(0, 1, 3, 0, 2, 3);
	adj._m13 = Cofactor(0, 1, 2, 0, 2, 3);

	adj._m20 = Cofactor(1, 2, 3, 0, 1, 3);
	adj._m21 = -Cofactor(0, 2, 3, 0, 1, 3);
	adj._m22 = Cofactor(0, 1, 3, 0, 1, 3);
	adj._m23 = -Cofactor(0, 1, 2, 0, 1, 3);

	adj._m30 = -Cofactor(1, 2, 3, 0, 1, 2);
	adj._m31 = Cofactor(0, 2, 3, 0, 1, 2);
	adj._m32 = -Cofactor(0, 1, 3, 0, 1, 2);
	adj._m33 = Cofactor(0, 1, 2, 0, 1, 2);

	// Dividiamo per il determinante
	float invDet = 1.0f / det;
	Mat4Core result;
	result._m00 = adj._m00 * invDet;
	result._m01 = adj._m01 * invDet;
	result._m02 = adj._m02 * invDet;
	result._m03 = adj._m03 * invDet;
	result._m10 = adj._m10 * invDet;
	result._m11 = adj._m11 * invDet;
	result._m12 = adj._m12 * invDet;
	result._m13 = adj._m13 * invDet;
	result._m20 = adj._m20 * invDet;
	result._m21 = adj._m21 * invDet;
	result._m22 = adj._m22 * invDet;
	result._m23 = adj._m23 * invDet;
	result._m30 = adj._m30 * invDet;
	result._m31 = adj._m31 * invDet;
	result._m32 = adj._m32 * invDet;
	result._m33 = adj._m33 * invDet;

	return result;
}

float Mat4Core::Det3x3(float a00, float a01, float a02, float a10, float a11, float a12, float a20, float a21, float a22) const
{
	return a00 * (a11 * a22 - a12 * a21)
		- a01 * (a10 * a22 - a12 * a20)
		+ a02 * (a10 * a21 - a11 * a20);
}

float Mat4Core::Cofactor(int i0, int i1, int i2, int j0, int j1, int j2) const {
	float m[3][3];
	int index = 0;

	for (int i = 0; i < 4; i++) {
		if (i == i0 || i == i1 || i == i2) {
			int jindex = 0;
			for (int j = 0; j < 4; j++) {
				if (j == j0 || j == j1 || j == j2) {
					// Ottieni il corretto elemento della matrice 4x4
					switch (i) {
					case 0:
						switch (j) {
						case 0: m[index][jindex] = _m00; break;
						case 1: m[index][jindex] = _m01; break;
						case 2: m[index][jindex] = _m02; break;
						case 3: m[index][jindex] = _m03; break;
						}
						break;
					case 1:
						switch (j) {
						case 0: m[index][jindex] = _m10; break;
						case 1: m[index][jindex] = _m11; break;
						case 2: m[index][jindex] = _m12; break;
						case 3: m[index][jindex] = _m13; break;
						}
						break;
					case 2:
						switch (j) {
						case 0: m[index][jindex] = _m20; break;
						case 1: m[index][jindex] = _m21; break;
						case 2: m[index][jindex] = _m22; break;
						case 3: m[index][jindex] = _m23; break;
						}
						break;
					case 3:
						switch (j) {
						case 0: m[index][jindex] = _m30; break;
						case 1: m[index][jindex] = _m31; break;
						case 2: m[index][jindex] = _m32; break;
						case 3: m[index][jindex] = _m33; break;
						}
						break;
					}
					jindex++;
				}
			}
			index++;
		}
	}

	return Det3x3(m[0][0], m[0][1], m[0][2],
		m[1][0], m[1][1], m[1][2],
		m[2][0], m[2][1], m[2][2]);
}

float Mat4Core::Determinant() const
{
	return _m00 * Cofactor(1, 2, 3, 1, 2, 3)
		 - _m01 * Cofactor(1, 2, 3, 0, 2, 3)
		 + _m02 * Cofactor(1, 2, 3, 0, 1, 3)
		 - _m03 * Cofactor(1, 2, 3, 0, 1, 2);
}

Mat4Core& Mat4Core::operator=(const Mat4Core& other)
{
	this->_m00 = other._m00;
	this->_m01 = other._m01;
	this->_m02 = other._m02;
	this->_m03 = other._m03;
	this->_m10 = other._m10;
	this->_m11 = other._m11;
	this->_m12 = other._m12;
	this->_m13 = other._m13;
	this->_m20 = other._m20;
	this->_m21 = other._m21;
	this->_m22 = other._m22;
	this->_m23 = other._m23;
	this->_m30 = other._m30;
	this->_m31 = other._m31;
	this->_m32 = other._m32;
	this->_m33 = other._m33;

	return *this;
}

Mat4Core Mat4Core::operator*(const Mat4Core& other)
{
	Mat4Core result;

	// Prima riga
	result._m00 = _m00 * other._m00 + _m01 * other._m10 + _m02 * other._m20 + _m03 * other._m30;
	result._m01 = _m00 * other._m01 + _m01 * other._m11 + _m02 * other._m21 + _m03 * other._m31;
	result._m02 = _m00 * other._m02 + _m01 * other._m12 + _m02 * other._m22 + _m03 * other._m32;
	result._m03 = _m00 * other._m03 + _m01 * other._m13 + _m02 * other._m23 + _m03 * other._m33;

	// Seconda riga
	result._m10 = _m10 * other._m00 + _m11 * other._m10 + _m12 * other._m20 + _m13 * other._m30;
	result._m11 = _m10 * other._m01 + _m11 * other._m11 + _m12 * other._m21 + _m13 * other._m31;
	result._m12 = _m10 * other._m02 + _m11 * other._m12 + _m12 * other._m22 + _m13 * other._m32;
	result._m13 = _m10 * other._m03 + _m11 * other._m13 + _m12 * other._m23 + _m13 * other._m33;

	// Terza riga
	result._m20 = _m20 * other._m00 + _m21 * other._m10 + _m22 * other._m20 + _m23 * other._m30;
	result._m21 = _m20 * other._m01 + _m21 * other._m11 + _m22 * other._m21 + _m23 * other._m31;
	result._m22 = _m20 * other._m02 + _m21 * other._m12 + _m22 * other._m22 + _m23 * other._m32;
	result._m23 = _m20 * other._m03 + _m21 * other._m13 + _m22 * other._m23 + _m23 * other._m33;

	// Quarta riga
	result._m30 = _m30 * other._m00 + _m31 * other._m10 + _m32 * other._m20 + _m33 * other._m30;
	result._m31 = _m30 * other._m01 + _m31 * other._m11 + _m32 * other._m21 + _m33 * other._m31;
	result._m32 = _m30 * other._m02 + _m31 * other._m12 + _m32 * other._m22 + _m33 * other._m32;
	result._m33 = _m30 * other._m03 + _m31 * other._m13 + _m32 * other._m23 + _m33 * other._m33;

	return result;
}

Vec4Core Mat4Core::operator*(const Vec4Core& v) const
{
	return Vec4Core(
		m00() * v.x() + m01() * v.y() + m02() * v.z() + m03() * v.w(),
		m10() * v.x() + m11() * v.y() + m12() * v.z() + m13() * v.w(),
		m20() * v.x() + m21() * v.y() + m22() * v.z() + m23() * v.w(),
		m30() * v.x() + m31() * v.y() + m32() * v.z() + m33() * v.w()
	);
}

std::string Mat4Core::ToString() const
{
	return std::string("Mat4Core(") +
			    std::to_string(_m00) + ", " + std::to_string(_m01) + ", " + std::to_string(_m02) + ", " + std::to_string(_m03) + ",\n" +
		"\t " + std::to_string(_m10) + ", " + std::to_string(_m11) + ", " + std::to_string(_m12) + ", " + std::to_string(_m13) + ",\n" +
		"\t " + std::to_string(_m20) + ", " + std::to_string(_m21) + ", " + std::to_string(_m22) + ", " + std::to_string(_m23) + ",\n" +
		"\t " + std::to_string(_m30) + ", " + std::to_string(_m31) + ", " + std::to_string(_m32) + ", " + std::to_string(_m33) + ")";

}

Vec4Core::Vec4Core(float x, float y, float z, float w) : _x(x), _y(y), _z(z), _w(w)
{
}

Vec4Core Vec4Core::operator/(float scalar) const
{
	return Vec4Core(_x / scalar, _y / scalar, _z / scalar, _w / scalar);
}

Vec4Core Vec4Core::operator-(const Vec4Core& other) const
{
	return Vec4Core(_x - other._x, _y - other._y, _z - other._z, _w - other._w);
}

std::string Vec4Core::ToString() const
{
	return std::string("Vec4Core(") + std::to_string(_x) + ", " + std::to_string(_y) + ", " + std::to_string(_z) + ", " + std::to_string(_w) + ")";
}

float Vec4Core::x() const { return _x; }
float Vec4Core::y() const { return _y; }
float Vec4Core::z() const { return _z; }
float Vec4Core::w() const { return _w; }

Vec3Core::Vec3Core() : _x(0.0f), _y(0.0f), _z(0.0f) {}

Vec3Core::Vec3Core(const Vec3Core& other) : _x(other._x), _y(other._y), _z(other._z) {}

Vec3Core::Vec3Core(float x, float y, float z) : _x(x), _y(y), _z(z) {}

Vec3Core::Vec3Core(const Vec4Core& v) : _x(v.x()), _y(v.y()), _z(v.z())
{
}

float Vec3Core::x() const { return this->_x; }
float Vec3Core::y() const { return this->_y; }
float Vec3Core::z() const { return this->_z; }

void Vec3Core::setX(float x)
{
	this->_x = x;
}

void Vec3Core::setY(float y)
{
	this->_y = y;
}

void Vec3Core::setZ(float z)
{
	this->_z = z;
}

Vec3Core Vec3Core::Cross(const Vec3Core& v1, const Vec3Core& v2)
{
	return {
		v1.y() * v2.z() - v1.z() * v2.y(),
		v2.z() * v1.x() - v2.x() * v1.z(),
		v1.x() * v2.y() - v1.y() * v2.x() };
}

Vec3Core Vec3Core::Lerp(const Vec3Core& v1, const Vec3Core& v2, float t)
{
	return v1 + (v2 - v1) * t;
}

float Vec3Core::length() const
{
	return std::sqrt(x() * x() + y() * y() + z() * z());
}

Vec3Core& Vec3Core::Normalize()
{
	if (this->length() != 0.0f) {
		_x /= this->length();
		_y /= this->length();
		_z /= this->length();
	}
	return *this;
}

Vec3Core& Vec3Core::operator=(const Vec3Core& v)
{
	this->_x = v._x;
	this->_y = v._y;
	this->_z = v._z;
	return *this;
}

Vec3Core& Vec3Core::operator=(const Vec3Core* v)
{
	if (v != nullptr) {
		this->_x = v->_x;
		this->_y = v->_y;
		this->_z = v->_z;
	}
	return *this;
}

Vec3Core Vec3Core::operator*(const float scalar) const
{
	return { this->_x * scalar, this->_y * scalar, this->_z * scalar };
}

float Vec3Core::operator*(const Vec3Core& v) const
{
	return this->_x * v._x + this->_y * v._y + this->_z * v._z;
}

Vec3Core Vec3Core::operator+(const Vec3Core& v) const
{
	return { this->_x + v._x, this->_y + v._y, this->_z + v._z };
}

Vec3Core Vec3Core::operator+(const float s) const
{
	return { _x + s, _y + s, _z + s };
}

Vec3Core& Vec3Core::operator+=(const Vec3Core& v)
{
	this->_x += v.x();
	this->_y += v.y();
	this->_z += v.z();

	return *this;
}

Vec3Core& Vec3Core::operator+=(const float s)
{
	this->_x += s;
	this->_y += s;
	this->_z += s;

	return *this;
}

Vec3Core Vec3Core::operator-(const Vec3Core& v) const
{
	return { this->_x - v._x, this->_y - v._y, this->_z - v._z };
}

Vec3Core Vec3Core::operator-(const float s) const
{
	return { this->_x - s, this->_y - s, this->_z - s };
}

Vec3Core& Vec3Core::operator-=(const Vec3Core& v)
{
	_x -= v.x();
	_y -= v.y();
	_z -= v.z();
	return *this;
}

Vec3Core& Vec3Core::operator/=(const float s)
{
	if (s != 0) {
		_x /= s;
		_y /= s;
		_z /= s;
	}
	return *this;
}

Vec3Core Vec3Core::operator^(const Vec3Core& v) const
{
	return Cross(*this, v);
}

bool Vec3Core::operator==(const Vec3Core& v) const
{
	return (this->_x == v._x && this->_y == v._y && this->_z == v._z);
}

std::string Vec3Core::ToString() const
{
	return std::string("Vec3Core(") + std::to_string(_x) + ", " + std::to_string(_y) + ", " + std::to_string(_z) + ")";
}