// file name:	application.h
// author:		Victor Saar, Ingmar Rötzler
// e-mail:		vsaar@web.de, ingmar.r@web.de
// web site:	http://www.two-kings.de

#ifndef APPLICATION_H
#define APPLICATION_H

#include "main.h"

/*!
 *	\class	Application
 *	\author	Victor Saar, Ingmar Rötzler
 *	\brief	Main class for initialization of window and Direct3D.
 *
 *	This class automatically creates a window and initializises Direct3D.
 *	It contains the essential window and D3D data and interfaces.
 */
class Application
{
public:
	//!	Start application.
    Application(void);
	//!	Clean-up.
	~Application(void);

    //!	Create window.
	void initWindow(void);
	//!	Initialize Direct3D.
	void initDirect3D(void);
	//!	Initialize scene.
	void initScene(void);
	//!	Check for necessary device capabilities.
	void checkDeviceCaps(void);

	//!	Check the device state.
	bool checkDevice(void);

	//!	Clean-up window.
	void killWindow(void);
	//!	Clean-up Direct3D.
	void killDirect3D(void);

    //!	Get running.
	inline bool getRunning(void)
	{
		return _running;
	}

	//!	Get Direct3D device.
	inline LPDIRECT3DDEVICE9 getDevice(void)
	{
		return _device;
	}

	//!	Get window handle.
	inline HWND getHandle(void)
	{
		return _handle;
	}

	//!	Get screen width.
	inline unsigned int getWidth(void)
	{
		return _width;
	}

	//!	Get screen height.
	inline unsigned int getHeight(void)
	{
		return _height;
	}

	//!	Set running.
	inline void setRunning(bool running)
	{
		_running = running;
	}

private:
	//!	Running state of application.
	bool _running;

	//!	Window handle.
	HWND _handle;

    //!	Direct3D object.
	LPDIRECT3D9 _object;
	//!	Direct3D device.
	LPDIRECT3DDEVICE9 _device;
	//!	Presentation parameters.
	D3DPRESENT_PARAMETERS _presentParameters;
	//!	Device capabilities.
	D3DCAPS9 _deviceCaps;

	//!	Screen width.
	unsigned int _width;
	//!	Screen height.
	unsigned int _height;
	//!	Vertex processing.
	unsigned int _vertexProcessing;
	//!	Level of anisotropy.
	unsigned int _anisotropy;

	//!	Color format.
	D3DFORMAT _colorFormat;
	//!	Depth/Stencil buffer format.
	D3DFORMAT _depthStencilFormat;

	//! Multisampling type.
	D3DMULTISAMPLE_TYPE _multisampling;

	//!	Projection matrix.
	D3DXMATRIX _projection;
	//!	Screen aspect ratio.
	float _aspectRatio;
	//!	Field of view.
	float _fieldOfView;
	//!	Near clipping plane.
    float _nearPlane;
	//!	Far clipping plane.
    float _farPlane;
};

#endif