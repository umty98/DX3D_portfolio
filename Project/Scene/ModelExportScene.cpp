#include "Framework.h"
#include "ModelExportScene.h"

ModelExportScene::ModelExportScene()
{
	string name = "Swat";

	exporter = new ModelExporter(name);
	exporter->ExportModel();
	exporter->ExportClip("grabobject");
//	exporter->ExportClip("RifleSideStepRight");
	//exporter->ExportClip("Mutant Swiping");
	//exporter->ExportClip("Rifle Aiming Idle");//IDLERIFLE
	//exporter->ExportClip("Run Forward"); //FORWADRIFLE
	//exporter->ExportClip("Backwards Rifle Walk");//BACKRIFLE
	//exporter->ExportClip("Firing Rifle");//ATTACKRIFLE
	//exporter->ExportClip("Reload");//RELOADRIFLE
	//exporter->ExportClip("Pistol IdleStay");//IDLEPISTOL
	//exporter->ExportClip("Pistol Walk");//FORWARDPISTOL
	//exporter->ExportClip("Pistol Walk Backward");//BACKPISTOL
	//exporter->ExportClip("Shooting");//ATTACKPISTOL

	
	model = new Model(name);
	//model->scale *= 7.0f;
	//model->SetPivot({ -0.31f, 0.43f, 0.17f });
	//model->scale *= 100.0f;
}

ModelExportScene::~ModelExportScene()
{
	delete exporter;
	delete model;
}

void ModelExportScene::Update()
{
	model->Update();
}

void ModelExportScene::PreRender()
{
}

void ModelExportScene::Render()
{
	model->Render();
}

void ModelExportScene::PostRender()
{
	model->GetReader()->Debug();
}
