#include <cmath>

class waterObj {
public :
	waterObj(float x, float y) :
		moveX(0),
		moveY(0),
		popNum(0)
	{
		this->X = x;
		this->Y = y;
	};


	//void setMovement(float xl, float yl, float xr, float yr);
	void updateWater();
	void setMoveX(float moveX);
	void setMoveY(float moveY);
	float getX();
	float getY();
	void setPopNum();
	int getPopNum();
	float getMoveX();
	float getMoveY();

	~waterObj() {}
	

private:
	float X;
	float Y;
	float moveX;
	float moveY;
	int popNum;

	void setX(float X);

	void setY(float Y);

};