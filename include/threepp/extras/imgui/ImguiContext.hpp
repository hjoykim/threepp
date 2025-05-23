
#ifndef THREEPP_IMGUI_HELPER_HPP
#define THREEPP_IMGUI_HELPER_HPP

#include <imgui.h>
#ifdef _MSC_VER
#include <windows.h>
#include <imgui_impl_win32.h>
#else
#include <imgui_impl_glfw.h>
#endif
#include <imgui_impl_opengl3.h>

#include <functional>
#include <threepp/canvas/Monitor.hpp>

class ImguiContext {

public:
    explicit ImguiContext(void* window) {
        ImGui::CreateContext();
#ifdef _MSC_VER
        ImGui_ImplWin32_InitForOpenGL((HWND) window);
#else
        ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(window), true);
#endif
#if EMSCRIPTEN
        ImGui_ImplOpenGL3_Init("#version 300 es");
#else
        ImGui_ImplOpenGL3_Init("#version 330 core");
#endif
    }

    ImguiContext(ImguiContext&&) = delete;
    ImguiContext(const ImguiContext&) = delete;
    ImguiContext& operator=(const ImguiContext&) = delete;

    void render() {
        ImGui_ImplOpenGL3_NewFrame();
#ifdef _MSC_VER
        ImGui_ImplWin32_NewFrame();
#else
        ImGui_ImplGlfw_NewFrame();
#endif
        ImGui::NewFrame();

        onRender();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    virtual ~ImguiContext() {
        ImGui_ImplOpenGL3_Shutdown();
#ifdef _MSC_VER
        ImGui_ImplWin32_Shutdown();
#else
        ImGui_ImplGlfw_Shutdown();
#endif
        ImGui::DestroyContext();
    }

    void makeDpiAware() {

        if (!dpiAwareIsConfigured) {

            dpiAwareIsConfigured = true;

            const auto [dpiScaleX, _] = threepp::monitor::contentScale();

            ImGuiIO& io = ImGui::GetIO();
            io.FontGlobalScale = dpiScaleX;// Assuming dpiScaleX = dpiScaleY

            ImGuiStyle& style = ImGui::GetStyle();
            style.ScaleAllSizes(dpiScaleX);
        }
    }

protected:
    virtual void onRender() = 0;

private:
    bool dpiAware = false;
    bool dpiAwareIsConfigured = false;
};

class ImguiFunctionalContext: public ImguiContext {

public:
    explicit ImguiFunctionalContext(void* window, std::function<void()> f)
        : ImguiContext(window),
          f_(std::move(f)) {}


protected:
    void onRender() override {
        f_();
    }

private:
    std::function<void()> f_;
};

#endif//THREEPP_IMGUI_HELPER_HPP
