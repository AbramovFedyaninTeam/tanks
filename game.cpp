#include <iostream>
#include <time.h> 
#include <math.h> 
#include <SFML/Graphics.hpp>

using namespace sf;

float offsetX=0, offsetY=0;

const int MAX_Y = 22;
const int MAX_X = 40;
const int COUNT_OF_VRAG_WEAPONS = 2;
const int COUNT_OF_PLAYER_WEAPONS = 2;
const int WEAPON_SIZE = 20;


/*
L 	Light Robot 	(50)
M 	Medium Robot 	(100)
H 	Heavy Robot 	(350)
P 	Anti-RobotPac 	(300)
A 	Artillery 		(150)
D 	Landmine 		(40)
B 	Border
*/

String TileMap[MAX_Y] =
    {
    					 //Border
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
    "B                                      B",
    "B                                      B",
    "B                   D                  B",
    "B                         H            B",
    "B                                      B",
    "B                   D                  B",
    "B                                      B",
    "B                                      B",
    "B                   D                  B",
    "B                                      B",
    "B                          L           B",
    "B                   D                  B",
    "B                                      B",
    "B                                      B",
    "B                   D                  B",
    "B                          M           B",
    "B                                      B",
    "B                   D                  B",
    "B                                      B",
    "B                                      B",
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",

    }; 

class Weapon {

public:
	float x;
	float y;
	float dx;
	float dy;
	float max_speed;
	char type;
	int price;
	int health;
	int damage;
	int count_of_bullets;
	bool isVrag;

	//Мины не движутся
	//пушки тоже
	//роботы:
	//Движется по прямой. Если кого-то встретил, стреляет в него. Если столкнулся со стеной, меняет направление случайным образом

    Weapon() {}

    void update()
    {    
        x += dx;
        y += dy;

        if (x < 2)         dx =  0.001 * max_speed * (25 + rand() % 76);
        if (x > MAX_X - 1) dx = -0.001 * max_speed * (25 + rand() % 76);
        if (y < 2)         dy =  0.001 * max_speed * (25 + rand() % 76);
        if (y > MAX_Y - 1) dy = -0.001 * max_speed * (25 + rand() % 76);
    }

};
/*Структура "оружие"
Игрок
Тип
Стоимость
Координаты
Количество жизней
Урон при выстреле
Количество выстрелов (0 - пока не надоест, 1 - мина, артиллерия - 50 патронов)*/

Weapon set_weapon_data(float x, float y, float dx, float dy, float max_speed, char type) {
	Weapon weapon;
	weapon.x = x;
	weapon.y = y;
	weapon.dx = dx;
	weapon.dy = dy;
	weapon.max_speed = max_speed;
	weapon.type = type;

	return weapon;
}

char get_point_state(int x, int y, String* tilemap, Weapon* vrag_weapon, Weapon* player_weapon) {

	char state = tilemap[y][x];

	for (int vrag_weapon_id = 0; vrag_weapon_id < COUNT_OF_VRAG_WEAPONS; vrag_weapon_id++) {
		if (round(vrag_weapon[vrag_weapon_id].x) == x + 1 && round(vrag_weapon[vrag_weapon_id].y) == y + 1) {
			state = vrag_weapon[vrag_weapon_id].type;
		}
	}

	for (int player_weapon_id = 0; player_weapon_id < COUNT_OF_PLAYER_WEAPONS; player_weapon_id++) {
		if (round(player_weapon[player_weapon_id].x) == x + 1 && round(player_weapon[player_weapon_id].y) == y + 1) {
			state = player_weapon[player_weapon_id].type;
		}
	}

	return state;
}

int main()
{
    RenderWindow window( VideoMode(MAX_X * WEAPON_SIZE, MAX_Y * WEAPON_SIZE), "Test!");
    srand (time(NULL));
	char state;   

    Weapon vrag_weapon[COUNT_OF_VRAG_WEAPONS];
    								//x   y   dx   dy  speed type	
    vrag_weapon[0] = set_weapon_data(21, 15, 0.5,  0.1, 0.5, 'H'); //= {22, 3, 0.5,  0.1, 'H', 350, 250, 75, 0, true};
    vrag_weapon[1] = set_weapon_data(21, 18, 0.3, -0.1, 0.8, 'M'); //= {22, 8, 0.3, -0.1, 'M', 100, 175, 45, 0, true};
    
    Weapon player_weapon[COUNT_OF_PLAYER_WEAPONS];
    							      //x   y   dx   dy  speed type	
    player_weapon[0] = set_weapon_data(15, 15, 0.5,  0.1, 0.5, 'H'); //= {22, 3, 0.5,  0.1, 'H', 350, 250, 75, 0, true};
    player_weapon[1] = set_weapon_data(13, 18, 0.3, -0.1, 0.8, 'M'); //= {22, 8, 0.3, -0.1, 'M', 100, 175, 45, 0, true};
    



    RectangleShape rectangle( Vector2f(WEAPON_SIZE, WEAPON_SIZE));

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

		for (int vrag_weapon_id = 0; vrag_weapon_id < COUNT_OF_VRAG_WEAPONS; vrag_weapon_id++) {
			vrag_weapon[vrag_weapon_id].update();
		}
		for (int player_weapon_id = 0; player_weapon_id < COUNT_OF_PLAYER_WEAPONS; player_weapon_id++) {
			player_weapon[player_weapon_id].update();
		}

        offsetX = 0;
        offsetY = 0;

        window.clear(Color::White);

		/*		L 	Light Robot 	(50)		M 	Medium Robot 	(100		H 	Heavy Robot 	(350)
		P	Anti-RobotPac 	(300)		A 	Artillery 		(150)		D 	Landmine 		(40)		B 	Border		*/
        for (int coord_y = 0; coord_y < MAX_Y; coord_y++)
            for (int coord_x = 0; coord_x < MAX_X ; coord_x++)
            { 
            	state = get_point_state (coord_x, coord_y, TileMap, vrag_weapon, player_weapon);

                if (state == 'B') rectangle.setFillColor(Color::Black);

                if (state == 'L') rectangle.setFillColor(Color::Red);
                if (state == 'M') rectangle.setFillColor(Color::Yellow);
                if (state == 'H') rectangle.setFillColor(Color::Blue);
                if (state == 'P') rectangle.setFillColor(Color::Magenta);
                if (state == 'A') rectangle.setFillColor(Color::Green);
                if (state == 'D') rectangle.setFillColor(Color::Green);

                if (state == ' ') continue;

                rectangle.setPosition(coord_x * WEAPON_SIZE - offsetX, coord_y * WEAPON_SIZE - offsetY) ; 
                window.draw(rectangle);
            }

        window.display();
    }

    return 0;
}