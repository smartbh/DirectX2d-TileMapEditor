#include "stdafx.h"

void Scene01::Init()
{
	pl = new Player();
	mon = new Monster();
	map = new ObTileMap();
	map->SetWorldPos(Vector2(-app.GetHalfWidth(), -app.GetHalfHeight()));

	map->SetTile(Int2(0, 0), Int2(2 ,2), 1);
	map->SetTile(Int2(1, 1), Int2(0 ,0), 2);
}

void Scene01::Release()
{
}

void Scene01::Update()
{
	ImGui::Text("FPS : %d", TIMER->GetFramePerSecond());
	ImGui::SliderFloat2("Scale", (float*)&map->scale, 0.0f, 100.0f);

	pl->Update();
	mon->SetTarget(pl->GetPos());
	mon->Update();
	map->Update();

	CAM->position = pl->GetPos();
}

void Scene01::LateUpdate()
{
	
}

void Scene01::Render()
{
	map->Render();
	pl->Render();
	mon->Render();

	//								L    T    R    B
	//DWRITE->RenderText(L"hi", RECT{100, 100, 100, 100}, 300.0f);
	DWRITE->RenderText(L"¾È³ç\n¾È³ç", RECT{ 100, 100, (long)app.GetWidth(), (long)app.GetHeight() }, 100.0f,
		L"±Ã¼­", Color(1.0f, 0.0f, 0.0f, 1.0f), DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_ITALIC);
}

void Scene01::ResizeScreen()
{

}
