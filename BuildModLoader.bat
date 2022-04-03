@echo off
xcopy "D:\GitHub\Downloaded Projects\sa2-mod-loader\bin\*.*" "D:\Jeux\Sonic Adventure 2\" /s /y /d
pause
xcopy "D:\GitHub\Downloaded Projects\sa2-mod-loader\bin\SA2ModLoader.dll" "D:\Jeux\Sonic Adventure 2\mods\" /s /y /d
del "D:\Jeux\Sonic Adventure 2\SA2ModLoader.dll"
pause