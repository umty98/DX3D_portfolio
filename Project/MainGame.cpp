#include "Framework.h"
#include "MainGame.h"

MainGame::MainGame()
{
	srand(time(NULL));
	//Add(string key, wstring font, Vector3 color = Vector3(1, 1, 1), float size = 20.0f

	FONT     ->Add("D2Coding", L"D2Coding", { 1, 1, 1 }, 40.0f);
	FONTBLINK->Add("D2Coding", L"D2Coding", { 1, 1, 1 }, 40.0f);

	Initialize();

	//scene = new TutorialScene();
	//scene = new TextureScene();
	//scene = new TerrainScene();
	//scene = new TerrainEditorScene();

	//scene = new ModelExportScene();
	//scene = new ModelAnimationScene();
	//scene = new CollisionScene();

	//SCENE->Create("Grid", new GridScene());


	//SCENE->Create("Collision", new CollisionScene());
	//SCENE->Create("Start", new ModelAnimationScene());
	//SCENE->Create("Start", new InstancingScene());
	//SCENE->Create("Start", new ModelInstancingScene());
//	SCENE->Create("Export", new ModelExportScene());
//	SCENE->Create("Start", new LightScene());
//	//SCENE->Create("Start", new DeferredRenderScene());
	//SCENE->Create("Start", new FrustumCullingScene());
//	SCENE->Create("Start", new TerrainEditorScene());
	//SCENE->Create("Start", new BillboardScene());
//	SCENE->Create("Start", new ParticleScene());
//	SCENE->Create("Start", new ParticleEditorScene());
//	SCENE->Create("Start", new WaterScene());
//	SCENE->Create("Start", new CollisionScene());


	SCENE->Create("Start", new ShootingScene());

	//	SCENE->Create("Start", new SampleMap());


//	SCENE->Add("Grid");
//	SCENE->Add("Start");
}

MainGame::~MainGame()
{
	SceneManager::Delete();

	Release();
}

void MainGame::Update()
{
	SCENE->Update();

	Time::GetInstance()->Update();
	Keyboard::GetInstance()->Update();

	if (KEY_DOWN(VK_TAB))
		Collider::HideCollider();
}

void MainGame::Render()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (isWireFrame)
	{
		RS->ChangeState(D3D11_FILL_WIREFRAME);
	}
	else
	{
		RS->ChangeState(D3D11_FILL_SOLID);
	}

	SCENE->PreRender();

	Device::GetInstance()->Clear();

	Environment::GetInstance()->SetViewport();

	Environment::GetInstance()->SetEnvironment();

	SCENE->Render();
	Time::GetInstance()->Render();

	Environment::GetInstance()->PostSet();

	FONT->GetDC()->BeginDraw();
	FONTBLINK->GetDC()->BeginDraw();

	SCENE->PostRender();
	Environment::GetInstance()->PostRender();
	
	FONT->GetDC()->EndDraw();
	FONTBLINK->GetDC()->EndDraw();

	ImGui::Checkbox("WireFrame", &isWireFrame);

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	Device::GetInstance()->Present();
}

void MainGame::Initialize()
{
	//Device::GetInstance();
	StateManager::GetInstance();
	Environment::GetInstance();
	Keyboard::GetInstance();
	Time::GetInstance();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(DEVICE, DC);
}

void MainGame::Release()
{
	Time::Delete();
	Device::Delete();
	Shader::Delete();
	Keyboard::Delete();
	Environment::Delete();
	StateManager::Delete();
	Texture::Delete();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
