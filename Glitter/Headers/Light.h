#include "glitter.hpp"

class Light
{
public:
    glm::vec3 position = glm::vec3(0.0f);

    glm::vec3 ambient = glm::vec3(0.2f); // usually set to a low intensity because we don't want the ambient color to be too dominant
    glm::vec3 diffuse = glm::vec3(0.5f); // usually set to the exact color we'd like a light to have; often a bright white color
    glm::vec3 specular = glm::vec3(1.0f); //kept at vec3(1.0) shining at full intensity.
};