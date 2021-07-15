#include "pch.h"
#include "Character.h"
#include "EntityManager.h"
#include "StateManager.h"


Character::Character(EntityManager* l_entityMgr)
	: EntityBase(l_entityMgr), m_spriteSheet(m_entityManager->getContext()->m_textureManager),
	m_jumpVelocity(250), m_hitpoints(5)
{
	m_name = "character";
}

Character::~Character()
{}

void Character::move(const Direction& l_dir)
{
	if (getState() == EntityState::Dying) return;
	m_spriteSheet.setDirection(l_dir);
	if (l_dir == Direction::Left) accelerate(-m_speed.x, 0);
	else accelerate(m_speed.x, 0);
	if (getState() == EntityState::Idle)
		setState(EntityState::Walking);
}

void Character::jump()
{
	if (getState() == EntityState::Dying || getState() == EntityState::Jumping)
	{
		return;
	}
	setState(EntityState::Jumping);
	addVelocity(0, -m_jumpVelocity);
}

void Character::attack()
{
	if (getState() == EntityState::Dying ||
		getState() == EntityState::Jumping ||
		getState() == EntityState::Attacking)
		return;
	setState(EntityState::Attacking);
}

void Character::load(const std::string& l_path)
{
	std::ifstream file;
	file.open(Utils::GetWorkingDirectory() + l_path);
	if (!file.is_open())
	{
		std::cerr << "! Failed to open: " << l_path << "\n";
		return;
	}
	std::string line;

	while (std::getline(file, line))
	{
		if (line[0] == '|') continue;
		std::stringstream keystream(line);

		std::string type;
		keystream >> type;

		if (type == "Name")
			keystream >> m_name;
		else if (type == "Spritesheet")
		{
			std::string path;
			keystream >> path;
			m_spriteSheet.loadSheet("media/SpriteSheets/" + path);
		}
		else if (type == "Hitpoints")
			keystream >> m_hitpoints;
		else if (type == "BoundingBox")
		{
			sf::Vector2f boundingSize;
			keystream >> boundingSize.x >> boundingSize.y;
			setSize(boundingSize.x, boundingSize.y);
		}
		else if (type == "DamageBox")
		{
			keystream >> m_attackAABBOffset.x >> m_attackAABBOffset.y >> m_attackAABB.width >> m_attackAABB.height;
		}
		else if (type == "Speed")
			keystream >> m_speed.x >> m_speed.y;
		else if (type == "JumpVelocity")
			keystream >> m_jumpVelocity;
		else if (type == "MaxVelocity")
			keystream >> m_maxVelocity.x >> m_maxVelocity.y;
		else
			std::cout << "! Unknown specifier: " << m_type << "\n";
	}
	file.close();
}

void Character::animate()
{
	EntityState state = getState();
	if (state == EntityState::Walking && m_spriteSheet.getCurrentAnimation()->getName() != "Walk")
		m_spriteSheet.setAnimation("Walk", true, true);
	else if (state == EntityState::Jumping && m_spriteSheet.getCurrentAnimation()->getName() != "Jump")
		m_spriteSheet.setAnimation("Jump", true, false);
	else if (state == EntityState::Idle && m_spriteSheet.getCurrentAnimation()->getName() != "Idle")
		m_spriteSheet.setAnimation("Idle", true, true);
}

void Character::update(float l_dT)
{
	EntityBase::update(l_dT);
	if (m_attackAABB.width != 0 && m_attackAABB.height != 0)
	{
		updateAttackAABB();
	}
	if (!getState() != EntityState::Dying && getState() != EntityState::Attacking)
	{
		if (abs(m_velocity.y) >= 0.001f)
			setState(EntityState::Jumping);
		else if (abs(m_velocity.x) >= 0.1f)
			setState(EntityState::Walking);
		else
			setState(EntityState::Idle);
	}
	else if (getState() == EntityState::Attacking)
	{
		if (!m_spriteSheet.getCurrentAnimation()->isPlaying())
			setState(EntityState::Idle);
	}
	else if (getState() == EntityState::Dying)
		std::cout << "Reset char" << "\n";
		//TODO: reset character when died 
	animate();
	m_spriteSheet.update(l_dT);
	m_spriteSheet.setSpritePosition(m_position);
}

void Character::draw(sf::RenderWindow* l_renderWnd)
{
	m_spriteSheet.draw(l_renderWnd);
}