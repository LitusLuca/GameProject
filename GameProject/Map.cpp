#include "pch.h"
#include "Map.h"
#include "StateManager.h"

Map::Map(SharedContext* l_context, BaseState* l_currentState)
	:m_context(l_context), m_defaultTile(l_context),
	m_maxMapSize(32, 32), m_tileCount(0), m_tileSetCount(0),
	m_mapGravity(512.f), m_loadNextMap(false),
	m_currentState(l_currentState)
{
	m_context->m_gameMap = this;
	loadTiles("tiles.cfg");
}

Map::~Map()
{
	purgeMap();
	purgeTileSet();
	m_context->m_gameMap = nullptr;
}

Tile* Map::getTile(unsigned int l_x, unsigned int l_y)
{
	auto itr = m_tileMap.find(convertCoords(l_x, l_y));
	return (itr != m_tileMap.end() ? itr->second : nullptr);
}

TileInfo* Map::getDefaultTile()
{
	return &m_defaultTile;
}

unsigned int Map::convertCoords(unsigned int l_x, unsigned int l_y)
{
	return (l_x * m_maxMapSize.x) + l_y; //x > y
}

float Map::getGravity() const
{
	return m_mapGravity;
}

unsigned int Map::getTileSize() const
{
	return Sheet::Tile_Size;
}

const sf::Vector2u& Map::getMapSize() const
{
	return m_maxMapSize;
}

const sf::Vector2f& Map::getPlayerStart() const
{
	return m_playerStart;
}

void Map::loadNext()
{
	m_loadNextMap = true;
}

void Map::update(float l_dT)
{
	if (m_loadNextMap)
	{
		purgeMap();
		m_loadNextMap = false;
		if (m_nextMap != "")
		{
			loadMap("media/maps/" + m_nextMap);
		}
		else
		{
			m_currentState->getStateManager()->switchTo(StateType::MainMenu);
		}
		m_nextMap = "";
	}
	sf::FloatRect viewSpace = m_context->m_window->getViewSpace();
	m_background.setPosition(viewSpace.left, viewSpace.top);
}

void Map::draw()
{
	sf::RenderWindow* l_wind = m_context->m_window->getRenderWindow();
	l_wind->draw(m_background);
	sf::FloatRect viewSpace = m_context->m_window->getViewSpace();

	sf::Vector2i tileBegin(
		floor(viewSpace.left / Sheet::Tile_Size),
		floor(viewSpace.top / Sheet::Tile_Size));

	sf::Vector2i tileEnd(
		ceil((viewSpace.left + viewSpace.width) / Sheet::Tile_Size),
		ceil((viewSpace.top + viewSpace.height) / Sheet::Tile_Size));

	unsigned int count = 0;
	for (int x = tileBegin.x; x <= tileEnd.x; ++x)
	{
		for (int y = tileBegin.y; y <= tileEnd.y; ++y)
		{
			if (x < 0 || y < 0) continue;
			Tile* tile = getTile(x, y);
			if (!tile) continue;
			sf::Sprite& sprite = tile->m_proporties->m_sprite;
			sprite.setPosition(x * Sheet::Tile_Size, y * Sheet::Tile_Size);
			l_wind->draw(sprite);
			++count;
		}
	}
}

void Map::purgeMap()
{
	m_tileCount = 0;
	for (auto& itr : m_tileMap)
	{
		delete itr.second;
	}
	m_tileMap.clear();
	m_context->m_entityManager->purge();
	if (m_backgroundTexture == "") return;
	m_context->m_textureManager->releaseResource(m_backgroundTexture);
	m_backgroundTexture = "";
}

void Map::purgeTileSet()
{
	for (auto& itr : m_tileSet)
	{
		delete itr.second;
	}
	m_tileSet.clear();
	m_tileSetCount = 0;
}

void Map::loadMap(const std::string& l_path)
{
	std::ifstream file;
	file.open(Utils::GetWorkingDirectory() + l_path);
	if (!file.is_open())
	{
		std::cerr << "! Failed loading map: " << l_path << "\n";
		return;
	}
	EntityManager* entityMgr = m_context->m_entityManager;
	std::string line;
	int playerId = -1;
	while (std::getline(file, line))
	{
		if (line[0] == '|') continue;
		std::stringstream keystream(line);
		
		std::string type;
		keystream >> type;
		if (type == "TILE")
		{
			int tileId = 0;
			keystream >> tileId;
			if (tileId < 0)
			{
				std::cout << "! Invalid tile ID: " << tileId << "\n";
				continue;
			}
			auto itr = m_tileSet.find(tileId);
			if (itr == m_tileSet.end())
			{
				std::cout << "! Tile ID was not found in the TileSet: " << tileId << "\n";
				continue;
			}
			sf::Vector2i tileCoords;
			keystream >> tileCoords.x >> tileCoords.y;
			if (tileCoords.x > m_maxMapSize.x || tileCoords.y > m_maxMapSize.y)
			{
				std::cout << "! tile is out of range: [" << tileCoords.x << ", " << tileCoords.y << "]\n";
				continue;
			}
			Tile* tile = new Tile();
			tile->m_proporties = itr->second;
			if (!m_tileMap.emplace(convertCoords(tileCoords.x, tileCoords.y), tile).second)
			{
				std::cout << "! duplicate tile: [" << tileCoords.x << ", " << tileCoords.y << "]\n";
				delete tile;
				tile = nullptr;
				continue;
			}
			std::string flag;
			keystream >> flag;
			tile->m_warp = false;
			if (flag == "WARP") tile->m_warp = true;
		}
		else if (type == "BACKGROUND")
		{
			if (m_backgroundTexture != "") continue;
			keystream >> m_backgroundTexture;
			if (!m_context->m_textureManager->requireResource(m_backgroundTexture))
			{
				m_backgroundTexture = "";
				continue;
			}
			sf::Texture* texture = m_context->m_textureManager->getResource(m_backgroundTexture);
			m_background.setTexture(*texture);
			sf::Vector2f viewSize = m_currentState->getView().getSize();
			sf::Vector2u textureSize = texture->getSize();
			sf::Vector2f scaleFactor;
			scaleFactor.x = viewSize.x / textureSize.x;
			scaleFactor.y = viewSize.y / textureSize.y;
			m_background.setScale(scaleFactor);
		}
		else if (type == "SIZE")
		{
			keystream >> m_maxMapSize.x >> m_maxMapSize.y;
		}
		else if (type == "GRAVITY")
		{
			keystream >> m_mapGravity;
		}
		else if (type == "DEFAULT_FRICTION")
		{
			keystream >> m_defaultTile.m_friction.x >> m_defaultTile.m_friction.y;
		}
		else if (type == "NEXTMAP")
		{
			keystream >> m_nextMap;
		}
		else if (type == "PLAYER")
		{
			if (playerId != -1) continue;
			playerId = entityMgr->add(EntityType::Player);
			if (playerId < 0) continue;
			float playerX = 0; float playerY = 0;
			keystream >> playerX >> playerY;
			entityMgr->find(playerId)->setPosition(playerX, playerY);
			m_playerStart = sf::Vector2f(playerX, playerY);
		}
	}
	file.close();
}

void Map::loadTiles(const std::string& l_path)
{
	std::ifstream file;
	file.open(Utils::GetWorkingDirectory() + l_path);
	if (!file.is_open())
	{
		std::cerr << "! Failed loading tile set file: " << l_path << "\n";
		return;
	}
	std::string line;
	while (std::getline(file, line))
	{
		if (line[0] == '|') continue;
		
		std::stringstream keystream(line);
		int tileId;
		keystream >> tileId;
		if (tileId < 0) continue;
		TileInfo* tile = new TileInfo(m_context, "TileSheet", tileId);
		keystream >> tile->m_name >> tile->m_friction.x >> tile->m_friction.y >> tile->m_deadly;
		if (!m_tileSet.emplace(tileId, tile).second)
		{
			std::cout << "! duplicate tile detected: " << tile->m_name << "\n";
			delete tile;
		}
	}
	file.close();
}