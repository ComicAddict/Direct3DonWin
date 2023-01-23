#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices) : count((UINT)indices.size()) {
	D3D11_BUFFER_DESC d{};
	d.BindFlags = D3D11_BIND_INDEX_BUFFER;
	d.Usage = D3D11_USAGE_DEFAULT;
	d.CPUAccessFlags = 0u;
	d.MiscFlags = 0u;
	d.ByteWidth = UINT(count * sizeof(unsigned short));
	d.StructureByteStride = sizeof(unsigned short);
	

	D3D11_SUBRESOURCE_DATA s{};
	s.pSysMem = indices.data();

	GetDevice(gfx)->CreateBuffer(&d, &s, &pIndexBuffer);
}

void IndexBuffer::Bind(Graphics& gfx) noexcept {
	GetContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::GetCount() const noexcept {
	return count;
}