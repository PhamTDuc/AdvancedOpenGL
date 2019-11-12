#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "freetype/freetype.h"
#include "Shader.h"
#include <iostream>
#include <vector>

class QuadCurve
{
private:
	std::vector<glm::vec3> m_points;
	unsigned int VAO, VBO;
public:
	using iterator = std::vector<glm::vec3>::iterator;
	using citerator = std::vector<glm::vec3>::const_iterator;
	using size_type = std::vector<glm::vec3>::size_type;

	QuadCurve(const std::vector<glm::vec3> &points)
	{
		m_points.insert(m_points.end(), points.begin(), points.end());
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, m_points.size() * sizeof(glm::vec3), &m_points[0],GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void updateBuffer()
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, m_points.size() * sizeof(glm::vec3), &m_points[0], GL_DYNAMIC_DRAW);		
	}

	void update(unsigned int index,const glm::vec3 &val)
	{
		m_points[index] = val;
		updateBuffer();
	}

	void push_back(const glm::vec3& val)
	{
		m_points.push_back(val);
		updateBuffer();
	}

	void push_back(glm::vec3&& val)
	{
		m_points.push_back(val);
		updateBuffer();
	}

	void pop_back()
	{
		m_points.pop_back();
		updateBuffer();
	}

	template<typename InputIterator>
	iterator insert(citerator position, InputIterator first, InputIterator last)
	{
		m_points.insert(position, first, last);
		updateBuffer();
	}

	iterator erase(citerator first, citerator last)
	{
		m_points.erase(first, last);
		updateBuffer();
	}

	void clear() noexcept
	{
		m_points.clear();
		updateBuffer();
	}

	void reserve(size_type n)
	{
		m_points.reserve(n);
	}
};

int main()
{

	std::cin.get();
	return 0;
}