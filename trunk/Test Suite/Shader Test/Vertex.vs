varying vec3 ModelPosition;   // ��������� ������� � ��������� ������������

varying float LightIntensity;   // ������������� ����� � �������

const vec3 LightPosition = vec3(10.0, 10.0, 10.0);   // ������������ ��������� ����� � ������������ ������

const float SpecularContribution = 0.3;   // ���� ����������� �����

const float DiffuseContribution = 0.7;   // ���� ����������� �����

const float AmbientContribution = 0.2;   // ���� �������� �����

const float Shininess = 16.0;   // ����������� �������� ������

void main()
{   
    // ��������� ��������� ������� � ������������ ������
    vec3 ecPosition = vec3(gl_ModelViewMatrix * gl_Vertex);
    
    // ��������� ���������� ������� � ������������ ������
    vec3 ecNormal = normalize(gl_NormalMatrix * gl_Normal);
    
    // ��������� ������ �� ������� � ��������� �����
    vec3 ecLightVec = normalize(LightPosition - ecPosition);
    
    // ��������� ������ ����������� �����
    vec3 ecReflectVec = reflect(-ecLightVec, ecNormal);
    
    // ��������� ������ ����������� �� �����������
    vec3 ecViewVec = normalize(-ecPosition);
    
    // ��������� ������������� ����������� �����
    float diffuse = abs(dot(ecLightVec, ecNormal));
    
    // ��������� ������������� ��������� ����������� �����
    float specular = 0.0;
   
    if (diffuse > 0.0)
    {
    	specular = pow(abs(dot(ecReflectVec, ecViewVec)), Shininess);
    }
    
    // ��������� ����� �������������� ����� � �������    
    LightIntensity  = DiffuseContribution * diffuse +
                      SpecularContribution * specular +
                      AmbientContribution;
    
    // ��������� ��������� ���������� �������                    
    ModelPosition = vec3(gl_Vertex);

	// ��������� ��������� ������� � ������������ ���������
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
