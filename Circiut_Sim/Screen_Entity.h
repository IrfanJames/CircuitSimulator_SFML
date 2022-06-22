#pragma once

#include "Screen_Global.h"
#include "SFML/Graphics.hpp";
using sf::Sprite; using sf::Texture; using sf::RenderWindow;

class Entity {
public:
	float x, y, angle;
	Sprite sprite;

	Entity() { x = W / 2; y = H / 2; angle = 0.0f; }

	Entity(Texture *t, float _x, float _y, float Angle = 0.0f) {
		x = _x; y = _y; angle = Angle;
		sprite.setTexture(*t);
		sprite.setOrigin(t->getSize().x / 2, 0);
	}

	virtual void update() {};

	void draw(RenderWindow& app, int Gap) {
		
		/*Just On Sqr*/
		x = x - (int)x % Gap;
		y = y - (int)y % Gap;

		sprite.setPosition(x, y);
		sprite.setRotation(angle);
		app.draw(sprite);
	}
	~Entity() {
		;
	}

};