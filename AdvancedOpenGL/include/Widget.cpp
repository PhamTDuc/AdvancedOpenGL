#include "Widget.h"

//unsigned int Widget::VAO, Widget::VBO;
//Shader Widget::shader("shader/button/vertex.vs","shader/button/fragment.fs");
//Shader Widget::shader;

//void Widget::draw(Shader &shader,glm::vec3 &color,unsigned int VAO, int x, int y, int w, int h)
//{
//	shader.use();
//	shader.setVec2("translate", glm::vec2(x, y));
//	shader.setVec2("scale", glm::vec2(w, h));
//	shader.setVec3("color", color);
//
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	glBindVertexArray(VAO);
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//	glBindVertexArray(0);
//}

TextRenderer Widget::textrenderer;
ShapeRenderer Widget::shaperenderer;