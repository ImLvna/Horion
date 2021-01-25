#include "Target.h"

#include <regex>

#include "../Horion/Module/ModuleManager.h"

C_LocalPlayer** localPlayer;

void Target::init(C_LocalPlayer** cl) {
	localPlayer = cl;
}

bool Target::isValidTarget(C_Entity* ent) {
	if (ent == NULL)
		return false;

	if (ent == g_Data.getLocalPlayer())
		return false;


	if (!ent->isAlive())
		return false;

	

	

	// Temporarily removed from gui, tons of false negatives
	

	if (FriendList::findPlayer(ent->getNameTag()->getText()) && !moduleMgr->getModule<NoFriends>()->isEnabled())
		return false;

	

	

	

	if (!(*localPlayer)->canAttack(ent, false))
		return false;

	

	return true;
}

bool Target::containsOnlyASCII(const std::string& string) {
	for (auto c : string) {
		int n = static_cast<unsigned char>(c);
		if (n > 127 && n != -89) {
			return false;
		}
	}
	return true;
}
