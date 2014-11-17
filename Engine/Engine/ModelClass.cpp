#include "ModelClass.h"

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;

	m_Texture = 0;

	m_model = 0;
}

ModelClass::ModelClass(const ModelClass& other)
{

}

ModelClass::~ModelClass()
{

}

bool ModelClass::Initialize(ID3D11Device* device, WCHAR* textureFilename)
{
	bool result;

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}
	return true;
}

bool ModelClass::Initialize( ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
{
	bool result;

	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;

}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffer(deviceContext);
	return;
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertice;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;
// 
// 	m_vertexCount = 4;
// 
// 	m_indexCount = 6;

	vertice = new VertexType[m_vertexCount];
	if (!vertice)
	{
		return false;
	}

	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	for (i = 0; i<m_vertexCount;i++)
	{
		vertice[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertice[i].texture  = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		vertice[i].normal   = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

// 	vertice[0].position = D3DXVECTOR3(-2.0f, 2.0f, 0.0f);
// 	vertice[0].texture = D3DXVECTOR2(0.0f, 0.0f);
// 	vertice[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
// 
// 	vertice[1].position = D3DXVECTOR3(2.0f, 2.0f, 0.0f);
// 	vertice[1].texture = D3DXVECTOR2(1.0f, 0.0f);
// 	vertice[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
// 
// 	vertice[2].position = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
// 	vertice[2].texture = D3DXVECTOR2(1.0f, 1.0f);
// 	vertice[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
// 
//   	vertice[3].position = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
//  	vertice[3].texture = D3DXVECTOR2(0.0f, 1.0f);
// 	vertice[3].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
// 
// 	indices[0] = 0;
// 	indices[1] = 1;
//     indices[2] = 2;
// 
//  	indices[3] = 2;
//  	indices[4] = 3;
//  	indices[5] = 0;
		

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)*m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertice;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)*m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete []vertice;
	vertice = 0;

	delete []indices;
	indices = 0;

	return true;
}

void ModelClass::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();

	ReleaseModel();

	return ;
}

void ModelClass::RenderBuffer(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

void ModelClass::ShutdownBuffers()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

bool ModelClass::LoadTexture( ID3D11Device* device, WCHAR* filename)
{
	bool result;

	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	result= m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}
	
	return true;
}

void ModelClass::ReleaseTexture()
{
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

bool ModelClass::LoadModel( char* filename)
{
	ifstream fin;
	char input;
	int i;

	fin.open(filename);

	if (fin.fail())
	{
		return false;
	}

	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}

	fin >> m_vertexCount;

	m_indexCount = m_vertexCount;

	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}

	fin.get(input);
	fin.get(input);

	for(i = 0; i<m_vertexCount; i++)
	{
		fin>>m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin>>m_model[i].tu>>m_model[i].tv;
		fin>>m_model[i].nx>>m_model[i].ny>>m_model[i].nz;
	}

	fin.close();

	return true;

}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete []m_model;
		m_model = 0;
	}

	return ;
}
