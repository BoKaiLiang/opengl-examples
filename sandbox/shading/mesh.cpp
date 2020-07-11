#include "mesh.h"

#include <glad/glad.h>

#include <iostream>
#include <vector>

Mesh::Mesh() {

}

Mesh::Mesh(const Mesh& other)
{
    m_Vertex_Array_ID = other.m_Vertex_Array_ID;
    m_Vertex_Buffer_ID = other.m_Vertex_Buffer_ID;
    m_Index_Buffer_ID = other.m_Index_Buffer_ID;

    m_Vertices.clear();
    m_Vertices.assign(other.m_Vertices.begin(), other.m_Vertices.end());

    m_Normals.clear();
    m_Normals.assign(other.m_Normals.begin(), other.m_Normals.end());

    m_TexCoords.clear();
    m_TexCoords.assign(other.m_TexCoords.begin(), other.m_TexCoords.end());

    m_Tangents.clear();
    m_Tangents.assign(other.m_Tangents.begin(), other.m_Tangents.end());

    m_Colors.clear();
    m_Colors.assign(other.m_Colors.begin(), other.m_Colors.end());

    m_Indices.clear();
    m_Indices.assign(other.m_Indices.begin(), other.m_Indices.end());
}

Mesh::~Mesh() {

}

void Mesh::AddVertex(const glm::vec3& vertex)
{
    m_Vertices.push_back(vertex);
}

void Mesh::AddNormal(const glm::vec3& normal)
{
    m_Normals.push_back(normal);
}

void Mesh::AddTexCoord(const glm::vec2& uv)
{
    m_TexCoords.push_back(uv);
}

void Mesh::AddTangent(const glm::vec3& tangent)
{
    m_Tangents.push_back(tangent);
}

void Mesh::AddColor(const glm::vec4& color)
{
    m_Colors.push_back(color);
}

void Mesh::AddIndex(unsigned int index)
{
    m_Indices.push_back(index);
}

bool Mesh::computeNormals() {
    
    //The number of the vertices
    int nV = m_Vertices.size();

    //The number of the triangles
    int nT = m_Indices.size() / 3;

    std::vector<glm::vec3> norm(nV); //Array for the normals

    //Scan all the triangles. For each triangle add its
    //normal to norm's vectors of triangle's vertices
    for (int t = 0; t < nT; t++) {

        //Get indices of the triangle t
        int i1 = m_Indices[3 * t];
        int i2 = m_Indices[3 * t + 1];
        int i3 = m_Indices[3 * t + 2];

        //Get vertices of the triangle
        const glm::vec3& v1 = m_Vertices[i1];
        const glm::vec3& v2 = m_Vertices[i2];
        const glm::vec3& v3 = m_Vertices[i3];

        //Compute the triangle's normal
        glm::vec3 dir = glm::normalize(glm::cross(v2 - v1, v3 - v1));

        //Accumulate it to norm array for i1, i2, i3
        norm[i1] += dir;
        norm[i2] += dir;
        norm[i3] += dir;
    }

    //Normalize the normal's length and add it.
    m_Normals.clear();
    for (int i = 0; i < nV; i++) {
        AddNormal(glm::normalize(norm[i]));
    }

    return true;
}

// http://www.terathon.com/code/tangent.html
bool Mesh::ComputeTangents() {
    //The number of the vertices
    size_t nV = m_Vertices.size();

    if (m_TexCoords.size() != nV || m_Normals.size() != nV)
        return false;

    //The number of the triangles
    size_t nT = m_Indices.size() / 3;

    std::vector<glm::vec3> tan1(nV);
    std::vector<glm::vec3> tan2(nV);

    //Scan all the triangles. For each triangle add its
    //normal to norm's vectors of triangle's vertices
    for (size_t t = 0; t < nT; t++) {

        //Get indices of the triangle t
        int i1 = m_Indices[3 * t];
        int i2 = m_Indices[3 * t + 1];
        int i3 = m_Indices[3 * t + 2];

        //Get vertices of the triangle
        const glm::vec3& v1 = m_Vertices[i1];
        const glm::vec3& v2 = m_Vertices[i2];
        const glm::vec3& v3 = m_Vertices[i3];

        const glm::vec2& w1 = m_TexCoords[i1];
        const glm::vec2& w2 = m_TexCoords[i2];
        const glm::vec2& w3 = m_TexCoords[i3];

        float x1 = v2.x - v1.x;
        float x2 = v3.x - v1.x;
        float y1 = v2.y - v1.y;
        float y2 = v3.y - v1.y;
        float z1 = v2.z - v1.z;
        float z2 = v3.z - v1.z;

        float s1 = w2.x - w1.x;
        float s2 = w3.x - w1.x;
        float t1 = w2.y - w1.y;
        float t2 = w3.y - w1.y;

        float r = 1.0f / (s1 * t2 - s2 * t1);
        glm::vec3 sdir((t2 * x1 - t1 * x2) * r,
            (t2 * y1 - t1 * y2) * r,
            (t2 * z1 - t1 * z2) * r);
        glm::vec3 tdir((s1 * x2 - s2 * x1) * r,
            (s1 * y2 - s2 * y1) * r,
            (s1 * z2 - s2 * z1) * r);

        tan1[i1] += sdir;
        tan1[i2] += sdir;
        tan1[i3] += sdir;

        tan2[i1] += tdir;
        tan2[i2] += tdir;
        tan2[i3] += tdir;
    }

    //Normalize the normal's length and add it.
    m_Tangents.clear();
    for (size_t i = 0; i < nV; i++) {
        const glm::vec3& n = m_Normals[i];
        const glm::vec3& t = tan1[i];

        // Gram-Schmidt orthogonalize
        glm::vec3 tangent = t - n * glm::dot(n, t);

        // Calculate handedness

        AddTangent(tangent);
    }

    return true;
}

void Mesh::Setup()
{
    glGenVertexArrays(1, &m_Vertex_Array_ID);
    glBindVertexArray(m_Vertex_Array_ID);

    unsigned int vertices_size = m_Vertices.size() * sizeof(glm::vec3);
    unsigned int normals_size = m_Normals.size() * sizeof(glm::vec3);
    unsigned int texcoords_size = m_TexCoords.size() * sizeof(glm::vec2);
    unsigned int tangents_size = m_Tangents.size() * sizeof(glm::vec3);
    unsigned int indices_size = m_Indices.size() * sizeof(unsigned int);

    unsigned int attributes_size = vertices_size + normals_size + texcoords_size + tangents_size;

    glGenBuffers(1, &m_Vertex_Buffer_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vertex_Buffer_ID);

    glBufferData(GL_ARRAY_BUFFER, attributes_size, 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_size, m_Vertices.data());
    glBufferSubData(GL_ARRAY_BUFFER, vertices_size, normals_size, m_Normals.data());
    glBufferSubData(GL_ARRAY_BUFFER, vertices_size + normals_size, texcoords_size, m_TexCoords.data());
    glBufferSubData(GL_ARRAY_BUFFER, vertices_size + normals_size + texcoords_size, tangents_size, m_Tangents.data());

    glGenBuffers(1, &m_Index_Buffer_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Index_Buffer_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, m_Indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertices_size));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(vertices_size + normals_size));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertices_size + normals_size + texcoords_size));

    glBindVertexArray(0);
}

void Mesh::Draw()
{
    glBindVertexArray(m_Vertex_Array_ID);
    glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, NULL);
}