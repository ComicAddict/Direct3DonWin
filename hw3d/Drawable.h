#pragma once
#include "Graphics.h"
#include <DirectXMath.h>

class Bindable;

class Drawable {

public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual DirectX::XMMATRIX GetTransform() const noexcept = 0;
	void Draw()
};
