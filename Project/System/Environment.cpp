#include "Framework.h"
#include "Environment.h"

Environment::Environment()
{
    SetViewport();
    CreatePerspective();
    CreateOrthographic();

    lightBuffer = new LightBuffer();

    mainCamera = new Camera();
    cameraFP   = new CameraFP();
    cameraTP   = new CameraTP();
}

Environment::~Environment()
{
    delete   persBuffer;
    delete  lightBuffer;

    delete mainCamera;
    delete cameraFP;
    delete cameraTP;
}

void Environment::SetViewport(UINT width, UINT height)
{

    viewPort.TopLeftX = 0.0f;
    viewPort.TopLeftY = 0.0f;
    viewPort.Width    = width;
    viewPort.Height   = height;
    viewPort.MinDepth = 0.0f;
    viewPort.MaxDepth = 1.0f;

    DC->RSSetViewports(1, &viewPort);
}

void Environment::CreatePerspective()
{
    persBuffer = new MatrixBuffer();

    persMatrix = XMMatrixPerspectiveFovLH
    (
        XM_PIDIV4,
        WIN_WIDTH / WIN_HEIGHT,
        0.1f,
        1000.0f
    );

    persBuffer->SetData(persMatrix);
}

void Environment::CreateOrthographic()
{
    orthoBuffer = new MatrixBuffer();

    orthoMatrix = XMMatrixOrthographicOffCenterLH(0.0f, WIN_WIDTH, 0.0f, WIN_HEIGHT, -1.0f, 1.0f);

    orthoBuffer->SetData(orthoMatrix);

    UIViewBuffer = new ViewBuffer();
}

void Environment::DebugLight(int lightIndex)
{
    string label = "Light_" + to_string(lightIndex);

    LightBuffer::LightData& light = lightBuffer->data.lights[lightIndex];

    if (ImGui::TreeNode(label.c_str()))
    {
        ImGui::Checkbox("Active", (bool*)&light.active);

        const char* list[] = { "Directional", "Point", "Spot", "Capsule" };
        ImGui::Combo("Type", &light.type, list, 4);

        ImGui::ColorEdit3("Color", (float*)&light.color);
        ImGui::SliderFloat3("Direction", (float*)&light.direction, -1, 1);

        ImGui::DragFloat3("Position", (float*)&light.position);
        
        ImGui::SliderFloat("Range", &light.range, 1, 1000);

        ImGui::SliderFloat("Inner", &light.inner, 0, light.outer);
        ImGui::SliderFloat("Outer", &light.outer, light.inner, 180.0f);

        ImGui::SliderFloat("Length", &light.length, 0, 500.0f);

        ImGui::TreePop();
    }
}

void Environment::SetEnvironment()
{
    if (!isGameMode)
        mainCamera->Update();
    else
    {
        if (!isFPmode)
            cameraTP->Update();

        else
            cameraFP->Update();
    }

    if (GetAsyncKeyState('P') & 0x8000) {
        if (!pKeyPressed)
        {
            isGameMode = !isGameMode;
            pKeyPressed = true;

            if (!isGameMode)
            {
                if (!isFPmode)
                {
                    mainCamera->translation = cameraTP->translation;
                    mainCamera->rotation    = -cameraTP->rotation;            
                  //  mainCamera->rotation.x = -cameraTP->rotation.x;
                }
                else
                {
                    mainCamera->translation = cameraFP->translation;
                    mainCamera->rotation    = -cameraFP->rotation;    
                   // mainCamera->translation.x = -cameraFP->rotation.x;
                }

            }



        }
    }
    else
        pKeyPressed = false;


    if (GetAsyncKeyState('V') & 0x8000) {
        if (!vKeyPressed) {
            isFPmode = !isFPmode;
            vKeyPressed = true;

            if (!isFPmode)
                cameraTP->rotation = cameraFP->rotation;

            else
                cameraFP->rotation = cameraTP->rotation;
        }
    }
    else {
        vKeyPressed = false;
    }


    // mainCamera->Update();
    // cameraFP->Update();

    lightBuffer->SetPSBuffer(0);
    persBuffer->SetVSBuffer(2);

    StateManager::GetInstance()->Set();
}

void Environment::PostSet()
{
    UIViewBuffer->SetVSBuffer(1);
     orthoBuffer->SetVSBuffer(2);

    StateManager::GetInstance()->PostSet();
}

void Environment::PostRender()
{
    if (ImGui::TreeNode("LightOption"))
    {
        if (ImGui::Button("Add"))
            lightBuffer->data.lightCount++;

        for (UINT i = 0; i < lightBuffer->data.lightCount; i++)
        {
            DebugLight(i);
        }

        ImGui::ColorEdit3("AmbientLight", (float*)&lightBuffer->data.ambientLight);
        ImGui::ColorEdit3("AmbientCeil ", (float*)&lightBuffer->data.ambientCeil);

        ImGui::TreePop();
    }

    mainCamera->Debug();
    cameraTP->Debug();
}
