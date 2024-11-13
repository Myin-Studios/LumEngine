#pragma once

#include "../Shaders/Shader.h"
#include "../Colors/Color.h"

namespace MaterialTypes
{
	enum MaterialType
	{
		PBR,
		Lambert,
		BlinnPhong
	};
}

class Material
{
public:
	Material() = default;

	Material(Shader* s)
	{
		shader = s;
	}
	virtual ~Material()
	{
		delete shader;
		shader = nullptr;
	}
	
	Shader* GetShader() const { return this->shader; }

protected:
	void SetShader(Shader* s) { this->shader = s; }

private:
	Shader* shader = nullptr;
};

class PBR : public Material
{
private:
	Color::Color _Albedo;
	float _Metallic = 0.0f;
	float _Roughness = 0.5f;
	float _AO = 0.0f;
	float _Alpha = 1.0f;

public:
	PBR()
	{
		_Albedo = Color::Color();

		this->SetShader(new Shader("Resources/Shaders/PBRVert.glsl", "Resources/Shaders/PBRFrag.glsl"));
	}

	const Color::Color GetAlbedo() { return this->_Albedo; }
	const float GetMetallic() { return this->_Metallic; }
	const float GetSmoothness() { return this->_Roughness; }
	const float GetAO() { return this->_AO; }
	const float GetAlpha() { return this->_Alpha; }

	void SetAlbedo(Color::Color albedo)
	{
		this->_Albedo = albedo;
		this->GetShader()->setVec3("albedo", glm::vec3(this->_Albedo.r(), this->_Albedo.g(), this->_Albedo.b()));
	}
	void SetMetallic(float metallic)
	{
		this->_Metallic = metallic;
		this->GetShader()->setFloat("metallic", this->_Metallic);
	}
	void SetRoughness(float smoothness)
	{
		this->_Roughness = smoothness;
		this->GetShader()->setFloat("roughness", this->_Roughness);
	}
	void SetAO(float ao)
	{
		this->_AO = ao;
		this->GetShader()->setFloat("ao", this->_AO);
	}
	void SetAlpha(float alpha)
	{
		this->_Alpha = alpha;
		this->GetShader()->setFloat("alpha", this->_Alpha);
	}
};

class BlinnPhong : public Material
{
public:
	BlinnPhong() = default;
};

//--------------------------------{ SKYBOX }--------------------------------

class ProceduralSkybox : public Material
{
public:
	ProceduralSkybox() : Material(new Shader("Resources/Shaders/baseVert.glsl", "Resources/Shaders/baseFrag.glsl"))
	{

	}
};