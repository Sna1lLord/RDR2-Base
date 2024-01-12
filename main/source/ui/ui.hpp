#ifndef _UI_HPP
#define _UI_HPP

#include <common.hpp>
#include "../../utility/rage/rage.hpp"
#include "fonts.hpp"

struct frameContext_t {
    ID3D12CommandAllocator* m_command_allocator;
    ID3D12Resource* m_resource;
    D3D12_CPU_DESCRIPTOR_HANDLE m_descriptor_handle;
};

struct directX12_t {
    ID3D12Device* m_device = nullptr;
	ID3D12DescriptorHeap* m_descriptor_heap_back_buffers;
	ID3D12DescriptorHeap* m_descriptor_heap_imgui_render;
	ID3D12GraphicsCommandList* m_command_list;
	ID3D12CommandQueue* m_command_queue;

	UINT m_buffers_counts = -1;
	frameContext_t* m_frame_context;
};

class Ui {
public:
    HWND hwnd;
    directX12_t directx;
    bool initialized;
	bool opened;
	std::vector<const char*> submenuList = {ICON_FA_USER, ICON_FA_GLOBE, ICON_FA_CAR, ICON_FA_MOON};
	int currentSubmenu;
	ImFont* optionFont;
    ImFont* iconFont;
public:
    static Ui* Instance();
	void Initialize(ImGuiIO& io);
	void Uninitialize();
	void Tick();
};

#endif