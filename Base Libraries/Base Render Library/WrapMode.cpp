#include "WrapMode.h"

namespace Render
{
	WrapMode :: WrapMode ( int s, int t, int r )
	{
		WrapS = s;

		WrapT = t;

		WrapR = r;
	}

	//------------------------------------- Setup Mode ------------------------------------
			
	void WrapMode :: Setup ( int target )
	{
		glTexParameteri ( target, GL_TEXTURE_WRAP_S, WrapS );
			
		glTexParameteri ( target, GL_TEXTURE_WRAP_T, WrapT );
				
		glTexParameteri ( target, GL_TEXTURE_WRAP_R, WrapR );
	}
}