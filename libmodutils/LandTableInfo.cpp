#include "stdafx.h"
#include "LandTableInfo.h"
#include <fstream>
#include <iostream>
using std::default_delete;
using std::ifstream;
using std::ios;
using std::istream;
using std::list;
using std::shared_ptr;
using std::streamoff;
using std::string;
#ifdef _MSC_VER
using std::wstring;
#endif /* _MSC_VER */

LandTableInfo::LandTableInfo(const char *filename)
{
	ifstream str(filename, ios::binary);
	init(str);
	str.close();
}

LandTableInfo::LandTableInfo(const wchar_t *filename)
{
	ifstream str(filename, ios::binary);
	init(str);
	str.close();
}

LandTableInfo::LandTableInfo(const string &filename)
{
	ifstream str(filename, ios::binary);
	init(str);
	str.close();
}

LandTableInfo::LandTableInfo(const wstring &filename)
{
	ifstream str(filename, ios::binary);
	init(str);
	str.close();
}

LandTableInfo::LandTableInfo(istream &stream) { init(stream); }

LandTable *LandTableInfo::getlandtable() { return landtable; }

const string &LandTableInfo::getauthor() { return author; }

const string &LandTableInfo::gettool() { return tool; }

const string &LandTableInfo::getdescription() { return description; }

const uint8_t *LandTableInfo::getmetadata(uint32_t identifier, uint32_t &size)
{
	auto elem = metadata.find(identifier);
	if (elem == metadata.end())
	{
		size = 0;
		return nullptr;
	}
	else
	{
		size = elem->second.size;
		return elem->second.data;
	}
}

static const string empty;
const string &LandTableInfo::getlabel(void *data)
{
	auto elem = labels1.find(data);
	if (elem == labels1.end())
		return empty;
	else
		return elem->second;
}

void *LandTableInfo::getdata(const string &label)
{
	auto elem = labels2.find(label);
	if (elem == labels2.end())
		return nullptr;
	else
		return elem->second;
}

static string getstring(istream &stream)
{
	auto start = stream.tellg();
	while (stream.get() != 0)
		;
	auto size = stream.tellg() - start;
	char *buf = new char[(unsigned int)size];
	stream.seekg(start);
	stream.read(buf, size);
	string result(buf);
	delete[] buf;
	return result;
}

template<typename T>
static inline void fixptr(T *&ptr, intptr_t base)
{
	if (ptr != nullptr)
		ptr = (T *)((uint8_t *)ptr + base);
}

void LandTableInfo::fixbasicmodelpointers(NJS_MODEL *model, intptr_t base)
{
	fixptr(model->points, base);
	fixptr(model->normals, base);
	if (model->meshsets != nullptr)
	{
		fixptr(model->meshsets, base);
		if (fixedpointers.find(model->meshsets) == fixedpointers.end())
		{
			fixedpointers.insert(model->meshsets);
			for (int i = 0; i < model->nbMeshset; i++)
			{
				fixptr(model->meshsets[i].meshes, base);
				fixptr(model->meshsets[i].attrs, base);
				fixptr(model->meshsets[i].normals, base);
				fixptr(model->meshsets[i].vertcolor, base);
				fixptr(model->meshsets[i].vertuv, base);
			}
		}
	}
	fixptr(model->mats, base);
}

void LandTableInfo::fixchunkmodelpointers(NJS_CNK_MODEL *model, intptr_t base)
{
	fixptr(model->vlist, base);
	fixptr(model->plist, base);
}

void LandTableInfo::fixobjectpointers(NJS_OBJECT *object, intptr_t base, bool chunk)
{
	if (object->model != nullptr)
	{
		object->model = (uint8_t *)object->model + base;
		if (fixedpointers.find(object->model) == fixedpointers.end())
		{
			fixedpointers.insert(object->model);
			if (chunk)
				fixchunkmodelpointers(object->chunkmodel, base);
			else
				fixbasicmodelpointers(object->basicmodel, base);
		}
	}
	if (object->child != nullptr)
	{
		object->child = (NJS_OBJECT *)((uint8_t *)object->child + base);
		if (fixedpointers.find(object->child) == fixedpointers.end())
		{
			fixedpointers.insert(object->child);
			fixobjectpointers(object->child, base, chunk);
		}
	}
	if (object->sibling != nullptr)
	{
		object->sibling = (NJS_OBJECT *)((uint8_t *)object->sibling + base);
		if (fixedpointers.find(object->sibling) == fixedpointers.end())
		{
			fixedpointers.insert(object->sibling);
			fixobjectpointers(object->sibling, base, chunk);
		}
	}
}

void LandTableInfo::fixlandtablepointers(LandTable *landtable, intptr_t base)
{
	if (landtable->COLList != nullptr)
	{
		landtable->COLList = (COL *)((uint8_t *)landtable->COLList + base);
		for (int i = 0; i < landtable->COLCount; i++)
			if (landtable->COLList[i].Model != nullptr)
			{
				landtable->COLList[i].Model = (NJS_OBJECT *)((uint8_t *)landtable->COLList[i].Model + base);
				if (fixedpointers.find(landtable->COLList[i].Model) == fixedpointers.end())
				{
					fixedpointers.insert(landtable->COLList[i].Model);
					bool chunk = false;
					if (landtable->ChunkModelCount != -1)
						chunk = i < landtable->ChunkModelCount;
					else
						chunk = landtable->COLList[i].Flags < 0;
					fixobjectpointers(landtable->COLList[i].Model, base, chunk);
				}
			}
	}
	fixptr(landtable->TextureName, base);
}

template<typename T>
static inline void readdata(istream &stream, T &data)
{
	stream.read((char *)&data, sizeof(T));
}

void LandTableInfo::init(istream &stream)
{
	uint64_t magic;
	readdata(stream, magic);
	uint8_t version = magic >> 56;
	magic &= FormatMask;
	if (version != CurrentVersion) // unrecognized file version
		return;
	if (magic != SA2LVL)
		return;
	uint32_t landtableoff;
	readdata(stream, landtableoff);
	landtableoff -= headersize;
	uint32_t tmpaddr;
	readdata(stream, tmpaddr);
	int mdlsize = tmpaddr - headersize;
	uint8_t *landtablebuf = new uint8_t[mdlsize];
	allocatedmem.push_back(shared_ptr<uint8_t>(landtablebuf, default_delete<uint8_t[]>()));
	stream.read((char *)landtablebuf, mdlsize);
	landtable = (LandTable *)(landtablebuf + landtableoff);
	intptr_t landtablebase = (intptr_t)landtablebuf - headersize;
	fixlandtablepointers(landtable, landtablebase);
	fixedpointers.clear();
	uint32_t chunktype;
	readdata(stream, chunktype);
	while (chunktype != ChunkTypes_End)
	{
		uint32_t chunksz;
		readdata(stream, chunksz);
		auto chunkbase = stream.tellg();
		auto nextchunk = chunkbase + (streamoff)chunksz;
		switch (chunktype)
		{
		case ChunkTypes_Label:
			while (true)
			{
				void *dataptr;
				readdata(stream, dataptr);
				uint32_t labelptr;
				readdata(stream, labelptr);
				if (dataptr == (void *)-1 && labelptr == UINT32_MAX)
					break;
				dataptr = (uint8_t *)dataptr + landtablebase;
				tmpaddr = (uint32_t)stream.tellg();
				stream.seekg((uint32_t)chunkbase + labelptr);
				string label = getstring(stream);
				stream.seekg(tmpaddr);
				labels1[dataptr] = label;
				labels2[label] = dataptr;
			}
			break;
		case ChunkTypes_Author:
			author = getstring(stream);
			break;
		case ChunkTypes_Tool:
			tool = getstring(stream);
			break;
		case ChunkTypes_Description:
			description = getstring(stream);
			break;
		default:
			uint8_t *buf = new uint8_t[chunksz];
			allocatedmem.push_back(shared_ptr<uint8_t>(buf, default_delete<uint8_t[]>()));
			stream.read((char *)buf, chunksz);
			Metadata meta = { chunksz, buf };
			metadata[chunktype] = meta;
			break;
		}
		stream.seekg(nextchunk);
		readdata(stream, chunktype);
	}
}