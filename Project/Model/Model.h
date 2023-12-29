#pragma once

class Model : public Transform
{
public:
	Model(string name, wstring shaderFile = L"09Light");
	~Model();

	void Update();
	void Render();
	void RenderInstanced(UINT instanceCount);

	ModelReader* GetReader() { return reader; }

protected:
	ModelReader* reader;
};
