#pragma once

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include "Mesh.h"

class Model
{
public:
	//model data
	vector<Mesh> meshes;
	vector<Texture> textures_loaded;
	string directory;

	Model(string const &path);
	void Draw(Shader shader);
private:
	//function
	void loadModel(string const &path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};

