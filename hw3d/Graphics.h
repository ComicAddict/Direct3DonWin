#pragma once
#include "WinSus.h"
#include "CustomExceptions.h"
#include <d3d11.h>
#include <wrl.h>
#include <d3d11sdklayers.h>
#include <vector>
#include <memory>
#include <random>
#include <d3dcompiler.h>


class Graphics {
	friend class Bindable;
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
	class CExcept : public Exception {
	public:
		CExcept(int line, const char* file, std::string s) noexcept;
		const char* what() const noexcept override;
	private:
		const char* str;

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
	~Graphics() = default;
	void EndFrame();
	void ClearBuffer(float r, float g, float b);
	void DrawTestTriangle(float angle, float x, float y);
private:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
};