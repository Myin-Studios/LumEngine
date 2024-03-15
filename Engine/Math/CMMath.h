#pragma once

#include <iostream>
#include <cmath>

using namespace std;

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

	CMQuat operator*(CMQuat q) const
	{
		double _x = 0;
		double _y = 0;
		double _z = 0;
		double _w = 0;

		_w = (this->m_W * q.m_W - this->m_X * q.m_X - this->m_Y * q.m_Y - this->m_Z * q.m_Z);
		_x = (this->m_W * q.m_X + q.m_W * this->m_X + this->m_Y * q.m_Z - q.m_Y * this->m_Z);
		_y = (this->m_W * q.m_Y + q.m_W * this->m_Y + this->m_Z * q.m_X - q.m_Z * this->m_X);
		_z = (this->m_W * q.m_Z + q.m_W * this->m_Z + this->m_X * q.m_Y - q.m_X * this->m_Y);

		return CMQuat(_x, _y, _z, _w);
	}
	CMQuat& operator=(CMQuat q)
	{
		CMQuat temp(m_X = q.m_X,
			m_Y = q.m_Y,
			m_Z = q.m_Z,
			m_W = q.m_W);
		return temp;
	}
private:
	double m_X = 0;
	double m_Y = 0;
	double m_Z = 0;
	double m_W = 1;
};

CMQuat::CMQuat()
{
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
{
}

/// <summary>
/// CryoMoon 3D Vector
/// </summary>
class CMVec3
{
public:
	CMVec3();
	CMVec3(const CMVec2 v);
	CMVec3(int xVal, int yVal, int zVal);
	CMVec3(float xVal, float yVal, float zVal);
	CMVec3(double xVal, double yVal, double zVal);
	CMVec3(long xVal, long yVal, long zVal);
	~CMVec3();

	//------------------{ Coordinates (readonly) definitions }------------------

	const double& x = 0; //x coordinate (readonly).
	const double& y = 0; //y coordinate (readonly).
	const double& z = 0; //z coordinate (readonly).

	//------------------{ Vectors multiplications }------------------

	
	static constexpr auto dot(const CMVec3& v1, const CMVec3& v2) { return (v1.m_X * v2.m_X + v1.m_Y * v2.m_Y, v1.m_Z * v2.m_Z); }
	
	static CMVec3 cross(const CMVec3& v1, const CMVec3& v2)
	{
		return CMVec3(v1.y * v2.z - v1.z * v2.y, v2.z * v1.x - v2.x * v1.z, v1.x * v2.y - v1.y * v2.x);
	}

	//------------------{ Vectors directions }------------------

	/// <summary>
	/// Right direction.
	/// </summary>
	/// <returns>CMVec3</returns>
	CMVec3 right() { return CMVec3(1, 0, 0); }
	
	/// <summary>
	/// Left direction.
	/// </summary>
	/// <returns>CMVec3</returns>
	CMVec3 left() { return CMVec3(-1, 0, 0); }
	
	/// <summary>
	/// Up direction.
	/// </summary>
	/// <returns>CMVec3</returns>
	CMVec3 up() { return CMVec3(0, 1, 0); }
	
	/// <summary>
	/// Down direction.
	/// </summary>
	/// <returns>CMVec3</returns>
	CMVec3 down() { return CMVec3(0, -1, 0); }
	
	/// <summary>
	/// Forward direction.
	/// </summary>
	/// <returns>CMVec3</returns>
	CMVec3 forward() { return CMVec3(0, 0, 1); }
	
	/// <summary>
	/// Backward direction.
	/// </summary>
	/// <returns>CMVec3</returns>
	CMVec3 backward() { return CMVec3(0, 0, -1); }

	//------------------{ Vectors normalization }------------------

	/// <summary>
	/// Vector normalization.
	/// </summary>
	/// <returns>CMVec3</returns>
	CMVec3 normalize()
	{
		double _x = 0, _y = 0, _z = 0;
		float _len = (sqrt(pow(m_X, 2) + pow(m_Y, 2) + pow(m_Z, 2)));
		_x = m_X / _len;
		_y = m_Y / _len;
		_z = m_Z / _len;

		return CMVec3(_x, _y, _z);
	}

	//------------------{ Vectors rotation }------------------

	CMVec3 rotate(CMQuat q)
	{
		CMQuat v_quat(x, y, z, 0);
		
		q = q.normalize();

		v_quat = v_quat * q;
		v_quat = v_quat.normalize();

		return CMVec3(v_quat.x, v_quat.y, v_quat.z);
	}

	CMVec3 rotate(CMVec3 v)
	{
		CMVec3 v_norm = v.normalize(); // Normalizza il vettore di input

		float angle = *this * v / this->length() * v.length(); // Calcola la lunghezza del vettore
		
		cout << angle << endl;

		//cout << angle << endl;

		// Converti l'angolo da gradi a radianti
		float ang_rad = angle * 3.14159265358979323846 / 180.0;

		//cout << sin(ang_rad / 2) << endl;

		// Costruisci il quaternione di rotazione
		CMQuat rot_quat(
			v_norm.x * sin(ang_rad / 2),
			v_norm.y * sin(ang_rad / 2),
			v_norm.z * sin(ang_rad / 2),
			cos(ang_rad / 2)
		);

		// Normalizza il quaternione di rotazione
		rot_quat = rot_quat.normalize(); 

		// Costruisci il quaternione coniugato
		CMQuat rot_quat_con(rot_quat.x * -1, rot_quat.y * -1, rot_quat.z * -1, rot_quat.w);

		// Esegui la rotazione
		CMQuat in_v_quat(this->m_X, this->m_Y, this->m_Z, 0);
		CMQuat out_quat = rot_quat * in_v_quat * rot_quat_con;
		//cout << out_quat.y << endl;
		return CMVec3(out_quat.x, out_quat.y, out_quat.z);
	}

	CMVec3 rotate(double xDeg, double yDeg, double zDeg)
	{
		//Deg to rad
		double xRad = xDeg * 3.14159265358979323846 / 180.0;
		double yRad = yDeg * 3.14159265358979323846 / 180.0;
		double zRad = zDeg * 3.14159265358979323846 / 180.0;

		// Costruisci il quaternione di rotazione
		CMQuat rot_quat(
			sin(xRad / 2),
			sin(yRad / 2),
			sin(zRad / 2),
			1
		);

		// Normalizza il quaternione di rotazione
		//rot_quat = rot_quat.normalize();

		// Costruisci il quaternione coniugato
		CMQuat rot_quat_con(rot_quat.x * -1, rot_quat.y * -1, rot_quat.z * -1, rot_quat.w);

		// Esegui la rotazione
		CMQuat in_v_quat(this->m_X, this->m_Y, this->m_Z, 0);
		CMQuat out_quat = rot_quat * in_v_quat;
		out_quat = out_quat * rot_quat_con;

		out_quat = out_quat.normalize();

		//cout << out_quat.y << endl;
		return CMVec3(out_quat.x, out_quat.y, out_quat.z);
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
	CMVec3& operator*(const auto scalar)
	{
		CMVec3 v(this->m_X * scalar, this->m_Y * scalar, this->m_Z * scalar);
		return v;
	}
	
	/// <summary>
	/// Dot product of two vectors.
	/// </summary>
	/// <param name="v1">(CMVec3)</param>
	/// <param name="v2">(CMVec3)</param>
	/// <returns>int, float, double or long</returns>
	double& operator*(const CMVec3& v)
	{
		double s = this->m_X * v.m_X + this->m_Y * v.m_Y + this->m_Z * v.m_Z;
		return s;
	}
	
	CMVec3& operator+(const CMVec3& v)
	{
		CMVec3 temp(m_X += v.m_X, m_Y += v.m_Y, m_Z += v.m_Z);

		return temp;
	}
	CMVec3& operator+(const auto s)
	{
		CMVec3 temp(m_X += s, m_Y += s, m_Z += s);
		return temp;
	}
	CMVec3& operator-(const CMVec3& v)
	{
		CMVec3 temp(m_X -= v.m_X, m_Y -= v.m_Y, m_Z -= v.m_Z);

		return temp;
	}
	CMVec3& operator-(const auto s)
	{
		CMVec3 temp(m_X -= s, m_Y -= s, m_Z -= s);
		return temp;
	}
	CMVec3& operator=(const CMVec3& v)
	{
		CMVec3 temp(m_X = v.m_X, m_Y = v.m_Y, m_Z + v.m_Z);
		return temp;
	};
	bool& operator==(const CMVec3& v)
	{
		bool val = m_X == v.m_X && m_Y == v.m_Y && m_Z == v.m_Z;
		return val;
	}

private:
	double m_X = 0, m_Y = 0, m_Z = 0;

	double length() const {
		return std::sqrt(x * x + y * y + z * z);
	}
};

ostream& operator<<(ostream& dest, const CMVec3& v) { return dest << "(" << v.x << ", " << v.y << ", " << v.z << ")"; }

CMVec3::CMVec3() : x(m_X), y(m_Y), z(m_Z)
{
}

inline CMVec3::CMVec3(const CMVec2 v) : x(m_X), y(m_Y), z(m_Z)
{
	*this = v;
}

inline CMVec3::CMVec3(int xVal, int yVal, int zVal) : x(m_X), y(m_Y), z(m_Z)
{
	m_X = xVal;
	m_Y = yVal;
	m_Z = zVal;
}

inline CMVec3::CMVec3(float xVal, float yVal, float zVal) : x(m_X), y(m_Y), z(m_Z)
{
	m_X = xVal;
	m_Y = yVal;
	m_Z = zVal;
}

inline CMVec3::CMVec3(double xVal, double yVal, double zVal) : x(m_X), y(m_Y), z(m_Z)
{
	m_X = xVal;
	m_Y = yVal;
	m_Z = zVal;
}

inline CMVec3::CMVec3(long xVal, long yVal, long zVal) : x(m_X), y(m_Y), z(m_Z)
{
	m_X = xVal;
	m_Y = yVal;
	m_Z = zVal;
}

CMVec3::~CMVec3()
{
}