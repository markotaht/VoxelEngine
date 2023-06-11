#include "ArrayTexture.h"

ArrayTexture::ArrayTexture(ResourceManager* resourceManager) :resourceManager{ resourceManager }
{
	//init();
}

void ArrayTexture::init()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

	//glTextureStorage3D(texture, 1, GL_RGB8, 512, 512, textures.size());
	glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		1,
		GL_RGB8, 512, 512, 6);

	for (auto pair : textures) {
		Texture* tempTexture = new Texture();
		tempTexture->loadToGlArray(pair.second.c_str(), currentLayer, texture);
		tempTexture->setArrayTextureData(currentLayer, texture);
		currentLayer++;
		resourceManager->AddResource(pair.first, tempTexture);
	}
	

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void ArrayTexture::addTexture(std::string name, std::string path)
{

	textures.push_back(std::pair<std::string, std::string>(name, path));
	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
	Texture* tempTexture = new Texture();
	if (tempTexture->loadToMemory(path)) {
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
			0, 0, 0, 
			currentLayer,
			tempTexture->getWidth(), 
			tempTexture->getHeight(),
			1,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			tempTexture->getPixels()->pixels);
		//tempTexture->freeSurface();
		tempTexture->setArrayTextureData(currentLayer++, texture);
		resourceManager->AddResource(name, tempTexture);
	}*/
}
