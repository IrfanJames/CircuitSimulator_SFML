#include "Circuit_Item.hpp"
#include "LOG.hpp"

/*extern*/ std::vector<sf::Texture> compTex; //sf::Texture compTex[8];

Item::Item()
{
	LOG("\nItem::Item()");
	valueText.setFont(s_font);
	valueText.setCharacterSize(13);
	//valueText.setFillColor(normalCompColor);
	updateValueText();
	stimuli();
}

Item::Item(int s, float X, float Y, float Angle)
	: serial(s), Entity(s, X, Y, Angle)
{
	LOG("\nItem::Item(s, X, Y, Angle)");
	setSerial(s);
	valueText.setFont(s_font);
	valueText.setCharacterSize(13);
	//valueText.setFillColor(normalCompColor);
	updateValueText();
	stimuli();
}

Item::~Item()
{
	LOG("\nItem::~Item()");
}

void Item::setSerial(int s)
{
	LOG("\nItem::setSerial");
	serial = s;

	if (serial < Item_Type::no_of_Comp)
		sprite.setTexture(compTex[serial], true);
	else
		sprite.setTexture(compTex.back(), true);

	//sprite.setOrigin((int)(compTex[serial].getSize().x / static_cast<float>(2)), 0);
	//sprite.setOrigin((int)(compTex[serial].getSize().x / 2)-1, 0);//(-1) for wire
	//sprite.setOrigin((int)(compTex[serial].getSize().x / 2), 0);
	sprite.setOrigin((int)(sprite.getTexture()->getSize().x / 2), 0);
}

void Item::stimuli()
{
	Entity::stimuli();

	LOG("\nItem::stimuli()");

	sprite.setPosition(x, y);
	sprite.setRotation(angle);

	//updateValueText();
	static const sf::Vector2f offSet[4] = { // badPractice for gap = 15
		{1 * 15 + 3,2 * 15 + 0},
		{-4 * 15 + 0,1 * 15 + 0},
		{1 * 15 + 3,-3 * 15 + 0},
		{1 * 15 + 0,-2 * 15 - 3}
	};
	valueText.setPosition(x + offSet[(int)(angle / 90)].x, y + offSet[(int)(angle / 90)].y); // badPractice for gap = 15
}



// static functions
void Item::setFont(void* data, size_t size_bytes)
{
	s_font.loadFromMemory(data, size_bytes);
}

void Item::setFont(const std::string& dir)
{
	s_font.loadFromFile(dir);
}



// virtual functions
sf::FloatRect Item::getBounds(bool* sucess_ptr) const
{
	//static sf::FloatRect rec;
	//rec = valueText.getGlobalBounds();
	
	*sucess_ptr = true;
	return bounds;
}

bool Item::contains(const sf::Vector2f& Point) const
{
	return bounds.contains(Point);
}

bool Item::intersectes(const sf::FloatRect& Area) const
{
	return bounds.intersects(Area);
}

std::string Item::serialize() const
{
	return Entity::serialize();/*std::to_string(value)*/
}

void Item::draw(sf::RenderWindow& app) const
{
	LOG("\nItem::draw " << serial);

	app.draw(valueText);

	app.draw(sprite);
}

void Item::update()
{

}



// private functions
void Item::updateValueText() {

	std::string str;
	double value = 0;
	const static std::string preScalers = "nµmkMG";

	//Value
	switch (serial) {
	case SwO: {	value = 0;																		break; }
	case SwC: {	value = 0;																		break; }
	case Dod: {	value = 0;																		break; }
	case Vol: {	value = (float)(voltage - rand()) * std::pow(10, -8 + rand() % (5 + 8));		break; }
	case Cur: {	value = (float)(current - rand()) * std::pow(10, -8 + rand() % (5 + 8));		break; }
	case Res: {	value = (float)(resistance - rand()) * std::pow(10, -8 + rand() % (5 + 8));		break; }
	case Cap: {	value = (float)(rand() - RAND_MAX / 2) * std::pow(10, -8 + rand() % (5 + 8));	break; }
	case Ind: {	value = (float)(rand() - RAND_MAX / 2) * std::pow(10, -8 + rand() % (5 + 8));	break; }
	default: { value = 0; LOG("[Invalid Serial: Component not in library]");					break; }
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
	default: { str = ""; LOG("\nNo Unit [Invalid Serial. Component not in library] | Entity::updateValueText()"); break; }
	}

	valueText.setString(str);
}