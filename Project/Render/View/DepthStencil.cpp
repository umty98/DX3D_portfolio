#include "Framework.h"
#include "DepthStencil.h"

DepthStencil::DepthStencil(UINT width, UINT height, bool isStencil)
	:width(width), height(height), isStencil(isStencil)
{
	CreateDSVTexture();
	CreateDSV();
	CreateSRV();
}

DepthStencil::~DepthStencil()
{
	dsvTexture->Release();
	       dsv->Release();
	       srv->Release();
}

void DepthStencil::Clear()
{
	DC->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DepthStencil::CreateDSVTexture()
{
    D3D11_TEXTURE2D_DESC depthDesc;

    depthDesc.Width              = width;
    depthDesc.Height             = height;
    depthDesc.MipLevels          = 1;
    depthDesc.ArraySize          = 1;
    depthDesc.Format             = isStencil ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_R32_TYPELESS;
    depthDesc.SampleDesc.Count   = 1;
    depthDesc.SampleDesc.Quality = 0;
    depthDesc.Usage              = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    depthDesc.CPUAccessFlags     = 0;
    depthDesc.MiscFlags          = 0;

    DEVICE->CreateTexture2D(&depthDesc, nullptr, &dsvTexture);
}

void DepthStencil::CreateDSV()
{
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

    dsvDesc.Format        = isStencil ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    DEVICE->CreateDepthStencilView(dsvTexture, &dsvDesc, &dsv);
}

void DepthStencil::CreateSRV()
{
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

    srvDesc.Format                   = isStencil ? DXGI_FORMAT_R24_UNORM_X8_TYPELESS : DXGI_FORMAT_R32_FLOAT;
    srvDesc.ViewDimension            = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2DArray.MipLevels = 1;

    DEVICE->CreateShaderResourceView(dsvTexture, &srvDesc, &srv);
}
