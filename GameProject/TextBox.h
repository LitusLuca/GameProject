#pragma once

#include "Window.h"
#include "TextBox.h"

using MessageContainer = std::vector<std::string>;
class TextBox
{
//public functions
public:
	//constructor / destructor
	TextBox();
	TextBox(int l_visible, int l_charSize, int l_width, sf::Vector2f l_screenPos);
	~TextBox();

	//setup
	void setup(int l_visible, int l_charSize, int l_width, sf::Vector2f l_screenPos);

	//adding / deleting messages
	void add(std::string l_message);
	void clear();

	//render
	void render(sf::RenderWindow& l_window);

//private data
private:
	MessageContainer m_messages;
	int m_numVisible;
	sf::RectangleShape m_backdrop;
	sf::Font m_font;
	sf::Text m_content;


};

