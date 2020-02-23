#include <iostream>
#include <GaiaCore.h>



#ifdef _DEBUG
int main()
#else
#include <Windows.h>
int WINAPI
WinMain(HINSTANCE hinstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
    GaiaCore* g = new GaiaCore();
    g->init();
    std::cout << "Hello World!\n";
}