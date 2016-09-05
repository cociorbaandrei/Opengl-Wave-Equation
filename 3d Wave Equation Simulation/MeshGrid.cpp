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
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(Vertex), &m_vertices[0]);
	glDrawElements(
		GL_TRIANGLES,      // mode
		m_indices.size() * sizeof(Index),    // count
		GL_UNSIGNED_INT,   // type
		(GLvoid*)0           // element array buffer offset
	);
	glBindVertexArray(0);
	m_shader->stopUsing();
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

	glBindVertexArray(0);
}
