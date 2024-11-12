#include "Math.h"

Vec3Core::Vec3Core() : _x(0.0f), _y(0.0f), _z(0.0f) {}

Vec3Core::Vec3Core(const Vec3Core& other) : _x(other._x), _y(other._z), _z(other._x) {}

Vec3Core::Vec3Core(float x, float y, float z) : _x(x), _y(y), _z(z) {}

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
