// // demo_3d.cpp
// #include <mgl2/mgl.h>

// //
// int main()
// {
//     mglGraph gr;
//     gr.SetSize( 1400, 900 );
//     gr.Title( "Surface: $z=\\sin(x)\\,\\cos(y)$" );

//     // 设置 3D 范围
//     gr.SetRanges( -M_PI, M_PI, -M_PI, M_PI, -1.2, 1.2 );
//     gr.Axis();
//     gr.Box();  // 3D 边框

//     // 生成网格数据
//     const int NX = 100, NY = 100;
//     mglData   xs( NX ), ys( NY ), zs( NX, NY );
//     for ( int i = 0; i < NX; ++i )
//         xs.a[ i ] = -M_PI + ( 2.0 * M_PI ) * i / ( NX - 1 );
//     for ( int j = 0; j < NY; ++j )
//         ys.a[ j ] = -M_PI + ( 2.0 * M_PI ) * j / ( NY - 1 );
//     for ( int i = 0; i < NX; ++i )
//         for ( int j = 0; j < NY; ++j )
//             zs.a[ i + NX * j ] = sin( xs.a[ i ] ) * cos( ys.a[ j ] );

//     // 伪彩色映射与曲面
//     gr.Light( true );       // 光照
//     gr.SetQuality( 3 );     // 提升曲面质量
//     gr.Surf( xs, ys, zs );  // 绘制曲面
//     gr.Cont( zs, "k" );     // 等值线
//     gr.Colorbar( "" );      // 色带

//     // 视角
//     gr.Rotate( 50, 30 );  // 旋转视角（俯仰/方位）

//     // 公式标签
//     gr.Label( 'x', "$x$", 0 );
//     gr.Label( 'y', "$y$", 0 );
//     gr.Label( 'z', "$z$", 0 );

//     gr.WritePNG( "surface_3d.png" );
//     return 0;
// }

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
// #include <mgl2/mgl_gl.h>  // MathGL OpenGL 渲染头文件
#include <mgl2/mgl.h>

// 初始化 MathGL OpenGL 渲染上下文
void init_mgl_gl( mglGL& gr, int width, int height )
{
    gr.SetSize( width, height );
    gr.SetOrigin( 0, 0 );
    gr.SetRanges( -5, 5, -1, 5 );
    // 绑定 MathGL 到当前 OpenGL 上下文
    gr.InitGL();
}

// 实时绘制（随参数变化）
void draw_mgl_gl( mglGL& gr, float param )
{
    gr.Clear();  // 清空画布
    // 绘制带参数的函数：ReLU(x) * param
    gr.FPlot( Form( "max(0,x)*%g", param ), "r-" );
    gr.Text( -3, 4, Form( "$\\text{ReLU}(x) \\times %.1f$", param ), 0 );
    gr.Axis();
    gr.Grid();
    gr.FinishGL();  // 完成 OpenGL 渲染
}

int main()
{
    // （省略 GLFW/ImGui 初始化代码，同方法1）

    // --------------------------
    // 步骤 3：初始化 MathGL OpenGL 渲染
    // --------------------------
    int   plot_width = 600, plot_height = 400;
    mglGL gr;  // MathGL OpenGL 绘图上下文
    init_mgl_gl( gr, plot_width, plot_height );

    // 交互参数：控制函数缩放
    float relu_param = 1.0f;

    // --------------------------
    // 主循环
    // --------------------------
    while ( ! glfwWindowShouldClose( window ) )
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ImGui 窗口
        ImGui::Begin( "MathGL + ImGui (OpenGL)" );

        // 交互控件：调整函数缩放参数
        ImGui::SliderFloat( "ReLU 缩放系数", &relu_param, 0.1f, 2.0f );

        // 实时绘制 MathGL 图形（OpenGL 直接渲染到纹理）
        draw_mgl_gl( gr, relu_param );

        // 显示 MathGL 渲染结果（ImGui 绘制 OpenGL 纹理）
        ImGui::Image( ( void* )( intptr_t )gr.GetTex(), ImVec2( plot_width, plot_height ) );

        ImGui::End();

        // （省略 ImGui 渲染/缓冲区交换代码，同方法1）
    }

    // 资源清理
    gr.ReleaseGL();  // 释放 MathGL OpenGL 资源
    // （省略其他清理代码）
    return 0;
}