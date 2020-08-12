#pragma once

#include "glStuff.h"
#include <vector>

// use this as a hook to perform initialisation
void(__cdecl* glutSetCursor)(int cursor) = *(void(__cdecl**)(int))0x140966068;
void(__cdecl* uploadModelTransformBuf)(DWORD* a1, int a2) = (void(__cdecl*)(DWORD * a1, int a2))0x140444fa0;

bool& performTransform = *(bool*)0x14cc57bb0;

// vars that will contain the requiered info about GL buffers to perform the transform
GLuint buf_tex;


// check if a buffer target is okay to allow on non-Nvidia platforms
// (used to prevent crashes in glBindBuffer and glBufferData)
// only parameter buffer NV targets cause issues
bool checkBufferTargetOk(GLenum target)
{
	return !(target == GL_VERTEX_PROGRAM_PARAMETER_BUFFER_NV || target == GL_FRAGMENT_PROGRAM_PARAMETER_BUFFER_NV);
}


int __stdcall stub() { return 1; }

bool enable_chara_skinning;
bool use_TexSubImage;
bool force_BGRA_upload;
GLint tex_upload_format;
bool shader_amd_farc;

std::wstring ExePath() {
	WCHAR buffer[MAX_PATH];
	GetModuleFileNameW(NULL, buffer, MAX_PATH);
	return std::wstring(buffer);
}

std::wstring DirPath() {
	std::wstring exepath = ExePath();
	std::wstring::size_type pos = exepath.find_last_of(L"\\/");
	return exepath.substr(0, pos);
}

std::wstring CONFIG_FILE_STRING = DirPath() + L"\\plugins\\Novidia.ini";
LPCWSTR CONFIG_FILE = CONFIG_FILE_STRING.c_str();


void loadConfig()
{
	enable_chara_skinning = GetPrivateProfileIntW(L"general", L"enable_chara_skinning", 1, CONFIG_FILE) > 0 ? true : false;
	use_TexSubImage = GetPrivateProfileIntW(L"general", L"use_TexSubImage", 1, CONFIG_FILE) > 0 ? true : false;
	force_BGRA_upload = GetPrivateProfileIntW(L"general", L"force_BGRA_upload", 1, CONFIG_FILE) > 0 ? true : false;
	shader_amd_farc = GetPrivateProfileIntW(L"general", L"shader_amd_farc", 1, CONFIG_FILE) > 0 ? true : false;
}


void InjectCode(void* address, const std::vector<uint8_t> data)
{
	const size_t byteCount = data.size() * sizeof(uint8_t);

	DWORD oldProtect;
	VirtualProtect(address, byteCount, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(address, data.data(), byteCount);
	VirtualProtect(address, byteCount, oldProtect, nullptr);
}