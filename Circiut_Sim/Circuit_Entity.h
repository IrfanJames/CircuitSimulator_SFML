#pragma once

#include "Circuit_Global.h"
#include <iostream>
#include "SFML/Graphics.hpp";

sf::Texture compTex[8];
enum componentType {
	Cap,
	Cur,
	Dod,
	Ind,
	Res,
	SwO,
	Vol,
	SwC
};
class Entity {
private:
	sf::Font s_font;

public:

	double resistance = 1000;
	double voltage =	5;
	double current =	0.0002;

public:
	int serial = 0;
	float x, y, angle;
	sf::Sprite sprite;
	sf::Text valueText;

	Entity() {
		x = W / 2; y = H / 2; angle = 0.0f;
		s_font.loadFromFile("assets/Fonts/CalibriL_1.ttf"); /*CALIBRI_1*/
		valueText.setFont(s_font);
	}
	Entity(int s, float _x, float _y, float Angle = 0.0f) {
		x = _x; y = _y; angle = Angle;
		serial = s;
		sprite.setTexture(compTex[s]);
		sprite.setOrigin(compTex[s].getSize().x / 2, 0);

		s_font.loadFromFile("assets/Fonts/CalibriL_1.ttf"); /*CALIBRI_1*/
		valueText.setFont(s_font);
		/*valueText.setString(" ");*/ updateValueText();
		//valueText.setFillColor(normalCompColor);
		valueText.setCharacterSize(13);
	}

	sf::Vector2f endNodePos() {
		return sf::Vector2f(x - 75 * (int)sin(angle * DegToRad), y + 75 * (int)cos(angle * DegToRad));
	}
	
	void updateValueText() {
		
		std::string str;
		double value = 0;
		static std::string preScalers = "nµmkMG";

		/*Value*/
		switch (serial) {
		case Cap: {						break; }
		case Cur: {	value = current;	break; }
		case Dod: {						break; }
		case Ind: {						break; }
		case Res: {	value = resistance;	break; }
		case SwO: {						break; }
		case Vol: {	value = voltage;	break; }
		case SwC: {						break; }
		default:  { value = 0; break; }
		}

		/*Value -> prescaler */ {

			if (value == 0) { str = "0 "; }
			else if (1 <= value) {
				double temp = value;
				int c = 0;
				for (; c < 3; c++) {
					temp /= 1000;
					if (temp < 1) break;
					value = temp;
				}

				str = std::to_string(value);

				while (str.back() == '0') str.pop_back();
				if (str.back() == '.') str.pop_back();
				str += " ";

				if (c != 0) str += preScalers[2 + c];

			}
			else if (value < 1) {
				int c = 0;
				for (; c < 3; c++) {
					value *= 1000;
					if (value >= 1) break;
				}

				str = std::to_string(value);

				while (str.back() == '0') str.pop_back();
				if (str.back() == '.') str.pop_back();
				str += " ";

				str += preScalers[2 - c];

			}

		}

		/*Unit*/
		switch (serial) {
		case Cap: { str += "F";		break; }
		case Cur: { str += "A";		break; }
		case Dod: { str += "";		break; }
		case Ind: { str += "H";		break; }
		case Res: { str += "Ohm";	break; }
		case SwO: { /*str += "";*/	break; }
		case Vol: { str += "V";		break; }
		case SwC: { /*str += "";*/	break; }
		default:  { str += "[No Unit: Component not in library]"; break; }
		}

		valueText.setString(str);
		//valueText.setString("ABC");
	}
	


	void draw(sf::RenderWindow& app) {

		/*Just On Sqr
		//x = trim(x, Gap);
		//y = trim(y, Gap);*/
		
		//s_font.loadFromFile("assets/Fonts/CalibriL_1.ttf"); /*CALIBRI_1*/
		valueText.setFont(s_font);
		static int offSet[4][2] = {
			{1  * 15 + 3,2  * 15 + 0},
			{-4 * 15 + 0,1  * 15 + 0},
			{1  * 15 + 3,-3 * 15 + 0},
			{1  * 15 + 0,-2 * 15 - 3}
		};
		valueText.setPosition(x + offSet[(int)(angle / 90)][0], y + offSet[(int)(angle / 90)][1]); // badPractice for gap = 15
		app.draw(valueText);

		sprite.setPosition(x, y);
		sprite.setRotation(angle);
		app.draw(sprite);
	}

	virtual void update() {};

	~Entity() { ; }

};