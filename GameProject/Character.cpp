#include "pch.h"
#include "Character.h"
#include "EntityManager.h"
#include "StateManager.h"

// TODO: getHurt method
Character::Character(EntityManager* l_entityManager) :
	EntityBase(l_entityManager),
	m_spriteSheet(m_entityManager->getContext()->m_textureManager),
	m_jumpVelocity(250), m_hitPoints(5)
{
	m_name = "Character";
}

Character::~Character()
{
}

void Character::move(const Direction& l_dir)
{
	if (getState() == EntityState::Dying) return;
	m_spriteSheet.setDirection(l_dir);
	if (l_dir == Direction::Left) accelerate(-m_speed.x, 0);
	else accelerate(m_speed.x, 0);
	if (getState() == EntityState::Idle)
	{
		setState(EntityState::Walking);
	}
}

void Character::jump()
{
	if (getState() == EntityState::Dying ||
		getState() == EntityState::Jumping) return;
	setState(EntityState::Jumping);
	accelerate(0, -m_jumpVelocity);
}

void Character::load(const std::string& l_path)
{
	std::ifstream file;
	file.open(Utils::GetWorkingDirectory() + l_path);
	if (!file.is_open())
	{
		std::cout << "! Failed loading character file: " + l_path << std::endl;
	}
	std::string line;
	while (std::getline(file, line))
	{
		if (line[0] == (char)"|") continue;
		std::stringstream keystream(line);
		std::string type;
		if (type == "NAME") keystream >> m_name;
		else if (type == "SpriteSheet")
		{
			std::string path;
			keystream >> path;
			m_spriteSheet.loadSheet("media/SpriteSheets" + path);
		}
		else if (type == "Hitpoints") keystream >> m_hitPoints;
		else if (type == "BoundingBox")
		{
			sf::Vector2f boundigBox;
			keystream >> boundigBox.x >> boundigBox.y;
			setSize(boundigBox.x, boundigBox.y);
		}
		else if (type == "DamageBox")
			keystream >> m_attackAABBoffset.x >> m_attackAABBoffset.y >>
			m_attackAABB.width >> m_attackAABB.height;
		else if (type == "Speed")
			keystream >> m_speed.x >> m_speed.y;
		else if (type == "JumpVelocity")
			keystream >> m_jumpVelocity;
		else if (type == "MaxVelocity")
			keystream >> m_maxVelocity.x >> m_maxVelocity.y;
	}
	file.close();
}

void Character::animate()
{
	EntityState state = getState();
	if (state == EntityState::Idle && m_spriteSheet.getCurrentAnim()->getName() != "Idle")
		m_spriteSheet.setAnimation("Idle", true, true);
	else if (state == EntityState::Walking && m_spriteSheet.getCurrentAnim()->getName() != "Walk")
		m_spriteSheet.setAnimation("Walk", true, true);
	else if (state == EntityState::Jumping && m_spriteSheet.getCurrentAnim()->getName() != "Jump")
		m_spriteSheet.setAnimation("Jump", true, false);
	else if (state == EntityState::Dying && m_spriteSheet.getCurrentAnim()->getName() != "Death")
		m_spriteSheet.setAnimation("Death", true, false);
}

void Character::updateAttackAABB()
{
	// TODO: Attacking method / update attackAABB
}

void Character::update(float l_dT)
{
	EntityBase::update(l_dT);
	if (m_attackAABB.width != 0 && m_attackAABB.height != 0)
		updateAttackAABB();
	if (getState() != EntityState::Dying)
	{
		if (abs(m_velocity.y) >= 0.001f)
			setState(EntityState::Jumping);
		else if (abs(m_velocity.x) >= 0.1f)
			setState(EntityState::Walking);
		else
			setState(EntityState::Idle);
	}
	else if (getState() == EntityState::Dying)
		if (!m_spriteSheet.getCurrentAnim()->isPlaying())
			m_entityManager->remove(m_id);
	animate();
	m_spriteSheet.update(l_dT);
	m_spriteSheet.setSpritePosition(m_position);
}

void Character::draw(sf::RenderWindow* l_window)
{
	m_spriteSheet.draw(l_window);
}