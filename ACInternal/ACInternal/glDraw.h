#pragma once
#pragma comment(lib, "OpenGL32.lib")
#include <Windows.h>
#include <stdio.h>
#include <gl/GL.h>
#include "mem.h"
#include "vector_helper.h"


namespace rgb
{
	const GLubyte red[3] = { 255, 0, 0 };
	const GLubyte green[3] = { 0, 255, 0 };
	const GLubyte gray[3] = { 55, 55, 55 };
	const GLubyte light_gray[3] = { 192, 192, 192 };
	const GLubyte black[3] = { 0, 0, 0 };
}

namespace GL
{
	void setupOrtho();
	void restoreGL();

	void drawFilledRect(float x, float y, float width, float height, const GLubyte color[3]);
	void drawOutline(float x, float y, float width, float height, float line_width, const GLubyte color[3]);

	class Font
	{
	public:
		bool is_built = false;
		unsigned int base;
		HDC hdc = nullptr;
		int height;
		int width;

		void build(int height);
		void print(float x, float y, unsigned char color[3], char* format, ...);

		Vector3 centerText(float x, float y, float width, float text_width, float text_height);
		float centerText(float x, float width, float text_width);
	};
}