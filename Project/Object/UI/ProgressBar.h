#pragma once

class ProgressBar : public Quad
{
public:
	ProgressBar(wstring frontImageFile, wstring backImageFile);
	~ProgressBar();

	void Render();

	void SetValue(float value);

private:
	Texture* backImage;

	FloatValueBuffer* buffer;
};
