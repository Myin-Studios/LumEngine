#pragma once

#include <iostream>
#include <cmath>

using namespace std;

class CMMat3x3
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
    CMMat3x3();
    explicit CMMat3x3(double val);
    explicit CMMat3x3(double m00 = 0, double m01 = 0, double m02 = 0, double m10 = 0, double m11 = 0, double m12 = 0, double m20 = 0, double m21 = 0, double m22 = 0);

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

CMMat3x3::CMMat3x3() = default;
CMMat3x3::CMMat3x3(double val)
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
CMMat3x3::CMMat3x3(double m00, double m01, double m02, double m10, double m11, double m12, double m20, double m21,
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

class CMQuat
{
public:
	CMQuat();
	CMQuat(double, double, double, double);
	~CMQuat();

	const double& x = 0;
	const double& y = 0;
	const double& z = 0;
	const double& w = 1;

	CMQuat normalize()
	{
		double _x = 0, _y = 0, _z = 0, _w = 0;
		float _len = sqrt(pow(m_X, 2) + pow(m_Y, 2) + pow(m_Z, 2) + pow(m_W, 2));
		_x = m_X / _len;
		_y = m_Y / _len;
		_z = m_Z / _len;
		_w = m_W / _len;

		return CMQuat(_x, _y, _z, _w);
	}

	CMQuat operator*(const CMQuat& q) const
	{
		double _x = 0;
		double _y = 0;
		double _z = 0;
		double _w = 0;

		_w = (this->m_W * q.m_W - this->m_X * q.m_X - this->m_Y * q.m_Y - this->m_Z * q.m_Z);
		_x = (this->m_W * q.m_X + q.m_W * this->m_X + this->m_Y * q.m_Z - q.m_Y * this->m_Z);
		_y = (this->m_W * q.m_Y + q.m_W * this->m_Y + this->m_Z * q.m_X - q.m_Z * this->m_X);
		_z = (this->m_W * q.m_Z + q.m_W * this->m_Z + this->m_X * q.m_Y - q.m_X * this->m_Y);

		return {_x, _y, _z, _w};
	}

    CMQuat operator*=(const CMQuat& q)
    {
        double _x = 0;
        double _y = 0;
        double _z = 0;
        double _w = 0;

        _w = (this->m_W * q.m_W - this->m_X * q.m_X - this->m_Y * q.m_Y - this->m_Z * q.m_Z);
        _x = (this->m_W * q.m_X + q.m_W * this->m_X + this->m_Y * q.m_Z - q.m_Y * this->m_Z);
        _y = (this->m_W * q.m_Y + q.m_W * this->m_Y + this->m_Z * q.m_X - q.m_Z * this->m_X);
        _z = (this->m_W * q.m_Z + q.m_W * this->m_Z + this->m_X * q.m_Y - q.m_X * this->m_Y);

        this->m_X = _x;
        this->m_Y = _y;
        this->m_Z = _z;
        this->m_W = _w;
        return {this->m_X, this->m_Y, this->m_Z, this->m_W};
    }

	CMQuat operator=(const CMQuat& q)
	{
		return {m_X = q.m_X,
                m_Y = q.m_Y,
                m_Z = q.m_Z,
                m_W = q.m_W};
	}
private:
	double m_X = 0;
	double m_Y = 0;
	double m_Z = 0;
	double m_W = 1;
};

CMQuat::CMQuat() : x(m_X), y(m_Y), z(m_Z), w(m_W)
{
    this->m_X = 0;
    this->m_Y = 0;
    this->m_Z = 0;
    this->m_W = 1;
}

CMQuat::CMQuat(double xVal, double yVal, double zVal, double wVal) : x(m_X), y(m_Y), z(m_Z), w(m_W)
{
	m_X = xVal;
	m_Y = yVal;
	m_Z = zVal;
	m_W = wVal;
}

CMQuat::~CMQuat()
{
}

/// <summary>
/// CryoMoon 2D Vector
/// </summary>
class CMVec2
{
public:
	const double& x = 0;
	const double& y = 0;

private:
	double m_X = 0, m_Y = 0;

public:
	CMVec2();
	CMVec2(const int xVal, const int yVal);
	CMVec2(const float xVal, const float yVal);
	CMVec2(const double xVal, const double yVal);
	CMVec2(const long xVal, const long yVal);
	~CMVec2();

	static constexpr auto dot(const CMVec2& v1, const CMVec2& v2)
	{
		return (v1.m_X * v2.m_X + v1.m_Y * v2.m_Y);
	}
	static constexpr auto dot(const CMVec2& v1, auto scalar) { return v1.m_X * scalar + v1.m_Y * scalar; }

	CMVec2 right() { return CMVec2(1, 0); }
	CMVec2 left() { return CMVec2(-1, 0); }
	CMVec2 up() { return CMVec2(0, 1); }
	CMVec2 down() { return CMVec2(0, -1); }

	CMVec2 normalize()
	{
		double _x = 0, _y = 0;

		_x = m_X / (sqrt(pow(m_X, 2) + pow(m_Y, 2)));
		_y = m_Y / (sqrt(pow(m_X, 2) + pow(m_Y, 2)));
		
		return CMVec2(_x, _y);
	}

	//Operators overloading.

	CMVec2& operator+(const CMVec2& v)
	{
		CMVec2 temp(m_X += v.m_X, m_Y += v.m_Y);

		return temp;
	}
	CMVec2& operator+(const auto s)
	{
		CMVec2 temp(m_X += s, m_Y += s);
		return temp;
	}
    CMVec2 operator+=(const CMVec2& v)
    {
        return CMVec2(this->m_X += v.x, this->m_Y += v.y);;
    }
	CMVec2& operator=(const CMVec2& v)
	{
		CMVec2 temp(m_X = v.m_X, m_Y = v.m_Y);
		return temp;
	};
	bool& operator==(const CMVec2& v)
	{
		bool val = m_X == v.m_X && m_Y == v.m_Y;
		return val;
	}
};

ostream& operator<<(ostream& dest, const CMVec2& v) { return dest << "(" << v.x << ", " << v.y << ")"; }

CMVec2::CMVec2() : x(m_X), y(m_Y)
{
}

inline CMVec2::CMVec2(int xVal, int yVal) : x(m_X), y(m_Y)
{
	m_X = xVal;
	m_Y = yVal;
}

inline CMVec2::CMVec2(float xVal, float yVal) : x(m_X), y(m_Y)
{
	m_X = xVal;
	m_Y = yVal;
}

inline CMVec2::CMVec2(double xVal, double yVal) : x(m_X), y(m_Y)
{
	m_X = xVal;
	m_Y = yVal;
}

inline CMVec2::CMVec2(long xVal, long yVal) : x(m_X), y(m_Y)
{
	m_X = xVal;
	m_Y = yVal;
}

CMVec2::~CMVec2()
= default;

/// <summary>
/// CryoMoon 3D Vector
/// </summary>
class CMVec3
{
public:
	CMVec3();
	explicit CMVec3(double v);
	CMVec3(int xVal, int yVal, int zVal);
	CMVec3(float xVal, float yVal, float zVal);
	CMVec3(double xVal, double yVal, double zVal);
	CMVec3(long xVal, long yVal, long zVal);
	~CMVec3() = default;

	//------------------{ Coordinates (readonly) definitions }------------------

	[[nodiscard]] double x() const { return m_X; };
	[[nodiscard]] double y() const { return m_Y; };
	[[nodiscard]] double z() const { return m_Z; };

	//------------------{ Vectors multiplications }------------------
	
	static constexpr auto dot(const CMVec3& v1, const CMVec3& v2) { return (v1.m_X * v2.m_X + v1.m_Y * v2.m_Y + v1.m_Z * v2.m_Z); }
	
	static CMVec3 cross(const CMVec3& v1, const CMVec3& v2)
	{
		return {v1.y() * v2.z() - v1.z() * v2.y(), v2.z() * v1.x() - v2.x() * v1.z(), v1.x() * v2.y() - v1.y() * v2.x()};
	}

	//------------------{ Vectors directions }------------------

	/// <summary>
	/// Right direction.
	/// </summary>
	/// <returns>CMVec3</returns>
	static CMVec3 right() { return {1, 0, 0}; }
	
	/// <summary>
	/// Left direction.
	/// </summary>
	/// <returns>CMVec3</returns>
	static CMVec3 left() { return {-1, 0, 0}; }
	
	/// <summary>
	/// Up direction.
	/// </summary>
	/// <returns>CMVec3</returns>
	static CMVec3 up() { return {0, 1, 0}; }
	
	/// <summary>
	/// Down direction.
	/// </summary>
	/// <returns>CMVec3</returns>
	static CMVec3 down() { return {0, -1, 0}; }
	
	/// <summary>
	/// Forward direction.
	/// </summary>
	/// <returns>CMVec3</returns>
	static CMVec3 forward() { return {0, 0, 1}; }
	
	/// <summary>
	/// Backward direction.
	/// </summary>
	/// <returns>CMVec3</returns>
	static CMVec3 backward() { return {0, 0, -1}; }

	//------------------{ Vectors normalization }------------------

	/// <summary>
	/// Vector normalization.
	/// </summary>
	/// <returns>CMVec3</returns>
    CMVec3& normalize() {
        if (this->length() != 0.0f) {
            m_X /= this->length();
            m_Y /= this->length();
            m_Z /= this->length();
        }
        return *this;
    }

    //------------------{ Orthogonality }---------------------

    [[deprecated("Don't use this method since is not implemented.")]] static CMVec3 orthogonalize(const CMVec3& v)
    {
        return {1, 1, 1};
    }

	//------------------{ Vectors rotation }------------------

	CMVec3 rotate(const CMQuat& q)
	{
        CMVec3 v_norm = this->normalize();
        CMVec3 v_angle = CMVec3(q.x, q.y, q.z);

        v_angle = v_angle.normalize();

        double cos_theta = dot(v_norm, v_angle) / (v_norm.length() * v_angle.length());

        double angle_rad = acos(cos_theta);

        CMQuat rot = CMQuat(v_norm.m_X * sin(angle_rad / 2.0),
                            v_norm.m_Y * sin(angle_rad / 2.0),
                            v_norm.m_Z * sin(angle_rad / 2.0),
                            cos(angle_rad / 2));

        rot.normalize();

        CMQuat rot_conJ = CMQuat(-rot.x, -rot.y, -rot.z, rot.w);

        CMQuat inVtoQ(v_norm.x(), v_norm.y(), v_norm.z(), 1);

        CMQuat out = rot * inVtoQ;

        out *= rot_conJ;

        cout << out.x << endl;

        CMVec3 outV(out.x, out.y, out.z);

        return outV;

	}

    //CMVec3 rotate(CMQuat v)
    //{
    //	CMVec3 v_norm = v.normalize(); // Normalizza il vettore di input

    //	double angle = *this * v / this->length() * v.length(); // Calcola la lunghezza del vettore
    //
    //	//cout << angle << endl;

    //	//cout << angle << endl;

    //	// Converti l'angolo da gradi a radianti
    //	double ang_rad = angle * 3.14159265358979323846 / 180.0;

    //	//cout << sin(ang_rad / 2) << endl;

    //	// Costruisci il quaternione di rotazione
    //	CMQuat rot_quat(
    //		v_norm.x * sin(ang_rad / 2),
    //		v_norm.y * sin(ang_rad / 2),
    //		v_norm.z * sin(ang_rad / 2),
    //		1
    //	);

    //	// Normalizza il quaternione di rotazione
    //	rot_quat = rot_quat.normalize();

    //	// Costruisci il quaternione coniugato
    //	CMQuat rot_quat_con(rot_quat.x * -1, rot_quat.y * -1, rot_quat.z * -1, rot_quat.w);

    //	// Esegui la rotazione
    //	CMQuat in_v_quat(this->m_X, this->m_Y, this->m_Z, 0);
    //	CMQuat out_quat = rot_quat * in_v_quat * rot_quat_con;
    //	//cout << out_quat.y << endl;
    //	return {out_quat.x, out_quat.y, out_quat.z};
    //}

	void rotate(double angleX, double angleY, double angleZ)
	{
		//Deg to rad
		//angleX = angleX * 3.14159265358979323846 / 180.0;
		//angleY = angleY * 3.14159265358979323846 / 180.0;
		//angleZ = angleZ * 3.14159265358979323846 / 180.0;

        CMMat3x3 rotX = CMMat3x3(1,             0,               0,
                                 0, cos(angleX), -sin(angleX),
                                 0, sin(angleX), cos(angleX));

        CMMat3x3 rotY = CMMat3x3(cos(angleY), 0, -sin(angleY),
                                 0,              1,                0,
                                 sin(angleY), 0, cos(angleY));

        CMMat3x3 rotZ = CMMat3x3(cos(angleZ), -sin(angleZ), 0,
                                 sin(angleZ), cos(angleZ),   0,
                                 0,                            0, 1);

        this->normalize();

        CMMat3x3 _yz(this->m_X * 1, this->m_X * 0, this->m_X * 0,
                     this->m_Y * 0, this->m_Y * 1, this->m_Y * -1,
                     this->m_X * 0, this->m_X * 1, this->m_X * 1);

        CMMat3x3 _xz(this->m_X * 1, this->m_X * 0, this->m_X * -1,
                     this->m_Y * 0, this->m_Y * 1, this->m_Y * 0,
                     this->m_X * 1, this->m_X * 0, this->m_X * 1);

        CMMat3x3 _xy(this->m_X * 1, this->m_X * -1, this->m_X * 0,
                     this->m_Y * 1, this->m_Y * 1, this->m_Y * 0,
                     this->m_X * 0, this->m_X * 0, this->m_X * 1);

        

        cout << "(" << this->m_X << ", " << this->m_Y << ", " << this->m_Z << ")" << endl;

        if (angleX > 0)
           *this = *this * rotX;

        cout << "(" << this->m_X << ", " << this->m_Y << ", " << this->m_Z << ")" << endl;

        if (angleY > 0)
            *this = *this * rotY;

        cout << "(" << this->m_X << ", " << this->m_Y << ", " << this->m_Z << ")" << endl;

        if (angleZ > 0)
            *this = *this * rotZ;

        cout << "(" << this->m_X << ", " << this->m_Y << ", " << this->m_Z << ")" << endl;
    }

	//if (out_quat.x != 0 || out_quat.y != 0 || out_quat.z != 0)
	//	out_quat = out_quat.normalize();

	//in_v_quat = CMQuat(in_v_quat.x != 0 ? in_v_quat.x : 0, in_v_quat.y != 0 ? in_v_quat.y : 0, in_v_quat.z != 0 ? in_v_quat.z : 0, in_v_quat.w);


	//------------------{ Operators overloading }------------------

	/// <summary>
	/// Dot product of a vector and a scalar constant.
	/// </summary>
	/// <param name="v1">(CMVec3)</param>
	/// <param name="scalar">(int, float, double or long)</param>
	/// <returns>CMVec3</returns>
	CMVec3 operator*(const double scalar) const
	{
		return {this->m_X * scalar, this->m_Y * scalar, this->m_Z * scalar};
	}
	
	/// <summary>
	/// Dot product of two vectors.
	/// </summary>
	/// <param name="v1">(CMVec3)</param>
	/// <param name="v2">(CMVec3)</param>
	/// <returns>int, float, double or long</returns>
	double operator*(const CMVec3& v) const
	{
		return this->m_X * v.m_X + this->m_Y * v.m_Y + this->m_Z * v.m_Z;
	}

    /// <summary>
    /// Product between a vector and a matrix (3x3).
    /// </summary>
    /// <param name="m">(CMMat3x3)</param>
    /// <returns>CMVec3</returns>
    CMVec3 operator*(const CMMat3x3& m) const
    {
        double _x = this->m_X * m.m00() + this->m_X * m.m01() + this->m_X * m.m02();
        double _y = this->m_X * m.m00() + this->m_X * m.m01() + this->m_X * m.m02();
        double _z = this->m_X * m.m00() + this->m_X * m.m01() + this->m_X * m.m02();

        return {_x, _y, _z};
    }

	CMVec3& operator+(const CMVec3& v)
	{
		CMVec3 temp(m_X += v.m_X, m_Y += v.m_Y, m_Z += v.m_Z);

		return temp;
	}
	CMVec3 operator+(const auto s) const
	{
		return {m_X + s, m_Y + s, m_Z + s};
	}
	CMVec3 operator-(const CMVec3& v) const
	{
		return {m_X - v.m_X, m_Y - v.m_Y, m_Z - v.m_Z};
	}
	CMVec3 operator-(const auto s)
	{
        return {m_X - s, m_Y - s, m_Z - s};
	}

	CMVec3& operator=(const CMVec3& v)
	= default;

	bool operator==(const CMVec3& v) const
	{
		return (m_X == v.m_X && m_Y == v.m_Y && m_Z == v.m_Z);
	}

private:
	double m_X = 0, m_Y = 0, m_Z = 0;

	[[nodiscard]] double length() const {
		return std::sqrt(x() * x() + y() * y() + z() * z());
	}
};

ostream& operator<<(ostream& dest, const CMVec3& v) { return dest << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")"; }

CMVec3::CMVec3()
{
    m_X = 0;
    m_Y = 0;
    m_Z = 0;
}

inline CMVec3::CMVec3(const double v)
{
    m_X = v;
    m_Y = v;
    m_Z = v;
}

inline CMVec3::CMVec3(int xVal, int yVal, int zVal)
{
	m_X = xVal;
	m_Y = yVal;
	m_Z = zVal;
}

inline CMVec3::CMVec3(float xVal, float yVal, float zVal)
{
	m_X = xVal;
	m_Y = yVal;
	m_Z = zVal;
}

inline CMVec3::CMVec3(double xVal, double yVal, double zVal)
{
	m_X = xVal;
	m_Y = yVal;
	m_Z = zVal;
}

inline CMVec3::CMVec3(long xVal, long yVal, long zVal)
{
	m_X = xVal;
	m_Y = yVal;
	m_Z = zVal;
}

