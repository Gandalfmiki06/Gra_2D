
#include "pch.h"
#include "dllkot.h"
#include <iostream>
#define DLL_EXPORT __declspec(dllexport)

extern "C" {
    DLL_EXPORT void kot() {
        std::cout << "Przyk³adowy DLL" << std::endl;
    }
}