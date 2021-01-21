#include "waterObj.h"


float waterObj::getX() {
	return this->X;
}
float waterObj::getY() {
	return this->Y;
}

float waterObj::getMoveX() {
	return this->moveX;
}
float waterObj::getMoveY() {
	return this->moveY;
}

void waterObj::setMoveX(float moveX) {
	this->moveX = moveX;
}

void waterObj::setMoveY(float moveY) {
	this->moveY = moveY;
}

void waterObj::updateWater() {
	setX(getX() + getMoveX());
	setY(getY() + getMoveY());
}

void waterObj::setX(float X) {
	this->X = X;
}

void waterObj::setY(float Y) {
	this->Y = Y;
}

int waterObj::getPopNum() {
	return this->popNum;
}

void waterObj::setPopNum() {
	this->popNum = this->popNum + 1;
}