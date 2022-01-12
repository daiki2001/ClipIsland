#include "Raki_DX12B.h"
#include "Raki_imguiMgr.h"
#include "Raki_DX12B.h"
#include "Raki_WinAPI.h"

bool ImguiMgr::InitImgui(ID3D12Device *dev, HWND hwnd)
{
	//imgui�p�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 0;
	desc.NumDescriptors = 1;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	HRESULT result;

	result = dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&dsvHeapForImgui));

	//�������s
	if (result != S_OK) {
		return false;
	}

	//imgui�̏�����
	if (ImGui::CreateContext() == nullptr) {
		bool imgui_isNotCreateContext = false;
		assert(!imgui_isNotCreateContext);
	}

	//windows�p������
	bool bin_result = ImGui_ImplWin32_Init(Raki_WinAPI::GetHWND());
	if (!bin_result) {
		return false;
	}

	//DirectX12�p������
	bin_result = ImGui_ImplDX12_Init(
		Raki_DX12B::Get()->GetDevice(),
		3,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		dsvHeapForImgui.Get(),
		dsvHeapForImgui.Get()->GetCPUDescriptorHandleForHeapStart(),
		dsvHeapForImgui.Get()->GetGPUDescriptorHandleForHeapStart()
	);
	if (!bin_result) {
		return false;
	}

	//����
	return true;
}

void ImguiMgr::StartDrawImgui(const char *windowTitle, int posX, int posY)
{
	//imgui�`��J�n
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//�E�B���h�E�\��
	ImGui::Begin(windowTitle);
	ImGui::SetWindowSize(ImVec2(posX, posY), ImGuiCond_::ImGuiCond_FirstUseEver);
}

void ImguiMgr::EndDrawImgui()
{
	//�\���I���A�`��
	ImGui::End();
	ImGui::Render();

	//�f�X�N���v�^�q�[�v���Z�b�g
	Raki_DX12B::Get()->GetGCommandList()->SetDescriptorHeaps(
		1, dsvHeapForImgui.GetAddressOf()
	);

	//�`����s
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), Raki_DX12B::Get()->GetGCommandList());
}

void ImguiMgr::FinalizeImgui()
{
	//�I�����������s
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool myImgui::InitializeImGui(ID3D12Device *dev, HWND hwnd)
{
	bool result = ImguiMgr::Get()->InitImgui(dev, hwnd);
	return result;
}

void myImgui::StartDrawImGui(const char *windowTitle, int sizeX, int sizeY)
{
	ImguiMgr::Get()->StartDrawImgui(windowTitle, sizeX, sizeY);
}

void myImgui::EndDrawImGui()
{
	ImguiMgr::Get()->EndDrawImgui();
}

void myImgui::FinalizeImGui()
{
	ImguiMgr::Get()->FinalizeImgui();
}
