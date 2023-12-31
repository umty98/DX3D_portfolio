#include "Framework.h"
#include "FontBlink.h"

FontBlink::FontBlink()
{
	D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &factory);

	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&writeFactory);

	IDXGIDevice* dxgiDevice;
	DEVICE->QueryInterface(&dxgiDevice);

	factory->CreateDevice(dxgiDevice, &device);

	device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &deviceContext);

	IDXGISurface* dxgiSurface;
	Device::GetInstance()->GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), (void**)&dxgiSurface);

	D2D1_BITMAP_PROPERTIES1 bp;
	bp.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	bp.dpiX = 96;
	bp.dpiY = 96;
	bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bp.colorContext = nullptr;

	deviceContext->CreateBitmapFromDxgiSurface(dxgiSurface, &bp, &bitmap);
	deviceContext->SetTarget(bitmap);
}

FontBlink::~FontBlink()
{

	factory->Release();
	writeFactory->Release();

	bitmap->Release();
	device->Release();
	deviceContext->Release();

	for (pair<string, FontDesc*> pair : fonts)
		delete pair.second;
}

void FontBlink::Add(string key, wstring font, Vector3 color, float size, DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch)
{
	if (fonts.count(key) > 0)
		return;

	FontDesc* desc = new FontDesc;

	D2D1::ColorF colorF(color.x, color.y, color.z);
	deviceContext->CreateSolidColorBrush(colorF, &desc->brush);

	writeFactory->CreateTextFormat(font.c_str(), nullptr, weight, style, stretch, size, L"ko", &desc->format);

	desc->format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	desc->format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);

	fonts.emplace(key, desc);
}

void FontBlink::RenderText(wstring text, string key, Vector2 pos, Vector2 size)
{
	if (fonts.count(key) == 0)
		return;

	UpdateOpacity();

	fonts[key]->brush->SetOpacity(opacity);

	if (size.x == 0 && size.y == 0)
	{
		size.x = text.size() * fonts[key]->format->GetFontSize();
		size.y = fonts[key]->format->GetFontSize();
	}

	D2D1_RECT_F rect;
	rect.left = pos.x - size.x * 0.5f;
	rect.right = pos.x + size.x * 0.5f;
	rect.top = pos.y - size.y * 0.5f;
	rect.bottom = pos.y + size.y * 0.5f;

	deviceContext->DrawTextW(text.c_str(), text.size(), fonts[key]->format, &rect, fonts[key]->brush);
}

void FontBlink::UpdateOpacity()
{
	opacity += opacityChange;
	if (opacity <= 0.0f || opacity >= 1.0f)
	{
		opacityChange = -opacityChange; // 불투명도가 0이나 1에 도달하면 방향 전환
	}
}
