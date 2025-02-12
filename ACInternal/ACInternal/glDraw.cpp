#include "glDraw.h"

void GL::setupOrtho()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glViewport(0, 0, viewport[2], viewport[3]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
}

void GL::restoreGL()
{
	glPopMatrix();
	glPopAttrib();
}

void GL::drawFilledRect(float x, float y, float width, float height, const GLubyte color[3])
{
	glColor3b(color[0], color[1], color[2]);
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();
}

void GL::drawOutline(float x, float y, float width, float height, float lineWidth, const GLubyte color[3])
{
	glLineWidth(lineWidth);
	glBegin(GL_LINE_STRIP);
	glColor3ub(color[0], color[1], color[2]);
	glVertex2f(x - 0.5f, y - 0.5f);
	glVertex2f(x + width + 0.5f, y + height - 0.5f);
	glVertex2f(x + width + 0.5f, y + height + 0.5f);
	glVertex2f(x - 0.5f, y + height + 0.5f);
	glVertex2f(x - 0.5f, y + height - 0.5f);
	glEnd();
}


void GL::Font::build(int height)
{
	hdc = wglGetCurrentDC();
	base = glGenLists(96);
	HFONT hFont = CreateFontA(-height, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Consolas");
	HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
	wglUseFontBitmaps(hdc, 32, 96, base);
	SelectObject(hdc, hOldFont);
	DeleteObject(hFont);

	is_built = true;
}
void GL::Font::print(float x, float y, unsigned char color[3], char* format, ...)
{
	glColor3ub(color[0], color[1], color[2]);
	glRasterPos2f(x, y);

	char text[100];
	va_list arguments;

	va_start(arguments, format);
	vsprintf_s(text, 100, format, arguments);
	va_end(arguments);

	glPushAttrib(GL_LIST_BIT);
	glListBase(base - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();

}

Vector3 GL::Font::centerText(float x, float y, float width, float text_width, float text_height)
{
	Vector3 text;
	text.x = x + (width + text_width) / 2;
	text.y = y + text_height;
	return text;
}

float GL::Font::centerText(float x, float width, float text_width)
{
	if (width > text_width)
	{
		float difference = width - text_width;
		return(x + (difference / 2));
	}
	else
	{
		float difference = text_width - width;
		return (x - (difference / 2));
	}
}