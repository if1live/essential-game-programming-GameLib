#ifndef INCLUDED_GAMELIB_INPUT_JOYSTICK_H
#define INCLUDED_GAMELIB_INPUT_JOYSTICK_H

namespace GameLib{
namespace Input{

class Joystick{
public:
	enum Button{
		BUTTON_UP = 128,
		BUTTON_DOWN,
		BUTTON_LEFT,
		BUTTON_RIGHT,
	};

	int analogNumber() const;
	int analog( int ) const; //�A�i���O�X�e�B�b�N�l�擾
	//�㉺���E�͏�̗񋓂Ŏ��܂��B�ق��̃{�^����0����̐����ŁB
	bool isOn( int ) const;
	bool isTriggered( int ) const;
	int buttonNumber() const;
	//�L�����H
	bool isEnabled() const;

	Joystick();
	~Joystick();
	operator void*() const;
	class Impl;
private:
	friend class Manager;
	Joystick( Impl* );
	Impl* mImpl;
};

} //namespace Input
} //namespace GameLib

#endif
