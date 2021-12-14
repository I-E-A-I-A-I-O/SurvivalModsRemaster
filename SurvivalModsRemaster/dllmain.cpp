// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "main.h"
#include "Script.hpp"
#include "UIScript.hpp"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        scriptRegister(hModule, ScriptMain);
        scriptRegisterAdditionalThread(hModule, UIScript::ScriptMain);
        break;

    case DLL_PROCESS_DETACH:
        OnAbort();
        UIScript::OnAbort();
        scriptUnregister(hModule);
        break;
    }
    return TRUE;
}

