#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
	//cameraPos, cameraZ, cameraY, cameraX, yaw, pitch, direction(same as cameraZ but not normalized), view mat?, proj mat? fov, screenRatio
public:
	glm::vec3 cameraPos, cameraZ, cameraY, cameraX;
	float yaw, pitch;//ignoring roll for now
	float fov, screenRatio;

	Camera(glm::vec3 _cameraPos, float _fov, float _screenRatio)
	{
		cameraPos = _cameraPos;
		cameraZ = glm::vec3(0.0f, 0.0f, -1.0f);//negative z axis(camera front)
		cameraY = glm::vec3(0.0f, 1.0f, 0.0f);
		cameraX = glm::cross(cameraY, cameraZ);
		yaw = -90.0f, pitch = 0.0f;
		fov = _fov;
		screenRatio = _screenRatio;
	}

	glm::mat4& getViewMat()//~
	{
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		cameraZ = glm::normalize(direction);

		//not here
		cameraX.x = sin(glm::radians(yaw));
		cameraX.y = 0;
		cameraX.z = -1 * cos(glm::radians(yaw));
		cameraX = glm::normalize(cameraX);
		//cameraY = 


		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraZ, cameraY);
		return view;
	}

	glm::mat4& getProjMat()
	{
		glm::mat4 proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(fov), screenRatio, 0.1f, 100.0f);
		return proj;
	}
};

#endif // !CAMERA_H

