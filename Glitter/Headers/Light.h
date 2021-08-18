#include "glitter.hpp"

enum class LightTypes {POINT = 0, DIRECTIONAL = 1};

class Light
{
public:
    glm::vec4 position = glm::vec4(glm::vec3(0.0f),1.0f);

    glm::vec3 ambient = glm::vec3(0.2f); // usually set to a low intensity because we don't want the ambient color to be too dominant
    glm::vec3 diffuse = glm::vec3(0.5f); // usually set to the exact color we'd like a light to have; often a bright white color
    glm::vec3 specular = glm::vec3(1.0f); //kept at vec3(1.0) shining at full intensity.

    LightTypes GetType(){return LightTypes::POINT;};
};