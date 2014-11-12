#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_camera = 0;
	m_model = 0;
	m_colorshader = 0;
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

	m_camera->SetPosition(0.0f, 0.0f, -20.0f);

	m_model = new ModelClass;
	if (!m_model)
	{
		return false;
	}

	result = m_model->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"model error", L"Error", MB_OK);
		return false;
	}

	m_colorshader = new ColorShaderClass;
	if (!m_colorshader)
	{
		return false;
	}

	result = m_colorshader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"color shader errror", L"Error", MB_OK);
		return false;
	}
	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_colorshader)
	{
		m_colorshader->Shutdown();
		delete m_colorshader;
		m_colorshader = 0;
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

	
	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	result = Render();
	if (!result)
	{
		return false;
	}
	return true;
}

bool GraphicsClass::Render()
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;
	m_D3D->BeginScene(0.0f,0.0f, 0.0f, 1.0f);

	m_camera->Render();

	m_camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	m_model->Render(m_D3D->GetDeviceContext());

	result = m_colorshader->Render(m_D3D->GetDeviceContext(), m_model->GetIndexCount(),worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}
	m_D3D->EndScene();
	return true;
}
