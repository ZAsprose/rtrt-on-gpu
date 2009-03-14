//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

varying vec4 FragmentColor;

varying vec2 FragmentPosition;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

void main()
{
	vec2 posA = abs( FragmentPosition );
	
	vec2 posB = abs( vec2( FragmentPosition.x + FragmentPosition.y, FragmentPosition.x - FragmentPosition.y ) );
			
	gl_FragColor = mix( FragmentColor, vec4( 0.0 ),
	                    0.3 * ( pow( posA.x, 0.3 ) + pow( posA.y, 0.3 ) + pow( posB.x, 0.3 ) + pow( posB.y, 0.3 ) ) );
}