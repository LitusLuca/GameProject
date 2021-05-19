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

void SpriteSheet::update(const sf::Time& l_time)
{
	m_animationCurrent->upate(l_dt);
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
