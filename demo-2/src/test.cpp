#include <glad/glad.h>
//
#include <GLFW/glfw3.h>
//
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// MathGL
#include <mgl2/mgl.h>
#include <stdexcept>
#include <vector>
//

bool MyRoundButton(const char* label, ImVec2 size, float rounding, ImU32 color)
{
    // 定义交互区域
    if (ImGui::InvisibleButton(label, size))
        return true; // 返回点击状态

    // 获取位置
    ImVec2 pos = ImGui::GetItemRectMin();
    ImVec2 pos_max = ImGui::GetItemRectMax();

    // 获取绘制列表
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // 绘制背景
    draw_list->AddRectFilled(pos, pos_max, color, rounding);

    // 绘制边框
    draw_list->AddRect(pos, pos_max, IM_COL32(255,255,255,255), rounding);

    // 绘制文字居中
    ImVec2 text_size = ImGui::CalcTextSize(label);
    ImVec2 text_pos = ImVec2(
        pos.x + (size.x - text_size.x) * 0.5f,
        pos.y + (size.y - text_size.y) * 0.5f
    );
    draw_list->AddText(text_pos, IM_COL32(255,255,255,255), label);

    return false;
}


static GLuint createOrUpdateTexture( GLuint tex, int w, int h, const unsigned char* rgb )
{
    if ( tex == 0 )
    {
        glGenTextures( 1, &tex );
        glBindTexture( GL_TEXTURE_2D, tex );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        // initial upload
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb );
    }
    else
    {
        glBindTexture( GL_TEXTURE_2D, tex );
        // update content
        glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, rgb );
    }
    glBindTexture( GL_TEXTURE_2D, 0 );
    return tex;
}

int main()
{
    // Init GLFW
    if ( ! glfwInit() )
    {
        return -1;
    }
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    GLFWwindow* window = glfwCreateWindow( 1280, 720, "MathGL + ImGui", nullptr, nullptr );
    if ( ! window )
    {
        return -1;
    }
    glfwMakeContextCurrent( window );
    glfwSwapInterval( 1 );

    // Init GL loader
    if ( ! gladLoadGLLoader( ( GLADloadproc )glfwGetProcAddress ) )
    {
        return -1;
    }
    // Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ( void )io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL( window, true );
    ImGui_ImplOpenGL3_Init( "#version 330" );

    // Prepare MathGL graph (software)
    const int W = 800, H = 500;
    mglGraph  gr( 0, W, H );  // kind=0: no OpenGL canvas; software buffer only
    // Prepare data
    mglData y( 100 );
    for ( int i = 0; i < y.GetNN(); ++i )
    {
        double x = ( double )i / ( y.GetNN() - 1 );
        y.a[ i ] = sin( 3.141592653589793 * ( 2 * x - 1 ) );  // similar to samples
    }
    // Draw once (you can redraw per frame if interactive)
    gr.SetOrigin( 0, 0, 0 );
    gr.Axis();
    gr.Box();
    gr.Plot( y, "o!rgb" );  // markers + per-point color palette (see sample docs)
    //
    // 输出到 SVG 文件
    gr.WriteSVG( "output.svg" );
    // 输出到 SVG 文件
    // gr.WriteSVG( "output.svg" );
    // Now we have RGB buffer
    int                  imgW   = gr.GetWidth();
    int                  imgH   = gr.GetHeight();
    const unsigned char* rgbPtr = gr.GetRGB();  // 8-bit RGB interleaved

    // Upload to GL texture
    GLuint tex = 0;
    tex        = createOrUpdateTexture( tex, imgW, imgH, rgbPtr );

    // Main loop
    while ( ! glfwWindowShouldClose( window ) )
    {
        glfwPollEvents();
        // Optionally: re-render MathGL content per frame if data changes
        // gr.Clf(); ... draw again ... rgbPtr = gr.GetRGB(); tex = createOrUpdateTexture(tex, imgW, imgH, rgbPtr);

        // ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin( "MathGL Plot" );
        ImVec2 avail = ImGui::GetContentRegionAvail();
        // Keep aspect
        float  aspect = ( float )imgW / ( float )imgH;
        ImVec2 size   = avail;
        if ( size.x / size.y > aspect )
            size.x = size.y * aspect;
        else
            size.y = size.x / aspect;
        ImGui::Image( ( ImTextureID )( intptr_t )tex, size );

        // 
        MyRoundButton( "Custom Button", ImVec2( 120, 40 ), 10.0f, IM_COL32( 100, 150, 250, 255 ) );
        // optional controls here...
        ImGui::End();



        // render
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize( window, &display_w, &display_h );
        glViewport( 0, 0, display_w, display_h );
        glClearColor( 0.05f, 0.05f, 0.08f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
        glfwSwapBuffers( window );
    }

    // Cleanup
    glDeleteTextures( 1, &tex );
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow( window );
    glfwTerminate();
    return 0;
}
