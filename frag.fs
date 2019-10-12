#version 330 core
in float high;
in vec3 fragpos;//存储每个片段的位置

out vec4 color;


uniform vec3 lightColor; //环境光
uniform vec3 lightPos;  //光的位置


uniform vec3 viewPos; //视点位置
//uniform vec4 ourColor;
void main(){
	if(high>1){
		color = vec4(1.0,0.0,0.0,1.0f);
	}
	else{
		color = vec4(0.0,1.0,0.0,1.0f);
	}
}
