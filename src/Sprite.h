#ifndef SPRITE_H
#define SPRITE_H
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "shader.h"
#include "glm-master/glm/glm.hpp"
#include "glm-master/glm/gtc/matrix_transform.hpp"
#include "glm-master/glm/gtc/type_ptr.hpp"
#include <stb_image.h>

enum class VaoType { CENTER, BOTTOM_LEFT };
class Sprite final {
	static std::unordered_map<std::string,unsigned int> textures;
	static std::unordered_map<VaoType, std::tuple<unsigned, unsigned, unsigned>> vaos;
	Sprite(unsigned int _texture, float _x = 0, float _y = 0, unsigned int _layer = 0, VaoType _VAO = VaoType::CENTER);
	static unsigned int generateTexture(std::string path);
	static std::tuple<unsigned, unsigned, unsigned> generateVao(const VaoType& v);
	glm::vec4 Pos;
	glm::vec4 Scale;
	glm::vec4 Color;
	glm::vec2 Offset;
	glm::vec2 Zoom;
	float angle;
	int layer;
	unsigned int texture;
	VaoType vaoType;
	bool isRendered;
    static std::vector<Sprite> renderList;
public:
	static void Initialize(void);
	static void DeleteTextures(void);
	~Sprite() = default;
	static void RenderAll(Shader& ourShader);
	static int Add(std::string _texture, float _x = 0, float _y = 0, unsigned int _layer = 0, VaoType _VAO = VaoType::CENTER);
	static void Delete(unsigned int _ID);
	void Delete();
	//MATRIX MAINPULATION
	//void setColor(int _c = -1, bool _marked = false);
	void setScale(float x, float y);
    void setColor(glm::vec4 color);
	void setPos(float x, float y);
    void addPos(float x, float y);
    static Sprite& get(unsigned int _ID);
};
#endif