#pragma once
using namespace glm;

class ObjectModel
{

public:
	ObjectModel(void);
	~ObjectModel(void);
	bool LoadModel(char* fileName);
	std::vector<vec3> GetVertices() const;
	std::vector<vec2> GetUvs() const;
	std::vector<vec3> GetNormals() const;


private:
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals;
};

