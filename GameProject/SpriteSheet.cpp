#include "pch.h"

#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(TextureManager* l_textMgr) :
	m_textureManager(l_textMgr), m_animationCurrent(nullptr), m_spriteScale(1.f, 1.f), m_direction(Direction::Right)
{

}

SpriteSheet::~SpriteSheet()
{
	releaseSheet();
}

void SpriteSheet::cropSprite(const sf::IntRect& l_rect)
{
	m_sprite.setTextureRect(l_rect);
}

bool SpriteSheet::loadSheet(const std::string& l_file)
{
	std::ifstream sheet;
	sheet.open(Utils::GetWorkingDirectory() + l_file);
	if (sheet.is_open())
	{
		releaseSheet();
		std::string line;
		while (std::getline(sheet, line))
		{
			if (line[0] == (int)"|") continue;
			std::stringstream keystream(line);
			std::string type;
			keystream >> type;
			if (type == "Texture")
			{
				if (m_texture != "")
				{
					continue;
				}
				std::string texture;
				keystream >> texture;
				if (!m_textureManager->requireRecource(m_texture))
				{
					std::cerr << "! Could not set up the texture: "
						<< m_texture << std::endl;
					continue;
				}
				m_texture = texture;
				m_sprite.setTexture(*m_textureManager->getRecource(m_texture));
			}
			else if (type == "Size")
			{
				keystream >> m_spriteSize.x >> m_spriteSize.y;
				setSpriteSize(m_spriteSize);
			}
			else if (type == "Scale")
			{
				keystream >> m_spriteScale.x >> m_spriteScale.y;
				m_sprite.setScale(m_spriteScale);
			}
			else if (type == "AnimationType")
			{
				keystream >> m_animType;
			}
			else if (type == "Animation")
			{
				std::string name;
				keystream >> name;
				if (m_animations.find(name) != m_animations.end())
				{
					std::cerr << "! Duplicate animation(" << name
						<< ") in: " << l_file << std::endl;
					continue;
				}
				Anim_Base* anim = nullptr;
				if (m_animType == "Directional")
				{
					anim = new Anim_Directional();
				}
				else
				{
					std::cerr << "! Unknown animation type: "
						<< m_animType << std::endl;
					continue;
				}
				keystream >> *anim;
				anim->setSpriteSheet(this);
				anim->setName(name);
				anim->reset();
				m_animations.emplace(name, anim);

				if (m_animationCurrent) continue;
				m_animationCurrent = anim;
				m_animationCurrent->play();
			}
		}
		sheet.close();
		return true;
	}
	std::cerr << "! Failed loading spritesheet: "
		<< l_file << std::endl;
	return false;
}

void SpriteSheet::releaseSheet()
{
	m_textureManager->releaseRecource(m_texture);
	m_animationCurrent = nullptr;
	while (m_animations.begin() != m_animations.end())
	{
		delete m_animations.begin()->second;
		m_animations.erase(m_animations.begin());
	}
}

Anim_Base* SpriteSheet::getCurrentAnim()
{
	return m_animationCurrent;
}

bool SpriteSheet::setAnimation(const std::string& l_name, const bool& l_play, const bool& l_loop)
{
	auto itr = m_animations.find(l_name);
	if (itr == m_animations.end()) return false;
	if (itr->second == m_animationCurrent) return false;
	if (m_animationCurrent) m_animationCurrent->stop();

	m_animationCurrent = itr->second;
	m_animationCurrent->setLooping(l_loop);
	if (l_play) m_animationCurrent->play();
	m_animationCurrent->cropSprite();
	return true;
}

void SpriteSheet::update(float l_dT)
{
	m_animationCurrent->update(l_dT);
}

void SpriteSheet::draw(sf::RenderWindow* l_wnd)
{
	l_wnd->draw(m_sprite);
}


void SpriteSheet::setSpriteSize(const sf::Vector2i& l_size)
{
	m_spriteSize = l_size;
	m_sprite.setOrigin(m_spriteSize.x / 2.f, m_spriteSize.y);
}

void SpriteSheet::setSpritePosition(const sf::Vector2f& l_pos)
{
	m_sprite.setPosition(l_pos);
}

void SpriteSheet::setDirection(const Direction& l_dir)
{
	if (l_dir == m_direction) return;
	m_direction = l_dir;
	m_animationCurrent->cropSprite();
}

sf::Vector2i SpriteSheet::getSpriteSize()
{
	return m_spriteSize;
}

Direction SpriteSheet::getDirection()
{
	return m_direction;
}
