#pragma once
#define GLM_FORCE_RADIANS
#include <include/glm/glm.hpp>
#include <include/glm/gtc/matrix_transform.hpp>
#include <include/GLFW/glfw3.h>

/*TODO: 
 * make camera abstract and create a orthographic/perspective type
 * 2D, and 3D types
 */
namespace core
{
    class camera
    {
    public:
        camera();
        camera(const camera& rhs);
        camera(glm::vec3& cameraPos, glm::vec3& cameraOrigin);
        camera(glm::vec3& cameraPos, glm::vec3& cameraOrigin, glm::vec3& cameraDir);
        ~camera();

        void        Update();
        void        Rotate(const glm::vec3& direction, float RotateByDegree);
        glm::mat4   getCameraMat() const;
        
    private:
        /***    Camera ID for uniform location  ***/
        GLuint _cameraID;
        /***                                    ***/
        glm::mat4 _Model;
        glm::mat4 _View;
        glm::mat4 _Perspective;

        glm::vec3 _cameraPos;
        glm::vec3 _cameraOrigin;
        glm::vec3 _cameraDirection;
    };
}
