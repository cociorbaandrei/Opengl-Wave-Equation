#include "stdafx.h"
#include "MeshGrid.h"
#include "GLShader.h"
#include "Camera.h"
#include "Window.h"
MeshGrid::MeshGrid(int size, glm::vec3 color, const char *vertex_path, const char *fragment_path)
{
	m_grid_size = size;
	m_color = color;
	try
	{
		m_shader = new GLShader(vertex_path, fragment_path);
		this->SetColor(m_color);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	

	this->Init();

}

MeshGrid::~MeshGrid()
{
	delete m_shader;
}


void MeshGrid::RecalculateNormals()
{
	for (int i = 0; i < m_vertices.size(); i++) m_vertices[i].normal = glm::vec3(0.0f);

	for (int i = 0; i < m_faces.size(); i++) {

		const int ia = m_faces[i].v[0];
		const int ib = m_faces[i].v[1];
		const int ic = m_faces[i].v[2];

		glm::vec3 e1 = m_vertices[ib].position - m_vertices[ia].position;
		glm::vec3 e2 = m_vertices[ib].position - m_vertices[ic].position;
		glm::vec3 no = glm::cross(e1, e2);

		m_vertices[ia].normal -= no;
		m_vertices[ib].normal -= no;
		m_vertices[ic].normal -= no;
	}
	//for (int i = 0; i < m_vertices.size(); i++) m_vertices[i].normal = glm::normalize(m_vertices[i].normal);
}

void MeshGrid::SetColor(glm::vec3 color)
{
	GLint colorLocation = glGetUniformLocation(m_shader->getProgram(), "m_color");
	m_shader->useProgram();
	glUniform3fv(colorLocation, 1, &m_color[0]);
}

void MeshGrid::Draw(Window * window, Camera * camera)
{
	m_shader->useProgram();
	
	glm::mat4 view = camera->getViewMatrix();
	glm::mat4 projection = projection = glm::perspective(glm::radians(45.0f), window->getAspectRatio(), 0.01f, 100.0f);

	GLint viewLocation = glGetUniformLocation(m_shader->getProgram(), "view");
	GLint projectionLocation = glGetUniformLocation(m_shader->getProgram(), "projection");
	GLint modelLocation = glGetUniformLocation(m_shader->getProgram(), "model");
	

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	


	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_DYNAMIC_DRAW);
	
	glDrawElements(
		GL_TRIANGLES,      // mode
		m_indices.size() * sizeof(Index),    // count
		GL_UNSIGNED_INT,   // type
		(GLvoid*)0           // element array buffer offset
	);
	glBindVertexArray(0);
}

MeshGrid* MeshGrid::Translate(glm::vec3 position)
{
	m_model = glm::translate(m_model, position);
	m_position = position;
	GLint modelLocation = glGetUniformLocation(m_shader->getProgram(), "model");
	m_shader->useProgram();
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &m_model[0][0]);
	return this;
}

MeshGrid* MeshGrid::Scale(glm::vec3 size)
{
	m_model = glm::scale(m_model, size);
	m_scale = size;
	GLint modelLocation = glGetUniformLocation(m_shader->getProgram(), "model");
	m_shader->useProgram();
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &m_model[0][0]);
	return this;

}

int MeshGrid::GetGridSize()
{
	return m_grid_size;
}


Vertex& MeshGrid::GetVertex(int i, int j)
{
	return m_vertices[i * (m_grid_size)+j];
}

GLShader * MeshGrid::GetShader()
{
	return m_shader;
}

glm::vec3 MeshGrid::getColor(double v, double vmin, double vmax)
{
	double dv;
	float r =1.f, g = 1.f, b = 1.f;
	if (v < vmin)
		v = vmin;
	if (v > vmax)
		v = vmax;
	dv = vmax - vmin;

	if (v < (vmin + 0.25 * dv)) {
		r = 0;
		g = 4 * (v - vmin) / dv;
	}
	else if (v < (vmin + 0.5 * dv)) {
		r = 0;
		b = 1 + 4 * (vmin + 0.25 * dv - v) / dv;
	}
	else if (v < (vmin + 0.75 * dv)) {
		r = 4 * (v - vmin - 0.5 * dv) / dv;
		b = 0;
	}
	else {
		g = 1 + 4 * (vmin + 0.75 * dv - v) / dv;
		b = 0;
	}

	return glm::vec3(r, g, b);
}

glm::mat4 MeshGrid::getModelMatrix()
{
	return m_model;
}


void MeshGrid::Init()
{
	m_vertices.resize(m_grid_size * m_grid_size);
	for (int i = 0; i < m_grid_size; i++) {
		for (int j = 0; j < m_grid_size; j++)
		{
			glm::vec3 position;
			position.x = (float)i;
			position.y = 0;
			position.z = (float)j;
			m_vertices[i * (m_grid_size)+j].position = position;
			m_vertices[i * (m_grid_size)+j].color = m_color;
		}
	}
	for (int i = 0; i < m_grid_size - 1; i++) {
		for (int j = 0; j < m_grid_size - 1; j++) {
			Index temp1 = { i * m_grid_size + j, i * m_grid_size + j + 1, (i + 1) * m_grid_size + j };
			Index temp2 = { (i + 1) * m_grid_size + j,  i * m_grid_size + j + 1, (i + 1) * m_grid_size + j + 1 };
			m_indices.push_back(temp1);
			m_indices.push_back(temp2);
			Face f1, f2;
			f1.v[0] = temp1.x;
			f1.v[1] = temp1.y;
			f1.v[2] = temp1.z;

			f2.v[0] = temp2.x;
			f2.v[1] = temp2.y;
			f2.v[2] = temp2.z;

			m_faces.push_back(f1);
			m_faces.push_back(f2);

		}
	}
	
	glGenBuffers(1, &m_ebo);
	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);

	
	glBindVertexArray(m_vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(Index), &m_indices[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));

	glBindVertexArray(0);
}

glm::vec3 MeshGrid::getCollisionWithRay(glm::vec3 rayDir, glm::vec3 rayPos)
{
	for (auto face : m_faces) {
		float t = -1;
		if (checkFaceCollisionWithRay(m_vertices[face.v[0]].position, m_vertices[face.v[1]].position, m_vertices[face.v[2]].position, rayPos, rayDir, &t)) {
			return glm::vec3(rayPos + t * rayDir);
		}
	}

	glm::vec3(0.f, 0.f, 0.f);
}


bool MeshGrid::checkFaceCollisionWithRay(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 O, glm::vec3 D, float * out)
{
	glm::vec3 e1, e2, //Edge1, Edge2
		P, Q, T;
	float det, inv_det, u, v, t;

	//Find vectors for two edges sharing V1
	e1 = v2 - v1;
	e2 = v3 - v1;

	//Begin calculating determinant - also used to calculate u parameter
	P = glm::cross(D, e2);
	det = glm::dot(e1, P);

	//NOT CULLING
	if (det > -EPSILON && det < EPSILON) return false;
	inv_det = 1.f / det;

	//calculate distance from V1 to ray origin
	T = O - v1;

	//Calculate u parameter and test bound
	u = glm::dot(T, P) * inv_det;

	//The intersection lies outside of the triangle
	if (u < 0.f || u > 1.f) return false;

	//Prepare to test v parameter
	Q = glm::cross(T, e1);
	
	//Calculate V parameter and test bound
	v = glm::dot(D, Q) * inv_det;
	//The intersection lies outside of the triangle
	if (v < 0.f || u + v  > 1.f) return false;

	t = glm::dot(e2, Q) * inv_det;
	if (t > EPSILON) { //ray intersection
		*out = t;
		return true;
	}
	return false;
}

