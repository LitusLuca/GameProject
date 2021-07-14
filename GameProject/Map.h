#pragma once
#include "SharedContext.h"
#include "BaseState.h"

enum Sheet
{
	Tile_Size = 80,
	Sheet_Width = 640,
	Sheet_Height = 160		
};

using TileID = unsigned int;

struct TileInfo
{
	TileInfo(SharedContext* l_context, const std::string& l_texture = "", TileID l_id = 0)
		: m_context(l_context), m_id(0), m_deadly(false)
	{
		TextureManager* textMgr = l_context->m_textureManager;
		if (l_texture == "")
		{
			m_id = l_id;
			return;
		}
		if (!textMgr->requireResource(l_texture)) return;
		m_texture = l_texture;
		m_id = l_id;
		m_sprite.setTexture(*textMgr->getResource(m_texture));
		sf::IntRect tileBoundaries(m_id % (Sheet::Sheet_Width / Sheet::Tile_Size) * Sheet::Tile_Size,
			m_id / (Sheet::Sheet_Width / Sheet::Tile_Size) * Sheet::Tile_Size,
			Sheet::Tile_Size, Sheet::Tile_Size);
		m_sprite.setTextureRect(tileBoundaries);
	}
	~TileInfo()
	{
		if (m_texture == "") return;
		m_context->m_textureManager->releaseResource(m_texture);
	}
	sf::Sprite m_sprite;
	TileID m_id;
	std::string m_name;
	sf::Vector2f m_friction;
	bool m_deadly;
	SharedContext* m_context;
	std::string m_texture;
};

struct Tile
{
	TileInfo* m_proporties;
	bool m_warp;
	// CONSIDER: More tile specific flags
};

using TileMap = std::unordered_map<TileID, Tile*>;
using TileSet = std::unordered_map<TileID, TileInfo*>;

class Map
{
public:
	Map(SharedContext* l_context, BaseState* l_currentState);
	~Map();
	Tile* getTile(unsigned int l_x, unsigned int l_y);
	TileInfo* getDefaultTile();
	float getGravity() const;
	unsigned int getTileSize() const;
	const sf::Vector2u& getMapSize() const;
	const sf::Vector2f& getPlayerStart() const;
	void loadMap(const std::string& l_path);
	void loadNext();
	void update(float l_dT);
	void draw();

private:
	unsigned int convertCoords(unsigned int l_x, unsigned int l_y);
	void loadTiles(const std::string& l_path);
	void purgeMap();
	void purgeTileSet();

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
	SharedContext* m_context;
};

