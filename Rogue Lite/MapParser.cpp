#include "MapParser.h"

MapParser* MapParser::s_Instance = nullptr;

MapParser::MapParser()
{
	Parse("MAP1", "Maps\\Map1.tmx");
	Parse("MAP2", "Maps\\Map2.tmx");
	Parse("MAP3", "Maps\\Map3.tmx");
}


bool MapParser::Load() // kiểm tra tải file được không
{
	if (Parse("MAP", "Maps\\Map1.tmx") == false) // đặt tên id và đường dẫn đến file map tmx
	{
		return false;
	}
	return true;
}

bool MapParser::Parse(string id, string source)
{
	TiXmlDocument xml;
	xml.LoadFile(source);
	if (xml.Error())
	{
		cerr << "Fail to load: " << source << endl;
		return false;
	}

	TiXmlElement* root = xml.RootElement();

	int rowcount = 0, colcount = 0, tilesize = 0;

	root->Attribute("width", &colcount);
	root->Attribute("height", &rowcount);
	root->Attribute("tilewidth", &tilesize);

	// parse tile sets
	TilesetList tilesets;
	for (TiXmlElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (e->Value() == string("tileset"))
		{
			tilesets.push_back(ParseTileset(e));
		}
	}

	// parse layer
	GameMap* gamemap = new GameMap();
	for (TiXmlElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (e->Value() == string("layer"))
		{
			TileLayer* tilelayer = ParseTileLayer(e, tilesets, tilesize, rowcount, colcount);
			gamemap->m_MapLayers.push_back(tilelayer);
		}
	}

	m_MapDict[id] = gamemap;
	return true;
}
Tileset MapParser::ParseTileset(TiXmlElement* xmlTileset)
{
	Tileset tileset;
	tileset.Name = xmlTileset->Attribute("name");
	xmlTileset->Attribute("firstgid", &tileset.FirstID);

	xmlTileset->Attribute("tilecount", &tileset.TileCount);
	tileset.LastID = (tileset.FirstID + tileset.TileCount) - 1;

	xmlTileset->Attribute("columns", &tileset.ColCount);
	tileset.RowCount = tileset.TileCount / tileset.ColCount;
	xmlTileset->Attribute("tilewidth", &tileset.TileSize);

	TiXmlElement* image = xmlTileset->FirstChildElement();
	tileset.Source = image->Attribute("source");
	return tileset;
}
TileLayer* MapParser::ParseTileLayer(TiXmlElement* xmlLayer, TilesetList tilesets, int tilesize, int rowcount, int colcount)
{
	TiXmlElement* data = NULL;
	for (TiXmlElement* e = xmlLayer->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (e->Value() == string("data"))
		{
			data = e;
			break;
		}
	}

	// parse layer tile map
	string matrix(data->GetText());
	istringstream iss(matrix);
	string id;

	TileMap tilemap(rowcount, vector<int>(colcount, 0));

	for (int row = 0; row < rowcount; row++)
	{
		for (int col = 0; col < colcount; col++)
		{
			getline(iss, id, ',');
			stringstream convertor(id);
			convertor >> tilemap[row][col];

			if (!iss.good())
			{
				break;
			}
		}
	}
	return new TileLayer(tilesize, rowcount, colcount, tilemap, tilesets);
}

MapParser::~MapParser()
{
	map<string, GameMap*>::iterator it;
	for (it = m_MapDict.begin(); it != m_MapDict.end(); it++)
	{
		it->second = nullptr;
	}
	m_MapDict.clear();
}

//MapParser::MapParser()
//{
//
//}