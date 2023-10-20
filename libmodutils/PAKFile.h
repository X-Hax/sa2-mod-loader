#pragma once
class PAKFile
{
	struct Entry
	{
		std::string long_path;
		void* data;
		int length;
	};

	using datatype = std::unordered_map<std::string, Entry>;
	using iterator = datatype::const_iterator;

public:
	PAKFile(const char* filename);
	PAKFile(const wchar_t* filename);
	PAKFile(const std::string& filename);
	PAKFile(const std::wstring& filename);
	PAKFile(std::istream& stream);

	~PAKFile();

	static bool is_pak(const char* filename);
	static bool is_pak(const wchar_t* filename);
	static bool is_pak(const std::string& filename);
	static bool is_pak(const std::wstring& filename);
	static bool is_pak(std::istream& stream);

	const datatype* data() const;

	iterator begin() const;
	iterator end() const;
	const Entry* find(std::string& name) const;

private:
	static const uint32_t magic = 0x6B617001;
	datatype entries;
	static bool check_header(std::istream& stream);
	void init(std::istream& stream);
};

