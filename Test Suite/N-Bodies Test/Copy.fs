#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect NextPositionTexture;  

uniform sampler2DRect NextVelocityTexture;

void main(void)
{                     
   gl_FragData[0] = texture2DRect ( NextPositionTexture, gl_TexCoord[0] );
   
   gl_FragData[1] = texture2DRect ( NextVelocityTexture, gl_TexCoord[0] );
}