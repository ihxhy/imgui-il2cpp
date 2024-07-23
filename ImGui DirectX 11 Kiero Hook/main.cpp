#include "Headers/vars.h"

inline Unity::CGameObject* LocalPlayer = NULL;
inline std::vector<Unity::CGameObject*> PlayerList(NULL);

inline ImColor PlayerSnaplineColor = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255);
static DWORD lastShotTime = 0;
static DWORD lasttick = 0;

bool WorldToScreen(Unity::Vector3 world, Vector2& screen)
{
	Unity::CCamera* Cameraobj = (Unity::CCamera*)Unity::GameObject::Find("FirstPersonCamera");

	if (!Cameraobj) {
		return false;
	}

	Unity::CCamera* CameraMain = (Unity::CCamera*)Cameraobj->GetComponent("Camera");

	if (!CameraMain)
	{
		return false;
	}
	 
	Unity::Vector3 buffer = CameraMain->CallMethodSafe<Unity::Vector3>("WorldToScreenPoint", world, Unity::eye::mono); 

	if (buffer.x > vars::screen_size.x || buffer.y > vars::screen_size.y || buffer.x < 0 || buffer.y < 0 || buffer.z < 0) 
	{
		return false;
	}

	if (buffer.z > 0.0f) 
	{
		screen = Vector2(buffer.x, vars::screen_size.y - buffer.y);
	}

	if (screen.x > 0 || screen.y > 0)
	{
		return true;
	}
}

void initvars() {
	if (IL2CPP::Initialize(true)) {
		printf("[ DEBUG ] Il2Cpp initialized\n");
	}
	else {
		printf("[ DEBUG ] Il2Cpp initialize failed, quitting...");
		Sleep(300);
		exit(0);
	}
	uintptr_t Base = (uintptr_t)GetModuleHandleA(NULL);
	uintptr_t GameAssembly = (uintptr_t)GetModuleHandleA("GameAssembly.dll");
	uintptr_t UnityPlayer = (uintptr_t)GetModuleHandleA("UnityPlayer.dll");

}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

FILE* f;

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;																													  
ID3D11RenderTargetView* mainRenderTargetView;																											  
																																						  
HANDLE mainthread{};																																	  																										  
																																						  
void InitImGui()																																		  
{																																						  
	ImGui::CreateContext();																																  
	ImGuiIO& io = ImGui::GetIO();																														  
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;																								  
	ImGui_ImplWin32_Init(window);																														  
	ImGui_ImplDX11_Init(pDevice, pContext);																												  
}																																						  
																																						  
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {																	  
																																						  
	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))																				  
		return true;																																	  
																																						  
	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);																						  
}																																						  



void Player_Loop()
{
	while (true)
	{
		void* m_pThisThread = IL2CPP::Thread::Attach(IL2CPP::Domain::Get());
		if (!m_pThisThread)
			continue;
		LocalPlayer = NULL;
		PlayerList.clear();
		auto list = Unity::Object::FindObjectsOfType<Unity::CComponent>("UnityEngine.Rigidbody");

		for (int i = 0; i < list->m_uMaxLength; i++)
		{
			if (!list->operator[](i)) {
				continue;
			};

			PlayerList.push_back(list->operator[](i)->GetGameObject());
		}
		IL2CPP::Thread::Detach(m_pThisThread);
		Sleep(1000);
	}
}

void MainLoop() {																																		  
																																						  
	DWORD currentTime = GetTickCount64();																												  

	if (vars::CameraFovChanger)																															  
	{																																					  
		auto Cameraobj = Unity::GameObject::Find("FirstPersonCamera");	

		if (Cameraobj != nullptr) {					 																									  
			Unity::CCamera* CameraMain = (Unity::CCamera*)Cameraobj->GetComponent("Camera");		

			if (CameraMain != nullptr)			 																										     
			{	

				CameraMain->SetFieldOfView(vars::CameraFov);	 																						     

			}																																			     
		}																																				     
	}																																					     
	
	if (PlayerList.size() > 0) {
		
		
		for (int i = 0; i < PlayerList.size(); i++)
		{
			if (!PlayerList[i]) 																		     
				continue;

			auto cTransform = PlayerList[i]->GetTransform();

			if (!cTransform) continue;

			auto playerPos = cTransform->GetPosition();

			Unity::Vector3 root_pos = playerPos;
			root_pos.y -= 1.f;
			Vector2 pos;

			if (WorldToScreen(root_pos, pos)) {
				ImColor Colortouse = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vars::screen_center.x, vars::screen_size.y), ImVec2(pos.x, pos.y), Colortouse, 1.5f);
			}
		}
	}



	if (currentTime - lasttick > 5)  
	{   
		   
		lasttick = currentTime;   
	}   

}   
   


bool init = false;

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{

	void* m_pThisThread = IL2CPP::Thread::Attach(IL2CPP::Domain::Get());
	if (m_pThisThread)
	{
		if (!init)
		{
			if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
			{
				pDevice->GetImmediateContext(&pContext);
				DXGI_SWAP_CHAIN_DESC sd;
				pSwapChain->GetDesc(&sd);
				window = sd.OutputWindow;
				ID3D11Texture2D* pBackBuffer;
				pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
				pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
				pBackBuffer->Release();
				oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
				InitImGui();
				init = true;
			}

			else
				return oPresent(pSwapChain, SyncInterval, Flags);
		}

		auto begin_scene = [&]() {
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			};

		auto end_scene = [&]() {
			ImGui::Render();
			};


		begin_scene();

		ImGui::SetNextWindowSize(ImVec2(500, 400), 0);

		if (ImGui::Begin("oDxe", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::Checkbox("CamerMain", &vars::CameraFovChanger);

			ImGui::Checkbox("WallHack", &vars::PlayerSnapline);

			if (vars::CameraFovChanger)
			{
				ImGui::SliderFloat("##CamFOV", &vars::CameraFov, 20, 180, "Camera FOV: %.0f");
			}
		}
		ImGui::End();
		try
		{
			MainLoop();
		}
		catch (...) {}


		end_scene();

		pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		IL2CPP::Thread::Detach(m_pThisThread);

	}
	
	return oPresent(pSwapChain, SyncInterval, Flags);

}

void CreateConsole() {
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	SetConsoleTitle("IL2CPP");
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
}

void initchair() {
	CreateConsole();
	initvars();
	IL2CPP::Callback::Initialize();
	kiero::bind(8, (void**)&oPresent, hkPresent);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Player_Loop, NULL, NULL, NULL); 
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook{ false };
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			initchair();
			init_hook = true;
		}
	} while (!init_hook); 
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE mod, DWORD reason, LPVOID lpReserved)
{
	if (reason == 1)
	{
		DisableThreadLibraryCalls(mod);
		CreateThread(nullptr, 0, MainThread, mod, 0, nullptr);
	}
	return TRUE;
}