varying vec2 TextureCoord;    

uniform sampler2D NextPositionTexture;  

uniform sampler2D NextVelocityTexture;

void main(void)
{                     
   gl_FragData[0] = texture2D( NextPositionTexture, TextureCoord );
   
   gl_FragData[1] = texture2D( NextVelocityTexture, TextureCoord );
}