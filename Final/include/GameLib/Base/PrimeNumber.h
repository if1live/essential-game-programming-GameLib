#ifndef INCLUDED_GAMELIB_PRIMENUMBER_H
#define INCLUDED_GAMELIB_PRIMENUMBER_H

namespace GameLib{
	namespace PrimeNumber{
		bool isPrimeNumber( int );
		//�������傫�ȍŏ��̑f����Ԃ�
		int next( int );
		//������菬���ȍő�̑f����Ԃ�
		int previous( int );
		//�����̕������B�����_�ȉ��͐؂�グ
		int sqrt( int );
	}
}

#endif
