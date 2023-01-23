#pragma once
#include "Bindable.h"

template<typename C>
class ConstantBuffer : public Bindable {
public:
	void Update(Graphics& gfx, const C& c) {
		D3D11_MAPPED_SUBRESOURCE msr;
		GetContext(gfx)->Map(pConstantBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
		memcpy(msr.pData, &c, sizeof(c));
		GetContext(gfx)->Unmap(pConstantBuffer.Get(), 0u);
	}

	ConstantBuffer(Graphics& gfx, const C& c) {
		D3D11_BUFFER_DESC d{};
		d.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		d.Usage = D3D11_USAGE_DYNAMIC;
		d.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		d.MiscFlags = 0u;
		d.ByteWidth = sizeof(c);
		d.StructureByteStride = 0u;
		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = &c;
		GetDevice(gfx)->CreateBuffer(&d, &sd, &pConstantBuffer);

		pContext->VSSetConstantBuffers(0u, 1u, pCB.GetAddressOf());
	}

	ConstantBuffer(Graphics& gfx) {
		D3D11_BUFFER_DESC d{};
		d.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		d.Usage = D3D11_USAGE_DYNAMIC;
		d.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		d.MiscFlags = 0u;
		d.ByteWidth = sizeof(C);
		d.StructureByteStride = 0u;
		GetDevice(gfx)->CreateBuffer(&d, nullptr, &pConstantBuffer);
	}
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C> {
	using ConstantBuffer<C>::pConstantBuffer;
	using Bindable::GetContext();
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& gfx) noexcept override {
		GetContext(gfx)->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	}
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>{
	using ConstantBuffer<C>::pConstantBuffer;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& gfx) noexcept override
	{
		GetContext(gfx)->PSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	}
};