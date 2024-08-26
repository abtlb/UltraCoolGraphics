#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"

class Model
{
public:
	Model(const char* path)
	{
		loadModel(path);
	}
	void Draw(Shader& shader)
	{
		for (Mesh& mesh : meshes)
		{
			mesh.Draw(shader);
		}
	}
private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> loadedTextures;

	void loadModel(std::string path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR LOADING MODEL: " << importer.GetErrorString() << std::endl;
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));//the folder that contains the model, we'll need it for other files

		processNode(scene->mRootNode, scene);
		
	}
	void processNode(aiNode* node, const aiScene* scene)
	{
		//meshes pointers are contained within the scenes
		//nodes contain indeces of those meshes
		//parent nodes contain parent meshes and children nodes contain children meshes => car(parent of)tire

		//process node meshes
		for (int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		//process children nodes
		for (int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> texutres;

		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.pos.x = mesh->mVertices[i].x;
			vertex.pos.y = mesh->mVertices[i].y;
			vertex.pos.z = mesh->mVertices[i].z;

			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;

			if (mesh->mTextureCoords[0])
			{
				vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
				vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
			}
			else
			{
				vertex.texCoords.x = vertex.texCoords.y = 0;
			}

			vertices.push_back(vertex);
		}

		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		if (mesh->mMaterialIndex >= 0)//mesh has material
		{
			auto material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<Texture> diffuseMaps = loadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
			texutres.insert(texutres.end(), diffuseMaps.begin(), diffuseMaps.end());
			std::vector<Texture> specularMaps = loadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
			texutres.insert(texutres.end(), specularMaps.begin(), specularMaps.end());
			
		}

		return Mesh(vertices, indices, texutres);
	}
	std::vector<Texture> loadMaterialTexture(aiMaterial* material, aiTextureType type, std::string typeName)
	{
		std::vector<Texture> textures;
		int texCount = material->GetTextureCount(type);
		for (int i = 0; i < texCount; i++)
		{
			aiString file;
			material->GetTexture(type, i, &file);

			//check if texture is already loaded
			bool texLoaded = false;
			for (int j = 0; j < loadedTextures.size(); i++)
			{
				if (strcmp(file.C_Str(), loadedTextures[j].path.C_Str()))
				{
					texLoaded = true;
					textures.push_back(loadedTextures[j]);
					break;
				}
			}

			if(!texLoaded)
			{
				Texture tex;
				tex.id = TextureFromFile(file.C_Str(), directory);
				tex.type = typeName;
				tex.path = file;
				textures.push_back(tex);
				loadedTextures.push_back(tex);
			}
		}
		return textures;
	}

	unsigned int TextureFromFile(const char* file, const std::string& dir)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, channelsNum;
		unsigned char* texData = stbi_load("wall.jpg", &width, &height, &channelsNum, 0);


		if (texData)
		{
			//internalFormat argument: how opengl is going to store the texture data
			//format argument: format of the pixel data provided
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(texData);
		}
		else
		{
			std::cerr << "Failed to load the texture";
		}

		//operations done on buffer are stored in the texture
		glBindTexture(GL_TEXTURE_2D, 0);

		return textureID;
	}
};

#endif // !MODEL_H
