#include "pch.h"
#include "Player.h"
#include "EntityManager.h"
#include "StateManager.h"

Player::Player(EntityManager* l_entityMgr)
	: Character(l_entityMgr)
{
	load("Player.char");
	m_type = EntityType::Player;

	EventManager* eventMgr = m_entityManager->getContext()->m_eventManager;
	eventMgr->addCallback<Player>(StateType::Game, "Player_MoveLeft", &Player::react, this);
	eventMgr->addCallback<Player>(StateType::Game, "Player_MoveRight", &Player::react, this);
	eventMgr->addCallback<Player>(StateType::Game, "Player_Jump", &Player::react, this);
}

Player::~Player()
{
	EventManager* eventMgr = m_entityManager->getContext()->m_eventManager;
	eventMgr->removeCallback(StateType::Game, "Player_MoveLeft");
	eventMgr->removeCallback(StateType::Game, "Player_MoveRight");
	eventMgr->removeCallback(StateType::Game, "Player_Jump");
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

void Player::onEntityCollision(EntityBase* l_collider, bool l_attack)
{
	std::cout << "Ouch" << "\n";
}