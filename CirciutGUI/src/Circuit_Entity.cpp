
#include "Circuit_Entity.hpp"
#include "Circuit_Global.hpp"
#include "LOG.hpp"


extern std::vector<sf::Texture> compTex; //sf::Texture compTex[8];

// public static variables
//sf::RectangleShape boarderDesign;

// private static variables
//sf::Font s_font;



Entity::Entity() {
	//LOG("\nEntity::Entity()");
	serial = 0; x = 0.0f; y = 0.0f; angle = 0.0f;
	value = (float)(rand() - RAND_MAX / 2) * std::pow(10, -8 + rand() % (5 + 8));

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

Entity::Entity(int s, float X, float Y, float Angle) {
	value = (float)(rand() - RAND_MAX / 2) * std::pow(10, -8 + rand() % (5 + 8));
	x = X; y = Y; angle = Angle;

	setSerial(s);

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

Entity::~Entity()
{
	//LOG("\nEntity::~Entity()");
}



void Entity::setSerial(int s) {
	serial = s;

	if (serial < Entity::no_of_Comp)
		sprite.setTexture(compTex[serial], true);
	else
		sprite.setTexture(compTex.back(), true);

	//sprite.setOrigin((int)(compTex[serial].getSize().x / static_cast<float>(2)), 0);
	//sprite.setOrigin((int)(compTex[serial].getSize().x / 2)-1, 0);//(-1) for wire
	//sprite.setOrigin((int)(compTex[serial].getSize().x / 2), 0);
	sprite.setOrigin((int)(sprite.getTexture()->getSize().x / 2), 0);
}

void Entity::stimuli() {
	//LOG("\nEntity::stimuli()");
	//x = trim(x, gap);
	//y = trim(y, gap);
	//angle = 90 * (int)((int)angle % 360) / 90;

	sprite.setPosition(x, y);
	sprite.setRotation(angle);

	boarder.setPosition(x, y);
	boarder.setRotation(angle);

	//if (0 <= angle && angle <= 90)
	//	sprite.setOrigin((int)(compTex[serial].getSize().x / 2) - 1, 0);
	//else /*if (180 <= angle && angle <= 270)*/
	//	sprite.setOrigin((int)(compTex[serial].getSize().x / 2) + 1, 0);

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

void Entity::updateValueText() {

	std::string str;
	double val = value;
	const static std::string preScalers = "nµmkMG";


	//Value -> "srt(v)" + "prescaler"
	{
		if (val < 0) { str = "-"; val = std::abs(val); }

		char prescaler = '#';
		if (val == 0) { str = "0 "; }
		else if (1 <= val) {
			double temp = val;
			int c = 0;
			for (; c < 3; c++) {
				temp /= 1000;
				if (temp < 1) break;
				val = temp;
			}
			if (c != 0)
			{
				if (5 < 2 + c) c = 3;
				prescaler = preScalers[2 + c];
			}
		}
		else if (val < 1) {
			int c = 0;
			for (; c < 3; c++) {
				val *= 1000;
				if (val >= 1) break;
			}

			if (0 > 2 - c) c = 2;
			prescaler = preScalers[2 - c];
		}

		str += std::to_string(val);
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



// const functions

int Entity::getSerial() const {
	return serial;
}

sf::Vector2f Entity::getEndPos() const {
	return sf::Vector2f(x - 75 * (int)sin(angle * DegToRad), y + 75 * (int)cos(angle * DegToRad));
}



// static functions
void Entity::setFont(void* data, size_t size_bytes) {
	s_font.loadFromMemory(data, size_bytes);
}

void Entity::setFont(const std::string& dir) {
	s_font.loadFromFile(dir);
}

void Entity::setboarderDesgin(const sf::Color& color/* = sf::Color(0, 255, 85 Or 0, 204, 102)*/)
{
	boarderDesign.setFillColor(sf::Color::Transparent);
	boarderDesign.setOutlineThickness(1.0f);
	boarderDesign.setOutlineColor(color);
}



// virtual functions
sf::FloatRect Entity::getBounds(bool* sucess_ptr) const
{
	*sucess_ptr = true;

	return bounds;
}

bool Entity::contains(const sf::Vector2f& Point) const
{
	return bounds.contains(Point);
}

bool Entity::intersectes(const sf::FloatRect& Area) const
{
	return bounds.intersects(Area);
}

std::string Entity::serialize() const
{
	return std::to_string(serial) + "\t" + std::to_string((int)x) + "\t" + std::to_string(y) + "\t" + std::to_string((int)angle) + "\n";
}

void Entity::draw(sf::RenderWindow& app) const 
{
	//LOG("\nEntity::draw " << serial);

	app.draw(valueText);

	app.draw(sprite);
}

void Entity::update()
{

}
