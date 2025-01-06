#pragma once

#include "Engine/GUI/Rendering/Vertices/Vertices.h"

#include <GL/glew.h>

#include <vector>
#include <string>
#include <unordered_map>

class GLBufferCache {
private:
    struct LODBuffers {
        GLuint VAO, VBO, EBO;
        int level;

        LODBuffers() : VAO(0), VBO(0), EBO(0), level(0) {}

        void Create() {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
        }

        void SetupBuffers(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
        {
			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

            // Set up vertex attributes ONCE for the VAO
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                (void*)offsetof(Vertex, Position));
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                (void*)offsetof(Vertex, Normal));
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                (void*)offsetof(Vertex, TexCoords));
            glEnableVertexAttribArray(2);

			glBindVertexArray(0);
        }
    };

    struct GLBuffers {
        std::vector<LODBuffers> lodBuffers;  // Array di buffer per ogni livello LOD

        LODBuffers* GetLODBuffers(int level) {
            // Crea nuovi buffer se il livello non esiste
            if (level >= lodBuffers.size()) {
                lodBuffers.resize(level + 1);
            }
            return &lodBuffers[level];
        }
    };

    std::unordered_map<std::string, GLBuffers> _bufferCache;

	static GLBufferCache* s_instance;

public:
    LODBuffers* GetOrCreateBuffers(const std::string& path, const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices, int lodLevel = 0) {
        auto& buffers = _bufferCache[path];
        auto* lodBuffers = buffers.GetLODBuffers(lodLevel);

        if (lodBuffers->VAO == 0) {  // Se i buffer non sono stati ancora creati
            lodBuffers->Create();
            lodBuffers->SetupBuffers(vertices, indices);
            lodBuffers->level = lodLevel;
        }

        return lodBuffers;
    }

    void Clear()
    {
		for (auto& pair : _bufferCache)
		{
			for (auto& lodBuffers : pair.second.lodBuffers)
			{
				glDeleteVertexArrays(1, &lodBuffers.VAO);
				glDeleteBuffers(1, &lodBuffers.VBO);
				glDeleteBuffers(1, &lodBuffers.EBO);
			}
		}
		_bufferCache.clear();
    }

	static GLBufferCache* GetInstance()
	{
		if (!s_instance)
		{
			s_instance = new GLBufferCache();
		}
		return s_instance;
	}
};

inline GLBufferCache* GLBufferCache::s_instance = nullptr;