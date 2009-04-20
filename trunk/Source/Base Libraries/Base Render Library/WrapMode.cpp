/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#include "WrapMode.h"

namespace Render
{
	//-------------------------------------- Public Constants -------------------------------------
	
	const WrapMode WrapMode :: Clamp ( GL_CLAMP, GL_CLAMP, GL_CLAMP );
	
	const WrapMode WrapMode :: Repeat ( GL_REPEAT, GL_REPEAT, GL_REPEAT );

	//---------------------------------------- Constructor ----------------------------------------

	WrapMode :: WrapMode ( int s, int t, int r )
	{
		WrapS = s;

		WrapT = t;

		WrapR = r;
	}

	//----------------------------------------- Setup Mode ----------------------------------------
			
	void WrapMode :: Setup ( int target )
	{
		glTexParameteri ( target, GL_TEXTURE_WRAP_S, WrapS );
			
		glTexParameteri ( target, GL_TEXTURE_WRAP_T, WrapT );
				
		glTexParameteri ( target, GL_TEXTURE_WRAP_R, WrapR );
	}
}