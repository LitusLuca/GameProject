#include "pch.h"
#include "Anim_Base.h"
#include "SpriteSheet.h"

Anim_Base::Anim_Base() :
	m_frameCurrent(0), m_frameStart(0), m_frameEnd(0), m_frameRow(0),
	m_frameTime(0.f), m_elapsedTime(0.f), m_frameActionStart(-1), m_frameActionEnd(-1),
	m_loop(false), m_playing(false) {}

Anim_Base::~Anim_Base() {}

void Anim_Base::play()
{
	m_playing = true;
}

void Anim_Base::pause()
{
	m_playing = false;
}

void Anim_Base::stop()
{
	m_playing = false;
	reset();
}

void Anim_Base::reset()
{
	m_frameCurrent = m_frameStart;
	m_elapsedTime = 0.f;
	cropSprite();
}

void Anim_Base::update(const float& l_dT)
{
	if (!m_playing) return;
	m_elapsedTime += l_dT;
	if (m_elapsedTime < m_frameTime) return;
	frameStep();
	cropSprite();
	m_elapsedTime = 0;
}

void Anim_Base::setSpriteSheet(SpriteSheet* l_sheet)
{
	m_spriteSheet = l_sheet;
}

void Anim_Base::setFrame(const Frame& l_frame)
{
	if ((l_frame >= m_frameStart && l_frame <= m_frameEnd) ||
		(l_frame >= m_frameEnd && l_frame <= m_frameStart))
	{
		m_frameCurrent = l_frame;
	}
}

bool Anim_Base::isInAction()
{
	if (m_frameActionStart == -1 || m_frameActionEnd == -1) return true;
	return (m_frameCurrent >= m_frameActionStart && m_frameCurrent <= m_frameActionEnd);
}

void Anim_Base::setName(std::string& l_name)
{
	m_name = l_name;
}

void Anim_Base::setLooping(const bool& l_looping)
{
	m_loop = l_looping;
}
