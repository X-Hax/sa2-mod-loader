#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <cstdint>
#include "ninja.h"

class AnimationFile
{
public:
	AnimationFile(const char *filename);
	AnimationFile(const wchar_t *filename);
	AnimationFile(const std::string &filename);
	AnimationFile(const std::wstring &filename);
	AnimationFile(std::istream &stream);

	NJS_MOTION *getmotion() const;
	int getmodelcount() const;
	bool isshortrot() const;
	const std::string &getlabel() const;

private:
	static const uint64_t SAANIM = 0x4D494E414153u;
	static const uint64_t FormatMask = 0xFFFFFFFFFFFFu;
	static const uint8_t CurrentVersion = 1;
	static const int headersize = 0x14;

	NJS_MOTION *motion;
	int modelcount;
	bool shortrot;
	std::string label;
	std::vector<std::shared_ptr<void>> allocatedmem;
	std::unordered_set<void *> fixedpointers;

	void init(std::istream &stream);
};