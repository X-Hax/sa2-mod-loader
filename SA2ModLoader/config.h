#pragma once

void LoadModLoaderSettings(LoaderSettings* loaderSettings, std::wstring gamePath);
int GetModCount();
std::string GetModName(int index);

extern std::wstring currentProfilePath;