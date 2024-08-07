#pragma once

#include "SFML/Graphics.hpp";

class Entity {

public:

	enum {
		Cap,
		Cur,
		Dod,
		Ind,
		Res,
		SwO,
		Vol,
		SwC,
		no_of_Comp
	};

	inline static sf::RectangleShape boarderDesign;

public:

	double resistance = 1000;
	double voltage = -5.032;
	double current = 0.030405;

public:

	float x, y, angle;
	//sf::Sprite::Drawable;
	sf::Sprite sprite;
	sf::Text valueText;
	sf::RectangleShape boarder;
	sf::FloatRect bounds;

private:

	inline static sf::Font s_font;

private:

	int serial = 0;

	void updateValueText();

public:

	Entity();

	Entity(int s, float X, float Y, float Angle = 0.0f);

	~Entity();

	void setSerial(int s);

	void stimuli();

public:

	int getSerial() const;

	sf::Vector2f getEndPos() const;

public:

	static void setFont(void* data, size_t size_bytes);

	static void setFont(const std::string& dir);

	static void setboarderDesgin(const sf::Color& color/* = sf::Color(0, 255, 85 Or 0, 204, 102)*/);

public:

	virtual sf::FloatRect getBounds(bool* sucess_ptr = nullptr) const;

	virtual bool contains(const sf::Vector2f& Point) const;

	virtual bool intersectes(const sf::FloatRect& Area) const;

	virtual std::string serialize() const;
	
	virtual void draw(sf::RenderWindow& app) const;

	virtual void update();
};