#pragma once

void LoadModLoaderSettings(LoaderSettings* loaderSettings, std::wstring gamePath);
int GetModCount();
std::string GetModName(int index);
bool IsGamePatchEnabled(const char* patchName);

extern std::wstring currentProfilePath;