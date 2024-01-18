//
// (c) 2021 Eduardo Doria.
//

#include "ScaleAction.h"

using namespace Supernova;

ScaleAction::ScaleAction(Scene* scene): TimedAction(scene){
    addComponent<ScaleActionComponent>({});
}

void ScaleAction::setAction(Vector3 startScale, Vector3 endScale, float duration, bool loop){
    TimedAction::setAction(duration, loop);

    ScaleActionComponent& scaleAction = getComponent<ScaleActionComponent>();

    scaleAction.startScale = startScale;
    scaleAction.endScale = endScale;
}