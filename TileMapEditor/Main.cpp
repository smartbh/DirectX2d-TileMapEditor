#include "stdafx.h"
#include "Main.h"

void Main::Init()
{
	map = new ObTileMap();
	map->SetWorldPos(Vector2(-app.GetHalfWidth(), -app.GetHalfHeight()));

	map->SetTile(Int2(0, 0), Int2(2, 2), 1);
	map->SetTile(Int2(1, 1), Int2(0, 0), 2);

	map->Save(map);

	imgIdx = 0;
}

void Main::Release()
{
}

void Main::Update()
{
	ImGui::Text("FPS : %d", TIMER->GetFramePerSecond());
	ImGui::SliderFloat2("Scale", (float*)&map->scale, 0.0f, 100.0f);

	map->RenderGui(pickingIdx, imgIdx);
	ImGui::Text("pickingIdx : %d , %d", pickingIdx.x, pickingIdx.y);
	ImGui::Text("imgIdx : %d", imgIdx);

	ImVec2 min = ImGui::GetWindowPos();
	ImVec2 max;
	max.x = min.x + ImGui::GetWindowSize().x;
	max.y = min.y + ImGui::GetWindowSize().y;

	if (!ImGui::IsMouseHoveringRect(min, max))
	{
		if (INPUT->KeyPress(VK_LBUTTON))
		{
			if (map->WorldPosToTileIdx(INPUT->GetMouseWorldPos(), mouseIdx))
			{
				map->SetTile(mouseIdx, pickingIdx, imgIdx);
			}
		}
	}
	
	ImGui::Text("mouseIdx : %d , %d", mouseIdx.x, mouseIdx.y);

	if (ImGui::Button("save"))
	{
		map->Save(map);
	}
	if (ImGui::Button("load"))
	{
		map->Load();
	}

	if (INPUT->KeyPress(VK_LEFT))
	{
		CAM->position.x -= 200.0f * DELTA;
	}
	if (INPUT->KeyPress(VK_RIGHT))
	{
		CAM->position.x += 200.0f * DELTA;
	}
	if (INPUT->KeyPress(VK_UP))
	{
		CAM->position.y += 200.0f * DELTA;
	}
	if (INPUT->KeyPress(VK_DOWN))
	{
		CAM->position.y -= 200.0f * DELTA;
	}

	map->Update();
}

void Main::LateUpdate()
{

}

void Main::Render()
{	//			L    T    R    B
	//DWRITE->RenderText(L"hi", RECT{100, 100, 100, 100}, 300.0f);
	DWRITE->RenderText(L"¾È³ç\n¾È³ç", RECT{ 100, 100, (long)app.GetWidth(), (long)app.GetHeight() }, 100.0f,
		L"±Ã¼­", Color(1.0f, 0.0f, 0.0f, 1.0f), DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_ITALIC);

	map->Render();
}

void Main::ResizeScreen()
{

}


int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR param, int command)
{
	app.SetAppName(L"TileMapEditor");
	app.SetInstance(instance);
	app.InitWidthHeight(1400, 800.0f);
	Main* main = new Main();
	int wParam = (int)WIN->Run(main);
	WIN->DeleteSingleton();
	SafeDelete(main);
	return wParam;
}