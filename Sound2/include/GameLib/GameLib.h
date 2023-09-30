#ifndef INCLUDED_GAMELIB_GAMELIB_H
#define INCLUDED_GAMELIB_GAMELIB_H

namespace GameLib{
	void halt( const char* filename, int line, const char* message );
}

//�}�N����
#define STRONG_ASSERT( exp ) ( ( !! ( exp ) ) || ( GameLib::halt( __FILE__, __LINE__, #exp ), 0 ) ) //���̍s�̈Ӗ��𗝉�����̂͑������
#define HALT( exp ) { GameLib::halt( __FILE__, __LINE__, #exp ); }
#define SAFE_DELETE( x ) { delete ( x ); ( x ) = 0; }
#define SAFE_DELETE_ARRAY( x ) { delete[] ( x ); ( x ) = 0; }

//�f�o�O�ƃ����[�X�ŕ��򂷂����
#ifndef NDEBUG
#define ASSERT( exp ) ( ( !! ( exp ) ) || ( GameLib::halt( __FILE__, __LINE__, #exp ), 0 ) )
#else //NDEBUG
#define ASSERT( exp )
#endif //NDEBUG

//�x�[�X�T�[�r�X�w�b�_
#include "Base/DebugStream.h"

#endif
