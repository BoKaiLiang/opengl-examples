#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

class Mesh {
public:

    Mesh();
    Mesh(const Mesh& other);
    ~Mesh();

    void AddVertex(const glm::vec3& vertex);
    void AddNormal(const glm::vec3& normal);
    void AddTexCoord(const glm::vec2& uv);
    void AddTangent(const glm::vec3& tangent);
    void AddColor(const glm::vec4& color);
    void AddIndex(unsigned int index);

    // const bool    hasColors() const { return m_Colors.size() > 0; }
    // const bool    hasNormals() const { return m_Normals.size() > 0; }
    // const bool    hasTexCoords() const { return m_TexCoords.size() > 0; }
    // const bool    hasTangents() const { return m_Tangents.size() > 0; }
    // const bool    hasIndices() const { return m_Indices.size() > 0; }

    // GLenum  getDrawMode() const;
    // std::vector<glm::ivec3>  getTriangles() const;

    inline const std::vector<glm::vec3>& GetVertices() const { return m_Vertices; }
    inline const std::vector<glm::vec3>& GetNormals() const { return m_Normals; }
    inline const std::vector<glm::vec2>& GetTexCoords() const { return m_TexCoords; }
    inline const std::vector<glm::vec3>& GetTangents() const { return m_Tangents; }
    inline const std::vector<glm::vec4>& GetColors() const { return m_Colors; }
    inline const std::vector<unsigned int>& GetIndices() const { return m_Indices; }

    bool    computeNormals();
    bool    ComputeTangents();
    void    Setup();
    void    Draw();

private:
    unsigned int m_Vertex_Array_ID = 0;
    unsigned int m_Index_Buffer_ID = 0;
    unsigned int m_Vertex_Buffer_ID = 0;

    std::vector<glm::vec3>  m_Vertices;
    std::vector<glm::vec3>  m_Normals;
    std::vector<glm::vec2>  m_TexCoords;
    std::vector<glm::vec3>  m_Tangents;
    std::vector<glm::vec4>  m_Colors;
    std::vector<unsigned int>   m_Indices;
};