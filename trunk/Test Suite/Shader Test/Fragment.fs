varying vec3 ModelPosition;   // ��������� ��������� � ��������� �����������

varying float LightIntensity;   // ������������� ����� �� ���������

const vec3 Spacing = vec3(1.0);   // ���������� ����� ��������

const float DotSize = 0.6;   // ������ �������

const vec3 ModelColor = vec3(1.0, 0.0, 0.0);   // ���� �����������

const vec3 PolkaDotColor = vec3(1.0);   // ���� �������

void main(void)
{
   // ������� ��������� ��������� � ���������������� ������� ���������
   vec3 scaledPoint = ModelPosition - (Spacing * floor(ModelPosition / Spacing));

   // �������� �������� � ����� ���������������� ������� ���������
   scaledPoint -= Spacing / 2.0;

   // ������� ���������� �� ��������� � ���������������� ������� ���������
   float scaledPointLength = length(scaledPoint);
   
   // ���������� ���������� � �������� ������
   float insideSphere = step(scaledPointLength, DotSize);
   
   // ���������� ����������� ���� ���������
   vec3 finalColor = vec3(mix(ModelColor, PolkaDotColor, insideSphere));

   // �������� �������� � ���������� ���������
   gl_FragColor = vec4(finalColor * LightIntensity, 1.0);
}
