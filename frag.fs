#version 330 core
in float high;
in vec3 fragpos;//�洢ÿ��Ƭ�ε�λ��

out vec4 color;


uniform vec3 lightColor; //������
uniform vec3 lightPos;  //���λ��


uniform vec3 viewPos; //�ӵ�λ��
//uniform vec4 ourColor;
void main(){
	if(high>1){
		color = vec4(1.0,0.0,0.0,1.0f);
	}
	else{
		color = vec4(0.0,1.0,0.0,1.0f);
	}
}
