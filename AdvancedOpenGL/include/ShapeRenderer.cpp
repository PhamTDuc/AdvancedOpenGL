#include "ShapeRenderer.h"

unsigned int ShapeRenderer::VAO, ShapeRenderer::VBO;
Shader ShapeRenderer::ShapeShader;

const float ShapeRenderer::vertices[] = {
	0, 0,
	1, 0,
	0, 1,
	1, 1,
};
void ShapeRenderer::createContext(unsigned int w, unsigned int h)
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

	ShapeShader = Shader("shader/button/vertex.vs", "shader/button/fragment.fs");
	updatewDim(w, h);
}

void ShapeRenderer::draw(const glm::vec3 &color,const glm::vec2 &translate,const glm::vec2 &scale)
{
	ShapeShader.use();
	ShapeShader.setVec3("color", color);
	ShapeShader.setVec2("translate", translate);
	ShapeShader.setVec2("scale", scale);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void ShapeRenderer::updatewDim(unsigned int w, unsigned int h)
{
	ShapeShader.use();
	ShapeShader.setVec2("wDim", w,h);
}