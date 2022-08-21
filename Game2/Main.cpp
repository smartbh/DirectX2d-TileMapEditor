#include "stdafx.h"
#include "Main.h"

void Main::Init()
{
	bg = new ObImage(L"bg.png");
	bg->scale = Vector2(224.0f, 2144.0f) * 2.0f;
	bg->space = SPACE::SCREEN;

	bg->pivot = OFFSET_B;
	bg->SetWorldPosY(-app.GetHalfHeight());

	pl = new ST_Player();
	en = new Enemy();

	//LIGHT->light.radius = 100.0f;
}

void Main::Release()
{
	SafeDelete(bg);
	SafeDelete(pl);
}

void Main::Update()
{
	/*ImGui::Text("FPS : %d", TIMER->GetFramePerSecond());

	if (ImGui::Button("vSync"))
	{
		cout << app.vSync << endl;
		app.vSync = !app.vSync;
	}*/

	LIGHT->SetLightPos(pl->col->GetWorldPivot());

	ImGui::SliderFloat("Select", &LIGHT->light.select, 0.0f, 1.0f);
	ImGui::SliderFloat2("LightPos", (float*)&LIGHT->light.screenPos, 0.0f, 2000.0f);
	ImGui::SliderFloat("Radius", &LIGHT->light.radius, 0.0f, 2000.0f);
	ImGui::ColorEdit3("LightColor", (float*)&LIGHT->light.lightColor);
	ImGui::ColorEdit3("OutColor", (float*)&LIGHT->light.outColor);

	bg->uv.y -= DELTA / bg->scale.y * 100.0f;
	bg->uv.w -= DELTA / bg->scale.y * 100.0f;

	bg->Update();
	pl->Update();
	en->Update();
}

void Main::LateUpdate()
{
	for (int i = 0; i < MAX; i++)
	{
		if (!pl->bullet[i].isfire) continue;

		if (pl->bullet[i].col->Intersect(en->col))
		{
			pl->bullet[i].isfire = false;
			pl->bullet[i].col->visible = false;

			en->TakeDamage(pl->bullet[i].damage);
		}
	}
}

void Main::Render()
{
	bg->Render();
	pl->Render();
	en->Render();
}

void Main::ResizeScreen()
{
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR param, int command)
{
	app.SetAppName(L"Game2");
	app.SetInstance(instance);
	app.InitWidthHeight(1600.0f, 800.0f);
	Main* main = new Main();
	int wParam = (int)WIN->Run(main);
	WIN->DeleteSingleton();
	SafeDelete(main);
	return wParam;
}