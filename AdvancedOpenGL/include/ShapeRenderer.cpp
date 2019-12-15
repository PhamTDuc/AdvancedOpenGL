#include "ShapeRenderer.h"

unsigned int ShapeRenderer::VAO, ShapeRenderer::VBO;

const float ShapeRenderer::vertices[] = {
	0, 0,
	1, 0,
	0, 1,
	1, 1,
};
void ShapeRenderer::createContext()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ShapeRenderer::draw(Shader& shader, glm::vec3 color, glm::vec2 translate, glm::vec2 scale)
{
	shader.use();
	shader.setVec3("color", color);
	shader.setVec2("translate", translate);
	shader.setVec2("scale", scale);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}