#if 0
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
#endif

#if 1
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
// #include "Model.h"

#include "mesh.h"

#include <iostream>
#include <sstream>
#include <filesystem>
#include <unordered_map>
#include <utility>

// #include "LoadObj.h"
#include "tiny_obj_loader.h"
#include "DebugLog.h"
#include "Model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
void renderQuad();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	DebugLog::Init();

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader* shader = Shader::Create("../assets/shaders/normal_mapping_vs.glsl", "../assets/shaders/normal_mapping_fs.glsl");
	Shader* base = Shader::Create("../assets/shaders/base_vert.glsl", "../assets/shaders/base_frag.glsl");
	// Shader basic("../assets/shaders/base_vert.glsl", "../assets/shaders/base_frag.glsl");

	// load textures
	// -------------
	unsigned int diffuseMap = loadTexture("../assets/textures/brickwall.jpg");
	unsigned int normalMap = loadTexture("../assets/textures/brickwall_normal.jpg");

	// Load model - backpack
	// ---------------------
	// Models models
	auto backpack = Model::LoadObjFile("../assets/models/backpack/backpack.obj");

	unsigned int backpack_diffuse = loadTexture("../assets/models/backpack/diffuse.jpg");
	unsigned int backpack_normal = loadTexture("../assets/models/backpack/normal.png");

	// shader configuration
	// --------------------
	glUseProgram(shader->GetID());
	shader->SetUniformInt("diffuseMap", 0);
	shader->SetUniformInt("normalMap", 1);

	glUseProgram(base->GetID());
	shader->SetUniformInt("diffuseMap", 0);

	// lighting info
	// -------------
	glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// configure view/projection matrices
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		#if 1
		glUseProgram(shader->GetID());
		shader->SetUniformMat4("projection", projection);
		shader->SetUniformMat4("view", view);
		// render normal-mapped quad
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		shader->SetUniformMat4("model", model);
		shader->SetUniformVec3("viewPos", camera.Position);
		shader->SetUniformVec3("lightPos", lightPos);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, backpack_diffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, backpack_normal);
		backpack->Render();
		// renderQuad();
		#endif

		#if 0
		glUseProgram(base->GetID());
		base->SetUniformMat4("projection", projection);
		base->SetUniformMat4("view", view);
		// render normal-mapped quad
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		base->SetUniformMat4("model", model);
		base->SetUniformVec3("viewPos", camera.Position);
		base->SetUniformVec3("lightPos", lightPos);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, backpack_diffuse.GetID());
		if (!activeNormal) {
			for (auto data : render_data) {

				glBindVertexArray(data.second.VAO);
				glDrawElements(GL_TRIANGLES, data.second.IndicesCount, GL_UNSIGNED_INT, NULL);
			}
		}
		// renderQuad();
		#endif

		// render light source (simply re-renders a smaller plane at the light's position for debugging/visualization)
		#if 0
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.1f));
		shader.SetUniformMat4("model", model);
		renderQuad();
		#endif

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// renders a 1x1 quad in NDC with manually calculated tangent vectors
// ------------------------------------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		// positions
		glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
		glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
		glm::vec3 pos3(1.0f, -1.0f, 0.0f);
		glm::vec3 pos4(1.0f, 1.0f, 0.0f);
		// texture coordinates
		glm::vec2 uv1(0.0f, 1.0f);
		glm::vec2 uv2(0.0f, 0.0f);
		glm::vec2 uv3(1.0f, 0.0f);
		glm::vec2 uv4(1.0f, 1.0f);
		// normal vector
		glm::vec3 nm(0.0f, 0.0f, 1.0f);

		// calculate tangent/bitangent vectors of both triangles
		glm::vec3 tangent1, bitangent1;
		glm::vec3 tangent2, bitangent2;
		// triangle 1
		// ----------
		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

		bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

		// triangle 2
		// ----------
		edge1 = pos3 - pos1;
		edge2 = pos4 - pos1;
		deltaUV1 = uv3 - uv1;
		deltaUV2 = uv4 - uv1;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


		bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


		float quadVertices[] = {
			// positions            // normal         // texcoords  // tangent                          // bitangent
			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
		};
		// configure plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(CameraMovement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
#endif