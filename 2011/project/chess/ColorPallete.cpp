#include "ColorPallete.h"
#include "Headers.h"


int ColorPallete::AddColor( RenderMoveColor &newColor )
{
	for( int i = 0; i < colors.size(); i++ )
	{
		if( newColor == colors[i] )
		{
			return i;
		}
	}

	if( colors.size() == 255 )
	{
		wxMessageBox("Maximum Number of Colors is 255");
		return 0;
	}

	colors.push_back(newColor);
	return colors.size()-1;
}

int ColorPallete::AddColor( int r, int g, int b )
{
	RenderMoveColor rc(r, g, b);
	for( int i = 0; i < colors.size(); i++ )
	{
		if( rc == colors[i] )
		{
			return i;
		}
	}
	if( colors.size() == 255 )
	{
		wxMessageBox("Maximum Number of Colors is 255");
		return 0;
	}
	colors.push_back(rc);
	return colors.size()-1;
}