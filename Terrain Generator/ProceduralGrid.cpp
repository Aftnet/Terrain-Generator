#include "ProceduralGrid.h"

ProceduralGrid::ProceduralGrid(GLuint width, GLuint height, GLfloat tilesize, GLuint htexturetiles, GLuint vtexturetiles)
{
	//Define width and height
	if(width < 1)
	{
		m_iWidth = 2;
	}
	else
	{
		m_iWidth = width+1;
	}
	if(height < 1)
	{
		m_iHeight = 2;
	}
	else
	{
		m_iHeight = height+1;
	}

	m_fTileSize = tilesize;
	m_fHsize = (float(m_iWidth-1))*m_fTileSize;
	m_fVsize = (float(m_iHeight-1))*m_fTileSize;

	SetNumVertices(m_iWidth*m_iHeight);
	SetNumSubsets(1);
	ResizeSubset(0, (2*(m_iWidth-1)*(m_iHeight-1)));

	//Generete base geometry
	GLuint i,j;
	for(i=0;i<m_iHeight;i++)
	{
		for(j=0;j<m_iWidth;j++)
		{
			m_pVertices[j+i*m_iWidth].Pos.x = (float(j) - (float(m_iHeight-1)*0.5f))*m_fTileSize;
			m_pVertices[j+i*m_iWidth].Pos.y = 0.0f;
			m_pVertices[j+i*m_iWidth].Pos.z = ((float(m_iHeight-1)*0.5f) - float(i))*m_fTileSize;
			m_pVertices[j+i*m_iWidth].Pos.w = 1.0f;

			m_pVertices[j+i*m_iWidth].Norm.x = 0.0f;
			m_pVertices[j+i*m_iWidth].Norm.y = 1.0f;
			m_pVertices[j+i*m_iWidth].Norm.z = 0.0f;
			m_pVertices[j+i*m_iWidth].Norm.w = 0.0f;

			m_pVertices[j+i*m_iWidth].Tex.x = float(htexturetiles) * float(j)/float(m_iWidth-1);
			m_pVertices[j+i*m_iWidth].Tex.y = float(vtexturetiles) * float(i)/float(m_iHeight-1);
			m_pVertices[j+i*m_iWidth].Tex.z = 0.0f;
			m_pVertices[j+i*m_iWidth].Tex.w = 0.0f;
		}
	}
	
	GenerateVertexBuffer();

	m_fMedianHeight = 0.0f;

	//Generate indices
	GLuint state=0;
	GLuint ctr=0;
	for(i=0;i<(m_iHeight-1);i++)
	{
		for(j=0;j<(m_iWidth-1);j++)
		{
			if(state==0)
			{
				m_pSubsets[0].pIndices[ctr]=j+(i+1)*m_iWidth;
				ctr++;
				m_pSubsets[0].pIndices[ctr]=j+i*m_iWidth;
				ctr++;
				m_pSubsets[0].pIndices[ctr]=(j+1)+(i+1)*m_iWidth;
				ctr++;

				m_pSubsets[0].pIndices[ctr]=(j+1)+i*m_iWidth;
				ctr++;
				m_pSubsets[0].pIndices[ctr]=(j+1)+(i+1)*m_iWidth;
				ctr++;
				m_pSubsets[0].pIndices[ctr]=j+i*m_iWidth;
				ctr++;
			}
			else
			{
				m_pSubsets[0].pIndices[ctr]=(j+1)+i*m_iWidth;
				ctr++;
				m_pSubsets[0].pIndices[ctr]=j+(i+1)*m_iWidth;
				ctr++;
				m_pSubsets[0].pIndices[ctr]=j+i*m_iWidth;
				ctr++;

				m_pSubsets[0].pIndices[ctr]=j+(i+1)*m_iWidth;
				ctr++;
				m_pSubsets[0].pIndices[ctr]=(j+1)+i*m_iWidth;
				ctr++;
				m_pSubsets[0].pIndices[ctr]=(j+1)+(i+1)*m_iWidth;
				ctr++;
			}
			state=(state+1)%2;
		}
		if((m_iWidth%2)==1)
		{
			state=(state+1)%2;
		}
	}

	GenerateSubsetIndicesBuffer(0);

	//Generate material properties
	m_pSubsets[0].Material.Specular[0] = 0.0f;
	m_pSubsets[0].Material.Specular[1] = 0.0f;
	m_pSubsets[0].Material.Specular[2] = 0.0f;
	m_pSubsets[0].Material.Specular[3] = 1.0f;
	m_pSubsets[0].Material.Emissive[0] = 0.0f;
	m_pSubsets[0].Material.Emissive[1] = 0.0f;
	m_pSubsets[0].Material.Emissive[2] = 0.0f;
	m_pSubsets[0].Material.Emissive[3] = 1.0f;
	m_pSubsets[0].Material.Diffuse[0] = 0.6f;
	m_pSubsets[0].Material.Diffuse[1] = 0.6f;
	m_pSubsets[0].Material.Diffuse[2] = 0.6f;
	m_pSubsets[0].Material.Diffuse[3] = 1.0f;
	m_pSubsets[0].Material.Ambient[0] = 0.4f;
	m_pSubsets[0].Material.Ambient[1] = 0.4f;
	m_pSubsets[0].Material.Ambient[2] = 0.4f;
	m_pSubsets[0].Material.Ambient[3] = 1.0f;
	m_pSubsets[0].Material.Shininess[0] = 80.0f;
}

ProceduralGrid::~ProceduralGrid(void)
{
}

void ProceduralGrid::Render(void)
{
	DrawSubset(0);
}

//Returns the grid's total width and height as the x and z components of a vector
Vector4 ProceduralGrid::GetGridSize(void) const
{
	Vector4 size(m_fHsize,0.0f,m_fVsize,0.0f);
	return size;
}

float ProceduralGrid::GetMedianHeight(void) const
{
	return m_fMedianHeight;
}

//xpos and ypos are in the grid's model space: it may be necessary to convert.
float ProceduralGrid::GetPointHeight(float xpos, float zpos) const
{
	Vector4 tmp1,tmp2;
	GLuint i,j;
	float w;
	float xposition = xpos;
	float zposition = zpos;
	float tmpy1,tmpy2,yposition;

	xposition += 0.5f*float(m_iWidth-1)*m_fTileSize;
	zposition -= 0.5f*float(m_iHeight-1)*m_fTileSize;
	i=GLuint(xposition/m_fTileSize);
	j=GLuint(0.0f - zposition/m_fTileSize);
	if(i>=m_iWidth)
	{return 0.0f;}
	if(j>=m_iHeight)
	{return 0.0f;}

	GetVertexAttrib(POSITION,i,j,tmp1);
	GetVertexAttrib(POSITION,i,j+1,tmp2);
	w = ((tmp1.z - zpos)/m_fTileSize);
	
	tmpy1 = (1.0f-w)*tmp1.y + w*tmp2.y;

	GetVertexAttrib(POSITION,i+1,j,tmp1);
	GetVertexAttrib(POSITION,i+1,j+1,tmp2);
	
	tmpy2 = (1.0f-w)*tmp1.y + w*tmp2.y;

	GetVertexAttrib(POSITION,i,j,tmp1);
	GetVertexAttrib(POSITION,i+1,j,tmp2);
	
	w = ((xpos - tmp1.x)/m_fTileSize);
	yposition = (1.0f-w)*tmpy1+w*tmpy2;

	return yposition;
}

Vector4 ProceduralGrid::GetPointNormal(float xpos, float zpos) const
{
	Vector4 postmp,tmp1,tmp2;
	GLuint i,j;
	float w;
	float xposition = xpos;
	float zposition = zpos;
	Vector4 tmpn1,tmpn2,Normal;
	Normal = Vector4(0.0f, 1.0f, 0.0f, 0.0f);

	xposition += 0.5f*float(m_iWidth-1)*m_fTileSize;
	zposition -= 0.5f*float(m_iHeight-1)*m_fTileSize;
	i=GLuint(xposition/m_fTileSize);
	j=GLuint(0.0f - zposition/m_fTileSize);
	if(i>=m_iWidth)
	{return Normal;}
	if(j>=m_iHeight)
	{return Normal;}

	GetVertexAttrib(POSITION,i,j,tmp1);
	w = ((tmp1.z - zpos)/m_fTileSize);

	GetVertexAttrib(NORMAL,i,j,tmp1);
	GetVertexAttrib(NORMAL,i,j+1,tmp2);

	tmpn1 = (1.0f-w)*tmp1 + w*tmp2;
	tmpn1.NormaliseSelf();

	GetVertexAttrib(NORMAL,i+1,j,tmp1);
	GetVertexAttrib(NORMAL,i+1,j+1,tmp2);

	tmpn2 = (1.0f-w)*tmp1 + w*tmp2;
	tmpn2.NormaliseSelf();

	GetVertexAttrib(POSITION,i,j,tmp1);
	w = ((xpos - tmp1.x)/m_fTileSize);

	Normal = (1.0f-w)*tmpn1 + w*tmpn2;
	Normal.NormaliseSelf();

	return Normal;
}

void ProceduralGrid::Reset(float height)
{
	GLuint i,j;
	for(i=0;i<m_iHeight;i++)
	{
		for(j=0;j<m_iWidth;j++)
		{
			m_pVertices[j+i*m_iWidth].Pos.x = (float(j) - (float(m_iHeight-1)*0.5f))*m_fTileSize;
			m_pVertices[j+i*m_iWidth].Pos.y = height;
			m_pVertices[j+i*m_iWidth].Pos.z = ((float(m_iHeight-1)*0.5f) - float(i))*m_fTileSize;
			m_pVertices[j+i*m_iWidth].Pos.w = 1.0f;
		}
	}
	NormalGen();
	m_fMedianHeight = height;
}

void ProceduralGrid::Smooth(GLuint iterations, GLuint centerweight)
{
	Vector4 vert,*temp;
	bool res;
	temp = new Vector4[m_uiNumVertices];
	GLuint i,j,it,ctr;
	for(it=0;it<iterations;it++)
	{
		for(i=0;i<(m_iHeight);i++)
		{
			for(j=0;j<(m_iWidth);j++)
			{
				ctr=0;
				temp[j+i*m_iWidth].x = 0.0f;
				temp[j+i*m_iWidth].y = 0.0f;
				temp[j+i*m_iWidth].z = 0.0f;
				temp[j+i*m_iWidth].w = 0.0f;

				res = GetVertexAttrib(POSITION,j,i,vert);
				if(res)
				{
					temp[j+i*m_iWidth]+= float(centerweight)*vert;
					ctr+=centerweight;
				}
				res = GetVertexAttrib(POSITION,(j-1),i,vert);
				if(res)
				{
					temp[j+i*m_iWidth]+= float(centerweight)*vert;
					ctr+=centerweight;
				}
				res = GetVertexAttrib(POSITION,(j-1),(i-1),vert);
				if(res)
				{
					temp[j+i*m_iWidth]+= float(centerweight)*vert;
					ctr+=centerweight;
				}
				res = GetVertexAttrib(POSITION,j,(i-1),vert);
				if(res)
				{
					temp[j+i*m_iWidth]+= float(centerweight)*vert;
					ctr+=centerweight;
				}
				res = GetVertexAttrib(POSITION,(j+1),(i-1),vert);
				if(res)
				{
					temp[j+i*m_iWidth]+= float(centerweight)*vert;
					ctr+=centerweight;
				}
				res = GetVertexAttrib(POSITION,(j+1),i,vert);
				if(res)
				{
					temp[j+i*m_iWidth]+= float(centerweight)*vert;
					ctr+=centerweight;
				}
				res = GetVertexAttrib(POSITION,(j+1),(i+1),vert);
				if(res)
				{
					temp[j+i*m_iWidth]+= float(centerweight)*vert;
					ctr+=centerweight;
				}
				res = GetVertexAttrib(POSITION,j,(i+1),vert);
				if(res)
				{
					temp[j+i*m_iWidth]+= float(centerweight)*vert;
					ctr+=centerweight;
				}
				res = GetVertexAttrib(POSITION,(j-1),(i+1),vert);
				if(res)
				{
					temp[j+i*m_iWidth]+= float(centerweight)*vert;
					ctr+=centerweight;
				}
				temp[j+i*m_iWidth].x = temp[j+i*m_iWidth].x/float(ctr);
				temp[j+i*m_iWidth].y = temp[j+i*m_iWidth].y/float(ctr);
				temp[j+i*m_iWidth].z = temp[j+i*m_iWidth].z/float(ctr);
				temp[j+i*m_iWidth].w = 1.0f;
			}
		}
		for(i=0;i<m_uiNumVertices;i++)
		{
			m_pVertices[i].Pos = temp[i];
		}
	}
	delete [] temp;
	NormalGen();
	CalculateMedianHeight();
}

void ProceduralGrid::RandomNoise(float magnitude)
{
	GLuint i,j;
	for(i=0;i<m_iHeight;i++)
	{
		for(j=0;j<m_iWidth;j++)
		{
			m_pVertices[j+i*m_iWidth].Pos.y += magnitude*((float(rand())/float(RAND_MAX))-0.5f);
		}
	}
	NormalGen();
	CalculateMedianHeight();
}

void ProceduralGrid::Fault(GLuint iterations, float initdisplacement, float dampening)
{
	GLuint it,i,j;
	float disp = initdisplacement;
	Vector4 p1,p2,fault;
	Vector4 proj;
	for(it=0;it<iterations;it++)
	{
		p1.x = m_fTileSize * float(m_iWidth) * ((float(rand())/float(RAND_MAX)) - 0.5f);
		p1.y = 0.0f;
		p1.z = m_fTileSize * float(m_iWidth) * (0.5f - (float(rand())/float(RAND_MAX)));
		p2.x = m_fTileSize * float(m_iWidth) * ((float(rand())/float(RAND_MAX)) - 0.5f);
		p2.y = 0.0f;
		p2.z = m_fTileSize * float(m_iWidth) * (0.5f - (float(rand())/float(RAND_MAX)));
		fault = p2 - p1;
		fault.w = 0.0f;
		for(i=0;i<(m_iHeight);i++)
		{
			for(j=0;j<(m_iWidth);j++)
			{
				proj = m_pVertices[j+i*m_iWidth].Pos;
				proj.y = 0.0f;
				proj = proj - p1;
				proj = CrossProduct(proj,fault);
				proj.NormaliseSelf();
				m_pVertices[j+i*m_iWidth].Pos.y += disp * proj.y;
			}
		}
		if(dampening > 0.0f && dampening < 1.0f)
		{
			disp = disp * dampening;
		}
		else
		{
			disp = disp * (float(iterations - it)/float(iterations));
		}
	}
	NormalGen();
	CalculateMedianHeight();
}

bool ProceduralGrid::GetVertexAttrib(VertexAttribute attr, GLuint column, GLuint row, Vector4 &output) const
{
	bool succeded = false;
	if(column < m_iWidth)
	{
		if(row < m_iHeight)
		{
			switch(attr)
			{
			case POSITION:
				output=m_pVertices[column+(row)*m_iWidth].Pos;
				succeded = true;
				break;
			case NORMAL:
				output=m_pVertices[column+(row)*m_iWidth].Norm;
				succeded = true;
				break;
			case TEXCOORDINATE:
				output=m_pVertices[column+(row)*m_iWidth].Tex;
				succeded = true;
				break;
			}
		}
	}
	return succeded;
}

void ProceduralGrid::NormalGen(void)
{
	GLuint i,j;
	Vector4 VertexPos,Neighbor1Pos,Neighbor2Pos,TotalNorm,Norm,v1,v2;
	bool r1,r2;
	for(i=0;i<m_iHeight;i++)
	{
		for(j=0;j<m_iWidth;j++)
		{
			TotalNorm.x = 0.0f;
			TotalNorm.y = 0.0f;
			TotalNorm.z = 0.0f;
			TotalNorm.w = 0.0f;
			GetVertexAttrib(POSITION,j,i,VertexPos);
			if((i+j)%2==0)
			{
				r1 = GetVertexAttrib(POSITION,j,(i-1),Neighbor1Pos);
				r2 = GetVertexAttrib(POSITION,(j+1),(i-1),Neighbor2Pos);
				if(r1 && r2)
				{
					v1 = Neighbor1Pos - VertexPos;
					v2 = Neighbor2Pos - VertexPos;
					Norm = CrossProduct(v1,v2);
					Norm.NormaliseSelf();
					TotalNorm+=Norm;
				}
				r1 = GetVertexAttrib(POSITION,(j+1),(i-1),Neighbor1Pos);
				r2 = GetVertexAttrib(POSITION,(j+1),i,Neighbor2Pos);
				if(r1 && r2)
				{
					v1 = Neighbor1Pos - VertexPos;
					v2 = Neighbor2Pos - VertexPos;
					Norm = CrossProduct(v1,v2);
					Norm.NormaliseSelf();
					TotalNorm+=Norm;
				}
				r1 = GetVertexAttrib(POSITION,(j+1),i,Neighbor1Pos);
				r2 = GetVertexAttrib(POSITION,(j+1),(i+1),Neighbor2Pos);
				if(r1 && r2)
				{
					v1 = Neighbor1Pos - VertexPos;
					v2 = Neighbor2Pos - VertexPos;
					Norm = CrossProduct(v1,v2);
					Norm.NormaliseSelf();
					TotalNorm+=Norm;
				}
				r1 = GetVertexAttrib(POSITION,(j+1),(i+1),Neighbor1Pos);
				r2 = GetVertexAttrib(POSITION,j,(i+1),Neighbor2Pos);
				if(r1 && r2)
				{
					v1 = Neighbor1Pos - VertexPos;
					v2 = Neighbor2Pos - VertexPos;
					Norm = CrossProduct(v1,v2);
					Norm.NormaliseSelf();
					TotalNorm+=Norm;
				}
				r1 = GetVertexAttrib(POSITION,j,(i+1),Neighbor1Pos);
				r2 = GetVertexAttrib(POSITION,(j-1),(i+1),Neighbor2Pos);
				if(r1 && r2)
				{
					v1 = Neighbor1Pos - VertexPos;
					v2 = Neighbor2Pos - VertexPos;
					Norm = CrossProduct(v1,v2);
					Norm.NormaliseSelf();
					TotalNorm+=Norm;
				}
				r1 = GetVertexAttrib(POSITION,(j-1),(i+1),Neighbor1Pos);
				r2 = GetVertexAttrib(POSITION,(j-1),i,Neighbor2Pos);
				if(r1 && r2)
				{
					v1 = Neighbor1Pos - VertexPos;
					v2 = Neighbor2Pos - VertexPos;
					Norm = CrossProduct(v1,v2);
					Norm.NormaliseSelf();
					TotalNorm+=Norm;
				}
				r1 = GetVertexAttrib(POSITION,(j-1),i,Neighbor1Pos);
				r2 = GetVertexAttrib(POSITION,(j-1),(i-1),Neighbor2Pos);
				if(r1 && r2)
				{
					v1 = Neighbor1Pos - VertexPos;
					v2 = Neighbor2Pos - VertexPos;
					Norm = CrossProduct(v1,v2);
					Norm.NormaliseSelf();
					TotalNorm+=Norm;
				}
				r1 = GetVertexAttrib(POSITION,(j-1),(i-1),Neighbor1Pos);
				r2 = GetVertexAttrib(POSITION,j,(i-1),Neighbor2Pos);
				if(r1 && r2)
				{
					v1 = Neighbor1Pos - VertexPos;
					v2 = Neighbor2Pos - VertexPos;
					Norm = CrossProduct(v1,v2);
					Norm.NormaliseSelf();
					TotalNorm+=Norm;
				}
			}
			else
			{
				r1 = GetVertexAttrib(POSITION,j,(i-1),Neighbor1Pos);
				r2 = GetVertexAttrib(POSITION,(j+1),i,Neighbor2Pos);
				if(r1 && r2)
				{
					v1 = Neighbor1Pos - VertexPos;
					v2 = Neighbor2Pos - VertexPos;
					Norm = CrossProduct(v1,v2);
					Norm.NormaliseSelf();
					TotalNorm+=Norm;
				}
				r1 = GetVertexAttrib(POSITION,(j+1),i,Neighbor1Pos);
				r2 = GetVertexAttrib(POSITION,j,(i+1),Neighbor2Pos);
				if(r1 && r2)
				{
					v1 = Neighbor1Pos - VertexPos;
					v2 = Neighbor2Pos - VertexPos;
					Norm = CrossProduct(v1,v2);
					Norm.NormaliseSelf();
					TotalNorm+=Norm;
				}
				r1 = GetVertexAttrib(POSITION,j,(i+1),Neighbor1Pos);
				r2 = GetVertexAttrib(POSITION,(j-1),i,Neighbor2Pos);
				if(r1 && r2)
				{
					v1 = Neighbor1Pos - VertexPos;
					v2 = Neighbor2Pos - VertexPos;
					Norm = CrossProduct(v1,v2);
					Norm.NormaliseSelf();
					TotalNorm+=Norm;
				}
				r1 = GetVertexAttrib(POSITION,(j-1),i,Neighbor1Pos);
				r2 = GetVertexAttrib(POSITION,j,(i-1),Neighbor2Pos);
				if(r1 && r2)
				{
					v1 = Neighbor1Pos - VertexPos;
					v2 = Neighbor2Pos - VertexPos;
					Norm = CrossProduct(v1,v2);
					Norm.NormaliseSelf();
					TotalNorm+=Norm;
				}
			}
			TotalNorm.NormaliseSelf();
			m_pVertices[j+i*m_iWidth].Norm = TotalNorm;
		}
	}
	GenerateVertexBuffer();
}

void ProceduralGrid::CalculateMedianHeight(void)
{
	GLuint i;
	m_fMedianHeight = 0.0f;
	for(i=0;i<m_uiNumVertices;i++)
	{
		m_fMedianHeight += m_pVertices[i].Pos.y;
	}
	m_fMedianHeight = m_fMedianHeight/float(m_uiNumVertices);
}
