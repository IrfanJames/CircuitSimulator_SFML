#pragma once

#include <iostream>
#include "Circuit_Global.hpp"
#include "SFML/Graphics.hpp";

//sf::Texture compTex[8];
std::vector<sf::Texture> compTex;

class Entity {
public:
	static const int noOfComps = 8;//when 10 toolColBox start with a y-offset
	static sf::Font s_font;
	static sf::RectangleShape boarderDesign;

public:
	double resistance = 1000;
	double voltage = -5.032;
	double current = 0.030405;

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

		//Value
		switch (serial) {
		case SwO: {	value = 0;																		break; }
		case SwC: {	value = 0;																		break; }
		case Dod: {	value = 0;																		break; }
		case Vol: {	value = (float)(voltage - rand())      * std::pow(10, -8 + rand() % (5 + 8));	break; }
		case Cur: {	value = (float)(current - rand())      * std::pow(10, -8 + rand() % (5 + 8));	break; }
		case Res: {	value = (float)(resistance - rand())   * std::pow(10, -8 + rand() % (5 + 8));	break; }
		case Cap: {	value = (float)(rand() - RAND_MAX / 2) * std::pow(10, -8 + rand() % (5 + 8));	break; }
		case Ind: {	value = (float)(rand() - RAND_MAX / 2) * std::pow(10, -8 + rand() % (5 + 8));	break; }
		default: { value = 0; std::cout << "[Invalid Serial: Component not in library]";			break; }
		}

		//Value -> v*prescaler
		{
			if (value < 0) { str = "-"; value = std::abs(value); }

			char prescaler = '#';
			if (value == 0) { str = "0 "; }
			else if (1 <= value) {
				double temp = value;
				int c = 0;
				for (; c < 3; c++) {
					temp /= 1000;
					if (temp < 1) break;
					value = temp;
				}
				if (c != 0) prescaler = preScalers[2 + c];
			}
			else if (value < 1) {
				int c = 0;
				for (; c < 3; c++) {
					value *= 1000;
					if (value >= 1) break;
				}
				prescaler = preScalers[2 - c];
			}

			str += std::to_string(value);
			while (str.back() == '0') str.pop_back();
			if (str.back() == '.') str.pop_back();
			str += " ";

			int count = 0;
			bool start_counting = false;
			for (int i = 0; i < str.size(); i++)
			{
				if (str[i] == '.')
					start_counting = true;

				if (start_counting && ('0' <= str[i] && str[i] <= '9'))
					count++;

				if (start_counting && count > 3) {
					str.erase(str.begin() + i);
					count--;
					i--;
				}
			}

			if (prescaler != '#')
				str += prescaler;
		}

		//Units
		switch (serial) {
		case Dod: { str = "";		break; }
		case SwO: { str = "";		break; }
		case SwC: { str = "";		break; }
		case Vol: { str += "V";		break; }
		case Cur: { str += "A";		break; }
		case Res: { str += "Ohm";	break; }
		case Cap: { str += "F";		break; }
		case Ind: { str += "H";		break; }
		default:  { str += "No Unit [Invalid Serial. Component not in library]"; break; }
		}

		valueText.setString(str);
	}

public:
	Entity() {
		serial = 0; x = 0.0f; y = 0.0f; angle = 0.0f;

		static const int A = 0, B = 15, C = 75; //Hard Code
		boarder = boarderDesign;
		boarder.setSize(sf::Vector2f(2 * B, C));
		boarder.setOrigin(B, A);

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
	Entity(int s, float X, float Y, float Angle = 0.0f) {
		serial = s; x = X; y = Y; angle = Angle;
		
		sprite.setTexture(compTex[serial]);
		sprite.setOrigin((int)(compTex[serial].getSize().x / 2), 0);

		boarder = boarderDesign;
		static const int A = 0, B = 15, C = 75; //Hard Code
		boarder.setSize(sf::Vector2f(2 * B, C));
		boarder.setOrigin(B, A);

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


		int A = 0, B = 15, C = 75, i = ((int)angle % 360 / 90) * 90;
		if (i == 0) { bounds.left = x - B; bounds.top = y - A; bounds.width = B + B; bounds.height = C + A; }
		else if (i == 90) { bounds.left = x - C; bounds.top = y - B; bounds.width = A + C; bounds.height = B + B; }
		else if (i == 180) { bounds.left = x - B; bounds.top = y - C; bounds.width = B + B; bounds.height = A + C; }
		else if (i == 270) { bounds.left = x - A; bounds.top = y - B; bounds.width = C + A; bounds.height = B + B; }


		//updateValueText();
		static const sf::Vector2f offSet[4] = { // badPractice for gap = 15
			{1 * 15 + 3,2 * 15 + 0},
			{-4 * 15 + 0,1 * 15 + 0},
			{1 * 15 + 3,-3 * 15 + 0},
			{1 * 15 + 0,-2 * 15 - 3}
		};
		valueText.setPosition(x + offSet[(int)(angle / 90)].x, y + offSet[(int)(angle / 90)].y); // badPractice for gap = 15
	}
	void draw(sf::RenderWindow& app)
	{
		app.draw(valueText);

		app.draw(sprite);
	}


	static void setFont(void* data, size_t size_bytes) {
		s_font.loadFromMemory(data, size_bytes);
	}

	static void setFont(const std::string &dir) {
		s_font.loadFromFile(dir);
	}

	static void setboarderDesgin(const sf::Color& color/* = sf::Color(0, 255, 85 Or 0, 204, 102)*/)
	{
		boarderDesign.setFillColor(sf::Color::Transparent);
		boarderDesign.setOutlineThickness(1.0f);
		boarderDesign.setOutlineColor(color);
	}

	~Entity() { ; }


	virtual void update() {};
};

sf::Font Entity::s_font;
sf::RectangleShape Entity::boarderDesign;