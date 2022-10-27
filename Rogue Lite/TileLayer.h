#pragma once
#ifndef TileLayer_h
#define TileLayer_h

#include <string>
#include "Layer.h"
#include <vector>

using namespace std;

// cấu trúc dữ liệu của ngói
struct Tileset
{
	int FirstID, LastID; // id đầu và cuối của mỗi ô
	int RowCount, ColCount; // số hàng và cột của các ô
	int TileCount, TileSize;
	string Name, Source;

};

using TilesetList = vector<Tileset>;
using TileMap = vector<vector<int>>; // 

// lớp ngói, xếp các ô thành map

class TileLayer : public Layer
{
private:
	int m_TileSize;
	int m_RowCount, m_ColCount;

	TileMap m_Tilemap;
	TilesetList m_Tilesets;

public:
	TileLayer(int tilesize, int rowcount, int colcount, TileMap tilemap, TilesetList tilesets); // load map
	virtual void Render(); // render map lên màng hình
	virtual void Update(); //

	inline TileMap GetTilemap() { return m_Tilemap; } 
};

#endif
