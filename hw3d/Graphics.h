#pragma once
#include "WinSus.h"
#include "CustomExceptions.h"
#include <d3d11.h>
#include <d3d11sdklayers.h>

class Graphics {
public:
	class Exception : public CustomException {
		using CustomException::CustomException;
	};
	class HrException : public Exception{
	public:
		HrException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;

	};
	class DeviceRemovedException : public HrException {
		using HrException::HrException ;
	public:
		const char* GetType() const noexcept override;
	};
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	void EndFrame();
	void ClearBuffer(float r, float g, float b);
private:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwap = nullptr;
	ID3D11DeviceContext* pContext = nullptr; 
	ID3D11RenderTargetView* pTarget = nullptr;
};