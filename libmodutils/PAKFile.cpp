#include "stdafx.h"
#include <algorithm>
#include "PAKFile.h"

PAKFile::PAKFile(const char* filename)
{
	std::ifstream str(filename, std::ios::binary);
	init(str);
	str.close();
}

PAKFile::PAKFile(const wchar_t* filename)
{
	std::ifstream str(filename, std::ios::binary);
	init(str);
	str.close();
}

PAKFile::PAKFile(const std::string& filename)
{
	std::ifstream str(filename, std::ios::binary);
	init(str);
	str.close();
}

PAKFile::PAKFile(const std::wstring& filename)
{
	std::ifstream str(filename, std::ios::binary);
	init(str);
	str.close();
}

PAKFile::PAKFile(std::istream& stream)
{
	init(stream);
}

PAKFile::~PAKFile()
{
	for (auto& entry : entries)
		delete[] entry.second.data;
}

template<typename T>
static inline void readdata(std::istream& stream, T& data)
{
	stream.read(reinterpret_cast<char*>(&data), sizeof(T));
}

bool PAKFile::is_pak(std::istream& file)
{
	const auto pos = file.tellg();
	const auto result = check_header(file);
	file.seekg(pos);

	return result;
}

bool PAKFile::is_pak(const char* filename)
{
	std::ifstream file(filename, std::ios::binary);
	return check_header(file);
}

bool PAKFile::is_pak(const wchar_t* filename)
{
	std::ifstream file(filename, std::ios::binary);
	return check_header(file);
}

bool PAKFile::is_pak(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);
	return check_header(file);
}

bool PAKFile::is_pak(const std::wstring& filename)
{
	std::ifstream file(filename, std::ios::binary);
	return check_header(file);
}

const PAKFile::datatype* PAKFile::data() const
{
	return &entries;
}

PAKFile::iterator PAKFile::begin() const
{
	return entries.begin();
}

PAKFile::iterator PAKFile::end() const
{
	return entries.end();
}

const PAKFile::Entry* PAKFile::find(std::string& name) const
{
	auto& entry = entries.find(name);
	if (entry == end())
		return nullptr;
	return &entry->second;
}

bool PAKFile::check_header(std::istream& stream)
{
	uint32_t magic;
	readdata(stream, magic);

	return magic == PAKFile::magic;
}

void PAKFile::init(std::istream& stream)
{
	if (!check_header(stream))
		return;
	stream.seekg(0x35, std::ios::cur);
	int numfiles;
	readdata(stream, numfiles);
	std::vector<std::string> paths;
	paths.reserve(numfiles);
	std::vector<std::string> names;
	names.reserve(numfiles);
	std::vector<int> lens;
	lens.reserve(numfiles);
	int strlen;
	for (int i = 0; i < numfiles; ++i)
	{
		readdata(stream, strlen);
		std::string path(strlen, 0);
		stream.read(&path[0], strlen);
		paths.push_back(path);
		readdata(stream, strlen);
		std::string name(strlen, 0);
		stream.read(&name[0], strlen);
		transform(name.begin(), name.end(), name.begin(), ::tolower);
		names.push_back(name);
		int len;
		readdata(stream, len);
		lens.push_back(len);
		stream.seekg(4, std::ios::cur);
	}
	for (int i = 0; i < numfiles; ++i)
	{
		char* data = new char[lens[i]];
		stream.read(data, lens[i]);
		entries.insert({ names[i], { paths[i], data, lens[i] } });
	}
}
