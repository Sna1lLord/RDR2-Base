#include "ui.hpp"
#include "../../utility/invoker/natives.hpp"
#include "../../utility/memory/signatures/signatures.hpp"
#include "../hooks/hooks.hpp"
#include "../../utility/invoker/queue.hpp"
#include "../features/features.hpp"

static Ui* uiInstance = nullptr;

Ui* Ui::Instance() {
	if (!uiInstance)
		uiInstance = new Ui();

	return uiInstance;
}

void Ui::Initialize(ImGuiIO& io) {
	ImFontConfig config;
	config.FontDataOwnedByAtlas = false;
	static const ImWchar iconRanges[] = { 0xf000, 0xf3ff, 0 };
	optionFont = io.Fonts->AddFontFromFileTTF(xorstr_("C:\\Windows\\Fonts\\Arial.ttf"), 18.f, &config);
	iconFont = io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 26.f, &config, iconRanges);

	ImVec4* colors = ImGui::GetStyle().Colors;
	ImGuiStyle style = ImGui::GetStyle();

	style.WindowBorderSize = 3;
	style.WindowRounding = 7;
	style.FrameRounding = 4;
	style.FrameBorderSize = 0.4f;
	style.FramePadding = ImVec2(8, 6);
	style.ChildRounding = 5;

	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.17647058823f, 0.20784313725f, 0.25098039215f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.27058823529f, 0.46274509803f, 0.74901960784f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.27058823529f, 0.46274509803f, 0.74901960784f, 1.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.f, 0.f, 0.f, 0.0f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.27058823529f, 0.46274509803f, 0.74901960784f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.f, 0.f, 0.f, 0.0f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 1.00f, 0.96f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 1.00f, 0.96f, 0.42f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 1.00f, 0.96f, 0.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 1.00f, 0.96f, 0.38f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 1.00f, 0.96f, 0.46f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 1.00f, 0.96f, 0.57f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.27058823529f, 0.46274509803f, 0.74901960784f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.27058823529f, 0.46274509803f, 0.74901960784f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.27058823529f, 0.46274509803f, 0.74901960784f, 0.9f);
	colors[ImGuiCol_Button] = ImVec4(0.f, 0.f, 0.f, 0.0f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.27058823529f, 0.46274509803f, 0.74901960784f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.f, 0.f, 0.f, 0.0f);
	colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.27058823529f, 0.46274509803f, 0.74901960784f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.00f, 1.00f, 0.96f, 0.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.00f, 1.00f, 0.96f, 0.40f);
	colors[ImGuiCol_TabActive] = ImVec4(0.00f, 1.00f, 0.96f, 0.32f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void Ui::Uninitialize() {
    ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (directx.m_device) { directx.m_device->Release(); }
	if (directx.m_descriptor_heap_back_buffers) { directx.m_descriptor_heap_back_buffers->Release(); }
	if (directx.m_descriptor_heap_imgui_render) { directx.m_descriptor_heap_imgui_render->Release(); }
	if (directx.m_command_list) { directx.m_command_list->Release(); }
	if (directx.m_command_queue) { directx.m_command_queue->Release(); }
	if (directx.m_frame_context->m_command_allocator) { directx.m_frame_context->m_command_allocator->Release(); }
	if (directx.m_frame_context->m_resource) { directx.m_frame_context->m_resource->Release(); }

    SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)(Hooks::Instance()->originalWndProc));

    uiInstance = nullptr;
    delete this;
}

void Ui::Tick() {
    if (GetAsyncKeyState(VK_INSERT) & 1)
        opened ^= true;

    ImGui::GetIO().MouseDrawCursor = opened;

    if (!opened)
        return;
    
    ImGui::SetNextWindowSize(ImVec2(400.f, 300.f));
    if (ImGui::Begin(xorstr_("Base"), &opened, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
        ImGui::BeginChild(xorstr_("TabChild"), ImVec2(70.f, 285.f), true);
        
        ImGui::PushFont(iconFont);
        for (int i = 0; i < (int)submenuList.size(); i++) {
            bool current = false;
            if (i == currentSubmenu) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.27058823529f, 0.46274509803f, 0.74901960784f, 1.00f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.27058823529f, 0.46274509803f, 0.74901960784f, 0.5f));
                current = true;
            }
            if (ImGui::Button(submenuList[i], ImVec2(55.f, 35.f))) {
                currentSubmenu = i;
            }
            if (current) {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        ImGui::PopFont();

        ImGui::EndChild();

        ImGui::PushFont(optionFont);

        ImVec2 windowPosition = ImGui::GetWindowPos();
        ImGui::SetNextWindowPos(ImVec2(windowPosition.x + 85.f, windowPosition.y + 8.f));
        ImGui::BeginChild(xorstr_("MainChild"), ImVec2(305.f, 285.f), true);

        switch (currentSubmenu) {
            case 0: {
                ImGui::Checkbox(xorstr_("Godmode"), &Config::Instance()->self.godmode);
                ImGui::Checkbox(xorstr_("Auto Heal"), &Config::Instance()->self.demiGodmode);

                ImGui::Checkbox(xorstr_("No-Clip"), &Config::Instance()->self.noClip);
                ImGui::SliderFloat(xorstr_("##"), &Config::Instance()->self.noClipSpeed, 0.1f, 10.f, "%.f");
                    
                if (ImGui::Button(xorstr_("Fill Cores"))) {
                    if (Features::Instance()->localPed) {
                        Queue::Instance()->Add([=]{
                            ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(Features::Instance()->localPed, 0, 100);
                            ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(Features::Instance()->localPed, 1, 100);
                            ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(Features::Instance()->localPed, 2, 100);
                        });
                    }
                }
                if (ImGui::Button(xorstr_("Gold Cores"))) {
                    if (Features::Instance()->localPed) {
                        Queue::Instance()->Add([=]{
                            ATTRIBUTE::_ENABLE_ATTRIBUTE_CORE_OVERPOWER(Features::Instance()->localPed, 0, 100.f, true);
                            ATTRIBUTE::_ENABLE_ATTRIBUTE_CORE_OVERPOWER(Features::Instance()->localPed, 1, 100.f, true);
                            ATTRIBUTE::_ENABLE_ATTRIBUTE_CORE_OVERPOWER(Features::Instance()->localPed, 2, 100.f, true);
                            LAW::CLEAR_BOUNTY(Features::Instance()->localPlayer);
                        });
                    }
                }
                if (ImGui::Button(xorstr_("Drunk"))) {
                    if (Features::Instance()->localPed) {
                        Queue::Instance()->Add([=]{
                            PED::_SET_PED_DRUNKNESS(Features::Instance()->localPed, true, 1.f);
                            AUDIO::SET_PED_IS_DRUNK(Features::Instance()->localPed, true);
                        });
                    }
                }

                break;
            }
            case 1: {
                if (ImGui::Button(xorstr_("Day"))) {
                    Queue::Instance()->Add([=]{
                        CLOCK::SET_CLOCK_TIME(12, 0, 0);
                    });
                }
                if (ImGui::Button(xorstr_("Night"))) {
                    Queue::Instance()->Add([=]{
                        CLOCK::SET_CLOCK_TIME(22, 0, 0);
                    });
                }

                break;
            }
            case 2: {
                if (ImGui::Button(xorstr_("Fill Cores"))) {
                    if (Features::Instance()->localHorse) {
                        Queue::Instance()->Add([=]{
                            ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(Features::Instance()->localHorse, 0, 100);
                            ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(Features::Instance()->localHorse, 1, 100);
                            ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(Features::Instance()->localHorse, 2, 100);
                        });
                    }
                }
                if (ImGui::Button(xorstr_("Gold Cores"))) {
                    if (Features::Instance()->localHorse) {
                        Queue::Instance()->Add([=]{
                            ATTRIBUTE::_ENABLE_ATTRIBUTE_CORE_OVERPOWER(Features::Instance()->localHorse, 0, 100.f, true);
                            ATTRIBUTE::_ENABLE_ATTRIBUTE_CORE_OVERPOWER(Features::Instance()->localHorse, 1, 100.f, true);
                            ATTRIBUTE::_ENABLE_ATTRIBUTE_CORE_OVERPOWER(Features::Instance()->localHorse, 2, 100.f, true);
                        });
                    }
                }

                break;
            }
            case 3: {
                if (ImGui::Button(xorstr_("Teleport To Waypoint"))) {
                    if (Features::Instance()->localPed) {
                        Queue::Instance()->Add([=]{
                            NativeVector coords = MAP::_GET_WAYPOINT_COORDS();

                            Entity entity = Features::Instance()->localPed;
                            if (PED::IS_PED_ON_MOUNT(Features::Instance()->localPed))
                                entity = Features::Instance()->localHorse;

                            ENTITY::SET_ENTITY_COORDS_NO_OFFSET(entity, Features::Instance()->LoadGround(coords), false, false, false);
                        });
                    }
                }

                break;
            }
        }

        ImGui::PopFont();
        ImGui::EndChild();

        ImGui::End();
    }
}