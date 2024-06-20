#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	glm::vec3 cameraPos;
	glm::vec3 cameraTarget;
	glm::vec3 worldUp;
	float fov;
	float screenRatio;

	Camera(glm::vec3 _cameraPos, glm::vec3 _cameraTarget, glm::vec3 _worldUp)
	{
		cameraPos = _cameraPos;
		cameraTarget = _cameraTarget;
		worldUp = _worldUp;
		fov = 45.0f;
		screenRatio = 1;
	}
};

#endif // !CAMERA_H

