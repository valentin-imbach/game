
#include "Command.hpp"
#include "Console.hpp"
#include "Game.hpp"
#include "EntityFactory.hpp"
#include "ClusterTemplates.hpp"

void Console::setCommands() {
	commands["tp"] = std::make_unique<Command<float, float>>([this](float x, float y){
		if (!player) return false;
		ecs->getComponent<PositionComponent>(player).position = vec(x, y);
		return true;
	});

	commands["clear"] = std::make_unique<Command<>>([this](){
		history.clear();
		return true;
	});

	// commands["hotload"] = std::make_unique<Command<>>([this](){
	// 	Sprite::loadSpriteSheets();
	// 	ItemPropertyTemplate::setTemplates();
	// 	ItemKindTemplate::setTemplates();
	// 	ItemTemplate::setTemplates();
	// 	ResourceTemplate::setTemplates();
	// 	return true;
	// });

	commands["kill"] = std::make_unique<Command<>>([this](){
		if (!player) return false;
		ecs->addComponent<DeathComponent>({}, player);
		return true;
	});

	commands["place"] = std::make_unique<Command<std::string>>([this](std::string str){
		if (!player) return false;
		PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(player);
		pair position = vec::round(positionComponent.position);
		ResourceId::value resourceId = ResourceId::from_string(str);
		StationId::value stationId = StationId::from_string(str);
		if (resourceId) return !!EntityFactory::createResource(resourceId, world->playerRealm->realmId, position);
		if (stationId) return !!EntityFactory::createStation(stationId, world->playerRealm->realmId, position);
		return false;
	});

	commands["summon"] = std::make_unique<Command<std::string>>([this](std::string str){
		if (!player) return false;
		PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(player);
		
		vec offset = vec::polar(noise::Float(ticks, 2*M_PI), 0.01f);
		vec position = positionComponent.position + offset;

		AnimalId::value animalId = AnimalId::from_string(str);
		EnemyId::value enemyId = EnemyId::from_string(str);
		if (animalId) return !!EntityFactory::createAnimal(animalId, game->world->playerRealm->realmId, position);
		if (enemyId) return !!EntityFactory::createEnemy(enemyId, game->world->playerRealm->realmId, position);
		return false;
	});

	commands["bomb"] = std::make_unique<Command<>>([this](){
		if (!player) return false;
		PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(player);
		vec position = positionComponent.position;
		return !!EntityFactory::createExplosive(world->playerRealm->realmId, position);
	});
	
	commands["give"] = std::make_unique<Command<std::string, int>>([this](std::string str, int n){
		if (!player || n <= 0) return false;
		ItemId::value itemId = ItemId::from_string(str);
		InventoryComponent& inventoryComponent = ecs->getComponent<InventoryComponent>(player);
		if (!itemId) return false;
		while (n > 0) {
			int batch = std::min(n, int(MAX_STACK));
			n -= batch;
			Entity item = EntityFactory::createItem(itemId, batch);
			Entity rest = inventoryComponent.inventory.add(item);
			if (rest) ecs->addComponent<DeathComponent>({}, rest);
		}
		return true;
	});

	commands["empty"] = std::make_unique<Command<>>([this](){
		if (!player) return false;
		InventoryComponent& inventoryComponent = ecs->getComponent<InventoryComponent>(player);
		inventoryComponent.inventory.clear(true);
		return true;
	});

	commands["tile"] = std::make_unique<Command<std::string>>([this](std::string str){
		GroundId::value groundId = GroundId::from_string(str);
		if (!player) return false;
		PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(player);
		pair position = vec::round(positionComponent.position);
		world->playerRealm->chunkManager.setGround(position, groundId);
		return true;
	});

	commands["wall"] = std::make_unique<Command<std::string>>([this](std::string str){
		WallId::value wallId = WallId::from_string(str);
		if (!player) return false;
		PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(player);
		pair position = vec::round(positionComponent.position);
		world->playerRealm->chunkManager.setWall(position, wallId);
		return true;
	});

	commands["weather"] = std::make_unique<Command<std::string>>([this](std::string str){
		WeatherId::value weatherId = WeatherId::from_string(str);
		if (!weatherId) return false;
		world->playerRealm->environment->weatherId = weatherId;
		return true;
	});

	commands["zoom"] = std::make_unique<Command<int>>([this](int zoom){
		if (!camera) return false;
		camera->zoom = zoom;
		return true;
	});

	commands["realm"] = std::make_unique<Command<int>>([this](int realmId){
		if (!player) return false;
		PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(player);
		positionComponent.realmId = realmId;

		Realm* realm = world->realmManager.getRealm(realmId);
		pair chunk = realm->chunkManager.getChunk(positionComponent.position);
		world->playerRealm->detach(player);

		realm->chunkManager.generateChunk(chunk, ChunkStage::LOADED, realm->environment.get());
		realm->attach(player, chunk);
		return true;
	});

	commands["tickspeed"] = std::make_unique<Command<int>>([this](int ts){
		if (!world) return false;
		world->tickSpeed = ts;
		return true;
	});

	commands["banner"] = std::make_unique<Command<std::string>>([this](std::string str){
		if (!world) return false;
		world->guiManager.addBanner(str);
		return true;
	});

	commands["effect"] = std::make_unique<Command<std::string, int>>([this](std::string str, int time){
		Effect::value effect = Effect::from_string(str);
		if (!player || !effect || time < 0) return false;
		EffectComponent& effectComponent = ecs->getComponent<EffectComponent>(player);
		effectComponent.effects[effect].end = world->ticks + time;
		return true;
	});
	
	commands["tools"] = std::make_unique<Command<>>([this](){
		if (!player) return false;
		InventoryComponent& inventoryComponent = ecs->getComponent<InventoryComponent>(player);
		for (ItemKind::value itemKind : {ItemKind::SWORD, ItemKind::BOW, ItemKind::AXE, ItemKind::PICK_AXE, ItemKind::HOE}) {
			Entity tool = EntityFactory::createTool(itemKind);
			Entity rest = inventoryComponent.inventory.add(tool);
			if (rest) ecs->addComponent<DeathComponent>({}, rest);
		}
		return true;
	});

	commands["boots"] = std::make_unique<Command<>>([this](){
		if (!player) return false;
		InventoryComponent& inventoryComponent = ecs->getComponent<InventoryComponent>(player);
		Entity boots = ecs->createEntity();
		ecs->addComponent<ItemComponent>({ItemId::NONE, 1, true}, boots);
		ecs->addComponent<ColliderComponent>({Shape(vec(0.4f, 0.4f))}, boots);
		ecs->addComponent<DurabilityComponent>({255,255}, boots);
		ecs->addComponent<NameComponent>({Textblock("Speedy Boots")}, boots);

		SpriteComponent spriteComponent = {{}, 0.5f};
		spriteComponent.effects[SpriteEffectId::BOUNCE] = {true, 0};
		spriteComponent.spriteStack.setSprite(0, Sprite(SpriteSheet::BOOTS, pair(0, 0)));
		ecs->addComponent<SpriteComponent>(spriteComponent, boots);

		ItemKindComponent itemKindComponent = {};
		itemKindComponent.itemKinds[ItemKind::CLOTHING_FEET] = true;
		itemKindComponent.itemProperties[ItemProperty::VANITY] = 7;
		ecs->addComponent<ItemKindComponent>(itemKindComponent, boots);

		ItemModComponent itemModComponent = {};
		itemModComponent.mods[ItemModId::SPEED] = 50;
		ecs->addComponent<ItemModComponent>(itemModComponent, boots);

		Entity rest = inventoryComponent.inventory.add(boots);
		if (rest) ecs->addComponent<DeathComponent>({}, rest);
		return true;
	});

	commands["shake"] = std::make_unique<Command<>>([this](){
		if (!camera) return false;
		camera->shake = world->ticks;
		return true;
	});

	commands["structure"] = std::make_unique<Command<std::string>>([this](std::string str){
		StructureId::value structureId = StructureId::from_string(str);
		if (!player || !structureId) return false;
		PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(player);
		pair pos = vec::round(positionComponent.position);
		EntityFactory::createStructure(structureId, world->playerRealm->realmId, pos);
		return true;
	});

	commands["cluster"] = std::make_unique<Command<std::string>>([this](std::string str){
		ClusterId::value clusterId = ClusterId::from_string(str);
		if (!player || !clusterId) return false;
		PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(player);
		pair position = vec::round(positionComponent.position);
		Cluster::templates[clusterId]->generate(world->ticks, world->playerRealm, position);
		return true;
	});

	commands["bucket"] = std::make_unique<Command<>>([this](){
		if (!player) return false;
		InventoryComponent& inventoryComponent = ecs->getComponent<InventoryComponent>(player);
		Entity bucket = EntityFactory::createBucket();
		Entity rest = inventoryComponent.inventory.add(bucket);
		if (rest) ecs->addComponent<DeathComponent>({}, rest);
		return true;
	});

	commands["loot"] = std::make_unique<Command<int>>([this](int value){
		if (!player) return false;
		InventoryComponent& inventoryComponent = ecs->getComponent<InventoryComponent>(player);
		Entity loot = Loot::fromValue(SDL_GetTicks(), value);
		Entity rest = inventoryComponent.inventory.add(loot);
		if (rest) ecs->addComponent<DeathComponent>({}, rest);
		return true;
	});

	commands["spawner"] = std::make_unique<Command<std::string>>([this](std::string str){
		if (!player) return false;
		PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(player);
		pair position = vec::round(positionComponent.position);
		EnemyId::value enemyId = EnemyId::from_string(str);
		if (!enemyId) return false;
		EntityFactory::createSpawner(positionComponent.realmId, position, enemyId);
		return true;
	});

	commands["boom"] = std::make_unique<Command<>>([this](){
		if (!player) return false;
		Entity boom = EntityFactory::createBoomerang();
		if (!boom) return false;
		InventoryComponent& inventoryComponent = ecs->getComponent<InventoryComponent>(player);
		Entity rest = inventoryComponent.inventory.add(boom);
		return true;
	});

	commands["staff"] = std::make_unique<Command<>>([this](){
		if (!player) return false;
		Entity staff = EntityFactory::createStaff();
		if (!staff) return false;
		InventoryComponent& inventoryComponent = ecs->getComponent<InventoryComponent>(player);
		Entity rest = inventoryComponent.inventory.add(staff);
		return true;
	});
}

