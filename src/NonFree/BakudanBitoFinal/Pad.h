#ifndef INCLUDED_PAD_H
#define INCLUDED_PAD_H

class Pad{
public:
	enum Button{
		A, //0�{�^����'d'
		B, //1�{�^����'x'
		U, //up
		D, //down
		L, //left
		R, //right
		Q, //quit�B�I���{�^��
	};
	//�Ȃ�ł���������{�^��
	static bool isOn( Button, int playerID = 0 );
	static bool isTriggered( Button, int playerID = 0 );
};

#endif