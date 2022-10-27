#pragma once
#ifndef MapParser_h
#define MapParser_h

#include <map>
#include <string>
#include "GameMap.h"
#include "TileLayer.h"
#include "tinyxml.h"	

class MapParser
{
private:
	static  MapParser* s_Instance;
	map<string, GameMap*> m_MapDict; // danh sách map

public:
	MapParser();
	bool Load();
	~MapParser();

	// lấy một map
	inline GameMap* GetMap(string id) { return m_MapDict[id]; }

	inline static  MapParser* GetInstance()
	{
		return s_Instance = (s_Instance != nullptr) ? s_Instance : new  MapParser();
	}

	// tải map
	bool Parse(string id, string source);
	Tileset ParseTileset(TiXmlElement* xmlTileset);
	TileLayer* ParseTileLayer(TiXmlElement* xmlLayer, TilesetList tilesets, int tilesize, int rowcount, int colcount);
};

#endif