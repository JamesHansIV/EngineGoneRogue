#include "Engine/Application/Application.h"
#include <iostream>

Application* CreateApplication();

int main(int  /*argc*/, char*  /*args*/[])
{
    auto *app = CreateApplication();

    app->Run();
    delete app;
    return 0;
}
