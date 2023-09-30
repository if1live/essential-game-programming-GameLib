#include "GameLib/GameLib.h"
#include <windows.h>
#include <cstdio>

bool gThrowWhenHalt = false;

//���̃t�@�C���ł�new���Ȃ��悤�z������B
//new�̒�������Ă΂ꂤ�邩�炾�B
namespace GameLib{

void halt( const char* filename, int line, const char* message ){
	char buf[ 8192 ];
	sprintf_s( buf, 8192, "%s:%d : %s", filename, line, message );
	if ( gThrowWhenHalt ){
		::OutputDebugStringA( buf );
		::OutputDebugStringA( "\n" );
		throw EXCEPTION_IGNORE; //��{�͖���
	}else{
		//���b�Z�[�W�{�b�N�X���o����
		MessageBoxA( 0, buf, "Critical Error!", MB_OK | MB_ICONERROR );
		//����
		*reinterpret_cast< int* >( 0 ) = 0;
	}
}

} //namespace GameLib

