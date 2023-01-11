#include "Graphics.h"
#include <ostream>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace wrl = Microsoft::WRL;

#define GFX_THROW_FAILED(hrcall) if(FAILED(hr = (hrcall))) throw Graphics::HrExceptions(__LINE__,__FILE__, hr);
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__,__FILE__,(hr))
Graphics::Graphics(HWND hWnd) {

	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	UINT creationFlags = 0u;
#if defined(_DEBUG)
	creationFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif
	HRESULT hr;

	hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		 &sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	);

	if (FAILED(hr))
		printf("there is an error in create device and swap chain");
	
	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	hr = pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	if (FAILED(hr))
		printf("there is an error in getting buffer");
	hr = pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pTarget
	);
	if (FAILED(hr))
		printf("there is an error in create render targetview");
}

void Graphics::DrawTestTriangle() {
	
	HRESULT hr;
	struct Vertex {
		float x;
		float y;
	};

	const Vertex vertices[] = {
		{0.0f, 0.5f},
		{0.5f, -0.5f},
		{-0.5f, -0.5f},
	};

	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	hr = pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);
	if (FAILED(hr))
		MessageBox(nullptr, "create buffer", "Unk Exception", MB_OK | MB_ICONEXCLAMATION);

	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	wrl::ComPtr<ID3DBlob> pBlob;

	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	hr = D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	if (FAILED(hr))
		MessageBox(nullptr, "pix shader read", "Unk Exception", MB_OK | MB_ICONEXCLAMATION);
	hr = pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	if (FAILED(hr))
		MessageBox(nullptr, "pix shader create", "Unk Exception", MB_OK | MB_ICONEXCLAMATION);
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	hr = D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	if (FAILED(hr))
		MessageBox(nullptr, "vert shader read", "Unk Exception", MB_OK | MB_ICONEXCLAMATION);
	hr = pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
	if (FAILED(hr))
		MessageBox(nullptr, "create vert shader", "Unk Exception", MB_OK | MB_ICONEXCLAMATION);
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	hr = pDevice->CreateInputLayout(ied, (UINT)std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout); 
	if (FAILED(hr))
		MessageBox(nullptr, "create input layout", "Unk Exception", MB_OK | MB_ICONEXCLAMATION);
	pContext->IASetInputLayout(pInputLayout.Get());

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp );

	pContext->Draw((UINT)std::size(vertices), 0u);
}

void Graphics::EndFrame() {
	HRESULT hr;
	if (FAILED(hr = pSwap->Present(1u, 0u)))
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
			throw pDevice->GetDeviceRemovedReason();
		else
			hr;
}

void Graphics::ClearBuffer(float r, float g, float b) {
	const float color[] = { r,g,b,1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
}

const char* Graphics::CExcept::what() const noexcept {
	return str;
}

Graphics::CExcept::CExcept(int line, const char* file, std::string s) noexcept 
	: 
	Exception(line, file) {
	std::ostringstream oss;
	oss << "line: " << line << ", file: " << file << "err: " << s;
	str = oss.str().c_str();
}