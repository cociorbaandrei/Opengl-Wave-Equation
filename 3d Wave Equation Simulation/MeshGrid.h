#ifndef MESHGRID_H
#define MESHGRID_H
#pragma once
class GLShader;
class Camera;
class Window;

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color = glm::vec3(1.f);

};
struct Index {
	GLuint x, y, z;
};
struct Face {
	std::vector<int> v = {0,0,0};
};
class MeshGrid
{
public:
	MeshGrid(int size, glm::vec3 color, const char *vertex_path, const char *fragment_path);
	~MeshGrid();

	void RecalculateNormals();
	void SetColor(glm::vec3 color);
	void Draw(Window* window, Camera* camera);
	MeshGrid* Translate(glm::vec3 position);
	MeshGrid* Scale(glm::vec3 size);
	int GetGridSize();
	Vertex& GetVertex(int i, int j);
	GLShader* GetShader();
	glm::vec3 getColor(double v, double vmin, double vmax);
private:
	void Init();
	GLuint m_vbo, m_vao, m_ebo;
	int m_grid_size;
	
	
	std::vector<Vertex> m_vertices;
	std::vector<Index> m_indices;
	std::vector<Face> m_faces;
	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::vec3 m_color;
	glm::mat4 m_model;

	GLShader* m_shader;
};


#endif