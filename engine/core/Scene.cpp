//
// (c) 2023 Eduardo Doria.
//

#include "Scene.h"

#include "object/Camera.h"
#include "Engine.h"

#include "subsystem/RenderSystem.h"
#include "subsystem/MeshSystem.h"
#include "subsystem/UISystem.h"
#include "subsystem/ActionSystem.h"
#include "subsystem/AudioSystem.h"
#include "subsystem/PhysicsSystem.h"
#include "util/Color.h"

using namespace cg;

Node* Scene::_ppersist_node=new Node("persist");
Scene::Scene(){
	registerComponent<MeshComponent>();
	registerComponent<ModelComponent>();
	registerComponent<BoneComponent>();
	registerComponent<SkyComponent>();
	registerComponent<FogComponent>();
	registerComponent<UIContainerComponent>();
	registerComponent<UILayoutComponent>();
	registerComponent<SpriteComponent>();
	registerComponent<SpriteAnimationComponent>();
    registerComponent<Transform>();
	registerComponent<CameraComponent>();
	registerComponent<LightComponent>();
	registerComponent<ActionComponent>();
	registerComponent<TimedActionComponent>();
	registerComponent<PositionActionComponent>();
	registerComponent<RotationActionComponent>();
	registerComponent<ScaleActionComponent>();
	registerComponent<ColorActionComponent>();
	registerComponent<AlphaActionComponent>();
	registerComponent<ParticlesComponent>();
	registerComponent<ParticlesAnimationComponent>();
	registerComponent<TextComponent>();
	registerComponent<UIComponent>();
	registerComponent<ImageComponent>();
	registerComponent<ButtonComponent>();
	registerComponent<TextEditComponent>();
	registerComponent<MeshPolygonComponent>();
	registerComponent<PolygonComponent>();
	registerComponent<AnimationComponent>();
	registerComponent<KeyframeTracksComponent>();
	registerComponent<MorphTracksComponent>();
	registerComponent<RotateTracksComponent>();
	registerComponent<TranslateTracksComponent>();
	registerComponent<ScaleTracksComponent>();
	registerComponent<TerrainComponent>();
	registerComponent<AudioComponent>();
	registerComponent<TilemapComponent>();
	registerComponent<Body2DComponent>();
	registerComponent<Joint2DComponent>();
	registerComponent<Body3DComponent>();
	registerComponent<Joint3DComponent>();

	registerSystem<PhysicsSystem>();
	registerSystem<ActionSystem>();
	registerSystem<MeshSystem>();
	registerSystem<UISystem>();
	registerSystem<RenderSystem>();
	registerSystem<AudioSystem>();

	camera = NULL_ENTITY;
	defaultCamera = NULL_ENTITY;

	backgroundColor = Vector4(0.0, 0.0, 0.0, 1.0); //sRGB
	shadowsPCF = true;

	hasSceneAmbientLight = false;
	ambientLight = Vector3(1.0, 1.0, 1.0);
	ambientFactor = 0.2;

	enableUIEvents = false;
	_proot_node = new Node("root");
}

Scene::~Scene(){
	destroy();

	std::vector<Entity> entityList = entityManager.getEntityList();
	for(int entity : entityList){
		destroyEntity(entity);
	}

}

void Scene::setCamera(Entity camera){
	if (findComponent<CameraComponent>(camera)){
		this->camera = camera;
		if (defaultCamera != NULL_ENTITY){
			destroyEntity(defaultCamera);
			defaultCamera = NULL_ENTITY;
		}
	}else{
		Log::error("Invalid camera entity: need CameraComponent");
	}
}

Entity Scene::getCamera() const{
	return camera;
}

Entity Scene::createDefaultCamera(){
	defaultCamera = createEntity();
	addComponent<CameraComponent>(defaultCamera, {});
	addComponent<Transform>(defaultCamera, {});

	CameraComponent& camera = getComponent<CameraComponent>(defaultCamera);
	camera.type = CameraType::CAMERA_2D;
	camera.transparentSort = false;

	return defaultCamera;
}

void Scene::setBackgroundColor(Vector4 color){
	this->backgroundColor = color;
}

void Scene::setBackgroundColor(float red, float green, float blue){
	setBackgroundColor(Vector4(red, green, blue, 1.0));
}

Vector4 Scene::getBackgroundColor() const{
	return backgroundColor;
}

void Scene::setShadowsPCF(bool shadowsPCF){
	this->shadowsPCF = shadowsPCF;
}

bool Scene::isShadowsPCF() const{
	return this->shadowsPCF;
}

void Scene::setAmbientLight(float ambientFactor, Vector3 ambientLight){
	this->ambientFactor = ambientFactor;
	this->ambientLight = ambientLight;
	this->hasSceneAmbientLight = true;
}

void Scene::setAmbientLight(float ambientFactor){
	this->ambientFactor = ambientFactor;
	this->hasSceneAmbientLight = true;
}

void Scene::setAmbientLight(Vector3 ambientLight){
	this->ambientLight = ambientLight;
	this->hasSceneAmbientLight = true;
}

float Scene::getAmbientLightFactor() const{
	return this->ambientFactor;
}

Vector3 Scene::getAmbientLightColor() const{
	return this->ambientLight;
}

bool Scene::isSceneAmbientLightEnabled() const{
	return this->hasSceneAmbientLight;
}

void Scene::setSceneAmbientLightEnabled(bool hasSceneAmbientLight){
	this->hasSceneAmbientLight = hasSceneAmbientLight;
}

bool Scene::canReceiveUIEvents(){
	if (Engine::getLastScene() == this || this->enableUIEvents){
		return true;
	}
	return false;
}

bool Scene::isEnableUIEvents() const{
	return this->enableUIEvents;
}

void Scene::setEnableUIEvents(bool enableUIEvents){
	this->enableUIEvents = enableUIEvents;
}

void Scene::addNode(Node* pnode, bool persist){
	if (persist){
		_ppersist_node->addChild(pnode);
	}else{
		_proot_node->addChild(pnode);
	}
}

void Scene::load(){
	if (camera == NULL_ENTITY){
		camera = createDefaultCamera();
	}
	
	for (auto const &pair: systems) {
		if (Engine::isViewLoaded()){
			pair.second->load();
		}
	}
}

void Scene::destroy(){
	for (auto const& pair : systems){
		pair.second->destroy();
	}
	if(this->_proot_node!=nullptr)
	{
		delete _proot_node;
	}
	else
	{
		Log::error("Scene::_proot_node is nullptr");
	}
	_proot_node=nullptr;
}

void Scene::render(){
	for (auto const& pair : systems){
		pair.second->render();
	}
	for( auto const& pnode: _proot_node->getAllChildren()){
		pnode->render();
	}
	for( auto const& pnode: _ppersist_node->getAllChildren()){
		pnode->render();
	}
}


void Scene::update(double dt){
	for (auto const& pair : systems){
		pair.second->update(dt);
	}
	for( auto const& pnode: _proot_node->getAllChildren()){
		pnode->update(dt);
	}
	for( auto const& pnode: _ppersist_node->getAllChildren()){
		pnode->update(dt);
	}
}

void Scene::updateSizeFromCamera(){
	getSystem<RenderSystem>()->updateCameraSize(getCamera());
}

Entity Scene::createEntity(){
    return entityManager.createEntity();
}

void Scene::destroyEntity(Entity entity){

	for (auto const& pair : systems){
		pair.second->entityDestroyed(entity);
	}

	componentManager.entityDestroyed(entity);
	
	entityManager.destroy(entity);
}

int32_t Scene::findBranchLastIndex(Entity entity){
	auto transforms = componentManager.getComponentArray<Transform>();

	size_t index = transforms->getIndex(entity);
	if (index < 0)
		return -1;

	size_t currentIndex = index + 1;
	std::vector<Entity> entityList;
	entityList.push_back(entity);

	bool found = false;
	while (!found){
		if (currentIndex < transforms.get()->size()){
			Transform& transform = componentManager.getComponentFromIndex<Transform>(currentIndex);
			//if not in list
			if (std::find(entityList.begin(), entityList.end(),transform.parent)==entityList.end()) {
				found = true;
			}else{
				entityList.push_back(transforms->getEntity(currentIndex));
				currentIndex++;
			}
		} else {
			found = true;
		}
	}

	currentIndex--;

	return currentIndex;
}

void Scene::addEntityChild(Entity parent, Entity child){
	Signature parentSignature = entityManager.getSignature(parent);
	Signature childSignature = entityManager.getSignature(child);
	
	Signature signature;
	signature.set(componentManager.getComponentType<Transform>(), true);

	if ( ((parentSignature & signature) == signature) && ((childSignature & signature) == signature) ){
		Transform& transformChild = componentManager.getComponent<Transform>(child);
		Transform& transformParent = componentManager.getComponent<Transform>(parent);

		auto transforms = componentManager.getComponentArray<Transform>();

		if (transformChild.parent != parent) {
			transformChild.parent = parent;

			//----------DEBUG
			//Log::debug("Add child - BEFORE");
			//for (int i = 0; i < transforms->size(); i++){
			//	auto transform = transforms->getComponentFromIndex(i);
			//	Log::debug("Transform %i - Entity: %i - Parent: %i: %s", i, transforms->getEntity(i), transform.parent, transform.name.c_str());
			//}
			//----------DEBUG

			//size_t parentIndex = transforms->getIndex(parent);
			size_t childIndex = transforms->getIndex(child);

			//find children of parent and child family
			size_t newIndex = findBranchLastIndex(parent) + 1;
			size_t lastChild = findBranchLastIndex(child);

			int length = lastChild - childIndex + 1;

			if (newIndex > childIndex) {
				newIndex = newIndex - length;
			}

			if (childIndex != newIndex) {
				if (length == 1) {
					transforms->moveEntityToIndex(child, newIndex);
				} else {
					transforms->moveEntityRangeToIndex(child, transforms->getEntity(lastChild),
													   newIndex);
				}
			}

			//----------DEBUG
			//Log::debug("Add child - AFTER");
			//for (int i = 0; i < transforms->size(); i++){
			//	auto transform = transforms->getComponentFromIndex(i);
			//	Log::debug("Transform %i - Entity: %i - Parent: %i: %s", i, transforms->getEntity(i), transform.parent, transform.name.c_str());
			//}
			//Log::debug("\n");
			//----------DEBUG
		}
	}

	sortComponentsByTransform(childSignature);
}

void Scene::sortComponentsByTransform(Signature entitySignature){
	// Mesh component
	if (entitySignature.test(getComponentType<MeshComponent>())){
		auto meshes = componentManager.getComponentArray<MeshComponent>();
		meshes->sortByComponent<Transform>(componentManager.getComponentArray<Transform>());
	}

	// Model component
	if (entitySignature.test(getComponentType<ModelComponent>())){
		auto models = componentManager.getComponentArray<ModelComponent>();
		models->sortByComponent<Transform>(componentManager.getComponentArray<Transform>());
	}

	// Bone component
	if (entitySignature.test(getComponentType<BoneComponent>())){
		auto bones = componentManager.getComponentArray<BoneComponent>();
		bones->sortByComponent<Transform>(componentManager.getComponentArray<Transform>());
	}

	// Polygon component
	if (entitySignature.test(getComponentType<PolygonComponent>())){
		auto polygons = componentManager.getComponentArray<PolygonComponent>();
		polygons->sortByComponent<Transform>(componentManager.getComponentArray<Transform>());
	}

	// UI layout component
	if (entitySignature.test(getComponentType<UILayoutComponent>())){
		auto layout = componentManager.getComponentArray<UILayoutComponent>();
		layout->sortByComponent<Transform>(componentManager.getComponentArray<Transform>());
	}

	// UI component
	if (entitySignature.test(getComponentType<UIComponent>())){
		auto ui = componentManager.getComponentArray<UIComponent>();
		ui->sortByComponent<Transform>(componentManager.getComponentArray<Transform>());
	}

	// Particles component
	if (entitySignature.test(getComponentType<ParticlesComponent>())){
		auto particles = componentManager.getComponentArray<ParticlesComponent>();
		particles->sortByComponent<Transform>(componentManager.getComponentArray<Transform>());
	}

	// Audio component
	if (entitySignature.test(getComponentType<AudioComponent>())){
		auto audios = componentManager.getComponentArray<AudioComponent>();
		audios->sortByComponent<Transform>(componentManager.getComponentArray<Transform>());
	}
}

void Scene::moveChildAux(Entity entity, bool increase, bool stopIfFound){
	Signature entitySignature = entityManager.getSignature(entity);
	
	Signature signature;
	signature.set(componentManager.getComponentType<Transform>(), true);

	if ((entitySignature & signature) == signature){
		auto transforms = componentManager.getComponentArray<Transform>();

		size_t entityIndex = transforms->getIndex(entity);
		Entity entityParent = transforms->getComponent(entity).parent;

		size_t nextIndex = entityIndex;
		if (increase){
			for (int i = (entityIndex+1); i < transforms->size(); i++){
				Transform& next = transforms->getComponentFromIndex(i);
				if (next.parent == entityParent){
					nextIndex = i;
					if (stopIfFound)
						break;
				}
			}
			nextIndex = findBranchLastIndex(transforms->getEntity(nextIndex));
		}else{
			for (int i = (entityIndex-1); i >= 0; i--){
				Transform& next = transforms->getComponentFromIndex(i);
				if (next.parent == entityParent){
					nextIndex = i;
					if (stopIfFound)
						break;
				}
			}
		}

		if (nextIndex != entityIndex)
			transforms->moveEntityToIndex(entity, nextIndex);
	}

	sortComponentsByTransform(entitySignature);
}

void Scene::moveChildToFirst(Entity entity){
	moveChildAux(entity, true, false);
}

void Scene::moveChildUp(Entity entity){
	moveChildAux(entity, true, true);
}

void Scene::moveChildDown(Entity entity){
	moveChildAux(entity, false, true);
}

void Scene::moveChildToLast(Entity entity){
	moveChildAux(entity, false, false);
}