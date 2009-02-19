varying vec3 ModelPosition;   // Положение вершины в модельном пространстве

varying float LightIntensity;   // Интенсивность света в вершине

const vec3 LightPosition = vec3(10.0, 10.0, 10.0);   // Расположение источника света в пространстве обзора

const float SpecularContribution = 0.3;   // Доля зеркального света

const float DiffuseContribution = 0.7;   // Доля рассеянного света

const float AmbientContribution = 0.2;   // Доля фонового света

const float Shininess = 16.0;   // Коэффициент резкости бликов

void main()
{   
    // Вычисляем положение вершины в пространстве обзора
    vec3 ecPosition = vec3(gl_ModelViewMatrix * gl_Vertex);
    
    // Вычисляем координаты нормали в пространстве обзора
    vec3 ecNormal = normalize(gl_NormalMatrix * gl_Normal);
    
    // Вычисляем вектор от вершины к источнику света
    vec3 ecLightVec = normalize(LightPosition - ecPosition);
    
    // Вычисляем вектор отраженного света
    vec3 ecReflectVec = reflect(-ecLightVec, ecNormal);
    
    // Вычисляем вектор направления на наблюдателя
    vec3 ecViewVec = normalize(-ecPosition);
    
    // Вычисляем интенсивность рассеянного света
    float diffuse = abs(dot(ecLightVec, ecNormal));
    
    // Вычисляем интенсивность зеркально отраженного света
    float specular = 0.0;
   
    if (diffuse > 0.0)
    {
    	specular = pow(abs(dot(ecReflectVec, ecViewVec)), Shininess);
    }
    
    // Вычисляем общую интенсивоность света в вершине    
    LightIntensity  = DiffuseContribution * diffuse +
                      SpecularContribution * specular +
                      AmbientContribution;
    
    // Сохраняем модельные координаты вершины                    
    ModelPosition = vec3(gl_Vertex);

	// Вычисляем положение вершины в пространстве отсечения
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
