#include "stdafx.h"

// Windows
#include <Windows.h>

// Standard
#include <string>
#include <algorithm> // for std::transform
#include <fstream>
#include <vector>
#include <sstream>

// Global
#include <SA2ModLoader.h>

// Local
#include "FileReplacement.h"
#include "FileSystem.h"
#include "direct3d.h"
#include "DDS.h"
#include "pvmx.h"
#include "PAKFile.h"
#include "magic.h"
#include "UsercallFunctionHandler.h"

// This
#include "TextureReplacement.h"

#define TOMAPSTRING(a) { a, #a }

using namespace std;

struct TexReplaceData : TexPackEntry
{
	int mod_index;
	string path;
};

static unordered_map<string, vector<TexPackEntry>> raw_cache;
static unordered_map<string, vector<pvmx::DictionaryEntry>> archive_cache;
static unordered_map<string, unordered_map<string, TexReplaceData>*> replace_cache;
static bool was_loading = false;

DataPointer(NJS_TEXMANAGE*, TexManageCache, 0x2670590);

UsercallFunc(int, FindCachedTextureIndex, (int gbix), (gbix), 0x42FAD0, rEAX, rEDI);
UsercallFunc(int, FindFreeTextureIndex, (int gbix), (gbix), 0x4309D0, rEAX, rESI);
UsercallFunc(int, CalcTexSize, (int mip_levels, int format, int width, signed int height, signed int depth, char tex_type), (mip_levels, format, width, height, depth, tex_type), 0x85EFD0, rEAX, rEAX, rECX, rEDX, stack4, stack4, stack1);
UsercallFunc(int, LoadPAKTextures, (const char* filename, NJS_TEXLIST* texlist), (filename, texlist), 0x430B10, rEAX, rECX, stack4);
UsercallFunc(signed int, LoadPRSTextures, (NJS_TEXLIST* texlist, const char* filename), (texlist, filename), 0x44C620, rEAX, rECX, stack4);
UsercallFunc(signed int, LoadEventPRSTextures, (NJS_TEXLIST* texlist, const char* filename, void* buffer), (texlist, filename, buffer), 0x44C6F0, rEAX, rEBX, rEDI, stack4);
FunctionPointer(void*, OperatorNew, (size_t size), 0x7A5DA1);
UsercallFuncVoid(LoadTexPixelData, (NJS_TEXMANAGE* a1, void* a2), (a1, a2), 0x433BD0, rESI, stack4);
UsercallFunc(NJS_TEXMANAGE*, LoadGVRData, (Uint32 gbix, void* buf), (gbix, buf), 0x430A20, rEAX, rEAX, rECX);

void __cdecl LoadTextureList_NoName_r(NJS_TEXLIST* texlist);
int __cdecl LoadPAKTextures_r(const char* filename, NJS_TEXLIST* texlist);
int __cdecl LoadPRSTextures_r(NJS_TEXLIST* texlist, const char* filename);
int __cdecl LoadEventPRSTextures_r(NJS_TEXLIST* texlist, const char* filename, void* data);

NJS_TEXMANAGE* GetCachedTexture(int gbix)
{
	auto v3 = FindCachedTextureIndex(gbix);
	if (v3 == -1)
	{
		v3 = FindFreeTextureIndex(gbix);
	}
	else
	{
		auto v4 = TexManageCache[v3].texsys;
		auto v5 = reinterpret_cast<Magic::RenderCore::Texture*>(v4->texsurface.pSurface);
		if (v5)
		{
			v5->__vftable->Destructor(v5, 1);
			v4->texsurface.pSurface = 0;
		}
	}
	return &TexManageCache[v3];
}

void texpack::init()
{
	LoadPAKTextures.Hook(LoadPAKTextures_r);
	LoadPRSTextures.Hook(LoadPRSTextures_r);
	LoadEventPRSTextures.Hook(LoadEventPRSTextures_r);
	WriteJump(LoadTextureList_NoName, LoadTextureList_NoName_r);
}

void ScanTextureReplaceFolder(const string& srcPath, int modIndex)
{
	if (srcPath.size() > MAX_PATH - 3)
		return;
	WIN32_FIND_DATAA data;
	char path[MAX_PATH];
	snprintf(path, sizeof(path), "%s\\*", srcPath.c_str());
	auto hFind = FindFirstFileA(path, &data);

	string lower = srcPath;
	transform(lower.begin(), lower.end(), lower.begin(), tolower);

	// No files found.
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		// NOTE: This will hide *all* files starting with '.'.
		// SADX doesn't use any files starting with '.',
		// so this won't cause any problems.
		if (data.cFileName[0] == '.')
		{
			continue;
		}

		const string fileName = string(data.cFileName);

		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			string original = fileName;
			transform(original.begin(), original.end(), original.begin(), ::tolower);

			string texPack = srcPath + '\\' + fileName;
			transform(texPack.begin(), texPack.end(), texPack.begin(), ::tolower);

			vector<TexPackEntry> index;
			if (texpack::parse_index(texPack, index))
			{
				unordered_map<string, TexReplaceData>* pvmdata;
				auto& iter = replace_cache.find(original);
				if (iter == replace_cache.end())
				{
					pvmdata = new unordered_map<string, TexReplaceData>;
					replace_cache.insert({ original, pvmdata });
				}
				else
					pvmdata = iter->second;
				for (const auto& idx : index)
				{
					string nameNoExt = GetBaseName(idx.name);
					StripExtension(nameNoExt);
					transform(nameNoExt.begin(), nameNoExt.end(), nameNoExt.begin(), tolower);
					(*pvmdata)[nameNoExt] = { idx.global_index, idx.name, idx.width, idx.height, modIndex, texPack };
				}
			}
		}
	} while (FindNextFileA(hFind, &data) != 0);

	FindClose(hFind);
}

void ReplaceTexture(const char* pvm_name, const char* tex_name, const char* file_path, uint32_t gbix, uint32_t width, uint32_t height)
{
	string original = pvm_name;
	StripExtension(original);
	transform(original.begin(), original.end(), original.begin(), ::tolower);

	string texPack = GetDirectory(file_path);
	transform(texPack.begin(), texPack.end(), texPack.begin(), ::tolower);

	string texFile = GetBaseName(file_path);
	transform(texFile.begin(), texFile.end(), texFile.begin(), ::tolower);

	unordered_map<string, TexReplaceData>* pvmdata;
	auto& iter = replace_cache.find(original);
	if (iter == replace_cache.end())
	{
		pvmdata = new unordered_map<string, TexReplaceData>;
		replace_cache.insert({ original, pvmdata });
	}
	else
		pvmdata = iter->second;
	string nameNoExt = tex_name;
	StripExtension(nameNoExt);
	transform(nameNoExt.begin(), nameNoExt.end(), nameNoExt.begin(), tolower);
	(*pvmdata)[nameNoExt] = { gbix, texFile, width, height, INT_MAX, texPack };
}

inline void check_loading()
{
	/*bool loading = LoadingFile != 0;

#ifdef _DEBUG
	if (!was_loading && was_loading != loading)
	{
		PrintDebug("\tBuilding cache...\n");
	}
#endif

	was_loading = loading;*/
}

#pragma region Index Parsing

/**
* \brief Parses custom texture index.
* \param path A valid path to a texture pack directory containing index.txt
* \param out A vector to populate.
* \return \c true on success.
*/
bool texpack::parse_index(const string& path, vector<TexPackEntry>& out)
{
	PrintDebug("Loading texture pack: %s\n", path.c_str());

	/*const auto it = raw_cache.find(path);

	if (it != raw_cache.end())
	{
		out = it->second;
		return true;
	}*/

	auto index = path + "\\index.txt";

	if (!FileExists(index))
	{
		return false;
	}

	check_loading();

	ifstream index_file(index);

	if (!index_file.is_open())
	{
		PrintDebug("Unable to open index file: %s\n", index.c_str());
		return false;
	}

	bool result = true;
	string line;
	uint32_t line_number = 0;

	// This reads the custom texture list from disk.
	// The format is: gbix,filename
	// e.g: 1000,MyCoolTexture.dds
	// TODO: Add GBIX bypass so texture packs can optionally ignore GBIX on load.

	while (!index_file.eof())
	{
		try
		{
			++line_number;
			getline(index_file, line);

			if (line.length() == 0 || line[0] == '#')
			{
				continue;
			}

			size_t comma = line.find(',');

			if (comma < 1)
			{
				PrintDebug("Invalid texture index entry on line %u (missing comma?)\n", line_number);
				result = false;
				break;
			}

			uint32_t width  = 0;
			uint32_t height = 0;

			uint32_t gbix = stoul(line.substr(0, comma));
			auto name = line.substr(comma + 1);

			comma = name.find(',');

			// check for an additional texture dimensions field
			if (comma != string::npos && comma > 0)
			{
				auto tmp = name;
				name = tmp.substr(0, comma);

				auto dimensions = tmp.substr(++comma);
				size_t separator = dimensions.find('x');

				// If no 'x' separator is found, try capital
				if (!separator || separator == string::npos)
				{
					separator = dimensions.find('X');
				}

				if (!separator || separator == string::npos)
				{
					PrintDebug("Invalid format for texture dimensions on line %u: %s\n",
					           line_number, dimensions.c_str());
					break;
				}

				width  = stoul(dimensions.substr(0, separator));
				height = stoul(dimensions.substr(++separator));
			}

			auto texture_path = path;
			texture_path.append("\\");
			texture_path.append(name);

			if (!FileExists(texture_path))
			{
				PrintDebug("Texture entry on line %u does not exist: %s\n",
				           line_number, texture_path.c_str());
				result = false;
				break;
			}

			out.push_back({ gbix, name, width, height });
		}
		catch (exception& exception)
		{
			PrintDebug("An exception occurred while parsing texture index on line %u: %s\n",
			           line_number, exception.what());
			out.clear();
			return false;
		}
	}

	if (!result)
	{
		out.clear();
	}

	//raw_cache[path] = out;
	return result;
}

bool get_archive_index(const string& path, ifstream& file, vector<pvmx::DictionaryEntry>& out)
{
	PrintDebug("Loading texture pack: %s\n", path.c_str());

	/*const auto it = archive_cache.find(path);

	if (it != archive_cache.end())
	{
		out = it->second;
		return true;
	}*/

	if (!FileExists(path))
	{
		return false;
	}

	check_loading();

	if (!file.is_open())
	{
		PrintDebug("Unable to open archive file: %s\n", path.c_str());
		return false;
	}

	if (pvmx::read_index(file, out))
	{
		//archive_cache[path] = out;
		return true;
	}

	return false;
}

void check_cache()
{
	/*if (LoadingFile)
	{
		return;
	}

	if (was_loading)
	{
#ifdef _DEBUG
		PrintDebug("\tClearing cache...\n");
#endif
		raw_cache.clear();
		archive_cache.clear();
	}

	was_loading = false;*/
}

#pragma endregion

#pragma region Texture Loading

bool get_dds_header(istream& file, DDS_HEADER& header)
{
	const auto pos = file.tellg();
	uint32_t magic = 0;
	file.read(reinterpret_cast<char*>(&magic), sizeof(uint32_t));

	if (magic != DDS_MAGIC)
	{
		file.seekg(pos);
		return false;
	}

	uint32_t size = 0;
	file.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));

	if (size != sizeof(DDS_HEADER))
	{
		file.seekg(pos);
		return false;
	}

	file.seekg(-static_cast<int>(sizeof(uint32_t)), ios_base::cur);
	file.read(reinterpret_cast<char*>(&header), sizeof(DDS_HEADER));
	file.seekg(pos);
	return true;
}

/**
 * \brief Reads the header from a DDS file.
 * \param path Path to the texture file.
 * \param header Output structure.
 * \return \c true if this file is a valid DDS file and \p header has been populated.
 */
bool get_dds_header(const string& path, DDS_HEADER& header)
{
	ifstream file(path, ios::binary);
	if (!file.is_open())
		return false;
	return get_dds_header(file, header);
}

/**
 * \brief Loads a texture from the provided file stream.
 * \param file The file stream containing the texture data.
 * \param offset Offset at which the texture data starts.
 * \param size The size of the texture data.
 * \param path The path containing the texture defined by \p name.
 * \param global_index Global texture index for cache lookup.
 * \param name The name of the texture.
 * \param mipmap If \c true, automatically generate mipmaps.
 * \param width If non-zero, overrides texture width info in \c NJS_TEXMEMLIST.
 * \param height If non-zero, overrides texture height info in \c NJS_TEXMEMLIST.
 * \return A pointer to the texture, or \c nullptr on failure.
 */
NJS_TEXMANAGE* load_texture_stream(istream& file, uint64_t offset, uint64_t size,
	const string& path, uint32_t global_index, const string& name, bool mipmap, uint32_t width, uint32_t height)
{
	// TODO: Implement custom texture queue to replace the global texture array
	auto texture = GetCachedTexture(global_index);

	// GetCachedTexture will only return null if over 2048 unique textures have been loaded.
	if (texture == nullptr)
	{
		PrintDebug("Failed to allocate global texture for gbix %u (likely exceeded max global texture limit)\n",
			global_index);
		return nullptr;
	}

	uint32_t mip_levels = mipmap ? D3DX_DEFAULT : 1;
	auto texture_path = path + "\\" + name;

	if (!mipmap)
	{
		DDS_HEADER header{};

		if (get_dds_header(file, header))
		{
			mip_levels = header.mipMapCount + 1;
		}
	}

	vector<uint8_t> buffer;
	buffer.resize(static_cast<size_t>(size));

	file.seekg(static_cast<size_t>(offset));
	file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

	auto tex = reinterpret_cast<Magic::RenderCore::Texture*>(OperatorNew(sizeof(Magic::RenderCore::Texture)));
	tex = InitTexture(tex);
	int _width = 0;
	int _height = 0;
	int depth = 0;
	int format = 0;
	int flag = 0;
	if (!g_pRenderDevice->__vftable->LoadTexture(g_pRenderDevice, tex, 1, buffer.data(), buffer.size(), &_width, &_height, &depth, &format, reinterpret_cast<int*>(&mip_levels), &flag))
	{
		PrintDebug("Failed to load texture %s\n", name.c_str());
		return nullptr;
	}

	tex->m_TexType = 1;
	tex->m_Width = _width;
	tex->m_Height = _height;
	tex->m_Depth = 1;
	if (format > 0 && format <= 31)
		tex->m_Format = format;
	if (mip_levels >= 0)
		tex->m_MipmapLevelCount = mip_levels;
	tex->m_TexFlags = flag;
	tex->m_ResPool = 1;
	tex->m_TexSize = CalcTexSize(mip_levels, format, _width, _height, depth, 0);

	auto w = !width ? _width : width;
	auto h = !height ? _height : height;

	// Now we assign some basic metadata from the texture entry and D3D texture, as well as the pointer to the texture itself.
	// A few things I know are missing for sure are:
	// NJS_TEXSURFACE::Type, Depth, Format, Flags. Virtual and Physical are pretty much always null.
	texture->count = 1; // Texture reference count.
	texture->tspparam = 0;
	texture->texparam = 0;
	texture->texflag = 0;
	texture->bank = -1;
	texture->texsys->count = 1; // Texture reference count.
	texture->texsys->globalIndex = global_index;
	texture->texsys->texsurface.nWidth = w;
	texture->texsys->texsurface.nHeight = h;
	texture->texsys->texsurface.TextureSize = tex->m_TexSize;
	texture->texsys->texsurface.pSurface = reinterpret_cast<Uint32*>(tex);

	for (auto& event : modCustomTextureLoadEvents)
	{
		event(texture, texture_path.c_str(), mip_levels);
	}

	return texture;
}

/**
 * \brief 
 * \param path The texture pack directory containing the texture.
 * \param global_index Global texture index for cache lookup.
 * \param name The name of the texture.
 * \param mipmap If \c true, automatically generate mipmaps.
 * \param width If non-zero, overrides texture width info in \c NJS_TEXMEMLIST.
 * \param height If non-zero, overrides texture height info in \c NJS_TEXMEMLIST.
 * \return A pointer to the texture, or \c nullptr on failure.
 */
NJS_TEXMANAGE* load_texture(const string& path, uint32_t global_index, const string& name,
	bool mipmap, uint32_t width, uint32_t height)
{
	auto texture_path = path + "\\" + name;

	if (!FileExists(texture_path))
	{
		PrintDebug("Texture does not exist: %s\n", texture_path.c_str());
		return nullptr;
	}

	ifstream file(texture_path, ios::ate | ios::binary | ios::in);
	auto size = static_cast<uint64_t>(file.tellg());
	file.seekg(0);
	return load_texture_stream(file, 0, size, path, global_index, name, mipmap, width, height);
}

/**
 * \brief Loads the specified texture from disk, or uses a cached texture if available.
 * \param path A valid path to a texture pack directory containing index.txt.
 * \param entry The entry containing the global index and filename.
 * \param mipmap If \c true, automatically generate mipmaps.
 * \return A pointer to the texture, or \c nullptr on failure.
 */
NJS_TEXMANAGE* load_texture(const string& path, const TexPackEntry& entry, bool mipmap)
{
	return load_texture(path, entry.global_index, entry.name, mipmap, entry.width, entry.height);
}

#pragma endregion

#pragma region PVM

static vector<NJS_TEXNAME> texname_overflow;
DataArray(NJS_TEXNAME, TexNameBuffer, 0x1940870, 0x200);

inline void dynamic_expand(NJS_TEXLIST* texlist, size_t count)
{
	if (count > TexNameBuffer.size())
	{
		static const NJS_TEXNAME dummy = {};

		texname_overflow.resize(count);
		fill(texname_overflow.begin(), texname_overflow.end(), dummy);

		texlist->textures = texname_overflow.data();
	}
	else if (!texname_overflow.empty())
	{
		texname_overflow.clear();
	}

	texlist->nbTexture = count;
}

/**
 * \brief Replaces the specified PVM with a texture pack virtual PVM.
 * \param path A valid path to a texture pack directory containing index.txt.
 * \param texlist The associated texlist.
 * \return \c true on success
 */
static bool replace_pvm(const string& path, NJS_TEXLIST* texlist, unordered_map<string, TexReplaceData>& replacements)
{
	if (texlist == nullptr)
	{
		return false;
	}

	vector<TexPackEntry> index;
	if (!texpack::parse_index(path, index))
	{
		return false;
	}

	auto pvm_name = GetBaseName(path);
	StripExtension(pvm_name);

	dynamic_expand(texlist, index.size());

	transform(pvm_name.begin(), pvm_name.end(), pvm_name.begin(), ::tolower);

	string pvm_path = "resource\\" + std::string(BaseResourceFolder) + pvm_name + ".prs";
	int modIdx = sadx_fileMap.getModIndex(pvm_path.c_str());

	for (uint32_t i = 0; i < texlist->nbTexture; i++)
	{
		NJS_TEXMANAGE* texture = nullptr;
		string lower = index[i].name;
		StripExtension(lower);
		transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
		const auto& iter2 = replacements.find(lower);
		if (iter2 != replacements.cend() && iter2->second.mod_index >= modIdx)
			texture = load_texture(iter2->second.path, iter2->second, true);

		if (!texture)
			texture = load_texture(path, index[i], true);

		if (texture == nullptr)
		{
			auto nbTexture = texlist->nbTexture;
			texlist->nbTexture = i;
			FreeTexList(texlist);
			texlist->nbTexture = nbTexture;
			return false;
		}

		texlist->textures[i].texaddr = reinterpret_cast<Uint32>(texture);
	}

	return true;
}

/**
 * \brief Replaces the specified PVM with a texture pack PVMX archive.
 * \param path The path to the PVMX archive. Used for caching and error handling.
 * \param file An opened file stream for the PVMX archive.
 * \param texlist The associated texlist.
 * \return \c true on success.
 */
static bool replace_pvmx(const string& path, ifstream& file, NJS_TEXLIST* texlist, unordered_map<string, TexReplaceData>& replacements)
{
	if (texlist == nullptr)
	{
		return false;
	}

	using namespace pvmx;
	vector<DictionaryEntry> index;

	if (!get_archive_index(path, file, index))
	{
		return false;
	}

	auto pvm_name = GetBaseName(path);
	StripExtension(pvm_name);

	dynamic_expand(texlist, index.size());

	transform(pvm_name.begin(), pvm_name.end(), pvm_name.begin(), ::tolower);

	string pvm_path = "resource\\" + std::string(BaseResourceFolder) + pvm_name + ".prs";
	int modIdx = sadx_fileMap.getModIndex(pvm_path.c_str());

	for (size_t i = 0; i < index.size(); i++)
	{
		auto& entry = index[i];

		NJS_TEXMANAGE* texture = nullptr;
		auto lower = entry.name;
		StripExtension(lower);
		transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
		const auto& iter2 = replacements.find(lower);
		if (iter2 != replacements.cend() && iter2->second.mod_index >= modIdx)
			texture = load_texture(iter2->second.path, iter2->second, true);

		if (!texture)
			texture = load_texture_stream(file, entry.offset, entry.size,
			                              path, entry.global_index, entry.name, true, entry.width, entry.height);

		if (texture == nullptr)
		{
			FreeTexList(texlist);
			return false;
		}

		texlist->textures[i].texaddr = reinterpret_cast<Uint32>(texture);
	}

	return true;
}

/**
 * \brief Replaces the specified PVM with a texture pack PAK archive.
 * \param path The path to the PAK archive. Used for caching and error handling.
 * \param file An opened file stream for the PAK archive.
 * \param texlist The associated texlist.
 * \return \c true on success.
 */
static bool replace_pak(const string& path, ifstream& file, NJS_TEXLIST* texlist, unordered_map<string, TexReplaceData>& replacements)
{
	if (texlist == nullptr)
	{
		return false;
	}

	PAKFile pak(file);

	auto pvm_name = GetBaseName(path);
	StripExtension(pvm_name);

	transform(pvm_name.begin(), pvm_name.end(), pvm_name.begin(), ::tolower);

	auto infent = pak.find(pvm_name + "\\" + pvm_name + ".inf");
	if (!infent)
		return false;
	int texcnt = infent->length / sizeof(PAKTexInf);
	auto texinf = reinterpret_cast<PAKTexInf*>(infent->data);

	dynamic_expand(texlist, texcnt);

	string pvm_path = "resource\\" + std::string(BaseResourceFolder) + pvm_name + ".prs";
	int modIdx = sadx_fileMap.getModIndex(pvm_path.c_str());

	for (size_t i = 0; i < texcnt; i++)
	{
		auto& entry = texinf[i];
		char entnam[sizeof(entry.filename) + 1]{};
		memcpy(entnam, entry.filename, sizeof(entry.filename));

		NJS_TEXMANAGE* texture = nullptr;
		string lower = entnam;
		transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
		const auto& iter2 = replacements.find(lower);
		if (iter2 != replacements.cend() && iter2->second.mod_index >= modIdx)
			texture = load_texture(iter2->second.path, iter2->second, true);

		if (!texture)
		{
			auto texfil = pak.find(pvm_name + "\\" + lower + ".dds");
			if (texfil)
			{
				std::istringstream texstr(std::string(reinterpret_cast<char*>(texfil->data), texfil->length));
				texture = load_texture_stream(texstr, 0, texfil->length,
					path, entry.globalIndex, entnam, true, entry.nWidth, entry.nHeight);
				if (texture)
					memcpy(&texture->texsys->globalIndex, &entry.globalIndex, sizeof(PAKTexInf) - offsetof(PAKTexInf, globalIndex));
			}
		}

		if (texture == nullptr)
		{
			FreeTexList(texlist);
			return false;
		}

		texlist->textures[i].texaddr = reinterpret_cast<Uint32>(texture);
	}

	return true;
}

static std::string get_replaced_path(const string& filename, const char* extension)
{
	// Since the filename can be passed in with or without an extension, first
	// we try getting a replacement with the filename as-is (with SYSTEM\ prepended).
	const string system_path = "resource\\" + std::string(BaseResourceFolder) + filename;
	std::string replaced = sadx_fileMap.replaceFile(system_path.c_str());

	// But if that failed, we can assume that it was given without an extension
	// (which is the intended use) and append one before trying again.
	if (!Exists(replaced))
	{
		const string system_path_ext = system_path + extension;
		replaced = sadx_fileMap.replaceFile(system_path_ext.c_str());
	}

	return replaced;
}

static void GetReplaceTextures(const char* filename, unordered_map<string, TexReplaceData>& replacements)
{
	auto pvm_name = GetBaseName(filename);
	StripExtension(pvm_name);

	transform(pvm_name.begin(), pvm_name.end(), pvm_name.begin(), ::tolower);

	const auto& repiter = replace_cache.find(pvm_name);
	if (repiter != replace_cache.cend())
		for (const auto& iter2 : *repiter->second)
		{
			const auto& iter3 = replacements.find(iter2.first);
			if (iter3 == replacements.end() || iter3->second.mod_index <= iter2.second.mod_index)
				replacements[iter2.first] = iter2.second;
		}

	auto replaced = GetBaseName(get_replaced_path(filename, ".PRS"));
	StripExtension(replaced);

	transform(replaced.begin(), replaced.end(), replaced.begin(), ::tolower);

	if (!replaced.compare(pvm_name))
	{
		const auto& repiter = replace_cache.find(replaced);
		if (repiter != replace_cache.cend())
			for (const auto& iter2 : *repiter->second)
			{
				const auto& iter3 = replacements.find(iter2.first);
				if (iter3 == replacements.end() || iter3->second.mod_index < iter2.second.mod_index)
					replacements[iter2.first] = iter2.second;
			}
	}
}

static bool try_texture_pack(const char* filename, NJS_TEXLIST* texlist)
{
	string filename_str(filename);
	StripExtension(filename_str);

	check_cache();
	//LoadingFile = true;

	const string replaced = get_replaced_path(filename_str, ".PRS");

	// If we can ensure this path exists, we can determine how to load it.
	if (!Exists(replaced))
	{
		return false;
	}

	unordered_map<string, TexReplaceData> replacements;
	GetReplaceTextures(filename, replacements);

	// If the replaced path is a file, we should check if it's a PVMX archive.
	if (IsFile(replaced))
	{
		ifstream pvmx(replaced, ios::in | ios::binary);
		if (pvmx::is_pvmx(pvmx))
			return replace_pvmx(replaced, pvmx, texlist, replacements);
		return PAKFile::is_pak(pvmx) && replace_pak(replaced, pvmx, texlist, replacements);
	}

	// Otherwise it's probably a directory, so try loading it as a texture pack.
	return replace_pvm(replaced, texlist, replacements);
}

int __cdecl LoadPAKTextures_r(const char* filename, NJS_TEXLIST* texlist)
{
	auto temp = *texlist;

	if (try_texture_pack(filename, &temp))
	{
		*texlist = temp;
		return 1;
	}
	return false;
}

static void ReplacePVMTexs(const string& filename, NJS_TEXLIST* texlist, const void* pvmdata, unordered_map<string, TexReplaceData>& replacements, bool mipmap)
{
	unsigned short flags = _byteswap_ushort(((const unsigned short*)pvmdata)[4]);
	int entrysize = 2;
	if (flags & 1) // global index
		entrysize += 4;
	if (flags & 2) // dimensions
		entrysize += 2;
	if (flags & 4) // format
		entrysize += 2;
	if (flags & 8) // filenames
		entrysize += 28;
	else
		return; // nothing we can do

	int modIdx = sadx_fileMap.getModIndex(filename.c_str());

	short numtex = _byteswap_ushort(((const unsigned short*)pvmdata)[5]);
	const char* entry = (const char*)pvmdata + 0xE;
	char fnbuf[29]{}; // extra null terminator at end
	vector<pair<NJS_TEXNAME&, TexReplaceData&>> texreps;
	for (int i = 0; i < numtex; i++)
	{
		memcpy(fnbuf, entry, 28);
		string tfn = fnbuf;
		transform(tfn.begin(), tfn.end(), tfn.begin(), ::tolower);
		const auto& iter2 = replacements.find(tfn);
		if (iter2 != replacements.cend() && iter2->second.mod_index >= modIdx)
		{
			auto v5 = reinterpret_cast<NJS_TEXMANAGE*>(texlist->textures[i].texaddr);
			if (v5)
			{
				auto v6 = v5->texsys;
				if (v6)
				{
					auto v7 = v5->count;
					if (v7 > 0)
					{
						v5->count = v7 - 1;
					}
					if (v6->count > 0)
					{
						--v6->count;
						auto v8 = v5->texsys;
						if (!v8->count)
						{
							auto v9 = reinterpret_cast<Magic::RenderCore::Texture*>(v8->texsurface.pSurface);
							if (v9)
							{
								v9->__vftable->Destructor(v9, 1);
								v8->texsurface.pSurface = 0;
							}
						}
					}
					auto v10 = v5->texsys;
					if (v10->count <= 0)
					{
						v10->globalIndex = -1;
						v10->texsurface.Type = 0;
						v10->texsurface.BitDepth = 0;
						v10->texsurface.PixelFormat = 0;
						v10->texsurface.nWidth = 0;
						v10->texsurface.nHeight = 0;
						v10->texsurface.TextureSize = 0;
						v10->texsurface.fSurfaceFlags = 0;
						v10->texsurface.pSurface = 0;
					}
					if (v5->count <= 0)
					{
						v5->bank = -1;
						v5->tspparam = 0;
						v5->texparam = 0;
						v5->texsys = 0;
						v5->texflag = 0;
					}
				}
			}
			texreps.push_back({ texlist->textures[i], iter2->second });
		}
		entry += entrysize;
	}
	for (auto& i : texreps)
		i.first.texaddr = reinterpret_cast<Uint32>(load_texture(i.second.path, i.second, mipmap));
}

int __cdecl LoadPRSTextures_r(NJS_TEXLIST* texlist, const char* filename)
{
	const std::string replaced = get_replaced_path(filename, ".PRS");
	const std::string replaced_extension = GetExtension(replaced);

	unordered_map<string, TexReplaceData> replacements;
	GetReplaceTextures(filename, replacements);

	if (LoadPRSTextures.Original(texlist, filename))
	{
		ReplacePVMTexs(replaced, texlist, &SomeBuffer, replacements, false);
		return true;
	}
	return false;
}

int __cdecl LoadEventPRSTextures_r(NJS_TEXLIST* texlist, const char* filename, void* buffer)
{
	const std::string replaced = get_replaced_path(filename, ".PRS");
	const std::string replaced_extension = GetExtension(replaced);

	unordered_map<string, TexReplaceData> replacements;
	GetReplaceTextures(filename, replacements);

	if (LoadEventPRSTextures.Original(texlist, filename, buffer))
	{
		ReplacePVMTexs(replaced, texlist, buffer, replacements, false);
		return true;
	}
	return false;
}

#pragma endregion

#pragma region PVR

void LoadTextureList_NoName_r(NJS_TEXLIST* texlist)
{
	NJS_TEXNAME* v1; // eax
	NJS_TEXINFO* v2; // ecx
	Uint32 v3; // esi
	NJS_TEXMANAGE* v13; // esi
	int v14; // ecx
	signed __int32 i; // [esp+28h] [ebp-1C0h]

	for (i = 0; i < texlist->nbTexture; ++i)
	{
		v1 = &texlist->textures[i];
		if ((v1->attr & NJD_TEXATTR_TYPE_MEMORY) != 0)
		{
			v2 = reinterpret_cast<NJS_TEXINFO*>(v1->filename);
			v3 = v1->texaddr;
			v13 = GetCachedTexture(v3);
			v13->texsys->texsurface.Type = v2->texsurface.Type;
			v13->texsys->texsurface.BitDepth = 0;
			if (v2->texsurface.Type)
			{
				v14 = v2->texsurface.Type & 0xF;
			}
			else
			{
				v14 = 5;
			}
			v13->texsys->texsurface.PixelFormat = v14;
			v13->texsys->texsurface.nWidth = v2->texsurface.nWidth;
			v13->texsys->texsurface.nHeight = v2->texsurface.nHeight;
			v13->texsys->texsurface.TextureSize = 0;
			v13->texsys->texsurface.fSurfaceFlags = 0;
			v13->texsys->texsurface.pSurface = 0;
			v13->texsys->globalIndex = v3;
			v13->texsys->count = 1;
			v13->count = 1;
			v13->tspparam = 0;
			v13->texparam = 0;
			v13->bank = 0;
			v13->texflag = 0;
			LoadTexPixelData(v13, v2->texaddr);
			v1->texaddr = reinterpret_cast<Uint32>(v13);
		}
		else
		{
			const string replaced = get_replaced_path(reinterpret_cast<const char*>(v1->filename), ".GVR");
			string ext = GetExtension(replaced);
			transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
			if (ext == "txt")
			{
				string filename_str = reinterpret_cast<const char*>(v1->filename);
				transform(filename_str.begin(), filename_str.end(), filename_str.begin(), tolower);

				vector<TexPackEntry> entries;
				if (!texpack::parse_index(replaced, entries))
					continue;

				for (const auto& i : entries)
				{
					const auto& name = i.name;

					const auto dot = name.find_last_of('.');
					if (dot == string::npos)
					{
						continue;
					}

					// Get the filename portion of the path.
					auto slash = name.find_last_of("/\\");
					slash = (slash == string::npos ? 0 : (slash + 1));
					if (slash > dot)
					{
						// Should not happen, but this usually means the
						// dot is part of some other path component, not
						// the filename.
						continue;
					}

					string texture_name = name.substr(slash, dot - slash);
					transform(texture_name.begin(), texture_name.end(), texture_name.begin(), tolower);

					if (filename_str != texture_name)
					{
						continue;
					}

					v1->texaddr = reinterpret_cast<Uint32>(load_texture(GetDirectory(replaced), i.global_index, name,
						true, i.width, i.height));
				}
			}
			else if (ext == "pak")
			{
				PAKFile pak(replaced);

				auto pvm_name = GetBaseName(replaced);
				StripExtension(pvm_name);
				transform(pvm_name.begin(), pvm_name.end(), pvm_name.begin(), ::tolower);

				auto infent = pak.find(pvm_name + "\\" + pvm_name + ".inf");
				if (!infent)
					continue;
				auto texinf = reinterpret_cast<PAKTexInf*>(infent->data);
				auto texfil = pak.find(pvm_name + "\\" + pvm_name + ".dds");
				if (texfil)
				{
					std::istringstream texstr(std::string(reinterpret_cast<char*>(texfil->data), texfil->length));
					NJS_TEXMANAGE* texture = load_texture_stream(texstr, 0, texfil->length,
						GetDirectory(replaced), texinf->globalIndex, pvm_name, true, texinf->nWidth, texinf->nHeight);
					if (texture)
						memcpy(&texture->texsys->globalIndex, &texinf->globalIndex, sizeof(PAKTexInf) - offsetof(PAKTexInf, globalIndex));
					v1->texaddr = reinterpret_cast<Uint32>(texture);
				}
			}
			else
			{
				auto file = fopen(replaced.c_str(), "rb");
				fseek(file, 0, SEEK_END);
				auto len = ftell(file);
				fseek(file, 0, SEEK_SET);
				char* buf = new char[len];
				fread(buf, len, 1, file);
				fclose(file);
				v1->texaddr = reinterpret_cast<Uint32>(LoadGVRData(v1->texaddr, buf));
				delete[] buf;
			}
		}
	}
}

#pragma endregion
