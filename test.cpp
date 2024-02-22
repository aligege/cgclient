#include<glad/glad.h> //glad的引入必须要在glfw的上面
#include<glfw/glfw3.h>
#include<iostream>
#include "global.h"

using namespace std;

//窗口的宽度
static int width = 800;
//窗口高度
static int height = 600;
//窗口的名字
static char  window_name[] = "cgclient";

void framebuffer_size_callback(GLFWwindow*,int,int);
void processInput(GLFWwindow*);
void renderOrder();

int main() {
	int argc = 0;
	char** argv = nullptr;
	cout << "v8engine progaming init start......" << "\n";
	auto ret = cg::global::v8engine.init();
	cout << "v8engine progaming init end......" << "\n";
	cout << "opengl progaming start......" << "\n";
	//函数库的初始化
	glfwInit();
	//设定opengl版本为3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//设定opengl为核心模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//创建窗口对象
	GLFWwindow* window = glfwCreateWindow(width, height, window_name, NULL, NULL);
	if (window == NULL) {
		cout << "opengl window start fail!gl progaming will terminate....." << endl;
		glfwTerminate(); //终结掉gl程序
		return -1;
	}
	//设定当前剩下文
	glfwMakeContextCurrent(window);
	//注册窗口大小改变回调
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//初始化gladLoader
	if (gladLoadGL(glfwGetProcAddress)<=0) {
		//如果无法获取到glfw指针就相当于失败，并且终止掉gl程序
		cout << "can`t get glfw pointer address!gl progaming will terminate....." << endl;
		glfwTerminate();
		return -1;
	}
	//执行到这里就是默认完成了所有初始化
   //glfw没有执行结束的时候就一直是死循环
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		renderOrder();
		//缓冲区交互
		//关于双缓存的补充:
		//如果使用单缓存的渲染模式，那么游戏画面可以会出现闪烁现象，原因是图形渲染出一帧的画面时，每一个像素点不是同时渲染出来的，
		//而是遵循流水线，由左往右，由上往下来进行渲染的。所以这个时候采用双缓存，当前缓存显示完，直接把后缓存的画面交换上去，则就可以避免游戏闪烁现象。
		//并且使用glfeSwapBuffers()函数进行交换。
		glfwSwapBuffers(window);
		//执行被触发的事件
		glfwPollEvents();
	}

	return 0;
}

/// <summary>
/// 回调函数。窗口大小改变时。
/// </summary>
void framebuffer_size_callback(GLFWwindow* window, int wid, int het) {
   //这是控制视口的函数。
   //前面两个参数是指视口的左下角的坐标偏移位置。(opengl的视口窗口默认左下角为原点0,0)
   //后面两个参数是指视口的宽度和高度
	glViewport(0, 0, wid, het);
	cout << "opengl window resolution change!" << "wid:" << wid << "  " << "het:" << het << endl;
}
/// <summary>
/// 输入函数
/// </summary>
void processInput(GLFWwindow* window){
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window,true);
	}
}
/// <summary>
/// 渲染指令
/// </summary>
void renderOrder() {
	//深绿色
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//关于glfwClear函数，这是一个清屏的函数。当前是去掉颜色缓存区
   //如果传入参数GL_DETCH_BUFFER_BIT(深度缓存区)和GL_STENCIL_BUFFER_BIT(模板缓存区)就是清掉对应的缓存区
	glClear(GL_COLOR_BUFFER_BIT);
}
