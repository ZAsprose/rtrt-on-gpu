/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#include "FilterMode.h"

namespace Render
{
	//---------------------------------------- Constructor ----------------------------------------

	FilterMode :: FilterMode ( int minification, int magnification )
	{
		Minification = minification;

		Magnification =  magnification;
	}

	//---------------------------------------- Setup Mode -----------------------------------------
			
	void FilterMode :: Setup ( int target )
	{
		glTexParameteri( target, GL_TEXTURE_MIN_FILTER, Minification );
				
		glTexParameteri( target, GL_TEXTURE_MAG_FILTER, Magnification );
	}
}