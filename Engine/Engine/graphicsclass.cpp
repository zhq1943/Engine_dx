#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_camera = 0;
	m_model = 0;
	//m_colorshader = 0;

	m_TextureShader = 0;

	m_Light = 0;
	m_LightShader = 0;
}

GraphicsClass::GraphicsClass( const GraphicsClass& other)
{

}

GraphicsClass::~GraphicsClass()
{

}

bool GraphicsClass::Initialize( int screenWidth, int screenHeigh, HWND hwnd )
{
	bool result;
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	result = m_D3D->Initialize(screenWidth, screenHeigh, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	m_camera = new CameraClass;
	if (!m_camera)
	{
		return false;
	}

	m_camera->SetPosition(0.0f, 0.0f, -5.0f);

	m_model = new ModelClass;
	if (!m_model)
	{
		return false;
	}

	//result = m_model->Initialize(m_D3D->GetDevice(), L"../Engine/tes.dds");
	result = m_model->Initialize(m_D3D->GetDevice(), "../Engine/data/Cube.txt", L"../Engine/data/tes.dds");
	if (!result)
	{
		MessageBox(hwnd, L"model error", L"Error", MB_OK);
		return false;
	}

// 	m_colorshader = new ColorShaderClass;
// 	if (!m_colorshader)
// 	{
// 		return false;
// 	}
// 
// 	result = m_colorshader->Initialize(m_D3D->GetDevice(), hwnd);
// 	if (!result)
// 	{
// 		return false;
// 	}

// 	m_TextureShader = new TextureShaderClass;
// 	if (!m_TextureShader)
// 	{
// 		return false;
// 	}
// 
// 	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
// 	if (!result)
// 	{
// 		MessageBox(hwnd, L"texture shader errror", L"Error", MB_OK);
// 		return false;
// 	}

	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"could not initialize the light shader", L"Error", MB_OK);
		return false;
	}

	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if (m_model)
	{
		m_model->Shutdown();
		delete m_model;
		m_model = 0;
	}

	if (m_camera)
	{
		delete m_camera;
		m_camera = 0;
	}
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

// 	if (m_colorshader)
// 	{
// 		delete m_colorshader;
// 		m_colorshader = 0;
// 	}

	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}
	
	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	static float rotation = 0.0f;
	rotation += (float)D3DX_PI*0.005f;

	if (rotation > 360)
	{
		rotation -= 360;
	}
	result = Render(rotation);
	if (!result)
	{
		return false;
	}
	return true;
}

bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;
	m_D3D->BeginScene(0.0f,0.0f, 0.0f, 1.0f);

	m_camera->Render();

	m_camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	D3DXMatrixRotationY(&worldMatrix, rotation);
	m_model->Render(m_D3D->GetDeviceContext());

	//result = m_colorshader->Render(m_D3D->GetDeviceContext(), m_model->GetIndexCount(),worldMatrix, viewMatrix, projectionMatrix);
	//result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_model->GetIndexCount(), worldMatrix, viewMatrix,projectionMatrix, m_model->GetTexture());
    result = m_LightShader->Render(m_D3D->GetDeviceContext(), 
									m_model->GetIndexCount(), 
									worldMatrix, 
									viewMatrix,
									projectionMatrix, 
									m_model->GetTexture(), 
									m_Light->GetDirection(), 
									m_Light->GetDiffuseColor());
	if (!result)
	{
		return false;
	}
	m_D3D->EndScene();
	return true;
}
