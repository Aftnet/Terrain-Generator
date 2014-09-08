varying vec4 diffuse,ambient;
varying vec3 normal,lightDir,halfVector;
varying float height;
uniform float terrmedheight;
uniform sampler2D Tex0S,Tex1S,Tex2S,Tex3S;


void main()
{
	vec3 n,halfV,viewV,ldir;
	float NdotL,NdotHV,slope;
	vec4 color = ambient;
	vec4 texel0,texel1,texel2,texel3,finaltexel;
	
	/* a fragment shader can't write a verying variable, hence we need
	a new variable to store the normalized interpolated normal */
	n = normalize(normal);
	
	/* compute the dot product between normal and ldir */
	NdotL = max(dot(n,lightDir),0.0);

	halfV = normalize(halfVector);
	NdotHV = max(dot(n,halfV),0.0);
	color += gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(NdotHV,gl_FrontMaterial.shininess);
	color += diffuse * NdotL;
	
	texel0 = texture2D(Tex0S,gl_TexCoord[0].st);
	texel1 = texture2D(Tex1S,gl_TexCoord[0].st);
	texel2 = texture2D(Tex2S,gl_TexCoord[0].st);
	texel3 = texture2D(Tex3S,gl_TexCoord[0].st);
	vec3 tmp = vec3(0.0,1.0,0.0);
	tmp = gl_NormalMatrix * tmp;
	slope = max(dot(n,tmp),0.0);
	if(height>terrmedheight+100.0)
	{
		slope = pow(slope,8.0);
		finaltexel = texel3 * (slope) + texel0 * (1.0 - slope);
	}
	else if(height>terrmedheight+97.0)
	{
		slope = pow(slope,5.0);
		finaltexel = (((height-(terrmedheight+97.0))/3.0)* texel3 + (((terrmedheight+100.0)-height)/3.0)*texel2) * (slope) + texel0 * (1.0 - slope);
	}
	else if(height>terrmedheight+25.0)
	{
		slope = pow(slope,3.0);
		finaltexel = texel2 * (slope) + texel0 * (1.0 - slope);
	}
	else if(height>terrmedheight+22.0)
	{
		slope = pow(slope,3.0);
		finaltexel = (((height-(terrmedheight+22.0))/3.0)* texel2 + (((terrmedheight+25.0)-height)/3.0)*texel1) * (slope) + texel0 * (1.0 - slope);
	}
	else
	{
		slope = pow(slope,3.0);
		finaltexel = texel1 * (slope) + texel0 * (1.0 - slope);
	}
	
	color = color * finaltexel;
	
	gl_FragColor = color;
}
