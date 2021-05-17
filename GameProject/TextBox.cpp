#include "pch.h"

#include "TextBox.h"

TextBox::TextBox()
{
	this->setup(5, 9, 200, sf::Vector2f(0, 0));
}
TextBox::TextBox(int l_visible, int l_charSize, int l_width, sf::Vector2f l_screenPos)
{
	this->setup(l_visible, l_charSize, l_width, l_screenPos);
}
TextBox::~TextBox()
{
	clear();
}
void TextBox::setup(int l_visible, int l_charSize, int l_width, sf::Vector2f l_screenPos)
{
	m_numVisible = l_visible;
	sf::Vector2f l_offset(2.f, 2.f);

	m_font.loadFromFile("arial.ttf");
	m_content.setFont(m_font);
	m_content.setString("");
	m_content.setCharacterSize(l_charSize);
	m_content.setPosition(l_screenPos + l_offset);

	m_backdrop.setSize(sf::Vector2f(l_width,(float) l_visible * (float)l_charSize * 1.2f));
	m_backdrop.setFillColor(sf::Color(90, 90, 90, 90));
	m_backdrop.setPosition(l_screenPos);
}

void TextBox::add(std::string l_message)
{
	m_messages.push_back(l_message);
	if (m_messages.size() > (unsigned)m_numVisible) m_messages.erase(m_messages.begin());
}

void TextBox::clear()
{
	m_messages.clear();
}

void TextBox::render(sf::RenderWindow& l_window)
{
	std::string l_content;
	for (auto &itr : m_messages)
	{
		l_content.append(itr + "\n");
	}
	if (l_content != "")
	{
		m_content.setString(l_content);
		l_window.draw(m_backdrop);
		l_window.draw(m_content);
	}
}