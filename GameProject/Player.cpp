#include "pch.h"
#include "Player.h"
#include "EntityManager.h"
#include "StateManager.h"

Player::Player(EntityManager* l_entityMgr) :
	Character(l_entityMgr)
{
	load("player.char");
	m_type = EntityType::Player;
	EventManager* events = m_entityManager->getContext()->m_eventManager;

	events->addCallback<Player>(StateType::Game, "Player_MoveLeft", &Player::react, this);
	events->addCallback<Player>(StateType::Game, "Player_MoveRight", &Player::react, this);
	events->addCallback<Player>(StateType::Game, "Player_Jump", &Player::react, this);
}

Player::~Player()
{
	EventManager* events = m_entityManager->getContext()->m_eventManager;
	events->removeCallback(StateType::Game, "Player_MoveLeft");
	events->removeCallback(StateType::Game, "Player_MoveRight");
	events->removeCallback(StateType::Game, "Player_Jump");
}
void Player::onEntityCollision(EntityBase* l_collider, bool l_attack)
{
	//TODO: on entityCollision
	std::cout << "TODO \n";
}

void Player::react(EventDetails* l_details)
{
	if (l_details->m_name == "Player_MoveLeft")
		Character::move(Direction::Left);
	else if (l_details->m_name == "Player_MoveRight")
		Character::move(Direction::Right);
	else if (l_details->m_name == "Player_Jump")
		Character::jump();
}

