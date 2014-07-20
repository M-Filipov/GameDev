#pragma once

class Game
{
public:
	Game(void);

	~Game(void);

	void run();

	void loadInvaders();

	void update();

	void render();

	bool checkCollision(int col, int row, int p);

	bool isProjInNpcX (int col, int row, int p);

	bool isProjInNpcY (int col, int row, int p);

	bool checkProjPlayerCollis(int p);

	void startSpecialScreen(int end);

	void close();

	void updateInvaders ();

	static const int ROWS = 5;

	static const int COLS = 11;



};

