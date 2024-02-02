#include "UINode.h"

namespace cg
{
    void UINode::setWidth(int width){
        this->_width = width;
    }

    void UINode::setHeight(int height){
        this->_height = height;
    }

    void UINode::setSize(int width, int height){
        this->_width = width;
        this->_height = height;
    }

    int UINode::getWidth() const{
        return _width;
    }

    int UINode::getHeight() const{
        return _height;
    }

    void UINode::setAnchorPoints(const Vector2& anchorPoints){
        this->_anchorPoints = anchorPoints;
    }

    Vector2 UINode::getAnchorPoints() const{
        return _anchorPoints;
    }

    void UINode::setAnchorPoints(float x, float y){
        this->_anchorPoints.x = x;
        this->_anchorPoints.y = y;
    }

    void UINode::setAnchorPointX(float x){
        this->_anchorPoints.x = x;
    }

    void UINode::setAnchorPointY(float y){
        this->_anchorPoints.y = y;
    }

    float UINode::getAnchorPointX() const{
        return _anchorPoints.x;
    }

    float UINode::getAnchorPointY() const{
        return _anchorPoints.y;
    }

    void UINode::setPosition(const Vector2& position){
        Node::setPosition(position.x, position.y,0);
    }

    void UINode::setPosition(float x, float y){
        Node::setPosition(x, y,0);
    }

    Vector2 UINode::getPosition() const{
        return Vector2(Node::getPositionX(), Node::getPositionY());
    }

    float UINode::getPositionX() const{
        return Node::getPositionX();
    }

    float UINode::getPositionY() const{
        return Node::getPositionY();
    }
}