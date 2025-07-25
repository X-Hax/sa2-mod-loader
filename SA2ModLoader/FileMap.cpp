/**
 * SADX Mod Loader
 * File remapper.
 */

#include "stdafx.h"
#include "FileMap.hpp"

#include <cctype>
#include <algorithm>

using std::list;
using std::string;
using std::transform;
using std::unordered_map;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Shlwapi.h>
#include "FileSystem.h"
#include "TextureReplacement.h"

/**
 * Replace slash characters with backslashes.
 * @param c Character.
 * @return If c == '/', '\\'; otherwise, c.
 */
inline int backslashes(int c)
{
	return c == '/' ? '\\' : c;
}

/**
 * Normalize a filename for the file replacement map.
 * @param filename Filename.
 * @return Normalized filename.
 */
string FileMap::normalizePath(const string& filename)
{
	return normalizePath(filename.c_str());
}

/**
 * Normalize a filename for the file replacement map.
 * @param filename Filename.
 * @return Normalized filename.
 */
string FileMap::normalizePath(const char* filename)
{
	string path = filename;
	transform(path.begin(), path.end(), path.begin(), backslashes);
	if (path.length() > 2 && (path[0] == '.' && path[1] == '\\'))
		path = path.substr(2, path.length() - 2);
	transform(path.begin(), path.end(), path.begin(), ::tolower);
	return path;
}

/**
 * Ignore a file.
 * @param ignoreFile File to ignore.
 * @param modIdx Index of the current mod.
 */
void FileMap::addIgnoreFile(const string& ignoreFile, int modIdx)
{
	string path = normalizePath(ignoreFile);
	m_fileMap[path] = { "nullfile", modIdx };
}

/**
 * Add a file replacement.
 * @param origFile Original filename.
 * @param modFile Mod filename.
 * @param modIndex Optional mod index.
 */
void FileMap::addReplaceFile(const std::string& origFile, const std::string& modFile, const int modIndex)
{
	string origFile_norm = normalizePath(origFile);

	// Check if the source file is already being replaced, and reject the new replacement if the current index is higher.
	auto iter = m_fileMap.find(origFile_norm);
	if (iter != m_fileMap.end() && modIndex < iter->second.modIndex)
		return;

	string modFile_norm = normalizePath(modFile);

	// Check if the destination file is being replaced.
	iter = m_fileMap.find(modFile_norm);
	if (iter != m_fileMap.end())
	{
		// Destination file is being replaced.
		// Use the replacement for the original file.
		setReplaceFile(origFile_norm, iter->second.fileName, iter->second.modIndex);
	}
	else
	{
		// Destination file is not already in the map.
		setReplaceFile(origFile_norm, modFile_norm, modIndex);
	}
}

/**
 * Remove a file replacement.
 * @param file Filename.
 */
void FileMap::unreplaceFile(const std::string& file)
{
	string file_norm = normalizePath(file);
	m_fileMap.erase(file_norm);
}

/**
* Swap two files.
* @param fileA First filename.
* @param fileB Second filename.
*/
void FileMap::swapFiles(const std::string& fileA, const std::string& fileB)
{
	string fileA_norm = normalizePath(fileA);
	string fileB_norm = normalizePath(fileB);

	string fileA_repl = fileB_norm;
	int fileA_ind = -1;
	string fileB_repl = fileA_norm;
	int fileB_ind = -1;

	// Check if the destination file is being replaced.
	auto iter = m_fileMap.find(fileA_repl);
	if (iter != m_fileMap.end())
	{
		// Destination file is being replaced.
		// Use the replacement for the original file.
		fileA_repl = iter->second.fileName;
		fileA_ind = iter->second.modIndex;
	}
	iter = m_fileMap.find(fileB_repl);
	if (iter != m_fileMap.end())
	{
		// Destination file is being replaced.
		// Use the replacement for the original file.
		fileB_repl = iter->second.fileName;
		fileB_ind = iter->second.modIndex;
	}

	setReplaceFile(fileA_norm, fileA_repl, fileA_ind);
	setReplaceFile(fileB_norm, fileB_repl, fileB_ind);
}

/**
 * Recursively scan a directory and add all files to the replacement map.
 * Destination is always relative to system/.
 * @param srcPath Path to scan.
 * @param modIdx Index of the current mod.
 */
void FileMap::scanFolder(const string& srcPath, int modIdx)
{
	scanFolder_int(srcPath, srcPath.length() + 1, modIdx);
}

void FileMap::scanPRSFolder(const string& srcPath)
{
	scanPRSFolder_int(srcPath, srcPath.length() + 1, -1);
}

/**
 * Recursively scan a directory and add all files to the replacement map.
 * Destination is always relative to system/.
 * (Internal recursive function)
 * @param srcPath Path to scan.
 * @param srcLen Length of original srcPath. (used for recursion)
 * @param modIdx Index of the current mod.
 */
void FileMap::scanFolder_int(const string& srcPath, int srcLen, int modIdx)
{
	WIN32_FIND_DATAA data;
	char path[MAX_PATH];
	snprintf(path, sizeof(path), "%s\\*", srcPath.c_str());
	HANDLE hFind = FindFirstFileA(path, &data);

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

		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (!_stricmp(data.cFileName, "prs"))
				scanPRSFolder_int(srcPath, srcLen, modIdx);
			// Recursively scan this directory.
			const string newSrcPath = srcPath + '\\' + string(data.cFileName);
			scanFolder_int(newSrcPath, srcLen, modIdx);
			continue;
		}

		// Create the mod filename and original filename.
		string modFile = srcPath + '\\' + string(data.cFileName);
		transform(modFile.begin(), modFile.end(), modFile.begin(), ::tolower);

		// Original filename.
		string origFile = "resource\\gd_pc\\" + modFile.substr(srcLen);

		if (!origFile.compare(0, 25, "resource\\gd_pc\\event_adx\\") ||
			!origFile.compare(0, 27, "resource\\gd_pc\\event_adx_e\\"))
		{
			// Original filename should have a ".ahx" extension.
			ReplaceFileExtension(origFile, ".ahx");
		}

		setReplaceFile(origFile, modFile, modIdx);
	} while (FindNextFileA(hFind, &data) != 0);

	FindClose(hFind);
}

void FileMap::scanPRSFolder_int(const string& srcPath, int srcLen, int modIdx)
{
	WIN32_FIND_DATAA data;
	char path[MAX_PATH];
	snprintf(path, sizeof(path), "%s\\prs\\*", srcPath.c_str());
	HANDLE hFind = FindFirstFileA(path, &data);

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

		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			continue;
		}

		// Create the mod filename and original filename.
		string modFile = srcPath + "\\prs\\" + string(data.cFileName);
		transform(modFile.begin(), modFile.end(), modFile.begin(), ::tolower);

		// Original filename.
		string origFile = "resource\\gd_pc\\";
		if (srcPath.size() > srcLen)
			origFile += srcPath.substr(srcLen) + '\\';
		origFile += data.cFileName;
		transform(origFile.begin(), origFile.end(), origFile.begin(), ::tolower);

		ReplaceFileExtension(origFile, ".prs");
		setReplaceFile(origFile, modFile, modIdx);

		ReplaceFileExtension(origFile, ".gvr");
		setReplaceFile(origFile, modFile, modIdx);
	} while (FindNextFileA(hFind, &data) != 0);

	FindClose(hFind);
}

void FileMap::scanTextureFolder(const string& srcPath, int modIndex)
{
	WIN32_FIND_DATAA data;
	char path[MAX_PATH];
	snprintf(path, sizeof(path), "%s\\*", srcPath.c_str());
	auto hFind = FindFirstFileA(path, &data);

	string lower = srcPath;
	transform(lower.begin(), lower.end(), lower.begin(), tolower);
	std::vector<TexPackEntry> entries;

	// First attempt to parse the root texture pack (for PVR files in the vanilla system folder).
	// If the path isn't a file or doesn't exist, this function will return false.
	if (texpack::parse_index(lower, entries))
	{
		for (const auto& i : entries)
		{
			auto name = i.name;
			// Remove the file extension.
			auto dot = name.find('.');
			name.resize(dot);

			auto original = "resource\\gd_pc\\" + name + ".gvr";
			transform(original.begin(), original.end(), original.begin(), tolower);

			auto index_path = srcPath + "\\index.txt";
			transform(index_path.begin(), index_path.end(), index_path.begin(), tolower);

			m_fileMap[original] = { index_path, modIndex };
		}
	}

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

		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && _stricmp(data.cFileName, "event"))
		{
			string original = "resource\\gd_pc\\" + fileName + ".prs";
			transform(original.begin(), original.end(), original.begin(), ::tolower);

			string texPack = srcPath + '\\' + fileName;
			transform(texPack.begin(), texPack.end(), texPack.begin(), ::tolower);

			// Since we don't attempt to parse this file, make sure it exists
			// before registering an empty texture pack directory.
			if (FileExists(texPack + "\\index.txt"))
			{
				m_fileMap[original] = { texPack, modIndex };
			}
		}
		else
		{
			string ext = GetExtension(fileName);
			transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

			if (ext != "pvmx")
			{
				continue;
			}

			string noExt = fileName;
			StripExtension(noExt);

			string original = "resource\\gd_pc\\" + noExt + ".prs";
			transform(original.begin(), original.end(), original.begin(), ::tolower);

			string texPack = srcPath + '\\' + fileName;
			transform(texPack.begin(), texPack.end(), texPack.begin(), ::tolower);

			m_fileMap[original] = { texPack, modIndex };
		}
	} while (FindNextFileA(hFind, &data) != 0);

	FindClose(hFind);

	snprintf(path, sizeof(path), "%s\\event\\*", srcPath.c_str());
	hFind = FindFirstFileA(path, &data);

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
			string original = "resource\\gd_pc\\event\\" + fileName + ".prs";
			transform(original.begin(), original.end(), original.begin(), ::tolower);

			string texPack = srcPath + "\\event\\" + fileName;
			transform(texPack.begin(), texPack.end(), texPack.begin(), ::tolower);

			// Since we don't attempt to parse this file, make sure it exists
			// before registering an empty texture pack directory.
			if (FileExists(texPack + "\\index.txt"))
			{
				m_fileMap[original] = { texPack, modIndex };
			}
		}
		else
		{
			string ext = GetExtension(fileName);
			transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

			if (ext != "pvmx")
			{
				continue;
			}

			string noExt = fileName;
			StripExtension(noExt);

			string original = "resource\\gd_pc\\event\\" + noExt + ".prs";
			transform(original.begin(), original.end(), original.begin(), ::tolower);

			string texPack = srcPath + "\\event\\" + fileName;
			transform(texPack.begin(), texPack.end(), texPack.begin(), ::tolower);

			m_fileMap[original] = { texPack, modIndex };
		}
	} while (FindNextFileA(hFind, &data) != 0);

	FindClose(hFind);
}

/**
 * Set a replacement file in the map.
 * Filenames must already be normalized!
 * (Internal function; handles memory allocation)
 * @param origFile Original file.
 * @param destFile Replacement filename.
 * @param modIdx Index of the current mod.
 */
void FileMap::setReplaceFile(const std::string& origFile, const std::string& destFile, int modIdx)
{
	// Update the main map.
	m_fileMap[origFile] = { destFile, modIdx };

#ifdef _DEBUG
	PrintDebug("Replaced file: \"%s\" = \"%s\"\n", origFile.c_str(), destFile.c_str());
#endif
}

/**
 * Get a filename from the file replacement map.
 * @param lpFileName Filename.
 * @return Replaced filename, or original filename if not replaced by a mod.
 */
const char* FileMap::replaceFile(const char* lpFileName) const
{
	// Check if the normalized filename is in the file replacement map.
	string path = normalizePath(lpFileName);
	if (path.length() > sa2dir.length() && path.compare(0, sa2dir.length(), sa2dir) == 0)
		path = path.substr(sa2dir.length(), path.length() - sa2dir.length());
	auto iter = m_fileMap.find(path);
	if (iter != m_fileMap.cend())
	{
		const string& newFileName = iter->second.fileName;
		return newFileName.c_str();
	}

	// File was not replaced by a mod.
	// Return the filename as-is.
	return lpFileName;
}

/**
 * Get a filename from the file replacement map.
 * @param[in] lpFileName Filename.
 * @param[out] modIndex Index of the mod that replaced a file, or -1 if no mod replaced it.
 * @return Replaced filename, or original filename if not replaced by a mod.
 */
const char* FileMap::replaceFile(const char* lpFileName, int& modIndex) const
{
	// Check if the normalized filename is in the file replacement map.
	string path = normalizePath(lpFileName);
	if (path.length() > sa2dir.length() && path.compare(0, sa2dir.length(), sa2dir) == 0)
		path = path.substr(sa2dir.length(), path.length() - sa2dir.length());
	auto iter = m_fileMap.find(path);
	if (iter != m_fileMap.cend())
	{
		modIndex = iter->second.modIndex;
		const string& newFileName = iter->second.fileName;
		return newFileName.c_str();
	}

	// File was not replaced by a mod.
	// Return the filename as-is.
	modIndex = -1;
	return lpFileName;
}

/**
 * Get the index of the mod that replaced a given file.
 * @param lpFileName Filename.
 * @return Index of the mod that replaced a file, or -1 if no mod replaced it.
 */
int FileMap::getModIndex(const char* lpFileName) const
{
	// Check if the normalized filename is in the file replacement map.
	string path = normalizePath(lpFileName);
	auto iter = m_fileMap.find(path);
	if (iter != m_fileMap.cend())
	{
		return iter->second.modIndex;
	}

	// File was not replaced by a mod.
	return -1;
}

/**
 * Clear the file replacement map.
 */
void FileMap::clear()
{
	m_fileMap.clear();
}

void FileMap::setSA2Dir(string dir)
{
	sa2dir = dir;
}