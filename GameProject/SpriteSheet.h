#pragma once

#include "TextureManager.h"
#include "Anim_Base.h"
#include "Direction.h"

using Animations = std::unordered_map<std::string, Anim_Base*>;

class SpriteSheet
{
public:
	SpriteSheet(TextureManager* l_textMgr);
	~SpriteSheet();

	void cropSprite(const sf::IntRect& l_rect);

	//Getter/Setters
	void setSpriteSize(const sf::Vector2i& l_size);
	void setSpritePosition(const sf::Vector2f& l_pos);
	void setDirection(const Direction& l_dir);

	sf::Vector2i getSpriteSize();
	Direction getDirection();

	// load and release sheets
	bool loadSheet(const std::string& l_file);
	void releaseSheet();

	Anim_Base* getCurrentAnim();
	bool setAnimation(const std::string& l_name, const bool& l_play = false, const bool& l_loop = false);

	void update(const sf::Time& l_time);
	void draw(sf::RenderWindow* l_wnd);

	

private:
	std::string m_texture;
	sf::Sprite m_sprite;
	sf::Vector2i m_spriteSize;
	sf::Vector2f m_spriteScale;
	Direction m_direction;
	std::string m_animType;
	Animations m_animations;
	Anim_Base* m_animationCurrent;
	TextureManager* m_textureManager;
};

