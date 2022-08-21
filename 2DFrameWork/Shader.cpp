#include "framework.h"

Shader::Shader(wstring file)
{
    ID3D10Blob* VsBlob;
    ID3D10Blob* PsBlob;

    wstring path = L"../Shaders/" + file + L".hlsl";

    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

    D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "VS", "vs_5_0", flags, 0, &VsBlob, nullptr);

    D3D->GetDevice()->CreateVertexShader(VsBlob->GetBufferPointer(), VsBlob->GetBufferSize(),
        nullptr, &vertexShader);

    D3D11_INPUT_ELEMENT_DESC LayoutDesc[2];

    LayoutDesc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
    if (file == L"1.Basic")
    {
        LayoutDesc[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,D3D11_INPUT_PER_VERTEX_DATA, 0 };
        D3D->GetDevice()->CreateInputLayout(LayoutDesc, 2, VsBlob->GetBufferPointer(),
            VsBlob->GetBufferSize(), &vertexLayout);
    }
    else if (file == L"2.Image")
    {
        LayoutDesc[1] = { "UV", 0,DXGI_FORMAT_R32G32_FLOAT, 0, 12,D3D11_INPUT_PER_VERTEX_DATA, 0 };
        D3D->GetDevice()->CreateInputLayout(LayoutDesc, 2, VsBlob->GetBufferPointer(),
            VsBlob->GetBufferSize(), &vertexLayout);
    }
    else if (file == L"3.TileMap")
    {
        D3D11_INPUT_ELEMENT_DESC LayoutDesc[5];
        LayoutDesc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
        LayoutDesc[1] = { "UV", 0,DXGI_FORMAT_R32G32_FLOAT, 0, 12,D3D11_INPUT_PER_VERTEX_DATA, 0 };
        LayoutDesc[2] = { "COLOR", 0,DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20,D3D11_INPUT_PER_VERTEX_DATA, 0 };
        LayoutDesc[3] = { "INDICES", 0,DXGI_FORMAT_R32_FLOAT, 0, 36,D3D11_INPUT_PER_VERTEX_DATA, 0 };
        LayoutDesc[4] = { "STATE", 0,DXGI_FORMAT_R32_FLOAT, 0, 40,D3D11_INPUT_PER_VERTEX_DATA, 0 };
        D3D->GetDevice()->CreateInputLayout(LayoutDesc, 5, VsBlob->GetBufferPointer(),
            VsBlob->GetBufferSize(), &vertexLayout);
    }

    VsBlob->Release();

    D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "PS", "ps_5_0", flags, 0, &PsBlob, nullptr);

    D3D->GetDevice()->CreatePixelShader(PsBlob->GetBufferPointer(), PsBlob->GetBufferSize(),
        nullptr, &pixelShader);

    PsBlob->Release();
}

Shader::~Shader()
{
    vertexShader->Release();
    pixelShader->Release();
    vertexLayout->Release();
}

void Shader::Set()
{
    //파이프라인에 바인딩
    D3D->GetDC()->VSSetShader(vertexShader, 0, 0);
    D3D->GetDC()->PSSetShader(pixelShader, 0, 0);
    D3D->GetDC()->IASetInputLayout(vertexLayout);
}
