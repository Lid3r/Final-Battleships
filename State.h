#pragma once

class State
{

public:
	State() {};
	
	virtual int getState() = 0;
	virtual void setState(int state) = 0;
};
