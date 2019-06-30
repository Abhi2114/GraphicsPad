#include "ShapeData.h"
#include <gtc/matrix_transform.hpp>

void ShapeData::transformNormals(const glm::mat4& rotationMatrix) {

	// apply the rotation and translation matrix to each and every normal
	for (GLuint i = 0; i < numVertices; ++i) {

		glm::vec3& oldNormal = vertices[i].normal;
		glm::vec4 newNormal = rotationMatrix * glm::vec4(oldNormal, 1.0f);
		// update the old normal to the new one
		oldNormal.x = newNormal.x;
		oldNormal.y = newNormal.y;
		oldNormal.z = newNormal.z;
	}
}