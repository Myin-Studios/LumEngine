#pragma once

#ifdef RGB
#undef RGB
#endif // RGB

namespace Color
{
	class RGB
	{
	private:
		float _r = 0.0f;
		float _g = 0.0f;
		float _b = 0.0f;

	public:
		RGB() : _r(0.0f), _g(0.0f), _b(0.0f) {}
		RGB(float r, float g, float b) : _r(r), _g(g), _b(b) {}

		float r() const { return this->_r; }
		float g() const { return this->_g; }
		float b() const { return this->_b; }
	};

	class RGBA : public RGB
	{
	private:
		float _a = 1.0f;

	public:
		RGBA() : RGB(0.0f, 0.0f, 0.0f), _a(1.0f)
		{}
		RGBA(float r, float g, float b, float a) : RGB(r, g, b), _a(a) {}

		const float a() { return this->_a; }
	};

	class HUE
	{

	};

	class Color : public RGBA
	{
	public:
		Color() = default;
		Color(Color&) = default;
		Color(RGBA& rgba) : RGBA(rgba) {}
		Color(float r, float g, float b) : RGBA(r, g, b, 1.0f) {}
	};
}