@echo off

if exist "%programfiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" call "%programfiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
if exist "%programfiles%\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" call "%programfiles%\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"


SET includes=/I src /I %cd%/SFML-3.0.0/include
SET links=/link /LIBPATH:%cd%/src /LIBPATH:%cd%/SFML-3.0.0/lib MSVCRT.lib LIBCMT.lib opengl32.lib User32.lib Shell32.lib Gdi32.lib Kernel32.lib Advapi32.lib Ole32.lib Oleaut32.lib uuid.lib odbc32.lib odbccp32.lib sfml-system.lib sfml-graphics.lib sfml-window.lib sfml-audio.lib freetype.lib winmm.lib flac.lib vorbisenc.lib vorbisfile.lib vorbis.lib ogg.lib ws2_32.lib
SET defines=/D RELEASE

cl /std:c++17 /EHsc /Fe"%cd%/build/Main.exe" /Fo"%cd%/build/" %includes% %defines% src/main.cpp %links%

if %errorlevel% neq 0 (
		color 0c
		echo Build failed!
) else (
		color 0a
		echo Build succeeded. Launching game...
		cd build
		Main.exe
)

