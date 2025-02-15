#pragma once

#include "../Colors/Color.h"
#include "../LumEngine/Engine/GUI/Rendering/Shaders/Shaders.h"
#include <memory>

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

	Material(std::unique_ptr<Shader> s)
	{
		shader = std::move(s);
	}
	
	Material(const Material& m)
	{
		this->shader = std::make_unique<Shader>(*m.GetShader());
	}

	virtual ~Material() {}

	Shader* GetShader() const { return this->shader.get(); }
	void SetShader(std::unique_ptr<Shader> s) { this->shader = std::move(s); }

	Material& operator=(const Material& other) {
		if (this != &other) {
			if (other.shader) {
				shader = std::make_unique<Shader>(*other.shader);
			}
		}
		return *this;
	}

private:
	std::unique_ptr<Shader> shader = nullptr;
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

		this->SetShader(std::make_unique<Shader>("Resources/Shaders/PBRVert.glsl", "Resources/Shaders/PBRFrag.glsl", "Resources/Shaders/outline.glsl"));
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

//--------------------------------{ Outline }--------------------------------

class Outline : public Material
{
public:
	Outline()
	{
		this->SetShader(std::make_unique<Shader>("Resources/Shaders/outlineVert.glsl", "Resources/Shaders/outlineFrag.glsl"));
	}
};

//--------------------------------{ SKYBOX }--------------------------------

class ProceduralSkybox : public Material
{
public:
	ProceduralSkybox()
	{
		this->SetShader(std::make_unique<Shader>("Resources/Shaders/baseVert.glsl", "Resources/Shaders/baseFrag.glsl"));
	}
};