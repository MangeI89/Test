#ifndef MESH_INCLUDED_H
#define MESH_INCLUDED_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "obj_loader.h"

using namespace std;
using namespace glm;

struct Vertex
{
public:
	Vertex() {};

	Vertex(const vec3& pos, const vec2& texCoord, const vec3& normal)
	{
		this->pos = pos;
		this->texCoord = texCoord;
		this->normal = normal;
	}

	vec3* GetPos() { return &pos; }
	vec2* GetTexCoord() { return &texCoord; }
	vec3* GetNormal() { return &normal; }

	vec3 GetPos2() { return pos; }
	vec2 GetTexCoord2() { return texCoord; }
	vec3 GetNormal2() { return normal; }

private:
	vec3 pos;
	vec2 texCoord;
	vec3 normal;
};

enum MeshBufferPositions
{
	POSITION_VB,
	TEXCOORD_VB,
	NORMAL_VB,
	//TRANSLATIONS_VB,
	INDEX_VB
};

class Mesh
{
public:
	Mesh(const string& fileName);
	Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	Mesh(vector<vec3>& vertices, vector<vec2>& texCoords, vector<vec3>& normals);

	void Draw();
	void Draw2();

	virtual ~Mesh();

protected:
private:
	static const unsigned int NUM_BUFFERS = 4;
	void operator=(const Mesh& mesh) {}
	Mesh(const Mesh& mesh) {}

	void InitMesh(const IndexedModel& model);

	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	GLuint instancingVBO;
	unsigned int m_numIndices;
	glm::vec2 translations[100]; //Remove this later
};

#endif