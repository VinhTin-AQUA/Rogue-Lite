#include "GameMap.h"

vector<Layer*> GameMap::GetMapLayer() { return m_MapLayers; }
void GameMap::Render()
{
	for (unsigned int i = 0; i < m_MapLayers.size(); i++)
	{
		m_MapLayers[i]->Render();// render từng map
	}
}
void GameMap::Update()
{
	for (unsigned int i = 0; i < m_MapLayers.size(); i++)
	{
		m_MapLayers[i]->Update();// update
	}
}