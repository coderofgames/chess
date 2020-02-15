#ifndef GL_FONT_H
#define GL_FONT_H
#include <Windows.h>
#include <GL/GL.h>

class GLFont
{
public:
	GLFont(){}
	~GLFont()
	{
		KillFont();
	}

	void BuildFont(HDC hDC, char* name);
	void KillFont();
	void Print(const char *fmt,...);
	void Draw(float x, float y,  char *buf);

	GLuint base;
	int widths[256];
	int height;

};


#endif