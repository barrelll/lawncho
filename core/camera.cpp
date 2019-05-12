#include <include/camera.h>

namespace core
{
    camera::camera()
        :
            _Model(glm::mat4(1.0f)), // An identity matrix, will always be set to 1
            _View(0.0f),
            _Perspective(0.0f),
            _cameraPos(0.0f),
            _cameraOrigin(0.0f),
            _cameraDirection(glm::vec3(0.0f, 1.0f, 0.0f))
    {};

    camera::camera(glm::vec3& cameraPos, glm::vec3& cameraOrigin)
        :
            _Model(glm::mat4(1.0f)), // An identity matrix, will always be set to 1
            _View(0.0f),
            _Perspective(0.0f),
            _cameraPos(cameraPos),
            _cameraOrigin(cameraOrigin),
            _cameraDirection(glm::vec3(0.0f, 1.0f, 0.0f))
    {
        // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        _Perspective    = glm::perspective( 45.0f,
                                            4.0f / 3.0f,
                                            0.1f,
                                            100.0f
                                            );

        _View           = glm::lookAt(      _cameraPos,
                                            _cameraOrigin,
                                            _cameraDirection
                                            );
    };

    camera::camera(glm::vec3& cameraPos, glm::vec3& cameraOrigin, glm::vec3& cameraDir)
        :
            _Model(glm::mat4(1.0f)), // An identity matrix, will always be set to 1
            _View(0.0f),
            _Perspective(0.0f),
            _cameraPos(cameraPos),
            _cameraOrigin(cameraOrigin),
            _cameraDirection(cameraDir)
    {
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        _Perspective    = glm::perspective( 45.0f,
                                            4.0f / 3.0f,
                                            0.1f,
                                            100.0f
                                            );

        _View           = glm::lookAt(      _cameraPos,
                                            _cameraOrigin,
                                            _cameraDirection
                                            );
    };

    camera::camera(const camera& rhs)
    {
        this->_Model            = rhs._Model;
        this->_View             = rhs._View;
        this->_Perspective      = rhs._Perspective;
        this->_cameraPos        = rhs._cameraPos;
        this->_cameraOrigin     = rhs._cameraOrigin;
        this->_cameraDirection  = rhs._cameraDirection;
    }

    camera::~camera()
    {};

    void camera::Update()
    {}

    void camera::Rotate(const glm::vec3& direction, float RotateByDegree)
    {
        _Model = glm::rotate(_Model, RotateByDegree, direction);
    }

    glm::mat4 camera::getCameraMat() const
    {
        glm::mat4 CameraMat = _Perspective * _View * _Model;
        return CameraMat;
    }
}
