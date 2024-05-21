#include "Sprite.h"
std::vector<Sprite> Sprite::renderList;
std::unordered_map<std::string, unsigned int> Sprite::textures;
std::unordered_map<VaoType, std::tuple<unsigned, unsigned, unsigned>> Sprite::vaos;
Sprite::Sprite(unsigned int _texture, float _x, float _y, unsigned int _layer, VaoType _VAO) : texture(_texture),layer(_layer),vaoType(_VAO) {
    Pos = glm::vec4(_x, _y, 0, 0);
    Scale = glm::vec4(1, 1, 1, 1);
    Offset = glm::vec2(0, 0);
    Zoom = glm::vec2(1, 1);
    Color = glm::vec4(1.0, 1.0, 1.0,1);
    angle = 0;
    isRendered = true;
}
void Sprite::Initialize() {
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    textures.insert({"box",generateTexture("../res/box.png") });
    vaos.insert({ VaoType::CENTER,generateVao(VaoType::CENTER) });
    vaos.insert({ VaoType::BOTTOM_LEFT,generateVao(VaoType::BOTTOM_LEFT) });
}
void Sprite::DeleteTextures() {
    for (auto& t : textures) {
        std::cout << "Deleteing texID=" << t.second << std::endl;
        glDeleteTextures(1, &t.second);
    }
    for (auto& v : vaos) {
        std::cout << "Deleteing VAO=" << std::get<0>(v.second) << " VBO="<< std::get<1>(v.second) << " EBO=" << std::get<2>(v.second) << std::endl;
    }
    textures.clear();
}
unsigned int Sprite::generateTexture(std::string path) {
    unsigned texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return texture;
}
//TO DO 
std::tuple<unsigned, unsigned, unsigned> Sprite::generateVao(const VaoType &v){
    const float vertices_center[] = {
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
    };
    const float vertices_bottom_left[] = {
        // positions          // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
         1.0f,  0.0f, 0.0f,   1.0f, 0.0f, // bottom right
         0.0f,  0.0f, 0.0f,   0.0f, 0.0f, // bottom left
         0.0f,  1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    const unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    switch (v)
    {
        case VaoType::CENTER: {
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_center), vertices_center, GL_STATIC_DRAW);
            //std::cout << "VBO: binding VaoType::CENTER" << std::endl;
            break;
        }
        case VaoType::BOTTOM_LEFT: {
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_bottom_left), vertices_bottom_left, GL_STATIC_DRAW);
            //std::cout << "VBO: binding VaoType::BOTTOM_LEFT" << std::endl;
            break;
        }
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    return std::tuple<unsigned, unsigned, unsigned>{ VAO,VBO,EBO };
}
/// <summary>
/// Allocates new sprite for rendering
/// </summary>
/// <param name="_texture"></param>
/// <param name="_x"></param>
/// <param name="_y"></param>
/// <param name="_layer"></param>
/// <param name="_VAO"></param>
/// <returns>Index in renderList to allocated sprite</returns>
int Sprite::Add(std::string _texture, float _x, float _y, unsigned int _layer, VaoType _VAO) {
    auto tex = textures.find(_texture);
    int i = 0;
    for (std::vector<Sprite>::iterator it = renderList.begin(), finish = renderList.end(); it != finish; it++,i++) {
        if (it->layer > _layer) {
            //allocate at it//layer doesnt exsits//
            //std::cout << "Layer \"" << _layer << "\" doesnt exsit - added before first of \""<< it->layer <<"\" at [" << i << "]" << std::endl;
            renderList.insert(it, Sprite(tex->second, _x, _y, _layer, _VAO));
            return i;
        }
        if (it->layer == _layer) {//layer exists//find if any object in this layer is not rendered
            //find last object of _layer layer
            auto finish2 = std::find_if(it, finish, [&_layer](const Sprite& s) -> bool {
            return _layer != s.layer;
            });
            if (finish2 != finish) {
                finish2++;
            }
            //find not rendered among this layer
            i--;
            auto notRendered = std::find_if(it, finish2, [&i](const Sprite& s) -> bool {
                i++;
                return !s.isRendered;
            });
            if (notRendered == finish2) {//all are rendered add new at back of this layer
                //std::cout << "Layer \"" << _layer << "\" exsits - all rendered - added as the last at [" << i + 1 << "]" << std::endl;
                finish2--;
                renderList.insert(finish2, Sprite(tex->second, _x, _y, _layer, _VAO));
                return i;
            }
            //replace notRendered
            //std::cout << "Layer \"" << _layer << "\" exsits - replacing not rendered at [" << i << "]" << std::endl;
            renderList[i] = Sprite(tex->second, _x, _y, _layer, _VAO);
            return i;
        }
    }
    //if _layer is highest in vector (layer doesnt exsits)
    //std::cout << "Layer \"" << _layer << "\" doesnt exsit - added at [" << i << "]" << std::endl;
    renderList.push_back(Sprite(tex->second, _x, _y, _layer, _VAO));
    return i;
}
void Sprite::Delete(unsigned int _ID) {
    renderList[_ID].isRendered = false;
}
void Sprite::Delete() {
    isRendered = false;
}
void Sprite::RenderAll(Shader& ourShader) {
    if (renderList.empty()) { return; }
    ourShader.use();
    //get uniform locations
    static unsigned int transLoc = glGetUniformLocation(ourShader.ID, "transform");
    static unsigned int colorLoc = glGetUniformLocation(ourShader.ID, "ourColor");
    static unsigned int texZoomLoc = glGetUniformLocation(ourShader.ID, "texZoom");
    static unsigned int texOffsetLoc = glGetUniformLocation(ourShader.ID, "texOffset");

    //Matrix[col][row]; 
    for (auto& s : renderList) {
        if (!s.isRendered) { continue; }
        glm::mat4 trans = glm::mat4(1.0f);
        glm::mat4 temp = glm::mat4(1.0f);
        //Position
        temp = glm::mat4(1.0f);
        temp[3][0] = s.Pos.x;
        temp[3][1] = s.Pos.y;
        trans = trans * temp;
        //Rotation
        temp = glm::mat4(1.0f);
        float alfa = s.angle;
        temp[0][0] = cos(alfa);
        temp[0][1] = -sin(alfa);
        temp[1][0] = sin(alfa);
        temp[1][1] = cos(alfa);
        trans = trans * temp;
        //Scale
        temp = glm::mat4(1.0f);
        temp[0][0] = s.Scale.x / 16.0;
        temp[1][1] = s.Scale.y / 9.0;
        trans = trans * temp;
        //Bind VAO
        glBindVertexArray(std::get<0>(vaos.at(s.vaoType)));
        //Bind texture
        glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, s.texture);
        //Assign uniforms
        glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));
        glUniform4f(colorLoc, s.Color.x, s.Color.y, s.Color.z, s.Color.w);
        glUniform2f(texOffsetLoc, s.Offset.x, s.Offset.y);
        glUniform2f(texZoomLoc, s.Zoom.x, s.Zoom.y);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}
void Sprite::setScale(float x, float y) {
    Scale.x = x; Scale.y = y;
}
void Sprite::setPos(float x, float y) {
    Pos.x = x; Pos.y = y;
}
void Sprite::addPos(float x, float y){
    Pos.x += x; Pos.y += y;
}
Sprite& Sprite::get(unsigned int _ID){
    return renderList[_ID];
}
void Sprite::setColor(glm::vec4 color){
    Color = color;
}