#pragma once
#ifndef GameMap_h
#define GameMap_h

#include <vector>
#include "Layer.h"

using namespace std;

// quản lý và render toàn bộ map lên màng hình

class GameMap
{
private:
	vector<Layer*> m_MapLayers;
	friend class MapParser;
public:
	vector<Layer*> GetMapLayer();
	void Render();
	void Update();
};


#endif
