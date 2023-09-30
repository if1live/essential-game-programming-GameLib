#include "GameLib/Base/Conversion.h"

namespace GameLib{

namespace {

int toString16( char* out, unsigned a, int n ){ //n��16�i���������̍ő包��
	static const char table[ 16 ] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F', };
	for ( int i = 0; i < n; ++i ){ //��̌����珇�ɏ���
		int shift = ( n - 1 - i ) * 4;
		unsigned mask = 0xf << shift;
		unsigned t = a & mask;
		t >>= shift;
		out[ i ] = table[ t ];
	}
	return n;
}

//�擪��0�����̂܂܂ɂ��ĕԂ��B
//���̃}�C�i�X����𒆂ł��Ȃ����ƌ����΁Aunsigned�Ŏ󂯎���
//�}�C�i�X��int������Ȃ����Aint�Ŏ󂯎���unsigned������Ȃ�����ł���
void toString10Core( char* out,  unsigned a, int n ){ //n��10�i���������̍ő包���B�ő�10
	//10�i�萔
	static const int d[ 9 ] = { 
		10,
		100,
		1000,
		10000,
		100000,
		1000000,
		10000000,
		100000000,
		1000000000,
	};
	//10�Ŋ��������܂���i�[���āA10�Ŋ���B
	for ( int i = 0; i < n - 1; ++i ){
		int q = a / d[ n - 2 - i ];
		out[ i ] = static_cast< char >( q );
		a -= q * d[ n - 2 - i ];
	}
	out[ n - 1 ] = static_cast< char >( a );
}

//���̃}�C�i�X����𒆂ł��Ȃ����ƌ����΁Aunsigned�Ŏ󂯎���
//�}�C�i�X��int������Ȃ����Aint�Ŏ󂯎���unsigned������Ȃ�����ł���
int toString10( char* out,  unsigned a, int n, bool minus ){ //n��10�i���������̍ő包���B�ő�10
	char* p = out;
	//�}�C�i�X�Ȃ�}�C�i�X��f���o��
	if ( minus ){
		*p++ = '-';
	}
	char s[ 10 ]; //���ԃo�b�t�@
	toString10Core( s, a, n );
	//0���X�L�b�v
	int begin = n;
	for ( int i = 0; i < n; ++i ){
		if ( s[ i ] != 0 ){
			begin = i;
			break;
		}
	}
	if ( begin == n ){ //�ꌅ���Ȃ��B�܂�0
		*p++ = '0';
	}else{
		for ( int i = begin; i < n; ++i ){
			*p++ = '0' + s[ i ];
		}
	}
	return static_cast< int >( p - out );
}

} //namespace {}

int toString10( char* out, char a ){
	bool minus = false;
	if ( a < 0 ){
		minus = true;
		a = -a;
	}
	return toString10( out, a, 3, minus );
}

int toString10( char* out, unsigned char a ){
	return toString10( out, a, 3, false );
}

int toString10( char* out, short a ){
	bool minus = false;
	if ( a < 0 ){
		minus = true;
		a = -a;
	}
	return toString10( out, a, 5, minus );
}

int toString10( char* out, unsigned short a ){
	return toString10( out, a, 5, false );
}

int toString10( char* out, int a ){
	bool minus = false;
	if ( a < 0 ){
		minus = true;
		a = -a;
	}
	return toString10( out, a, 10, minus );
}

int toString10( char* out, unsigned a ){
	return toString10( out, a, 10, false );
}

int toString16( char* out, char a ){
	return toString16( out, a, 2 );
}

int toString16( char* out, unsigned char a ){
	return toString16( out, a, 2 );
}

int toString16( char* out, short a ){
	return toString16( out, a, 4 );
}

int toString16( char* out, unsigned short a ){
	return toString16( out, a, 4 );
}

int toString16( char* out, int a ){
	return toString16( out, a, 8 );
}

int toString16( char* out, unsigned a ){
	return toString16( out, a, 8 );
}

int toString( char* out, float a, int precision ){
	char* p = out; //�������݃|�C���^
	if ( a < 0.f ){ //��������
		*p++ = '-';
		a = -a;
	}
	unsigned o = *reinterpret_cast< unsigned* >( &a );
	int e = ( o & 0x7f800000 ) >> 23;
	unsigned m = o & 0x007fffff; //�B��r�b�g��1�𑫂�
	//���ꐔ������
	if ( e == 0 ){
		*p++ = '0';
		return static_cast< int >( p - out );
	}else if ( e == 255 ){
		if ( m == 0 ){
			*p++ = 'I';
			*p++ = 'n';
			*p++ = 'f';
		}else{
			*p++ = 'N';
			*p++ = 'a';
			*p++ = 'N';
		}
		return static_cast< int >( p - out );
	}
	e -= 127; //�w���o�C�A�X
	e -= 23; //��������2^23�P�^�����w���������
	m += 0x00800000; //�B��r�b�g��1�𑫂�
	m <<= 8; //8�r�b�g�V�t�g
	e -= 8; //���̕����_�E��

	int e10 = 0;
	if ( e > 0 ){ //2�p����Z����10�ŏ�����B
		for ( int i = 0; i < e; ++i ){
			if ( m > 0x80000000 ){ //���ӂ��I
				m /= 10;
				++e10;
				if ( i + 2 < e ){ //�܂�2��ȏ�悸��Ȃ炱���ł���Ă��܂��B
					m <<= 2;
					i += 2;
				}
			}
			m <<= 1;
		}
	}else{
		e = -e;
		for ( int i = 0; i < e; ++i ){
			if ( m < 0x19000000 ){ //�P�^���グ�܂�
				m *= 10;
				--e10;
				if ( i + 2 < e ){ //�܂�2��ȏ㏜����Ȃ炱���ł���Ă��܂��B
					m >>= 2;
					i += 2;
				}
			}
			m >>= 1;
		}
	}
	char s[ 10 ]; //���ԓf���o���o�b�t�@
	toString10Core( s, m, 10 );
	//�ȉ��t�H�[�}�b�e�B���O

	//�擪��0�łȂ��悤�ɂ��炷�B��̍H�����P��������
	int zeroEnd = 0;
	for ( int i = 0; i < 10; ++i ){
		if ( s[ i ] != 0 ){
			break;
		}else{
			++zeroEnd;
		}
	}
	if ( zeroEnd == 10 ){ //�S��0����ȏꍇ�Ȃ̂Ŕ�����
		*p++ = '0';
		return static_cast< int >( p - out );
	}
	//�ڂ�
	for ( int i = 0; i < 10 - zeroEnd; ++i ){
		s[ i ] = s[ i + zeroEnd ];
	}
	for ( int i = 10 - zeroEnd; i < 10; ++i ){
		s[ i ] = 0;
	}
	e10 -= zeroEnd; //��ɂ��炵����������
	e10 += 9; //10���̈ʂ�1�ɂ���̂�9�P�^e�Ƀv���X

	//�o�͌���
	int n = ( precision < 9 ) ? precision : 9;
	//�l�̌ܓ�
	bool carry = ( s[ n ] >= 5 );
	s[ n ] = 0;
	if ( carry ){
		for ( int i = n - 1; i >= 0; --i ){
			++s[ i ];
			if ( s[ i ] == 10 ){
				s[ i ] = 0;
			}else{
				carry = false;
				break;
			}
		}
		if ( carry ){ //�ő包�܂ŌJ��オ���Ă���B�ɂ߂ċH
			s[ 0 ] = 1;
			for ( int i = 1; i < 10; ++i ){
				s[ i ] = 0;
			}
			++e10;
		}
	}
	//���ɁA��납��0�𐔂��Ă��̐�����n�����炷
	for ( int i = n - 1; i > 0; --i ){
		if ( s[ i ] == 0 ){
			--n;
		}else{
			break;
		}
	}
	//e10��[-2,precision)�ɂ����e�L�@�͎g��Ȃ��B���̕����Z���Ȃ邩�炾�B
	if ( ( e10 >= -2 ) && ( e10 < precision ) ){ 
		if ( e10 >= 0 ){ //���ʂ������̏ꍇ
			for ( int i = 0; i < n; ++i ){
				*p++ = '0' + s[ i ];
				if ( ( i == e10 ) && ( i != n - 1 ) ){ //�Ō�̐��Ȃ珬���_���p
					*p++ = '.'; //�����_�o��
				}
			}
			//e���ł����ꍇ�͗]����0���K�v
			for ( int i = n; i <= e10; ++i ){
				*p++ = '0';
			}
		}else{ //���ʂ������̏ꍇ
			*p++ = '0';
			*p++ = '.';
			if ( e10 == -2 ){
				*p++ = '0';
			}
			for ( int i = 0; i < n; ++i ){
				*p++ = '0' + s[ i ];
			}
		}
	}else{ //e�L�@
		//�ꌅ�o��
		*p++ = '0' + s[ 0 ];
		//�����_�o��
		*p++ = '.';
		for ( int i = 1; i < n; ++i ){
			*p++ = '0' + s[ i ];
		}
		if ( e10 != 0 ){
			*p++ = 'e';
			if ( e10 < 0 ){
				*p++ = '-';
				e10 = -e10;
			}
			int q = e10 / 10;
			if ( q != 0 ){	
				*p++ = '0' + static_cast< char >( q );
			}
			e10 -= q * 10;
			*p++ = '0' + static_cast< char >( e10 );
		}
	}
	return static_cast< int >( p - out );
}

int toString( char* out, double a, int precision ){
	return toString( out, static_cast< float >( a ), precision ); //TODO
}


} //namespace GameLib

