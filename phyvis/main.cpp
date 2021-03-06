#include <IMGUI/imgui.h>

#include "falton/physics.h"
#include "falton/math.h"

#include <cstdio>
#include <cstdlib>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "imgui_impl_glfw_gl3.h"
#include "imgui_impl_glfw_gl3.cpp"
#include "phyvis.h"
#include "phyvis.cpp"
#include "phyvis_overlay.h"
#include "phyvis_overlay.cpp"
#include "phyvis_entity.h"
#include "phyvis_entity.cpp"
#include "phyvis_render.h"
#include "phyvis_render.cpp"
#

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

int main(int, char**){

    const double FRAME_INTERVAL = 1.0 / 60.0;

    // Setup window
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Falton Debug Visualizer", NULL, NULL);
    glfwMakeContextCurrent(window);
    gl3wInit();

    // Setup ImGui binding
    ImGui_ImplGlfwGL3_Init(window, true);

    ImVec4 clear_color = ImColor(0, 0, 0);

    Phyvis::Context ctx;
    Phyvis::Init(&ctx, window);

    // Main loop
    glfwSwapInterval(1);
    while (!glfwWindowShouldClose(window))
    {

        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();
        
        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Phyvis::Tick(&ctx, display_w, display_h);
        ImGui::Render();
        
        glfwSwapBuffers(window);

    }

    Phyvis::Cleanup(&ctx);

    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();

    return 0;
}