//������ ��� �������������� ��������� ������ �������

uniform sampler2DRect   srcMap;//����������� ��������
uniform float           step;//��� ���������

void    main ()
{
    float   first    = gl_TexCoord [0].z;           // ������ ������� ���� (-1)/������ ������� ���� (1)
    float   dir      = gl_TexCoord [0].w;           // ��������� �� �������� (1)/��������� �� ����������� (-1)
    vec2    texCur   = gl_TexCoord [0].xy;	//�������� ��-� ���������
    vec2    texOther = texCur - sign ( first ) * vec2 ( 0.0, step );//�������� ���� ��� ���������
    vec4    cur      = texture2DRect ( srcMap, texCur );
    vec4    other    = texture2DRect ( srcMap, texOther );
    float   key      = first * ( cur.a - other.a );//��� ��������� ������� �������� �� ������
    
    if ( dir > 0.0 )                                // we want in first min (), in second -> max
    {
        if ( key < 0.0 )
            cur = other;
    }
    else                                            // we want v1 > v2
    {
        if ( key > 0.0 )
            cur = other;
    }
    
    gl_FragColor = cur;
}
