#pragma once

#include "Circuit_Global.h"
#include <iostream>
#include "SFML/Graphics.hpp";

class Entity {
public:
	static const int noOfComps = 8;//when 10 toolColBox start with a y-offset
	static sf::Font s_font;

public:
	double resistance = 1000;
	double voltage = -5;
	double current = -0.002;

	enum {
		Cap,
		Cur,
		Dod,
		Ind,
		Res,
		SwO,
		Vol,
		SwC
	};

public:
	int serial = 0;
	float x, y, angle;
	sf::Sprite sprite;
	sf::Text valueText;
	sf::RectangleShape boarder;
	sf::FloatRect bounds;

private:
	void updateValueText() {

		std::string str;
		double value = 0;
		const static std::string preScalers = "nµmkMG";

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
		default: { value = 0; break; }
		}

		/*Value -> prescaler */ {

			if (value < 0) { str = "-"; value = std::abs(value); }

			if (value == 0) { str = "0 "; }
			else if (1 <= value) {
				double temp = value;
				int c = 0;
				for (; c < 3; c++) {
					temp /= 1000;
					if (temp < 1) break;
					value = temp;
				}

				str += std::to_string(value);

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

				str += std::to_string(value);

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
		case Dod: { str = "";		break; }
		case Ind: { str += "H";		break; }
		case Res: { str += "Ohm";	break; }
		case SwO: { str = "";		break; }
		case Vol: { str += "V";		break; }
		case SwC: { str = "";		break; }
		default: { str += "[No Unit: Component not in library]"; break; }
		}


		valueText.setString(str);
	}

public:
	Entity() {
		serial = 0; x = W / 2; y = H / 2; angle = 0.0f;

		static const int A = 0, B = 15, C = 75; //Hard Code
		boarder.setSize(sf::Vector2f(2 * B, C));
		boarder.setOrigin(B, A);
		boarder.setFillColor(sf::Color(0, 0, 100, 0));
		boarder.setOutlineThickness(1.0f);
		boarder.setOutlineColor(sf::Color(0, 204, 102));

		bounds.left = x - B;
		bounds.top = y - A;
		bounds.width = B + B;
		bounds.height = C + A;

		valueText.setFont(s_font);
		valueText.setCharacterSize(13);
		//valueText.setFillColor(normalCompColor);
		updateValueText();
		stimuli();
	}
	Entity(int s, float _x, float _y, float Angle = 0.0f) {
		x = _x; y = _y; angle = Angle;
		serial = s;
		sprite.setTexture(compTex[s]);
		sprite.setOrigin(compTex[s].getSize().x / 2, 0);

		static const int A = 0, B = 15, C = 75; //Hard Code
		boarder.setSize(sf::Vector2f(2 * B, C));
		boarder.setOrigin(B, A);
		boarder.setFillColor(sf::Color(0, 0, 100, 0));
		boarder.setOutlineThickness(1.0f);
		boarder.setOutlineColor(sf::Color(0, 204, 102));

		bounds.left = x - B;
		bounds.top = y - A;
		bounds.width = B + B;
		bounds.height = C + A;

		valueText.setFont(s_font);
		valueText.setCharacterSize(13);
		//valueText.setFillColor(normalCompColor);
		updateValueText();
		stimuli();
	}

	sf::Vector2f getEndPos() const {
		return sf::Vector2f(x - 75 * (int)sin(angle * DegToRad), y + 75 * (int)cos(angle * DegToRad));
	}

	void stimuli() {

		//x = trim(x, gap);
		//y = trim(y, gap);
		//angle = 90 * (int)((int)angle % 360) / 90;

		sprite.setPosition(x, y);
		sprite.setRotation(angle);


		boarder.setPosition(x, y);
		boarder.setRotation(angle);


		int A = 0, B = 15, C = 75, i = (int)angle % 360;
		if (i == 0) { bounds.left = x - B; bounds.top = y - A; bounds.width = B + B; bounds.height = C + A; }
		else if (i == 90) { bounds.left = x - C; bounds.top = y - B; bounds.width = A + C; bounds.height = B + B; }
		else if (i == 180) { bounds.left = x - B; bounds.top = y - C; bounds.width = B + B; bounds.height = A + C; }
		else if (i == 270) { bounds.left = x - A; bounds.top = y - B; bounds.width = C + A; bounds.height = B + B; }


		//updateValueText();
		static const int offSet[4][2] = { // badPractice for gap = 15
			{1 * 15 + 3,2 * 15 + 0},
			{-4 * 15 + 0,1 * 15 + 0},
			{1 * 15 + 3,-3 * 15 + 0},
			{1 * 15 + 0,-2 * 15 - 3}
		};
		valueText.setPosition(x + offSet[(int)(angle / 90)][0], y + offSet[(int)(angle / 90)][1]); // badPractice for gap = 15
	}
	void draw(sf::RenderWindow& app) {

		app.draw(valueText);

		app.draw(sprite);
	}


	static void setFont(const std::string& dir) {
		s_font.loadFromFile(dir); /*CALIBRI_1*/
	}
	~Entity() { ; }


	virtual void update() {};
};

sf::Font Entity::s_font;