#pragma once
#include "SharedContext.h"
#include "BaseState.h"

enum Sheet
{
	Tile_Size = 32, Sheet_Width = 256, Sheet_Height = 256
};

using TileId = unsigned int;

struct TileInfo
{
	TileInfo(SharedContext* l_shared, const std::string& l_texture = "", TileId l_id = 0) :
		m_shared(l_shared), m_id(0), m_deadly(false)
	{
		TextureManager* tmgr = m_shared->m_textureManager;
		if (l_texture == "")
		{
			m_id = l_id;
			return;
		}
		if (!tmgr->requireRecource(l_texture)) return;
		m_texture = l_texture;
		m_id = l_id;
		m_sprite.setTexture(*tmgr->getRecource(m_texture));
		sf::IntRect tileBounds(m_id %
		(Sheet::Sheet_Width / Sheet::Tile_Size) * Sheet::Tile_Size,
			m_id/(Sheet::Sheet_Height/Sheet::Tile_Size)*Sheet::Tile_Size,
			Sheet::Tile_Size, Sheet::Tile_Size);
		m_sprite.setTextureRect(tileBounds);
	}
	~TileInfo()
	{
		if (m_texture == "") return;
		m_shared->m_textureManager->releaseRecource(m_texture);
	}

	sf::Sprite m_sprite;

	TileId m_id;
	std::string m_name;
	sf::Vector2f m_friction;
	bool m_deadly;

	SharedContext* m_shared;
	std::string m_texture;
};

struct Tile
{
	TileInfo* m_props;
	bool m_warp;
	//other flags
};

using TileMap = std::unordered_map<TileId, Tile*>;
using TileSet = std::unordered_map<TileId, TileInfo*>;

class Map
{
public:
	Map(SharedContext* l_shared, BaseState* l_currentState);
	~Map();

	Tile* getTile(unsigned int l_x, unsigned int l_y);
	TileInfo* getDefaultTile();
	float getGravity()const;
	unsigned int getTileSize()const;
	const sf::Vector2u& getMapSize()const;
	const sf::Vector2f& getPlayerStart()const;
	void loadMap(const std::string& l_path);
	void loadNext();
	void update(float l_dT);
	void draw();

private:
	unsigned int convertCoords(unsigned int l_x, unsigned int l_y);
	void loadTiles(const std::string& l_path);
	void purgeMap();
	void purgeTileSet();

private:
	TileSet m_tileSet;
	TileMap m_tileMap;
	sf::Sprite m_background;
	TileInfo m_defaultTile;
	sf::Vector2u m_maxMapSize;
	sf::Vector2f m_playerStart;
	unsigned int m_tileCount;
	unsigned int m_tileSetCount;
	float m_mapGravity;
	std::string m_nextMap;
	bool m_loadNextMap;
	std::string m_backgroundTexture;
	BaseState* m_currentState;
	SharedContext* m_shared;
};

