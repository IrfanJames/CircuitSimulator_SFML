#pragma once

#include "SFML/Graphics/Font.hpp"
#include "Circuit_Entity.hpp"

class Item : public Entity
{
public:

	enum Item_Type {
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

public:

	double value = 43531;
	double resistance = 1000;
	double voltage = -5.032;
	double current = 0.030405;

public:

	sf::Sprite sprite;
	sf::Text valueText;

private:

	inline static sf::Font s_font;

private:

	int serial = 0;
	
	void updateValueText();

public:

	Item();

	Item(int s, float X, float Y, float Angle = 0.0f);

	~Item();

	void setSerial(int s);

	void stimuli();


public:

	static void setFont(void* data, size_t size_bytes);

	static void setFont(const std::string& dir);


public:

	virtual sf::FloatRect getBounds(bool* sucess_ptr = nullptr) const override;

	virtual bool contains(const sf::Vector2f& Point) const override;

	virtual bool intersectes(const sf::FloatRect& Area) const override;

	virtual std::string serialize() const override;

	virtual void draw(sf::RenderWindow& app) const override;

	virtual void update() override;
};

