#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

struct Vertex
{
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;

    Vertex() = default;

    Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 uv, glm::vec3 tangent)
        : Position(pos), Normal(norm), TexCoords(uv), Tangent(tangent) { }
};


#if 1
class Mesh {
public:

    Mesh();
    Mesh(const Mesh& _mother);
    virtual ~Mesh();

    void    setDrawMode(GLenum _drawMode = GL_TRIANGLES);

    void    setColor(const glm::vec4& _color);
    void    addColor(const glm::vec4& _color);
    void    addColors(const std::vector<glm::vec4>& _colors);

    void    addVertex(const glm::vec3& _point);
    void    addVertices(const std::vector<glm::vec3>& _verts);
    void    addVertices(const glm::vec3* _verts, int _amt);

    void    addNormal(const glm::vec3& _normal);
    void    addNormals(const std::vector<glm::vec3>& _normals);

    void    addTangent(const glm::vec4& _tangent);
    void    addTangent(const glm::vec3& _tangent);

    void    addTexCoord(const glm::vec2& _uv);
    void    addTexCoords(const std::vector<glm::vec2>& _uvs);

    void    addIndex(unsigned int _i);
    void    addIndices(const std::vector<unsigned int>& _inds);
    void    addIndices(const unsigned int* _inds, int _amt);

    void    addTriangle(unsigned int index1, unsigned int index2, unsigned int index3);

    void    add(const Mesh& _mesh);

    const bool    hasColors() const { return m_colors.size() > 0; }
    const bool    hasNormals() const { return m_normals.size() > 0; }
    const bool    hasTexCoords() const { return m_texCoords.size() > 0; }
    const bool    hasTangents() const { return m_tangents.size() > 0; }
    const bool    hasIndices() const { return m_indices.size() > 0; }
    // Here is vbo
    // Vbo* getVbo();
    GLenum  getDrawMode() const;
    std::vector<glm::ivec3>  getTriangles() const;

    const std::vector<glm::vec4>& getColors() const;
    const std::vector<glm::vec3>& getTangents() const;
    const std::vector<glm::vec3>& getVertices() const;
    const std::vector<glm::vec3>& getNormals() const;
    const std::vector<glm::vec2>& getTexCoords() const;
    const std::vector<unsigned int>& getIndices() const;
    std::vector<Vertex> getBlocks();

    bool    computeNormals();
    bool    computeTangents();
    void    clear();

private:
    std::vector<glm::vec4>  m_colors;
    std::vector<glm::vec3>  m_tangents;
    std::vector<glm::vec3>  m_vertices;
    std::vector<glm::vec3>  m_normals;
    std::vector<glm::vec2>  m_texCoords;
    std::vector<unsigned int>   m_indices;

    std::vector<Vertex> m_vertex_block;

    GLenum    m_drawMode;
};
#endif