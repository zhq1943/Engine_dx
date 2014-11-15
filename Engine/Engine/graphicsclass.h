#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <Windows.h>
#include "d3dclass.h"
#include "cameraclass.h"
#include "ModelClass.h"
#include "Colorshaderclass.h"
#include "Textureshaderclass.h"

#include "Lightshaderclass.h"
#include "lightclass.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render(float);
private:
	D3DClass* m_D3D;
	CameraClass* m_camera;
	ModelClass* m_model;
	ColorShaderClass* m_colorshader;
	TextureShaderClass* m_TextureShader;

	LightClass* m_Light;
	LightShaderClass* m_LightShader;
};


#endif