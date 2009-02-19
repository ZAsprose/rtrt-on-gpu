varying vec3 ModelPosition;   // Положение фрагмента в модельных координатах

varying float LightIntensity;   // Интенсивность света во фрагменте

const vec3 Spacing = vec3(1.0);   // Расстояние между кружками

const float DotSize = 0.6;   // Размер кружков

const vec3 ModelColor = vec3(1.0, 0.0, 0.0);   // Цвет поверхности

const vec3 PolkaDotColor = vec3(1.0);   // Цвет кружков

void main(void)
{
   // Находим положение фрагмента в масштабированной системе координат
   vec3 scaledPoint = ModelPosition - (Spacing * floor(ModelPosition / Spacing));

   // Помещаем фрагмент в центр масштабированной системы координат
   scaledPoint -= Spacing / 2.0;

   // Находим расстояние до фрагмента в масштабированной системы координат
   float scaledPointLength = length(scaledPoint);
   
   // Сравниваем расстоняие с радиусом кружка
   float insideSphere = step(scaledPointLength, DotSize);
   
   // Определяем собственный цвет фрагмента
   vec3 finalColor = vec3(mix(ModelColor, PolkaDotColor, insideSphere));

   // Затеняем фрагмент и возвращаем результат
   gl_FragColor = vec4(finalColor * LightIntensity, 1.0);
}
