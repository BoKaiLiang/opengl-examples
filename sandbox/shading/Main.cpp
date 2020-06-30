#include "Core.h"

#include <filesystem>
#include <sstream>

#include "Camera.h"

#include "LoadObj.h"
#include "Light.h"

class ExampleApplication : public Application
{
public:
	ExampleApplication(unsigned int width, unsigned int height, const std::string& title)
		: Application(width, height, title),
        m_Light(glm::vec3(1.2f, 1.0f, 2.0f)),
        m_Camera(glm::vec3(0.0f, 0.0f, 3.0f))
	{
        // load sphere model
        LoadObjFile("../assets/models/Ball/3d-model.obj", Vertices);

        glGenBuffers(1, &m_SphereVBO);
        glGenVertexArrays(1, &m_SphereVAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_SphereVBO);
        glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertices), &Vertices[0], GL_STATIC_DRAW);

        glBindVertexArray(m_SphereVAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(offsetof(Vertex, Position)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(offsetof(Vertex, Normal)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(offsetof(Vertex, TexCoord)));
        glEnableVertexAttribArray(2);
        glBindVertexArray(0);

        m_PhongShader = Shader::Create("../assets/shaders/phong_vert.glsl", "../assets/shaders/phong_frag.glsl");

        m_BlinnPhong = Shader::Create("../assets/shaders/blinn-phong_vert.glsl", "../assets/shaders/blinn-phong_frag.glsl");

        m_Gouraud = Shader::Create("../assets/shaders/gouraud_vert.glsl", "../assets/shaders/gouraud_frag.glsl");

        m_Toon = Shader::Create("../assets/shaders/toon_vert.glsl", "../assets/shaders/toon_frag.glsl");
	}

	~ExampleApplication()
	{
        glDeleteBuffers(1, &m_SphereVBO);
        glDeleteVertexArrays(1, &m_SphereVAO);

        delete m_PhongShader;
        delete m_BlinnPhong;
        delete m_Gouraud;
        delete m_Toon;
	}

	virtual void OnEvent(Event& e) override
	{
        
	}

	virtual void OnUpdate(float dt) override
	{
        // Blinn-phong
        // ---------------------------------
        m_BlinnPhong->SetUniformVec3("light.position", m_Light.Position);
        m_BlinnPhong->SetUniformVec3("viewPos", m_Camera.Position);

        // light properties
        m_BlinnPhong->SetUniformVec3("light.ambient", m_Light.LightColor);
        m_BlinnPhong->SetUniformVec3("light.diffuse", m_Light.LightColor);
        m_BlinnPhong->SetUniformVec3("light.specular", m_Light.LightColor);

        // material properties
        glm::vec3 Ambient = DiffuseColor * 0.8f;
        m_BlinnPhong->SetUniformVec3("material.ambient", Ambient);
        m_BlinnPhong->SetUniformVec3("material.diffuse", DiffuseColor);
        m_BlinnPhong->SetUniformVec3("material.specular", SpecularColor); // specular lighting doesn't have full effect on this object's material
        m_BlinnPhong->SetUniformFloat("material.shininess", shininess);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(m_Camera.Zoom), m_Data.Width / (float)m_Data.Height, 0.1f, 100.0f);
        glm::mat4 view = m_Camera.GetViewMatrix();
        m_BlinnPhong->SetUniformMat4("projection", projection);
        m_BlinnPhong->SetUniformMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.008f, 0.008f, 0.008f));
        m_BlinnPhong->SetUniformMat4("model", model);

        // Gouraud
        // ---------------------------------
        m_Gouraud->SetUniformVec3("light.position", m_Light.Position);
        m_Gouraud->SetUniformVec3("viewPos", m_Camera.Position);

        // light properties
        m_Gouraud->SetUniformVec3("light.color", m_Light.LightColor);
        m_Gouraud->SetUniformVec3("light.ambient", m_Light.Ambient);

        // material properties
        m_Gouraud->SetUniformVec3("material.diffuse", Ambient);
        m_Gouraud->SetUniformVec3("material.diffuse", DiffuseColor);
        m_Gouraud->SetUniformVec3("material.specular", SpecularColor); // specular lighting doesn't have full effect on this object's material
        m_Gouraud->SetUniformFloat("material.shininess", shininess);

        // view/projection transformations
        m_Gouraud->SetUniformMat4("projection", projection);
        m_Gouraud->SetUniformMat4("view", view);

        // world transformation
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.008f, 0.008f, 0.008f));
        model = glm::translate(model, glm::vec3(0.0f, -10.0f, 0.0f));
        m_Gouraud->SetUniformMat4("model", model);

        // Toon
        // ---------------------------------
        m_Toon->SetUniformVec3("lightPos", m_Light.Position);
        m_Toon->SetUniformVec3("lightColor", m_Light.LightColor);
        m_Toon->SetUniformVec3("objectColor", DiffuseColor);
        m_Toon->SetUniformVec3("specColor", SpecularColor);
        m_Toon->SetUniformVec3("viewPos", m_Camera.Position);
        m_Toon->SetUniformFloat("shininess", shininess);


        // view/projection transformations
        m_Toon->SetUniformMat4("projection", projection);
        m_Toon->SetUniformMat4("view", view);

        // world transformation
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.008f, 0.008f, 0.008f));
        model = glm::translate(model, glm::vec3(0.0f, -50.0f, 0.0f));
        m_Toon->SetUniformMat4("model", model);
	}

	virtual void OnRender() override
	{
        // render the cube
        // glUseProgram(m_BlinnPhong->GetID());
        // glUseProgram(m_Gouraud->GetID());
        glUseProgram(m_Toon->GetID());
        glBindVertexArray(m_SphereVAO);
        glDrawArrays(GL_TRIANGLES, 0, Vertices.size());
	}

	virtual void OnImGui() override
	{
        ImGui::Begin("Material");
        ImGui::ColorEdit3("Diffuse Color", glm::value_ptr(DiffuseColor));
        ImGui::ColorEdit3("Specular Color", glm::value_ptr(SpecularColor));
        ImGui::SliderFloat("Shininess", &shininess, 8.0f, 256.0f);
        ImGui::End();

        ImGui::Begin("Light");
        ImGui::SliderFloat3("Position", glm::value_ptr(m_Light.Position), -10.0f, 10.0f);
        ImGui::ColorEdit3("Color", glm::value_ptr(m_Light.LightColor));
        ImGui::End();
	}

private:
	Shader* m_PhongShader = nullptr;
    Shader* m_BlinnPhong = nullptr;
    Shader* m_Gouraud = nullptr;
    Shader* m_Toon = nullptr;

	unsigned int m_SphereVAO = 0, m_SphereVBO = 0;

    std::vector<Vertex> Vertices;

    // Camera;
    Camera m_Camera;

    // Light
    Light m_Light;

    glm::vec3 DiffuseColor = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 SpecularColor = glm::vec3(1.0f, 1.0f, 1.0f);
    float shininess = 32.0f;
};


int main(void)
{
	auto app = new ExampleApplication(800, 600, "Example");
	app->Run();
}