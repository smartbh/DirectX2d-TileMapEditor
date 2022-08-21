#include "framework.h"

ObTileMap::ObTileMap()
{
    SafeRelease(vertexBuffer);
    SafeDeleteArray(vertices);
    for (int i = 0; i < 4; i++)
        SafeDelete(tileImages[i]);

    tileSize.x = 20;
    tileSize.y = 20;

    file = "map1.txt";
    scale = Vector2(50.0f, 50.0f);

    tileImages[0] = new ObImage(L"Tile.png");
    tileImages[0]->maxFrame = Int2(8, 6);
    tileImages[1] = new ObImage(L"Tile2.png");
    tileImages[1]->maxFrame = Int2(11, 7);
    tileImages[2] = new ObImage(L"pepe.png");
    tileImages[3] = nullptr;

    ResizeTile(tileSize);

}

ObTileMap::~ObTileMap()
{
    SafeRelease(vertexBuffer);
    SafeDeleteArray(vertices);

    for (int i = 0; i < 4; i++)
        SafeDelete(tileImages[i]);
}

void ObTileMap::Render()
{
    if (!visible)return;

    GameObject::Render();

    for (int i = 0; i < 4; i++)
    {
        if (tileImages[i])
        {
            D3D->GetDC()->PSSetShaderResources(i, 1, &tileImages[i]->SRV);
            D3D->GetDC()->PSSetSamplers(i, 1, &tileImages[i]->sampler);
        }
    }

    tileMapShader->Set();

    UINT stride = sizeof(VertexTile);
    UINT offset = 0;

    //버텍스버퍼 바인딩
    D3D->GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    D3D->GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    D3D->GetDC()->Draw(tileSize.x * tileSize.y * 6, 0);
}

bool ObTileMap::WorldPosToTileIdx(Vector2 WPos, Int2& TileIdx)
{
    WPos -= GetWorldPos();

    Vector2 tileCoord;

    //타일 맵좌표로 변환 식
    tileCoord.x = WPos.x / scale.x;
    tileCoord.y = WPos.y / scale.y;

    if ((tileCoord.x < 0) or (tileCoord.y < 0) or
        (tileCoord.x >= tileSize.x) or (tileCoord.y >= tileSize.y))
    {
        return false;
    }

    TileIdx.x = tileCoord.x;
    TileIdx.y = tileCoord.y;

    return true;

}

void ObTileMap::SetTile(Int2 TileIdx, Int2 FrameIdx, int ImgIdx, int TileState, Color color)
{
    int tileIdx = tileSize.x * TileIdx.y + TileIdx.x;

    vertices[tileIdx * 6 + 0].uv.x = FrameIdx.x / (float)tileImages[ImgIdx]->maxFrame.x;
    vertices[tileIdx * 6 + 1].uv.x = FrameIdx.x / (float)tileImages[ImgIdx]->maxFrame.x;
    vertices[tileIdx * 6 + 5].uv.x = FrameIdx.x / (float)tileImages[ImgIdx]->maxFrame.x;

    vertices[tileIdx * 6 + 2].uv.x = (FrameIdx.x + 1.0f) / (float)tileImages[ImgIdx]->maxFrame.x;
    vertices[tileIdx * 6 + 3].uv.x = (FrameIdx.x + 1.0f) / (float)tileImages[ImgIdx]->maxFrame.x;
    vertices[tileIdx * 6 + 4].uv.x = (FrameIdx.x + 1.0f) / (float)tileImages[ImgIdx]->maxFrame.x;

    vertices[tileIdx * 6 + 3].uv.y = FrameIdx.y / (float)tileImages[ImgIdx]->maxFrame.y;
    vertices[tileIdx * 6 + 1].uv.y = FrameIdx.y / (float)tileImages[ImgIdx]->maxFrame.y;
    vertices[tileIdx * 6 + 5].uv.y = FrameIdx.y / (float)tileImages[ImgIdx]->maxFrame.y;

    vertices[tileIdx * 6 + 2].uv.y = (FrameIdx.y + 1.0f) / (float)tileImages[ImgIdx]->maxFrame.y;
    vertices[tileIdx * 6 + 0].uv.y = (FrameIdx.y + 1.0f) / (float)tileImages[ImgIdx]->maxFrame.y;
    vertices[tileIdx * 6 + 4].uv.y = (FrameIdx.y + 1.0f) / (float)tileImages[ImgIdx]->maxFrame.y;

    for (int i = 0; i < 6; i++)
    {
        vertices[tileIdx * 6 + i].tileMapIdx = ImgIdx;
        vertices[tileIdx * 6 + i].color = color;
        vertices[tileIdx * 6 + i].tileState = TileState;
    }
    
    //갱신
    D3D->GetDC()->UpdateSubresource(vertexBuffer, 0, NULL, vertices, 0, 0);
}

void ObTileMap::RenderGui(Int2& GuiPickingIdx, int& ImgIdx)
{
    if (ImGui::InputInt("ImgIdx", &ImgIdx))
    {
        if (ImgIdx > 2)
        {
            ImgIdx = 0; //첫번째이미지로 돌리기
        }
        else if (ImgIdx < 0)
        {
            //마지막 이미지로 돌리기
            ImgIdx = 2;
        }
    }

    Int2 MF = tileImages[ImgIdx]->maxFrame;
    ImVec2 size;

    size.x = 300.0f / (float)MF.x;
    size.y = 300.0f / (float)MF.y;

    //텍스쳐 좌표
    ImVec2 LT, RB;
    int index = 0;
    for (UINT i = 0; i < MF.y; i++)
    {
        for (UINT j = 0; j < MF.x; j++)
        {
            if (j != 0)
            {
                //같은줄에 배치
                ImGui::SameLine();
            }
            //텍스쳐 좌표
            LT.x = 1.0f / MF.x * j;
            LT.y = 1.0f / MF.y * i;
            RB.x = 1.0f / MF.x * (j + 1);
            RB.y = 1.0f / MF.y * (i + 1);

            ImGui::PushID(index);
            if (ImGui::ImageButton((void*)tileImages[ImgIdx]->SRV, size, LT, RB))
            {
                GuiPickingIdx.x = j;
                GuiPickingIdx.y = i;
            }
            index++;
            ImGui::PopID();
        }
    }
}

void ObTileMap::ResizeTile(Int2 TileSize)
{
    VertexTile* Vertices = new VertexTile[TileSize.x * TileSize.y * 6];

    //세로
    for (int i = 0; i < TileSize.y; i++)
    {
        //가로
        for (int j = 0; j < TileSize.x; j++)
        {
            int tileIdx = TileSize.x * i + j;

            //0
            Vertices[tileIdx * 6].position.x = 0.0f + j;
            Vertices[tileIdx * 6].position.y = 0.0f + i;
            Vertices[tileIdx * 6].uv = Vector2(0.0f, 1.0f);
            //1                             
            Vertices[tileIdx * 6 + 1].position.x = 0.0f + j;
            Vertices[tileIdx * 6 + 1].position.y = 1.0f + i;
            Vertices[tileIdx * 6 + 1].uv = Vector2(0.0f, 0.0f);
            //2                             
            Vertices[tileIdx * 6 + 2].position.x = 1.0f + j;
            Vertices[tileIdx * 6 + 2].position.y = 0.0f + i;
            Vertices[tileIdx * 6 + 2].uv = Vector2(1.0f, 1.0f);
            //3
            Vertices[tileIdx * 6 + 3].position.x = 1.0f + j;
            Vertices[tileIdx * 6 + 3].position.y = 1.0f + i;
            Vertices[tileIdx * 6 + 3].uv = Vector2(1.0f, 0.0f);
            //4
            Vertices[tileIdx * 6 + 4].position.x = 1.0f + j;
            Vertices[tileIdx * 6 + 4].position.y = 0.0f + i;
            Vertices[tileIdx * 6 + 4].uv = Vector2(1.0f, 1.0f);
            //5
            Vertices[tileIdx * 6 + 5].position.x = 0.0f + j;
            Vertices[tileIdx * 6 + 5].position.y = 1.0f + i;
            Vertices[tileIdx * 6 + 5].uv = Vector2(0.0f, 0.0f);
        }
    }

    //Copy
    if (vertices)
    {
        Int2 Min = Int2(min(TileSize.x, tileSize.x), min(TileSize.y, tileSize.y));
        for (int i = 0; i < Min.y; i++)
        {
            for (int j = 0; j < Min.x; j++)
            {
                int SrcIdx = tileSize.x * i + j;
                int DestIdx = TileSize.x * i + j;
                for (int k = 0; k < 6; k++)
                {
                    Vertices[DestIdx * 6 + k] = vertices[SrcIdx * 6 + k];
                }
            }
        }
    }

    SafeDeleteArray(vertices);
    vertices = Vertices;
    tileSize = TileSize;
    SafeRelease(vertexBuffer);

    //CreateVertexBuffer
    {
        D3D11_BUFFER_DESC desc;
        desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = sizeof(VertexTile) * tileSize.x * tileSize.y * 6;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = vertices;
        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
        Check(hr);
    }
}

void ObTileMap::Save(ObTileMap* _sourceTileMap)
{
    /*
    0 Tile.png 8 6
    1 Tile2.png 11 7
    2 bono3.png 1 1
    3 N
    TileSize 100 100
    TileScale 50 50
    TilePosition -600 -400

    위에서부터
     tileImages index, 파일이름, x,y maxframe <<반복문 써야
     TileSize
     TileScale
     TilePosition 이렇게 해야
    */

    ofstream fout;
    string path = "../Contents/TileMap/" + file;
    //ios::out 쓰기용으로 열겟다
    fout.open(path.c_str(), ios::out);

    //int A = 10;
    //float B = 10.0f;
    //string C = "abc";

    string TileImageName1 = "Tile.png";
    string TileImageName2 = "Tile2.png";
    string TileImageName3 = "pepe.png";

    if (fout.is_open())
    {
        //fout << "A " << A << endl;
        //fout << "B " << B << endl;
        //fout << "C " << C << endl;

        //배열
        for (int i = 0; i < 4; i++)
        {
            if (tileImages[i] != NULL)
            {
                switch (i)
                {
                case 0:
                    fout << i << " " << TileImageName1 << " " << tileImages[i]->maxFrame.x << " " << tileImages[i]->maxFrame.y << endl;
                    break;
                case 1:
                    fout << i << " " << TileImageName2 << " " << tileImages[i]->maxFrame.x << " " << tileImages[i]->maxFrame.y << endl;
                    break;
                case 2:
                    fout << i << " " << TileImageName3 << " " << tileImages[i]->maxFrame.x << " " << tileImages[i]->maxFrame.y << endl;
                    break;
                }
            }
            else
                fout << i << " " << "N" << endl;
        }

        fout << "TileSize " << tileSize.x << " " << tileSize.y << endl;
        fout << "TileScale " << scale.x << " " << scale.y << endl;
        fout << "TilePosition " << _sourceTileMap->GetWorldPos().x << " " << _sourceTileMap->GetWorldPos().y << endl;


        //열엇으면 닫아라
        fout.close();
    }

}

void ObTileMap::Load()
{
    ifstream fin;
    string path = "../Contents/TileMap/" + file;
    fin.open(path.c_str(), ios::in);

    //int A = 10;
    //float B = 10.0f;
    //string C = "abc";
    string temp;

    int TileImageIndex1;
    int TileImageIndex2;
    int TileImageIndex3;
    int TileImageIndex4;

    string TileImageName1;
    int TileImageName1MaxFrameX;
    int TileImageName1MaxFrameY;

    string TileImageName2;
    int TileImageName2MaxFrameX;
    int TileImageName2MaxFrameY;

    string TileImageName3;
    int TileImageName3MaxFrameX;
    int TileImageName3MaxFrameY;

    string TileImageName4;

    int tileSizeX;
    int tileSizeY;

    float scaleX;
    float scaleY;

    float mapPosX;
    float mapPosY;

    if (fin.is_open())
    {
        /*fin >> A;
        fin >> B;
        fin >> C;

        cout << A << endl;*/
        fin >> TileImageIndex1;
        fin >> TileImageName1;
        fin >> TileImageName1MaxFrameX;
        fin >> TileImageName1MaxFrameY;

        cout << TileImageIndex1 << " " << TileImageName1 << " " << TileImageName1MaxFrameX << " " << TileImageName1MaxFrameY << endl;

        fin >> TileImageIndex2;
        fin >> TileImageName2;
        fin >> TileImageName2MaxFrameX;
        fin >> TileImageName2MaxFrameY;

        cout << TileImageIndex2 << " " << TileImageName2 << " " << TileImageName2MaxFrameX << " " << TileImageName2MaxFrameY << endl;

        fin >> TileImageIndex3;
        fin >> TileImageName3;
        fin >> TileImageName3MaxFrameX;
        fin >> TileImageName3MaxFrameY;

        cout << TileImageIndex3 << " " << TileImageName3 << " " << TileImageName3MaxFrameX << " " << TileImageName3MaxFrameY << endl;

        fin >> TileImageIndex4;
        fin >> TileImageName4;
        
        cout << TileImageIndex4 << " " << TileImageName4 << endl;

        fin >> temp;
        cout << temp << " ";
        fin >> tileSizeX;
        fin >> tileSizeY;
        cout << tileSizeX << " " << tileSizeY << endl;

        fin >> temp;
        cout << temp << " ";
        fin >> scaleX;
        fin >> scaleY;
        cout << scaleX << " " << scaleY << endl;

        fin >> temp;
        cout << temp << " ";
        fin >> mapPosX;
        fin >> mapPosY;
        cout << mapPosX << " " << mapPosY << endl;

        fin.close();
    }
}


