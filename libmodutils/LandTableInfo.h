#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <cstdint>
#include <SA2ModLoader.h>

enum LandTableFormat
{
	LandTableFormat_Invalid,
	LandTableFormat_SA2,
	LandTableFormat_SA2B
};

class LandTableInfo
{
public:
	struct Metadata { uint32_t size; const uint8_t *data; };

	LandTableInfo(const char *filename);
	LandTableInfo(const wchar_t *filename);
	LandTableInfo(const std::string &filename);
	LandTableInfo(const std::wstring &filename);
	LandTableInfo(std::istream &stream);

	LandTableFormat getformat();
	LandTable *getlandtable();
	const std::string &getauthor();
	const std::string &gettool();
	const std::string &getdescription();
	const uint8_t *getmetadata(uint32_t identifier, uint32_t &size);
	const std::string &getlabel(void *data);
	void *getdata(const std::string &label);

private:
	static const uint64_t SA2LVL = 0x4C564C324153u;
	static const uint64_t SA2BLVL = 0x4C564C42324153u;
	static const uint64_t FormatMask = 0xFFFFFFFFFFFFFFu;
	static const uint8_t CurrentVersion = 3;
	static const int headersize = 0x10;

	LandTableFormat format;
	LandTable *landtable;
	std::string author, tool, description;
	std::unordered_map<uint32_t, Metadata> metadata;
	std::unordered_map<void *, std::string> labels1;
	std::unordered_map<std::string, void *> labels2;
	std::vector<std::shared_ptr<void>> allocatedmem;
	std::unordered_set<void *> fixedpointers;

	enum ChunkTypes : uint32_t
	{
		ChunkTypes_Label = 0x4C42414C,
		ChunkTypes_Author = 0x48545541,
		ChunkTypes_Tool = 0x4C4F4F54,
		ChunkTypes_Description = 0x43534544,
		ChunkTypes_End = 0x444E45
	};

	void fixbasicmodelpointers(NJS_MODEL *model, intptr_t base);
	void fixchunkmodelpointers(NJS_CNK_MODEL *model, intptr_t base);
	void fixsa2bmodelpointers(SA2B_Model *model, intptr_t base);
	void fixobjectpointers(NJS_OBJECT *object, intptr_t base, bool chunk);
	void fixlandtablepointers(LandTable *landtable, intptr_t base);
	void init(std::istream &stream);
};