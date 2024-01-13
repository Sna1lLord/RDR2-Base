#include "../hooks.hpp"
#include "../../ui/ui.hpp"
#include "../../../utility/memory/signatures/signatures.hpp"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (Ui::Instance()->opened)
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

	return CallWindowProc(Hooks::Instance()->originalWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT __stdcall Hooks::Present(IDXGISwapChain3* swapChain, UINT syncInterval, UINT flags) {
    if (!Ui::Instance()->initialized) {
        if (SUCCEEDED(swapChain->GetDevice(__uuidof(ID3D12Device), (void**)&Ui::Instance()->directx.m_device))) {
            Ui::Instance()->hwnd = FindWindowA(0, xorstr_("Red Dead Redemption 2"));
			Hooks::Instance()->originalWndProc = (WNDPROC)SetWindowLongPtr(Ui::Instance()->hwnd, GWLP_WNDPROC, (LONG_PTR)WndProc);

            ImGui::CreateContext();

			ImGuiIO& io = ImGui::GetIO(); (void)io;
			ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			Ui::Instance()->Initialize(io);

			DXGI_SWAP_CHAIN_DESC Desc;
			swapChain->GetDesc(&Desc);
			Desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			Desc.OutputWindow = Ui::Instance()->hwnd;
			Desc.Windowed = ((GetWindowLongPtr(Ui::Instance()->hwnd, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

			Ui::Instance()->directx.m_buffers_counts = Desc.BufferCount;
			Ui::Instance()->directx.m_frame_context = new frameContext_t[Ui::Instance()->directx.m_buffers_counts];

			IDXGISwapChain* swapchain = *Signatures::Instance()->definitions.swapchain;
			Ui::Instance()->directx.m_command_queue = *Signatures::Instance()->definitions.commandQueue;

			D3D12_DESCRIPTOR_HEAP_DESC DescriptorImGuiRender = {};
			DescriptorImGuiRender.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			DescriptorImGuiRender.NumDescriptors = Ui::Instance()->directx.m_buffers_counts;
			DescriptorImGuiRender.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

			if (Ui::Instance()->directx.m_device->CreateDescriptorHeap(&DescriptorImGuiRender, IID_PPV_ARGS(&Ui::Instance()->directx.m_descriptor_heap_imgui_render)) != S_OK)
				return static_cast<decltype(&Present)>(Hooks::Instance()->originalPresent)(swapChain, syncInterval, flags);

			ID3D12CommandAllocator* Allocator;
			if (Ui::Instance()->directx.m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&Allocator)) != S_OK)
				return static_cast<decltype(&Present)>(Hooks::Instance()->originalPresent)(swapChain, syncInterval, flags);

			for (size_t i = 0; i < Ui::Instance()->directx.m_buffers_counts; i++) {
				Ui::Instance()->directx.m_frame_context[i].m_command_allocator = Allocator;
			}

			if (Ui::Instance()->directx.m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, Allocator, NULL, IID_PPV_ARGS(&Ui::Instance()->directx.m_command_list)) != S_OK ||
				Ui::Instance()->directx.m_command_list->Close() != S_OK)
				return static_cast<decltype(&Present)>(Hooks::Instance()->originalPresent)(swapChain, syncInterval, flags);

			D3D12_DESCRIPTOR_HEAP_DESC DescriptorBackBuffers;
			DescriptorBackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			DescriptorBackBuffers.NumDescriptors = Ui::Instance()->directx.m_buffers_counts;
			DescriptorBackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			DescriptorBackBuffers.NodeMask = 1;

			if (Ui::Instance()->directx.m_device->CreateDescriptorHeap(&DescriptorBackBuffers, IID_PPV_ARGS(&Ui::Instance()->directx.m_descriptor_heap_back_buffers)) != S_OK)
				return static_cast<decltype(&Present)>(Hooks::Instance()->originalPresent)(swapChain, syncInterval, flags);

			const auto RTVDescriptorSize = Ui::Instance()->directx.m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle = Ui::Instance()->directx.m_descriptor_heap_back_buffers->GetCPUDescriptorHandleForHeapStart();

			for (size_t i = 0; i < Ui::Instance()->directx.m_buffers_counts; i++) {
				ID3D12Resource* pBackBuffer = nullptr;
				Ui::Instance()->directx.m_frame_context[i].m_descriptor_handle = RTVHandle;
				swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&pBackBuffer));
				Ui::Instance()->directx.m_device->CreateRenderTargetView(pBackBuffer, nullptr, RTVHandle);
				Ui::Instance()->directx.m_frame_context[i].m_resource = pBackBuffer;
				RTVHandle.ptr += RTVDescriptorSize;
			}

            ID3D12DescriptorHeap* descriptorHeapImguiRender = Ui::Instance()->directx.m_descriptor_heap_imgui_render;
			ImGui_ImplWin32_Init(Ui::Instance()->hwnd);
			ImGui_ImplDX12_Init(
                Ui::Instance()->directx.m_device, 
                Ui::Instance()->directx.m_buffers_counts,
                DXGI_FORMAT_R8G8B8A8_UNORM, 
                descriptorHeapImguiRender, 
                descriptorHeapImguiRender->GetCPUDescriptorHandleForHeapStart(), 
                descriptorHeapImguiRender->GetGPUDescriptorHandleForHeapStart()
            );
			ImGui_ImplDX12_CreateDeviceObjects();
			ImGui::GetIO().ImeWindowHandle = Ui::Instance()->hwnd;
        }

        Ui::Instance()->initialized = true;
    }

    if (Ui::Instance()->directx.m_command_queue == nullptr)
		return static_cast<decltype(&Present)>(Hooks::Instance()->originalPresent)(swapChain, syncInterval, flags);

    ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Ui::Instance()->Tick();

	ImGui::EndFrame();

    frameContext_t& CurrentFrameContext = Ui::Instance()->directx.m_frame_context[swapChain->GetCurrentBackBufferIndex()];
	CurrentFrameContext.m_command_allocator->Reset();

	D3D12_RESOURCE_BARRIER Barrier;
	Barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	Barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	Barrier.Transition.pResource = CurrentFrameContext.m_resource;
	Barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	Ui::Instance()->directx.m_command_list->Reset(CurrentFrameContext.m_command_allocator, nullptr);
	Ui::Instance()->directx.m_command_list->ResourceBarrier(1, &Barrier);
	Ui::Instance()->directx.m_command_list->OMSetRenderTargets(1, &CurrentFrameContext.m_descriptor_handle, FALSE, nullptr);
	Ui::Instance()->directx.m_command_list->SetDescriptorHeaps(1, &Ui::Instance()->directx.m_descriptor_heap_imgui_render);

	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), Ui::Instance()->directx.m_command_list);
	Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	Ui::Instance()->directx.m_command_list->ResourceBarrier(1, &Barrier);
	Ui::Instance()->directx.m_command_list->Close();
	Ui::Instance()->directx.m_command_queue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&Ui::Instance()->directx.m_command_list));

    return static_cast<decltype(&Present)>(Hooks::Instance()->originalPresent)(swapChain, syncInterval, flags);
}