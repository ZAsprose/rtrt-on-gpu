//Шейдер для упорядочивания элементов внутри столбца

uniform sampler2DRect   srcMap;//сортируемая текстура
uniform float           step;//шаг сортровки

void    main ()
{
    float   first    = gl_TexCoord [0].z;           // первый элемент пары (-1)/второй элемент пары (1)
    float   dir      = gl_TexCoord [0].w;           // сортируем по убывания (1)/сортируем по возрастанию (-1)
    vec2    texCur   = gl_TexCoord [0].xy;	//получаем зн-е координат
    vec2    texOther = texCur - sign ( first ) * vec2 ( 0.0, step );//получаем пару для сортровки
    vec4    cur      = texture2DRect ( srcMap, texCur );
    vec4    other    = texture2DRect ( srcMap, texOther );
    float   key      = first * ( cur.a - other.a );//для сравнения первого значения со вторым
    
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
