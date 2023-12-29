#pragma once

class FontBlink : public Singleton<FontBlink>
{
	friend class Singleton;
private:
	struct FontDesc
	{
		ID2D1SolidColorBrush* brush = nullptr;
		IDWriteTextFormat* format = nullptr;

		~FontDesc()
		{
			brush->Release();
			format->Release();
		}


	};

public:
	FontBlink();
	~FontBlink();


	void Add(string key, wstring font, Vector3 color = Vector3(1, 1, 1), float size = 20.0f,
		DWRITE_FONT_WEIGHT  weight = DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE   style = DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL
	);

	void RenderText(wstring text, string key, Vector2 pos, Vector2 size = Vector2());

	ID2D1DeviceContext* GetDC() { return deviceContext; }

	IDWriteTextFormat* GetFormat(string key) { return fonts[key]->format; }

	void UpdateOpacity();

private:
	ID2D1Factory1* factory;
	IDWriteFactory* writeFactory;

	ID2D1Device* device;
	ID2D1DeviceContext* deviceContext;

	ID2D1Bitmap1* bitmap;

	map<string, FontDesc*> fonts;
	float opacity = 1.0f;
	float opacityChange = -0.0005f;

	bool isOpcaity;
};

