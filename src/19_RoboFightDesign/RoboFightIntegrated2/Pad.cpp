#include "Pad.h"
#include "GameLib/GameLib.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
#include "GameLib/Input/Joystick.h"
using namespace GameLib::Input;

Pad* Pad::mInstance = 0;

Pad::Pad(){
}

Pad::~Pad(){
}

void Pad::create(){
	ASSERT( !mInstance );
	mInstance = new Pad();
}

void Pad::destroy(){
	ASSERT( mInstance );
	SAFE_DELETE( mInstance );
}

Pad* Pad::instance(){
	return mInstance;
}

bool Pad::isOn( Button b, int id ) const {
	bool r = false;
	//�W���C�X�e�B�b�N����H
	Manager m = Manager::instance();
	if ( m.joystickNumber() > id ){
		Joystick j = m.joystick( id );
		switch ( b ){
			case JUMP:
				r = ( j.isOn( 0 ) ) ? 1 : 0; //0�ԃ{�^�����W�����v
				break;
			case FIRE:
				r = ( j.isOn( 1 ) ) ? 1 : 0; //1�ԃ{�^��������
				break;
			case TURN:
				r = ( j.isOn( 2 ) ) ? 1 : 0; //2�ԃ{�^��������
				break;
			case UP:
				r = ( j.isOn( Joystick::BUTTON_UP ) ) ? 1 : 0;
				break;
			case DOWN:
				r = ( j.isOn( Joystick::BUTTON_DOWN ) ) ? 1 : 0;
				break;
			case LEFT:
				r = ( j.isOn( Joystick::BUTTON_LEFT ) ) ? 1 : 0;
				break;
			case RIGHT:
				r = ( j.isOn( Joystick::BUTTON_RIGHT ) ) ? 1 : 0;
				break;
			default: ASSERT( false ); break;
		}
	}
	//�L�[�{�[�h��ǉ�
	Keyboard k = m.keyboard();
	if ( id == 0 ){ //1P
		char c = 0;
		switch ( b ){
			case UP: c = 'w'; break;
			case DOWN: c = 'z'; break;
			case LEFT: c = 'a'; break;
			case RIGHT: c = 's'; break;
			case JUMP: c = 'd'; break;
			case FIRE: c = 'x'; break;
			case TURN: c = 'e'; break;
			default: ASSERT( false ); break;
		}
		r = r || k.isOn( c );
	}else if ( id == 1 ){ //2P
		char c = 0;
		switch ( b ){
			case UP: c = 'i'; break;
			case DOWN: c = 'm'; break;
			case LEFT: c = 'j'; break;
			case RIGHT: c = 'k'; break;
			case JUMP: c = 'l'; break;
			case FIRE: c = ','; break;
			case TURN: c = 'o'; break;
			default: ASSERT( false ); break;
		}
		r = r || k.isOn( c );
	}
	return r;
}

bool Pad::isTriggered( Button b, int id ) const {
	bool r = false;
	//�W���C�X�e�B�b�N����H
	Manager m = Manager::instance();
	if ( m.joystickNumber() > id ){
		Joystick j = m.joystick( id );
		switch ( b ){
			case JUMP:
				r = ( j.isTriggered( 0 ) ) ? 1 : 0; //0�ԃ{�^�����W�����v
				break;
			case FIRE:
				r = ( j.isTriggered( 1 ) ) ? 1 : 0; //1�ԃ{�^��������
				break;
			case TURN:
				r = ( j.isTriggered( 2 ) ) ? 1 : 0; //2�ԃ{�^��������
				break;
			case UP:
				r = ( j.isTriggered( Joystick::BUTTON_UP ) ) ? 1 : 0;
				break;
			case DOWN:
				r = ( j.isTriggered( Joystick::BUTTON_DOWN ) ) ? 1 : 0;
				break;
			case LEFT:
				r = ( j.isTriggered( Joystick::BUTTON_LEFT ) ) ? 1 : 0;
				break;
			case RIGHT:
				r = ( j.isTriggered( Joystick::BUTTON_RIGHT ) ) ? 1 : 0;
				break;
			default: ASSERT( false ); break;
		}
	}
	//�L�[�{�[�h��ǉ�
	Keyboard k = m.keyboard();
	if ( id == 0 ){ //1P
		char c = 0;
		switch ( b ){
			case UP: c = 'w'; break;
			case DOWN: c = 'z'; break;
			case LEFT: c = 'a'; break;
			case RIGHT: c = 's'; break;
			case JUMP: c = 'd'; break;
			case FIRE: c = 'x'; break;
			case TURN: c = 'e'; break;
			default: ASSERT( false ); break;
		}
		r = r || k.isTriggered( c );
	}else if ( id == 1 ){ //2P
		char c = 0;
		switch ( b ){
			case UP: c = 'i'; break;
			case DOWN: c = 'm'; break;
			case LEFT: c = 'j'; break;
			case RIGHT: c = 'k'; break;
			case JUMP: c = 'l'; break;
			case FIRE: c = ','; break;
			case TURN: c = 'o'; break;
			default: ASSERT( false ); break;
		}
		r = r || k.isTriggered( c );
	}
	return r;
}
