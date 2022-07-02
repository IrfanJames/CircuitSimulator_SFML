#pragma once

#include "Circuit_Global.h"
#include "SFML/Graphics.hpp";

sf::Texture compTex[8];

class Entity {
public:
	int serial = 0;
	float x, y, angle;
	sf::Sprite sprite;

	Entity() { x = W / 2; y = H / 2; angle = 0.0f; }

	Entity(int s, float _x, float _y, float Angle = 0.0f) {
		x = _x; y = _y; angle = Angle;
		serial = s;
		sprite.setTexture(compTex[s]);
		sprite.setOrigin(compTex[s].getSize().x / 2, 0);
	}

	virtual void update() {};

	void draw(sf::RenderWindow& app, int Gap) {
		
		/*Just On Sqr
		//x = trim(x, Gap);
		//y = trim(y, Gap);*/

		sprite.setPosition(x, y);
		sprite.setRotation(angle);
		app.draw(sprite);
	}
	~Entity() {
		;
	}

};