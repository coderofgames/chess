#ifndef COLOR_PALLETE_H
#define COLOR_PALLETE_H


#include <vector>

using std::vector;

class RenderMoveColor
{
public:
	RenderMoveColor( int c1, int c2, int c3 )
	{
		r=c1; g=c2; b=c3;
	}

	inline bool operator == (RenderMoveColor &other)
	{
		return ((r==other.r)&&(g==other.g)&&(b==other.b));
	}

	inline void operator=(RenderMoveColor &other)
	{
		memcpy( (void*)this, (void*)&other, sizeof(this));
	}

	int r;
	int g;
	int b;


};

class ColorPallete
{
public:

	ColorPallete()
	{
		colors.push_back(RenderMoveColor(0,0,0));
	}
	~ColorPallete(){}

	int AddColor( RenderMoveColor &newColor );

	int AddColor( int r, int g, int b );

	inline RenderMoveColor& GetColor( int idx )
	{
		return (idx<colors.size() ? colors[idx] : colors[0]);
	}

	vector< RenderMoveColor > colors;
};



#endif