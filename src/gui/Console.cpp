
#include "Console.hpp"
#include "Game.hpp"
#include "ResourceTemplates.hpp"
#include "EntityFactory.hpp"
#include "utils/pathfinding.hpp"
#include "GenerationTree.hpp"

Console::Console(Game* game) : game(game) {
	// commands["tp"] = Command({Argument<int>(), Argument<int>()});
}

void Console::draw() {
	if (!active) return;
	pair windowSize = Window::instance->size;
	TextureManager::drawRect({20, windowSize.y / 2 + 10}, {300, windowSize.y / 2 - 30}, {0, 0, 0, 100}, false, true);
	for (uint i = 0; i < history.size(); i++) {
		pair position(40, windowSize.y - 60 - (history.size() - i) * 30);
		TextManager::drawText(history[i], position);
	}
	std::string indexText = "-";
	pair position(25, windowSize.y - 60 - (history.size() - index) * 30);
	TextManager::drawText(indexText, position);
	TextManager::drawText(input, {40, windowSize.y - 60});
}

bool Console::handleEvent(InputEvent event) {
	if (event.id == InputEventId::CONSOLE) {
		active = !active;
		return true;
	}
	if (!active) return false;
	if (event.id == InputEventId::TEXT) {
		if (input.size() < 22) {
			input += event.text;
			index = history.size();
		}
	} else if (event.id == InputEventId::BACKSPACE) {
		if (input.length() > 0) {
			input.pop_back();
			index = history.size();
			return true;
		}
	} else if (event.id == InputEventId::ESCAPE) {
		active = false;
		return true;
	} else if (event.id == InputEventId::RETURN) {
		if (execute(input)) {
			history.push_back(input);
			input.clear();
			index = history.size();
			return true;
		}
		return false;
	} else if (event.id == InputEventId::UP) {
		index = std::max(index, 1u) - 1;
		if (index < history.size()) {
			input = history[index];
			return true;
		}
	} else if (event.id == InputEventId::DOWN) {
		index = std::min(index + 1, uint(history.size()));
		if (index == history.size()) {
			input.clear();
			return true;
		} else if (index < history.size()) {
			input = history[index];
			return true;
		}
	}
	return true;
}

bool Console::execute(std::string input) {
	if (!game->world) return false;
	ECS& ecs = game->world->ecs;
	Entity player = game->world->player;
	Camera& camera = game->world->camera;
	std::vector<std::string> inputs = string::split(input);

	if (inputs[0] == "test") {
		Realm* realm = game->world->playerRealm;
		auto space = [realm](pair pos){ return realm->walkable(pos); };
		ai::seek(pair(0,0), pair(5,5), space, true);
	} else if (inputs[0] == "clear") {
		history.clear();
	} else if (inputs[0] == "refresh") {
		Sprite::loadSpriteSheets();
		ItemPropertyTemplate::setTemplates();
		ItemKindTemplate::setTemplates();
		ItemTemplate::setTemplates();
		ResourceTemplate::setTemplates();
	} else if (inputs[0] == "kill") {
		if (!player) return false;
		ecs.addComponent<DeathComponent>({}, player);
	} else if (inputs[0] == "god") {
		return false;
	} else if (inputs[0] == "tp") {
		if (inputs.size() < 3) return false;
		if (!player) return false;
		int x = std::stoi(inputs[1]);
		int y = std::stoi(inputs[2]);
		ecs.getComponent<PositionComponent>(player).position = vec(x, y);
	} else if (inputs[0] == "place") {
		if (inputs.size() < 2) return false;
		if (!player) return false;
		pair position = vec::round(ecs.getComponent<PositionComponent>(player).position);
		ResourceId::value resourceId = ResourceId::from_string(inputs[1]);
		StationId::value stationId = StationId::from_string(inputs[1]);
		if (resourceId) {
			Entity resource = EntityFactory::createResource(resourceId, game->world->playerRealm->realmId, position);
		} else if (stationId) {
			Entity station = EntityFactory::createStation(stationId, game->world->playerRealm->realmId, position);
		} else {
			return false;
		}
	} else if (inputs[0] == "summon") {
		if (inputs.size() < 2 || !player) return false;
		AnimalId::value animalId = AnimalId::from_string(inputs[1]);
		EnemyId::value enemyId = EnemyId::from_string(inputs[1]);
		int n = 1;
		if (inputs.size() > 2) n = std::stoi(inputs[2]);
		vec position = ecs.getComponent<PositionComponent>(player).position;
		if (animalId) {
			for (int i = 0; i < n; i++) game->world->inspect = EntityFactory::createAnimal(animalId, game->world->playerRealm->realmId, position);
			
		} else if (enemyId) {
			for (int i = 0; i < n; i++) game->world->inspect = EntityFactory::createEnemy(enemyId, game->world->playerRealm->realmId, position);
		} else {
			return false;
		}
	} else if (inputs[0] == "bomb") {
		if (!player) return false;
		vec position = ecs.getComponent<PositionComponent>(player).position;
		EntityFactory::createExplosive(game->world->playerRealm->realmId, position);
	} else if (inputs[0] == "give") {
		if (inputs.size() < 2 || !player) return false;
		ItemId::value itemId = ItemId::from_string(inputs[1]);
		if (!itemId) return false;
		uint count = 1;
		if (inputs.size() > 2) {
			// if (!isUInt(inputs[2])) return false;
			count = std::stoi(inputs[2]);
		}
		while (count > 0) {
			int batch = std::min(count, MAX_STACK);
			count -= batch;
			Entity item = EntityFactory::createItem(itemId, batch);
			Entity rest = ecs.getComponent<InventoryComponent>(player).inventory.add(item);
			if (rest) ecs.addComponent<DeathComponent>({}, rest);
		}
	} else if (inputs[0] == "empty") {
		if (!player) return false;
		ecs.getComponent<InventoryComponent>(player).inventory.clear(true);
	} else if (inputs[0] == "tile") {
		if (inputs.size() != 2 || !game->world || !player) return false;
		GroundId::value groundId = GroundId::from_string(inputs[1]);
		if (!groundId) return false;
		pair position = vec::round(ecs.getComponent<PositionComponent>(player).position);
		game->world->playerRealm->map->tiles[position.x][position.y]->groundId = groundId;
		game->world->playerRealm->map->updateStyle(position, true);
	} else if (inputs[0] == "weather") {
		WeatherId::value weatherId = WeatherId::from_string(inputs[1]);
		if (!weatherId) return false;
		if (weatherId) game->world->playerRealm->environment->weatherId = weatherId;
	} else if (inputs[0] == "zoom") {
		if (inputs.size() != 2) return false;
		int zoom = std::stoi(inputs[1]);
		camera.zoom = zoom;
	} else if (inputs[0] == "gui") {
		if (inputs.size() != 2) return false;
		uint scale = std::stoi(inputs[1]);
		GuiManager::scale = scale;
	} else if (inputs[0] == "save") {
		std::string path = "../saves/" + inputs[1] + ".binary";
		std::fstream file = std::fstream(path, std::ios::out | std::ios::binary);
		if (file) {
			game->world->serialise(file);
			file.close();
			LOG("World saved");
		} else {
			ERROR("No file");
		}
	} else if (inputs[0] == "load") {
		std::string path = "../saves/" + inputs[1] + ".binary";
		std::fstream file = std::fstream(path, std::ios::in | std::ios::binary);
		if (file) {
			game->world = std::make_unique<World>(file);
			file.close();
			LOG("World loaded");
		} else {
			ERROR("No file");
		}
	} else if (inputs[0] == "state") {
		if (inputs.size() != 2) return false;
		GameState::value state = GameState::from_string(inputs[1]);
		if (!state) return false;
		game->gameState = state;
	} else if (inputs[0] == "gui_box") {
		GuiManager::box = !GuiManager::box;
	} else if (inputs[0] == "collider_box") {
		game->world->colliderDraw = !game->world->colliderDraw;
	} else if (inputs[0] == "realm") {
		if (!player) return false;
		PositionComponent& positionComponent = ecs.getComponent<PositionComponent>(player);
		game->world->playerRealm->unlinkChunk(player, positionComponent.chunk);
		int realmId = std::stoi(inputs[1]);
		positionComponent.realmId = realmId;
		Realm* realm = game->world->realmManager.getRealm(realmId);
		realm->linkChunk(player, positionComponent.chunk);
	} else if (inputs[0] == "tickspeed") {
		if (inputs.size() != 2) return false;
		game->world->tickSpeed = std::stoi(inputs[1]);
	} else if (inputs[0] == "banner") {
		if (inputs.size() != 2) return false;
		game->world->guiManager.addBanner(inputs[1]);
	} else if (inputs[0] == "effect") {
		if (inputs.size() < 2) return false;
		Effect::value effect = Effect::from_string(inputs[1]);
		if (!effect) return false;
		uint time = 5000;
		if (inputs.size() > 2) time = std::stoi(inputs[2]);
		ecs.getComponent<EffectComponent>(player).effects[effect].end = game->world->ticks + time;
	} else if (inputs[0] == "tools") {
		for (ItemKind::value itemKind : {ItemKind::SWORD, ItemKind::BOW, ItemKind::AXE, ItemKind::PICK_AXE, ItemKind::HOE}) {
			Entity tool = EntityFactory::createTool(itemKind);
			Entity rest = ecs.getComponent<InventoryComponent>(player).inventory.add(tool);
			if (rest) ecs.addComponent<DeathComponent>({}, rest);
		}
	} else if (inputs[0] == "boots") {
		Entity boots = ecs.createEntity();
		if (!boots) return true;
		ecs.addComponent<ItemComponent>({ItemId::NONE, 1, true}, boots);
		ecs.addComponent<ColliderComponent>({Shape(vec(0.4f, 0.4f))}, boots);
		ecs.addComponent<DurabilityComponent>({255,255}, boots);

		ecs.addComponent<NameComponent>({Textblock("Speedy Boots")}, boots);

		SpriteComponent spriteComponent = {{}, 0.5f};
		spriteComponent.effects[SpriteEffectId::BOUNCE] = {true, 0};
		spriteComponent.spriteStack.setSprite(0, Sprite(SpriteSheet::BOOTS, pair(0, 0)));
		ecs.addComponent<SpriteComponent>(spriteComponent, boots);

		ItemKindComponent itemKindComponent = {};
		itemKindComponent.itemKinds[ItemKind::CLOTHING_FEET] = true;
		itemKindComponent.itemProperties[ItemProperty::VANITY] = 7;
		ecs.addComponent<ItemKindComponent>(itemKindComponent, boots);

		ItemModComponent itemModComponent = {};
		itemModComponent.mods[ItemModId::SPEED] = 50;
		ecs.addComponent<ItemModComponent>(itemModComponent, boots);

		Entity rest = ecs.getComponent<InventoryComponent>(player).inventory.add(boots);
		if (rest) ecs.addComponent<DeathComponent>({}, rest);
	} else if (inputs[0] == "shake") {
		camera.shake = game->world->ticks;
	} else if (inputs[0] == "structure") {
		if (inputs.size() < 2) return false;
		StructureId::value structureId = StructureId::from_string(inputs[1]);
		if (!structureId) return false;
		pair pos = vec::round(ecs.getComponent<PositionComponent>(player).position);
		EntityFactory::createStructure(structureId, game->world->playerRealm->realmId, pos);
	} else if (inputs[0] == "rock") {
		using namespace generation;
		auto rock = SurroundNode(new ResourceNode(ResourceId::BASALT_BOULDER), new SurroundNode(new ResourceNode(ResourceId::BASALT_ROCK), new ResourceNode(ResourceId::BASALT_PEBBLE), 4, 2), 5, 5);
		auto layout = rock.build();
		pair pos = vec::round(ecs.getComponent<PositionComponent>(player).position);
		layout->generate(game->world->playerRealm->realmId, pos);
	} else {
		return false;
	}


	return true;
}