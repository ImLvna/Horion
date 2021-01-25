#include "ModuleManager.h"
#include "../../Utils/Logger.h"
#include "../../Utils/Json.hpp"

using json = nlohmann::json;

ModuleManager::ModuleManager(GameData* gameData) {
	this->gameData = gameData;
}

ModuleManager::~ModuleManager() {
	initialized = false;
	auto lock = this->lockModuleListExclusive();
	this->moduleList.clear();
}

void ModuleManager::initModules() {
	logF("Initializing modules");
	{
		auto lock = this->lockModuleListExclusive();

		this->moduleList.push_back(std::shared_ptr<IModule>(new HudModule()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new RainbowSky()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new EditionFaker()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new AutoSprint()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new NoPacket()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Fucker()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new ClickGuiMod()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new FullBright()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new StackableItem()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new MidClick()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new NoFriends()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Derp()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new NightMode()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Zoom()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Nbt()));
		//this->moduleList.push_back(std::shared_ptr<IModule>(new Godmode()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Freelook()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new	NoPaintingCrash()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new Waypoints()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new TimeChanger()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new	Compass()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new VanillaPlus()));


#ifdef _DEBUG
		this->moduleList.push_back(std::shared_ptr<IModule>(new PacketLogger()));
		this->moduleList.push_back(std::shared_ptr<IModule>(new TestModule()));
#endif

		// Sort modules alphabetically
		std::sort(moduleList.begin(), moduleList.end(), [](auto lhs, auto rhs) {
			auto current = lhs;
			auto other = rhs;
			return std::string{*current->getModuleName()} < std::string{*other->getModuleName()};
		});

		initialized = true;
	}
	
	this->getModule<HudModule>()->setEnabled(true);
	this->getModule<ClickGuiMod>()->setEnabled(false);
}

void ModuleManager::disable() {
	auto lock = this->lockModuleList();
	for (auto& mod : this->moduleList) {
		if (mod->isEnabled())
			mod->setEnabled(false);
	}
}

void ModuleManager::onLoadConfig(void* confVoid) {
	auto conf = reinterpret_cast<json*>(confVoid);
	if (!isInitialized())
		return;
	auto lock = this->lockModuleList();
	for (auto& mod : this->moduleList) {
		mod->onLoadConfig(conf);
	}

	this->getModule<HudModule>()->setEnabled(true);
	this->getModule<ClickGuiMod>()->setEnabled(false);
}

void ModuleManager::onSaveConfig(void* confVoid) {
	auto conf = reinterpret_cast<json*>(confVoid);
	if (!isInitialized())
		return;
	auto lock = this->lockModuleList();
	for (auto& mod : this->moduleList) {
		mod->onSaveConfig(conf);
	}
}

void ModuleManager::onTick(C_GameMode* gameMode) {
	if (!isInitialized())
		return;
	auto lock = this->lockModuleList();
	for (auto& mod : this->moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onTick(gameMode);
	}
}

void ModuleManager::onAttack(C_Entity* attackEnt) {
	if (!isInitialized())
		return;

	auto lock = this->lockModuleList();
	for (auto& mod : this->moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onAttack(attackEnt);
	}
}

void ModuleManager::onKeyUpdate(int key, bool isDown) {
	if (!isInitialized())
		return;
	auto lock = this->lockModuleList();
	for (auto& mod : this->moduleList) {
		mod->onKeyUpdate(key, isDown);
	}
}

void ModuleManager::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (!isInitialized())
		return;
	auto mutex = this->lockModuleList();

	for (auto& mod : this->moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onPreRender(renderCtx);
	}
}

void ModuleManager::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	if (!isInitialized())
		return;
	auto mutex = this->lockModuleList();

	for (auto& mod : this->moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onPostRender(renderCtx);
	}
}

void ModuleManager::onSendPacket(C_Packet* packet) {
	if (!isInitialized())
		return;
	auto lock = this->lockModuleList();
	for (auto& it : moduleList) {
		if (it->isEnabled() || it->callWhenDisabled())
			it->onSendPacket(packet);
	}
}

std::vector<std::shared_ptr<IModule>>* ModuleManager::getModuleList() {
	return &this->moduleList;
}

int ModuleManager::getModuleCount() {
	return (int)moduleList.size();
}

int ModuleManager::getEnabledModuleCount() {
	int i = 0;
	auto lock = this->lockModuleList();
	for (auto& it : moduleList) {
		if (it->isEnabled()) i++;
	}
	return i;
}
void ModuleManager::onMove(C_MoveInputHandler* hand) {
	if (!isInitialized())
		return;
	auto lock = this->lockModuleList();
	for (auto& it : moduleList) {
		if (it->isEnabled() || it->callWhenDisabled())
			it->onMove(hand);
	}
}
void ModuleManager::onLevelRender() {
	if (!isInitialized())
		return;
	auto lock = this->lockModuleList();
	for (auto& it : moduleList) {
		if (it->isEnabled() || it->callWhenDisabled())
			it->onLevelRender();
	}
}

ModuleManager* moduleMgr = new ModuleManager(&g_Data);
