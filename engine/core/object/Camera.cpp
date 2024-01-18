//
// (c) 2021 Eduardo Doria.
//

#include "Camera.h"

#include "util/Angle.h"
#include "Engine.h"
#include "subsystem/RenderSystem.h"

using namespace cg;

Camera::Camera(Scene* scene): Object(scene){
    addComponent<CameraComponent>({});

    CameraComponent& cameraComponent = getComponent<CameraComponent>();
    Transform& transform = getComponent<Transform>();

    transform.position = Vector3(0, 0, 1);

    //ORTHO
    cameraComponent.left = 0;
    cameraComponent.right = Engine::getCanvasWidth();
    cameraComponent.bottom = 0;
    cameraComponent.top = Engine::getCanvasHeight();
    cameraComponent.orthoNear = -10;
    cameraComponent.orthoFar = 10;

    //PERSPECTIVE
    cameraComponent.y_fov = 0.75;

    if (Engine::getCanvasWidth() != 0 && Engine::getCanvasHeight() != 0) {
        cameraComponent.aspect = (float) Engine::getCanvasWidth() / (float) Engine::getCanvasHeight();
    }else{
        cameraComponent.aspect = 1.0;
    }

    cameraComponent.perspectiveNear = 1;
    cameraComponent.perspectiveFar = 200;

    // default CameraComponent is CAMERA_2D
    cameraComponent.type = CameraType::CAMERA_PERSPECTIVE;
}

Camera::Camera(Scene* scene, Entity entity): Object(scene, entity){
}

void Camera::activate(){
    scene->setCamera(entity);
}

void Camera::setOrtho(float left, float right, float bottom, float top, float near, float far){
    CameraComponent& camera = getComponent<CameraComponent>();

    camera.type = CameraType::CAMERA_ORTHO;

    camera.left = left;
    camera.right = right;
    camera.bottom = bottom;
    camera.top = top;
    camera.orthoNear = near;
    camera.orthoFar = far;
    
    camera.automatic = false;

    camera.needUpdate = true;
}

void Camera::setPerspective(float y_fov, float aspect, float near, float far){
    CameraComponent& camera = getComponent<CameraComponent>();

    camera.type = CameraType::CAMERA_PERSPECTIVE;

    camera.y_fov = Angle::defaultToRad(y_fov);
    camera.aspect = aspect;
    camera.perspectiveNear = near;
    camera.perspectiveFar = far;
    
    camera.automatic = false;

    camera.needUpdate = true;
}

void Camera::setType(CameraType type){
    CameraComponent& camera = getComponent<CameraComponent>();
    
    if (camera.type != type){
        camera.type = type;

        camera.needUpdate = true;
    }
}

CameraType Camera::getType() const{
    CameraComponent& camera = getComponent<CameraComponent>();
    
    return camera.type;
}

void Camera::setView(Vector3 view){
    CameraComponent& camera = getComponent<CameraComponent>();

    if (camera.view != view){
        camera.view = view;

        camera.needUpdate = true;
    }
}

void Camera::setView(const float x, const float y, const float z){
    setView(Vector3(x,y,z));
}

Vector3 Camera::getView() const{
    CameraComponent& camera = getComponent<CameraComponent>();
    return camera.view;
}

void Camera::setUp(Vector3 up){
    CameraComponent& camera = getComponent<CameraComponent>();
    
    if (camera.up != up){
        camera.up = up;

        camera.needUpdate = true;
    }
}

void Camera::setUp(const float x, const float y, const float z){
    setUp(Vector3(x,y,z));
}

Vector3 Camera::getUp() const{
    CameraComponent& camera = getComponent<CameraComponent>();
    return camera.up;
}

Vector3 Camera::getWorldView() const{
    CameraComponent& camera = getComponent<CameraComponent>();
    return camera.worldView;
}

Vector3 Camera::getWorldUp() const{
    CameraComponent& camera = getComponent<CameraComponent>();
    return camera.worldUp;
}

Vector3 Camera::getWorldRight() const{
    CameraComponent& camera = getComponent<CameraComponent>();
    return camera.worldRight;
}

void Camera::rotateView(float angle){
    if (angle != 0){
        CameraComponent& camera = getComponent<CameraComponent>();
        Transform& transf = getComponent<Transform>();

        Vector3 viewCenter(camera.view.x - transf.position.x, camera.view.y - transf.position.y, camera.view.z - transf.position.z);

        Matrix4 rotation;
        rotation = Matrix4::rotateMatrix(angle, camera.up);
        viewCenter = rotation * viewCenter;

        camera.view = Vector3(viewCenter.x + transf.position.x, viewCenter.y + transf.position.y, viewCenter.z + transf.position.z);

        camera.needUpdate = true;
    }
}

void Camera::rotatePosition(float angle){
    if (angle != 0){
        CameraComponent& camera = getComponent<CameraComponent>();
        Transform& transf = getComponent<Transform>();

        Vector3 positionCenter(transf.position.x - camera.view.x, transf.position.y - camera.view.y, transf.position.z - camera.view.z);

        Matrix4 rotation;
        rotation = Matrix4::rotateMatrix(angle, camera.up);
        positionCenter = rotation * positionCenter;

        transf.position = Vector3(positionCenter.x + camera.view.x, positionCenter.y + camera.view.y, positionCenter.z + camera.view.z);

        transf.needUpdate = true;
        camera.needUpdate = true;
    }
}

void Camera::elevateView(float angle){
    if (angle != 0){
        CameraComponent& camera = getComponent<CameraComponent>();
        Transform& transf = getComponent<Transform>();

        Vector3 viewCenter(camera.view.x - transf.position.x, camera.view.y - transf.position.y, camera.view.z - transf.position.z);

        Matrix4 rotation;
        rotation = Matrix4::rotateMatrix(angle, viewCenter.crossProduct(camera.up));
        viewCenter = rotation * viewCenter;

        camera.view = Vector3(viewCenter.x + transf.position.x, viewCenter.y + transf.position.y, viewCenter.z + transf.position.z);

        camera.needUpdate = true;
    }
}

void Camera::elevatePosition(float angle){
    if (angle != 0){
        CameraComponent& camera = getComponent<CameraComponent>();
        Transform& transf = getComponent<Transform>();

        Vector3 positionCenter(transf.position.x - camera.view.x, transf.position.y - camera.view.y, transf.position.z - camera.view.z);

        Matrix4 rotation;
        rotation = Matrix4::rotateMatrix(angle, positionCenter.crossProduct(camera.up));
        positionCenter = rotation * positionCenter;

        transf.position = Vector3(positionCenter.x + camera.view.x, positionCenter.y + camera.view.y, positionCenter.z + camera.view.z);

        transf.needUpdate = true;
        camera.needUpdate = true;
    }
}

void Camera::moveForward(float distance){
    if (distance != 0){
        CameraComponent& camera = getComponent<CameraComponent>();
        Transform& transf = getComponent<Transform>();

        Vector3 viewCenter(camera.view.x - transf.position.x, camera.view.y - transf.position.y, camera.view.z - transf.position.z);

        viewCenter.normalize();

        camera.view = camera.view + (viewCenter.normalize() * distance);
        transf.position = transf.position + (viewCenter.normalize() * distance);

        transf.needUpdate = true;
        camera.needUpdate = true;
    }
}

void Camera::walkForward(float distance){
    if (distance != 0){
        CameraComponent& camera = getComponent<CameraComponent>();
        Transform& transf = getComponent<Transform>();

        Vector3 viewCenter(camera.view.x - transf.position.x, camera.view.y - transf.position.y, camera.view.z - transf.position.z);

        Vector3 aux = viewCenter.dotProduct(camera.up) * camera.up / camera.up.squaredLength();

        Vector3 walkVector = viewCenter - aux;

        camera.view = camera.view + (walkVector.normalize() * distance);
        transf.position = transf.position + (walkVector.normalize() * distance);

        transf.needUpdate = true;
        camera.needUpdate = true;
    }
}

void Camera::slide(float distance){
    if (distance != 0){
        CameraComponent& camera = getComponent<CameraComponent>();
        Transform& transf = getComponent<Transform>();

        Vector3 viewCenter(camera.view.x - transf.position.x, camera.view.y - transf.position.y, camera.view.z - transf.position.z);

        Vector3 slideVector = viewCenter.crossProduct(camera.up);

        camera.view = camera.view + (slideVector.normalize() * distance);
        transf.position = transf.position + (slideVector.normalize() * distance);

        transf.needUpdate = true;
        camera.needUpdate = true;
    }
}

void Camera::setRenderToTexture(bool renderToTexture){
    CameraComponent& camera = getComponent<CameraComponent>();

	camera.renderToTexture = renderToTexture;
}

bool Camera::isRenderToTexture() const{
    CameraComponent& camera = getComponent<CameraComponent>();

	return camera.renderToTexture;
}

Framebuffer* Camera::getFramebuffer(){
    CameraComponent& camera = getComponent<CameraComponent>();

	return camera.framebuffer;
}

void Camera::setFramebufferSize(int width, int height){
    CameraComponent& camera = getComponent<CameraComponent>();

	camera.framebuffer->setWidth(width);
	camera.framebuffer->setHeight(height);

	if (camera.renderToTexture){
		scene->getSystem<RenderSystem>()->updateFramebuffer(camera);
		scene->getSystem<RenderSystem>()->updateCameraSize(entity);
	}
}

void Camera::setFramebufferFilter(TextureFilter filter){
    CameraComponent& camera = getComponent<CameraComponent>();

	camera.framebuffer->setMinFilter(filter);
	camera.framebuffer->setMagFilter(filter);

	if (camera.renderToTexture){
		scene->getSystem<RenderSystem>()->updateFramebuffer(camera);
	}
}

void Camera::setTransparentSort(bool transparentSort){
    CameraComponent& camera = getComponent<CameraComponent>();

    camera.transparentSort = transparentSort;
}

bool Camera::isTransparentSort() const{
    CameraComponent& camera = getComponent<CameraComponent>();

    return camera.transparentSort;
}

void Camera::updateCamera(){
    Transform& transform = getComponent<Transform>();
    CameraComponent& camera = getComponent<CameraComponent>();

    scene->getSystem<RenderSystem>()->updateCamera(camera, transform);
}