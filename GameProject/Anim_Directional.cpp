#include "pch.h"
#include "Anim_Directional.h"
#include "SpriteSheet.h"

void Anim_Directional::frameStep()
{
	if (m_frameStart < m_frameEnd) m_frameCurrent++;
	else m_frameCurrent--;

	if ((m_frameStart < m_frameEnd && m_frameCurrent > m_frameEnd) || (m_frameEnd < m_frameStart && m_frameCurrent < m_frameEnd))
	{
		if (m_loop)
		{
			m_frameCurrent = m_frameStart;
			return;
		}
		m_frameCurrent = m_frameEnd;
		pause();
	}
}

void Anim_Directional::cropSprite()
{
	sf::IntRect rect(
		m_spriteSheet->getSpriteSize().x * m_frameCurrent,
		m_spriteSheet->getSpriteSize().y * (m_frameRow + (short)m_spriteSheet->getDirection()),
		m_spriteSheet->getSpriteSize().x,
		m_spriteSheet->getSpriteSize().y
	);
	m_spriteSheet->cropSprite(rect);
}

void Anim_Directional::readIn(std::stringstream& l_stream)
{
	l_stream >> m_frameStart >> m_frameEnd >> m_frameRow >>
		m_frameTime >> m_frameActionStart >> m_frameActionEnd;
}
