#include "Framework.h"
#include "ProgressBar.h"

ProgressBar::ProgressBar(wstring frontImageFile, wstring backImageFile)
	:Quad(frontImageFile)
{
	backImage = Texture::Get(backImageFile);

	material->SetShader(L"04ProgressBar");

	buffer = new FloatValueBuffer();
	buffer->data.values[0] = 1.0f;
}

ProgressBar::~ProgressBar()
{
	delete buffer;
}

void ProgressBar::Render()
{
	buffer->SetPSBuffer(10);
	backImage->PSSetShaderResources(10);

	Quad::Render();
}

void ProgressBar::SetValue(float value)
{
	buffer->data.values[0] = value;
}

