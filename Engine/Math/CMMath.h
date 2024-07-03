/////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                   ///
///                                      THIS CODE IS PART OF:                                        ///
///                                       CryoMoon Engine (C)                                         ///
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

#pragma once

#include <iostream>
#include <cmath>

using namespace std;

class Mat3x3
{
private:
    double _m00 = 0;
    double _m01 = 0;
    double _m02 = 0;
    double _m10 = 0;
    double _m11 = 0;
    double _m12 = 0;
    double _m20 = 0;
    double _m21 = 0;
    double _m22 = 0;

public:
    Mat3x3();
    explicit Mat3x3(double val);
    explicit Mat3x3(double m00 = 0, double m01 = 0, double m02 = 0, double m10 = 0, double m11 = 0, double m12 = 0, double m20 = 0, double m21 = 0, double m22 = 0);

    [[nodiscard]] double m00() const { return this->_m00; }
    [[nodiscard]] double m01() const { return this->_m01; }
    [[nodiscard]] double m02() const { return this->_m02; }
    [[nodiscard]] double m10() const { return this->_m10; }
    [[nodiscard]] double m11() const { return this->_m11; }
    [[nodiscard]] double m12() const { return this->_m12; }
    [[nodiscard]] double m20() const { return this->_m20; }
    [[nodiscard]] double m21() const { return this->_m21; }
    [[nodiscard]] double m22() const { return this->_m22; }
};

Mat3x3::Mat3x3() = default;
Mat3x3::Mat3x3(double val)
{
    this->_m00 = val;
    this->_m01 = val;
    this->_m02 = val;
    this->_m10 = val;
    this->_m11 = val;
    this->_m12 = val;
    this->_m20 = val;
    this->_m21 = val;
    this->_m22 = val;
}
Mat3x3::Mat3x3(double m00, double m01, double m02, double m10, double m11, double m12, double m20, double m21,
               double m22) {
    this->_m00 = m00;
    this->_m01 = m01;
    this->_m02 = m02;
    this->_m10 = m10;
    this->_m11 = m11;
    this->_m12 = m12;
    this->_m20 = m20;
    this->_m21 = m21;
    this->_m22 = m22;
}

class Quat
{
public:
	Quat();
	Quat(double, double, double, double);
	~Quat();

	const double& x = 0;
	const double& y = 0;
	const double& z = 0;
	const double& w = 1;

	Quat normalize()
	{
		double _tx = 0, _ty = 0, _tz = 0, _tw = 0;
		float _len = sqrt(pow(_x, 2) + pow(_y, 2) + pow(_z, 2) + pow(_w, 2));
		_tx = _x / _len;
		_ty = _y / _len;
		_tz = _z / _len;
		_tw = _w / _len;

		return Quat(_x, _y, _z, _w);
	}

	Quat operator*(const Quat& q) const
	{
		double _tx = 0;
		double _ty = 0;
		double _tz = 0;
		double _tw = 0;

		_tw = (this->_w * q._w - this->_x * q._x - this->_y * q._y - this->_z * q._z);
		_tx = (this->_w * q._x + q._w * this->_x + this->_y * q._z - q._y * this->_z);
		_ty = (this->_w * q._y + q._w * this->_y + this->_z * q._x - q._z * this->_x);
		_tz = (this->_w * q._z + q._w * this->_z + this->_x * q._y - q._x * this->_y);

		return {_tx, _ty, _tz, _tw};
	}

    Quat operator*=(const Quat& q)
    {
        double _tx = 0;
        double _ty = 0;
        double _tz = 0;
        double _tw = 0;

        _tw = (this->_w * q._w - this->_x * q._x - this->_y * q._y - this->_z * q._z);
        _tx = (this->_w * q._x + q._w * this->_x + this->_y * q._z - q._y * this->_z);
        _ty = (this->_w * q._y + q._w * this->_y + this->_z * q._x - q._z * this->_x);
        _tz = (this->_w * q._z + q._w * this->_z + this->_x * q._y - q._x * this->_y);

        this->_x = _tx;
        this->_y = _ty;
        this->_z = _tz;
        this->_w = _tw;
        return {this->_x, this->_y, this->_z, this->_w};
    }

	Quat operator=(const Quat& q)
	{
		return {_x = q._x,
                _y = q._y,
                _z = q._z,
                _w = q._w};
	}
private:
	double _x = 0;
	double _y = 0;
	double _z = 0;
	double _w = 1;
};

Quat::Quat() : x(_x), y(_y), z(_z), w(_w)
{
    this->_x = 0;
    this->_y = 0;
    this->_z = 0;
    this->_w = 1;
}

Quat::Quat(double xVal, double yVal, double zVal, double wVal) : x(_x), y(_y), z(_z), w(_w)
{
    this->_x = xVal;
    this->_y = yVal;
    this->_z = zVal;
    this->_w = wVal;
}

Quat::~Quat()
{
}

/// <summary>
/// CryoMoon 2D Vector
/// </summary>
class Vec2
{
public:
	const double& x = 0;
	const double& y = 0;

private:
	double _x = 0, _y = 0;

public:
	Vec2();
	Vec2(const int xVal, const int yVal);
	Vec2(const float xVal, const float yVal);
	Vec2(const double xVal, const double yVal);
	Vec2(const long xVal, const long yVal);
	~Vec2();

	static constexpr auto dot(const Vec2& v1, const Vec2& v2)
	{
		return (v1._x * v2._x + v1._y * v2._y);
	}
	static constexpr double dot(const Vec2& v1, double scalar) { return v1._x * scalar + v1._y * scalar; }

	Vec2 right() { return Vec2(1, 0); }
	Vec2 left() { return Vec2(-1, 0); }
	Vec2 up() { return Vec2(0, 1); }
	Vec2 down() { return Vec2(0, -1); }

	Vec2 normalize()
	{
		double _tx = 0, _ty = 0;

		_tx = _x / (sqrt(pow(_x, 2) + pow(_y, 2)));
		_ty = _y / (sqrt(pow(_x, 2) + pow(_y, 2)));
		
		return Vec2(_tx, _ty);
	}

	//Operators overloading.

	Vec2& operator+(const Vec2& v)
	{
		Vec2 temp(_x += v._x, _y += v._y);

		return temp;
	}
	Vec2& operator+(const double s)
	{
		Vec2 temp(_x += s, _y += s);
		return temp;
	}
    Vec2 operator+=(const Vec2& v)
    {
        return Vec2(this->_x += v.x, this->_y += v.y);;
    }
	Vec2 operator=(const Vec2& v)
	{
		Vec2 temp(_x = v._x, _y = v._y);
		return temp;
	};
	bool operator==(const Vec2& v)
	{
		bool val = _x == v._x && _y == v._y;
		return val;
	}
};

ostream& operator<<(ostream& dest, const Vec2& v) { return dest << "(" << v.x << ", " << v.y << ")"; }

Vec2::Vec2() : x(_x), y(_y)
{
    _x = 0;
    _y = 0;
}

inline Vec2::Vec2(int xVal, int yVal) : x(_x), y(_y)
{
    _x = xVal;
    _y = yVal;
}

inline Vec2::Vec2(float xVal, float yVal) : x(_x), y(_y)
{
    _x = xVal;
    _y = yVal;
}

inline Vec2::Vec2(double xVal, double yVal) : x(_x), y(_y)
{
    _x = xVal;
    _y = yVal;
}

inline Vec2::Vec2(long xVal, long yVal) : x(_x), y(_y)
{
    _x = xVal;
    _y = yVal;
}

Vec2::~Vec2()
= default;

/// <summary>
/// CryoMoon 3D Vector
/// </summary>
class Vec3
{
public:
	Vec3();
	explicit Vec3(double v);
	Vec3(int xVal, int yVal, int zVal);
	Vec3(float xVal, float yVal, float zVal);
	Vec3(double xVal, double yVal, double zVal);
	Vec3(long xVal, long yVal, long zVal);
	~Vec3() = default;

	//------------------{ Coordinates (readonly) definitions }------------------

	[[nodiscard]] double x() const { return _x; };
	[[nodiscard]] double y() const { return _y; };
	[[nodiscard]] double z() const { return _z; };

	//------------------{ Vectors multiplications }------------------
	
	static constexpr auto dot(const Vec3& v1, const Vec3& v2) { return (v1._x * v2._x + v1._y * v2._y + v1._z * v2._z); }
	
	static Vec3 cross(const Vec3& v1, const Vec3& v2)
	{
		return {v1.y() * v2.z() - v1.z() * v2.y(), v2.z() * v1.x() - v2.x() * v1.z(), v1.x() * v2.y() - v1.y() * v2.x()};
	}

	//------------------{ Vectors directions }------------------

	/// <summary>
	/// Right direction.
	/// </summary>
	/// <returns>Vec3</returns>
	static Vec3 right() { return {1, 0, 0}; }
	
	/// <summary>
	/// Left direction.
	/// </summary>
	/// <returns>Vec3</returns>
	static Vec3 left() { return {-1, 0, 0}; }
	
	/// <summary>
	/// Up direction.
	/// </summary>
	/// <returns>Vec3</returns>
	static Vec3 up() { return {0, 1, 0}; }
	
	/// <summary>
	/// Down direction.
	/// </summary>
	/// <returns>Vec3</returns>
	static Vec3 down() { return {0, -1, 0}; }
	
	/// <summary>
	/// Forward direction.
	/// </summary>
	/// <returns>Vec3</returns>
	static Vec3 forward() { return {0, 0, 1}; }
	
	/// <summary>
	/// Backward direction.
	/// </summary>
	/// <returns>Vec3</returns>
	static Vec3 backward() { return {0, 0, -1}; }

	//------------------{ Vectors normalization }------------------

	/// <summary>
	/// Vector normalization.
	/// </summary>
	/// <returns>Vec3</returns>
    Vec3& normalize() {
        if (this->length() != 0.0f) {
            _x /= this->length();
            _y /= this->length();
            _z /= this->length();
        }
        return *this;
    }

	//------------------{ Vectors rotation }------------------

	/*Vec3 rotate(const Quat& q)
	{
        Vec3 v_norm = this->normalize();
        Vec3 v_angle = Vec3(q.x, q.y, q.z);

        v_angle = v_angle.normalize();

        double cos_theta = dot(v_norm, v_angle) / (v_norm.length() * v_angle.length());

        double angle_rad = acos(cos_theta);

        Quat rot = Quat(v_norm._x * sin(angle_rad / 2.0),
                            v_norm._y * sin(angle_rad / 2.0),
                            v_norm._z * sin(angle_rad / 2.0),
                        cos(angle_rad / 2));

        rot.normalize();

        Quat rot_conJ = Quat(-rot.x, -rot.y, -rot.z, rot.w);

        Quat inVtoQ(v_norm.x(), v_norm.y(), v_norm.z(), 1);

        Quat out = rot * inVtoQ;

        out *= rot_conJ;

        cout << out.x << endl;

        Vec3 outV(out.x, out.y, out.z);

        return outV;

	}*/

    //Vec3 rotate(Quat v)
    //{
    //	Vec3 v_norm = v.normalize(); // Normalizza il vettore di input

    //	double angle = *this * v / this->length() * v.length(); // Calcola la lunghezza del vettore
    //
    //	//cout << angle << endl;

    //	//cout << angle << endl;

    //	// Converti l'angolo da gradi a radianti
    //	double ang_rad = angle * 3.14159265358979323846 / 180.0;

    //	//cout << sin(ang_rad / 2) << endl;

    //	// Costruisci il quaternione di rotazione
    //	Quat rot_quat(
    //		v_norm.x * sin(ang_rad / 2),
    //		v_norm.y * sin(ang_rad / 2),
    //		v_norm.z * sin(ang_rad / 2),
    //		1
    //	);

    //	// Normalizza il quaternione di rotazione
    //	rot_quat = rot_quat.normalize();

    //	// Costruisci il quaternione coniugato
    //	Quat rot_quat_con(rot_quat.x * -1, rot_quat.y * -1, rot_quat.z * -1, rot_quat.w);

    //	// Esegui la rotazione
    //	Quat in_v_quat(this->_x, this->_y, this->_z, 0);
    //	Quat out_quat = rot_quat * in_v_quat * rot_quat_con;
    //	//cout << out_quat.y << endl;
    //	return {out_quat.x, out_quat.y, out_quat.z};
    //}

	/*void rotate(double angleX, double angleY, double angleZ)
	{
		//Deg to rad
		//angleX = angleX * 3.14159265358979323846 / 180.0;
		//angleY = angleY * 3.14159265358979323846 / 180.0;
		//angleZ = angleZ * 3.14159265358979323846 / 180.0;

        Mat3x3 rotX = Mat3x3(1, 0, 0,
                             0, cos(angleX), -sin(angleX),
                             0, sin(angleX), cos(angleX));

        Mat3x3 rotY = Mat3x3(cos(angleY), 0, -sin(angleY),
                             0, 1, 0,
                             sin(angleY), 0, cos(angleY));

        Mat3x3 rotZ = Mat3x3(cos(angleZ), -sin(angleZ), 0,
                             sin(angleZ), cos(angleZ), 0,
                             0, 0, 1);

        this->normalize();

        Mat3x3 _yz(this->_x * 1, this->_x * 0, this->_x * 0,
                     this->_y * 0, this->_y * 1, this->_y * -1,
                     this->_x * 0, this->_x * 1, this->_x * 1);

        Mat3x3 _xz(this->_x * 1, this->_x * 0, this->_x * -1,
                     this->_y * 0, this->_y * 1, this->_y * 0,
                     this->_x * 1, this->_x * 0, this->_x * 1);

        Mat3x3 _xy(this->_x * 1, this->_x * -1, this->_x * 0,
                     this->_y * 1, this->_y * 1, this->_y * 0,
                     this->_x * 0, this->_x * 0, this->_x * 1);

        

        cout << "(" << this->_x << ", " << this->_y << ", " << this->_z << ")" << endl;

        if (angleX > 0)
           *this = *this * rotX;

        cout << "(" << this->_x << ", " << this->_y << ", " << this->_z << ")" << endl;

        if (angleY > 0)
            *this = *this * rotY;

        cout << "(" << this->_x << ", " << this->_y << ", " << this->_z << ")" << endl;

        if (angleZ > 0)
            *this = *this * rotZ;

        cout << "(" << this->_x << ", " << this->_y << ", " << this->_z << ")" << endl;
    }*/

	//if (out_quat.x != 0 || out_quat.y != 0 || out_quat.z != 0)
	//	out_quat = out_quat.normalize();

	//in_v_quat = Quat(in_v_quat.x != 0 ? in_v_quat.x : 0, in_v_quat.y != 0 ? in_v_quat.y : 0, in_v_quat.z != 0 ? in_v_quat.z : 0, in_v_quat.w);


    //---------------------{ Interpolations }----------------------

    static Vec3& lerp(const Vec3& v1, const Vec3& v2, double t)
    {
        return {v1 + (v2 - v1) * t};
    }

	//------------------{ Operators overloading }------------------

    Vec3& operator=(const Vec3 v)
    {
        this->_x = v._x;
        this->_y = v._y;
        this->_z = v._z;
        return *this;
    }

	/// <summary>
	/// Dot product of a vector and a scalar constant.
	/// </summary>
	/// <param name="v1">(Vec3)</param>
	/// <param name="scalar">(int, float, double or long)</param>
	/// <returns>Vec3</returns>
	Vec3 operator*(const double scalar) const
	{
		return {this->_x * scalar, this->_y * scalar, this->_z * scalar};
	}
	
	/// <summary>
	/// Dot product of two vectors.
	/// </summary>
	/// <param name="v1">(Vec3)</param>
	/// <param name="v2">(Vec3)</param>
	/// <returns>int, float, double or long</returns>
    const double operator*(const Vec3& v) const
	{
		return this->_x * v._x + this->_y * v._y + this->_z * v._z;
	}

    /// <summary>
    /// Product between a vector and a matrix (3x3).
    /// </summary>
    /// <param name="m">(Mat3x3)</param>
    /// <returns>Vec3</returns>
    Vec3 operator*(const Mat3x3& m) const
    {
        double _x = this->_x * m.m00() + this->_x * m.m01() + this->_x * m.m02();
        double _y = this->_x * m.m00() + this->_x * m.m01() + this->_x * m.m02();
        double _z = this->_x * m.m00() + this->_x * m.m01() + this->_x * m.m02();

        return {_x, _y, _z};
    }

	Vec3& operator+(const Vec3& v) const
	{
		Vec3 temp(_x + v._x, _y + v._y, _z + v._z);

		return temp;
	}

	Vec3 operator+(const double s) const
	{
		return {_x + s, _y + s, _z + s};
	}
	Vec3 operator-(const Vec3& v) const
	{
		return {_x - v._x, _y - v._y, _z - v._z};
	}
	Vec3 operator-(const double s)
	{
        return {_x - s, _y - s, _z - s};
	}

	bool operator==(const Vec3& v) const
	{
		return (_x == v._x && _y == v._y && _z == v._z);
	}

private:
	double _x = 0, _y = 0, _z = 0;

	[[nodiscard]] double length() const {
		return std::sqrt(x() * x() + y() * y() + z() * z());
	}
};

ostream& operator<<(ostream& dest, const Vec3& v) { return dest << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")"; }

Vec3::Vec3()
{
    _x = 0;
    _y = 0;
    _z = 0;
}

inline Vec3::Vec3(const double v)
{
    _x = v;
    _y = v;
    _z = v;
}

inline Vec3::Vec3(int xVal, int yVal, int zVal)
{
    _x = xVal;
    _y = yVal;
    _z = zVal;
}

inline Vec3::Vec3(float xVal, float yVal, float zVal)
{
    _x = xVal;
    _y = yVal;
    _z = zVal;
}

inline Vec3::Vec3(double xVal, double yVal, double zVal)
{
    _x = xVal;
    _y = yVal;
    _z = zVal;
}

inline Vec3::Vec3(long xVal, long yVal, long zVal)
{
    _x = xVal;
    _y = yVal;
    _z = zVal;
}

