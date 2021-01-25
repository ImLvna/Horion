#pragma once

#include "../../Memory/GameData.h"
#include "Commands/BindCommand.h"
#include "Commands/BruhCommand.h"
#include "Commands/CoordsCommand.h"
#include "Commands/EjectCommand.h"
#include "Commands/FriendListCommand.h"
#include "Commands/HelpCommand.h"
#include "Commands/HideCommand.h"
#include "Commands/ICommand.h"
#include "Commands/ModulesCommand.h"
#include "Commands/SayCommand.h"
#include "Commands/ServerCommand.h"
#include "Commands/ToggleCommand.h"
#include "Commands/TopCommand.h"
#include "Commands/UnbindCommand.h"
#include "Commands/ConfigCommand.h"
#include "Commands/NameSpoofCommand.h"
#include "Commands/SetprefixCommand.h"
#include "Commands/NbtCommand.h"
#include "Commands/ExecuteCommand.h"
#include "Commands/WaypointCommand.h"


#ifdef _DEBUG
#include "Commands/TestCommand.h"
#endif

#include <algorithm>
#include <string>
#include <vector>

class CommandMgr {
private:
	GameData* gameData;
	std::vector<IMCCommand*> commandList;

public:
	CommandMgr(GameData* gm);
	~CommandMgr();

	char prefix = '.';

	void initCommands();
	void disable();
	std::vector<IMCCommand*>* getCommandList();

	void execute(char* message);
};

extern CommandMgr* cmdMgr;
