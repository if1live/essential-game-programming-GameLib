#ifdef _WIN32
#include <windows.h>
#undef min
#undef max
#endif

#include <cstring> //sprintf�ȂǂȂǁB�g�������Ȃ����d���Ȃ��B
#include <cstdio>
#include <locale>
#include "GameLib/GameLib.h"
#include "GameLib/Base/MemoryManager.h"

//#define STRONG_DEBUG //����Ă�l�p�f�o�O

#ifndef NDEBUG //�f�o�O�������f�o�O���
#define USE_DEBUG_INFO
#endif

//���̃t�@�C���̒���C++�̃��C�u�������g��Ȃ��ŏ�����Ă���B
//���̂��ƌ����΁Anew����Ă΂��ꏊ�Ȃ̂ɁA����new����悤�Ȃ��Ƃ������疳���ċA�Ɋׂ邩�炾�B
//C++�̃��C�u�����͒��ŏ����new����̂łǂ���g���Ă����Ƃ͔��f���������̂ł���B
//C����̊֐��͊�{�I�Ɏg���ȂƏ��������A�ǂ����Ă��g���ׂ����Ƃ����̂͂�����̂��B

namespace GameLib{

namespace{ //������x���蓮�삷��܂ł͂����Ă����B�f�o�O���ɂ����Ďd���Ȃ��B

using namespace std;

//�^�ʖ��B�R�[�h��Z���������B
typedef unsigned U4; //�o�C�g�������ʖ�
typedef unsigned short U2; //2�o�C�g�������ʖ�

//�ݒ�萔�Q
const U4 MAX_NORMAL_BLOCK_SIZE = 128 * 1024; //����ȏ��VirtualAlloc�Œ��ڊm��
const U4 HEAP_REGION_SIZE_BIT = 24; //OS�����C�ɂ����߂�̈�T�C�Y(�Ⴆ��20��1MB, 24��16MB)
const U4 ALIGN = 8; //�A���C���T�C�Y4,8,16�����肩��I�ڂ��B4��I�Ԏ���double���Ȃ����Ƃ�ۏႷ�邱�ƁB
//���o�萔�Q
const U4 HEAP_REGION_SIZE = ( 1 << HEAP_REGION_SIZE_BIT );
const U4 TABLE_NUMBER = ( HEAP_REGION_SIZE_BIT - 3 ) * 4; //�󂫃��X�g���̐�(4���Ƃɔ{�ɂȂ�̂ŃT�C�Y�r�b�g����4�{�����A4,8,12,16�Ǝn�܂邽�߁A���낢�날����-3)

//�u���b�N�t���O
/*
�ł����u���b�N����m�肽���̂́A������B�������Ƃ�������A�g�p���Ɍ��܂��Ă���B
����A�󂩂ǂ�����m�肽���̂�Heap���Ƃ킩���Ă��炾����A�����r�b�g���g���܂킷�B
*/
const U4 FLAG_LARGE_BLOCK = ( 1 << 0 ); //�ł����u���b�N�ł��B
const U4 FLAG_EMPTY = ( 1 << 0 ); //�󂢂Ă܂��B
const U4 FLAG_PREV_EMPTY = ( 1 << 1 ); //�O�̃u���b�N�͋󂫂ł�
const U4 SIZE_MASK = ~( FLAG_PREV_EMPTY | FLAG_EMPTY );

#ifdef USE_DEBUG_INFO
//�t�@�C�����n�b�V��
/*
�f�o�O���p�̃t�@�C�����́A�O���[�o���ϐ��ɂ܂Ƃ߂Ēu���Ă����B4�o�C�g��ɂ��񂾂��߂��B
*/
const U2 FILE_INDEX_MAX = 65521; //65536�ȉ��ő�̑f��
const U2 FILE_INDEX_UNKNOWN = FILE_INDEX_MAX - 1;
const char* gFileNames[ FILE_INDEX_MAX ];

U2 getFileNameIndex( const char* p ){
	if ( !p ){
		return FILE_INDEX_UNKNOWN;
	}
	//�n�b�V���l�v�Z
	ptrdiff_t address = p - static_cast< const char* >( 0 );
	U2 h = static_cast< U2 >( address % FILE_INDEX_MAX );
	//�n�b�V���l��Y�����̏����l�Ƃ��ăC���f�N�X�Q�b�g
	for ( U2 i = h; i < FILE_INDEX_UNKNOWN; ++i ){
		if ( !gFileNames[ i ] || ( gFileNames[ i ] == p ) ){
			gFileNames[ i ] = p;
			return i;
		}
	}
	//�������ɗ���悤���Ƒ����I����Ă�B���Ԃ񖞑��ȑ��x�ł͓����Ȃ��B
	for ( U2 i = 0; i < h; ++i ){
		if ( !gFileNames[ i ] || ( gFileNames[ i ] == p ) ){
			gFileNames[ i ] = p;
			return i;
		}
	}
	STRONG_ASSERT( false && "MemoryManager : FileName Table Full!" );
	return FILE_INDEX_UNKNOWN;
}
#endif //DEBUG_INFO

//char*�Q�b�g
template< class T > inline char* ptr( T* p ){
	return reinterpret_cast< char* >( p );
}

//�C�ӂ̌^�ɃL���X�g���ĕԂ��B�o�C�g�P�ʃI�t�Z�b�g�@�\�t��
template< class T > inline T* cast( void* p, int offsetInByte ){
	return reinterpret_cast< T* >( ptr( p ) + offsetInByte );
}

//�A�h���X���Q�b�g
template< class A, class B > inline U4 diff( A* p0, B* p1 ){
	ptrdiff_t t = ptr( p0 ) - ptr( p1 );
	return static_cast< U4 >( t );
}

//�|�C���^���A���C��
template< class T > inline T* align( T* p, U4 n ){
	ptrdiff_t address = ptr( p ) - static_cast< char* >( 0 );
	address += n - 1;
	address &= ~( n - 1 );
	return reinterpret_cast< T* >( address );
}

//�������A���C��
inline U4 align( U4 size, U4 n ){
	size += n - 1;
	size &= ~( n - 1 );
	return size;
}

#ifdef _WIN32
//OS����A���C�����ꂽ�����������炤
void* allocateAlignedMemory( void** originalPointer, U4 size, U4 alignSize ){
	//2�{�A�h���X��\��
	void* reserved = VirtualAlloc( NULL, size*2, MEM_RESERVE, PAGE_NOACCESS );
	STRONG_ASSERT( reserved && "MemoryManager : Address Space Full!" );
	//�K�؂ȃA���C���A�h���X���擾���āA����������̈�m��
	void* aligned = align( reserved, alignSize );
	void* commited = VirtualAlloc( aligned, size, MEM_COMMIT, PAGE_READWRITE );
	STRONG_ASSERT( commited && "MemoryManager : No Memory!" );
	STRONG_ASSERT( ( commited == aligned ) && "MemoryManager : Unexpected Error! VirtualAlloc() returned invalid value." );

	*originalPointer = reserved; //free�p�A�h���X��Ԃ�
	return commited;
}

//OS���烁���������炤�B�A���C���s�v
void* allocateMemory( U4 size ){
	U4 alignedSize = align( size, 64 * 1024 );
	void* p = VirtualAlloc( NULL, alignedSize , MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );
	STRONG_ASSERT( p && "MemoryManager : No Memory!" );
#ifndef NDEBUG //����
	memset( p, 0xfb, size ); //���g�p�̈�}�[�L���O
	if ( size < alignedSize ){ //�֎~�̈�}�[�L���O
		memset( ptr( p ) + size, 0xf9, alignedSize - size );
	}
#endif
	return p;
}

//OS�Ƀ�������Ԃ�
void deallocateMemory( void* p ){
	BOOL succeeded = VirtualFree( p, 0, MEM_RELEASE );
	STRONG_ASSERT( ( succeeded != 0 ) && "MemoryManager : Deallocation failed! The pointer must be invalid." );
}
#endif

struct Lock{
	Lock(){} //�f�t�H���g�R���X�g���N�^�͉������Ȃ��B�O���[�o���ɒu����鎖�Ԃ�z�肷��B
	Lock( bool f ){
		mLock = ( f ) ? 1 : 0;
	}
	void lock(){
		//���񂩎����ă_���Ȃ��U�X���b�h��؂�ւ���B
		int count = 0;
		while ( InterlockedCompareExchange( &mLock, 1, 0 ) != 0 ){
			++count;
			if ( count == 10 ){ //10�񂭂炢�H
				count = 0;
				Sleep( 0 );
			}
		}
	}
	bool tryLock(){
		return ( InterlockedCompareExchange( &mLock, 1, 0 ) == 0 );
	}
	void unlock(){
		InterlockedCompareExchange( &mLock, 0, mLock );
	}
	LONG mLock;
};

U4 getBitNumber( U4 a ){
	//�񕪌����Ńr�b�g�����m��
	U4 first = 0;
	U4 last = HEAP_REGION_SIZE_BIT;
	while ( last >= first + 2 ){ //����2�ȏ゠��ΌJ��Ԃ�
		U4 middle = ( first + last ) / 2;
		U4 middleV = ( 1 << middle ) - 1;
		if ( middleV < a ){
			first = middle;
		}else if ( middleV > a ){
			last = middle;
		}else{
			first = last = middle;
		}
	}
	//����0��1�ɂȂ��Ă���̂ŁAlast��Ԃ��B
	return last;
}

inline U4 getHeadIndex( U4 size ){
	U4 r;
	//�e�[�u����
	//�ŏ���8��4i�B(0,4,8...)�B�؎̂āB
	//����������32,40,48,56�̂悤��4���Ƃɔ{�B
	if ( size <= 32 ){	//32�o�C�g�܂ł͐��`�B4,8,12...�ƕ���
		r = size >> 2;
	}else{
		//�܂�size�����r�b�g�̐��Ȃ̂����m�肳����B
		U4 bits = getBitNumber( size );
		//4bit�̐��Ȃ�A[2bit*5, 2bit*8)�͈̔͂ɂ���B
		//8-9,10-11,12-13,14-15�̂ǂ͈̔͂��́A12�r�b�g�ځA�܂�2��4�̈ʂ�����΂����B
		//4bit�̐��Ȃ�1bit�E�V�t�g����0-7�ɂ��A���̉���2bit������
		U4 idx = ( size >> ( bits - 3 ) ) & 0x3;
		r = ( bits - 4 ) * 4 + idx;
	}
	return r;
}

//[Lb]��LargeBlock�̗�
class Heap{
public:
	Heap( void* reserved ) :
	mReserved( reserved ),
	mNext( 0 ),
	mLock( false ){
		//�ŏ��̂ł����u���b�N�����B-4��prevSize���P�`���Ă�B�ŏ��̃u���b�N���O�Ȃ�ĂȂ����炾�B
		U4 body = align( sizeof( Heap ) + VACANT_HEADER_SIZE - 4, ALIGN ) - VACANT_HEADER_SIZE;
		U4 bodySize = HEAP_REGION_SIZE - body - 8; //8�o�C�g���炷�̂́A���̃u���b�N�ɖ�����菑������4�o�C�g�ƁA�t���O���Z�b�g���鎟��4�{�o�C�g�̕��B

		//�����N������
		for ( int i = 0; i < TABLE_NUMBER; ++i ){
			U4 headAddr = diff( mHeads[ i ], this );
			U4* head = mHeads[ i ];
			head[ PREV_SIZE ] = head[ SIZE ] = 0xffffffff; //�N�����Ȃ��Ƃ���Ȃ̂Ŏ��ɂ����Ȑ������Ă����B
			head[ NEXT ] = head[ PREV ] = headAddr; //���[�v
		}
		//�ŏ��̃o�b�t�@�����X�g�ɑ}��
		insertToList( body, bodySize );
		setEmptySize( body, bodySize );
		U4 next = body + bodySize;
		setPrevEmptyFlag( next, bodySize );
#ifdef STRONG_DEBUG
check();
#endif
	}
	static Heap* create(){
		void* reserved;
		Heap* r = reinterpret_cast< Heap* >( allocateAlignedMemory( &reserved, HEAP_REGION_SIZE, HEAP_REGION_SIZE ) );

		//�����̃����o�𖄂߂�
		new( r ) Heap( reserved );
		return r;
	}
	void removeFromList( U4 addr ){
		if ( getSize( addr ) >= VACANT_HEADER_SIZE ){ //�T�C�Y������ĂȂ����̂͒x���J�����B
			U4 next = getNext( addr );
			U4 prev = getPrev( addr );
			setPrev( next, prev );
			setNext( prev, next );
		}
	}
	void insertToList( U4 addr, U4 size ){
		STRONG_ASSERT( addr < 0xfffff8 );
		U4 index = getHeadIndex( size - OCCUPYED_HEADER_SIZE + 4 );//�w�b�_�������āA4�o�C�g�̃u���b�N�ɐH�����ނ̂ŁA���̃T�C�Y�������B
		//�V�������X�g�ɍ������ށB�擪�ɑ}��
		U4 first = mHeads[ index ][ NEXT ];
		U4 last = getPrev( first );

		setNext( last, addr );
		setPrev( addr, last );
		setNext( addr, first );
		setPrev( first, addr );
	}

#ifdef USE_DEBUG_INFO
	void* allocate( U4 size, U4 debugInfo ){
#else
	void* allocate( U4 size ){
#endif
#ifdef STRONG_DEBUG
check();
#endif
		void* r = 0;
		U4 headIndex = getHeadIndex( size ) + 1; //���̃T�C�Y���\���ɓ��锠���~�����̂ŁA+1����K�v������
		for ( U4 i = headIndex; i < TABLE_NUMBER; ++i ){
			U4 head = diff( mHeads[ i ], this );
			U4 current = mHeads[ i ][ NEXT ];
			if ( current != head ){
				U4 currentSizeWithFlag = getSize( current );
				U4 currentSize = currentSizeWithFlag & SIZE_MASK;
				U4 next = current + currentSize;
				//8�o�C�g�A���C�������߂�A�h���X���v�Z(+4�͎��̃w�b�_�̍ŏ�4�o�C�g��j�󂷂�C������)
				U4 user = ( next + 4 - size ) & ~( ALIGN - 1 );
				//��������g�p���w�b�_�T�C�Y���������̂��u���b�N�擪�A�h���X
				U4 newBlock = user - OCCUPYED_HEADER_SIZE;
				//���̃u���b�N�̃w�b�_���ȏ�̂���̂ł���΁A����
				if ( newBlock >= current + VACANT_HEADER_SIZE ){
					//�܂����X�g����O��
					removeFromList( current );
					//�����̃T�C�Y���Đݒ�
					currentSize = newBlock - current;
					setEmptySize( current, currentSize );
					//�Đݒ肵���T�C�Y�Ń��X�g�ɍēo�^
					insertToList( current, currentSize );
					//�V�u���b�N�ݒ�
					setSize( newBlock, next - newBlock );
#ifdef USE_DEBUG_INFO
					setDebugInfo( newBlock, debugInfo );
#endif
					//�t���O�ݒ�
					setPrevEmptyFlag( newBlock, currentSize ); //�O�͐�΋󂢂Ă�
					resetPrevEmptyFlag( next ); //���̎��͎g���Ă�
#ifdef STRONG_DEBUG
check();
#endif
				}else{ //�c��Ȃ��B���̃u���b�N�����̂܂ܗp����B
					removeFromList( current ); //���X�g����؂藣��
#ifdef USE_DEBUG_INFO
					setDebugInfo( current, debugInfo );
#endif
					//�t���O�ݒ�
					resetEmptyFlag( current ); //�󂢂ĂȂ�
					resetPrevEmptyFlag( next ); //�󂢂ĂȂ����Ƃ�`����
					//�ԋp�A�h���X
					user = current + OCCUPYED_HEADER_SIZE;
#ifdef STRONG_DEBUG
check();
#endif
				}
				r = ptr( this ) + user;
#ifndef NDEBUG //�f�o�O�p�h��Ԃ�
				memset( r, 0xfb, size ); //���g�p��fb
				char* paddingBegin = ptr( r ) + size;
				U4 paddingSize = next + 4 - user - size; //+4�݂͂͂�����
				memset( paddingBegin, 0xf9, paddingSize ); //�g�p�֎~�̈��f9
#endif
				break; //�m�ۏI��
			}
		}
		return r;
	}
	void deallocate( void* p ){
#ifdef STRONG_DEBUG
check();
#endif
		U4 addr = diff( p, this ) - OCCUPYED_HEADER_SIZE;
		U4 sizeWithFlag = getSize( addr );
		//�폜�́A�܂��O��Ƃ̗Z�����\���ǂ����𒲂ׂ�B
		bool prevIsEmpty = ( ( sizeWithFlag & FLAG_PREV_EMPTY ) != 0 );
		U4 size = ( sizeWithFlag & SIZE_MASK );
		U4 next = addr + size;
#ifndef NDEBUG 
		//�t�߂̐������`�F�b�N
		if ( prevIsEmpty ){
			U4 prevSize = getPrevSize( addr );
			U4 prev = addr - prevSize;
			U4 prevSizeWithFlag = getSize( prev );
			STRONG_ASSERT( prevSizeWithFlag & FLAG_EMPTY );
			STRONG_ASSERT( prevSize == ( prevSizeWithFlag & SIZE_MASK ) );
		}
		U4 nextSizeWithFlagDebug = getSize( next );
		STRONG_ASSERT( !( nextSizeWithFlagDebug & FLAG_PREV_EMPTY ) ); //�g���Ă�񂾂���󂢂Ă�킯�Ȃ��ł��B

		//����ς݃}�[�N�œh��Ԃ�
		ptrdiff_t fillSize = ptr( this ) + next - ptr( p );
		if ( fillSize > 0 ){
			memset( p, 0xfd, fillSize ); //�g�p�ς݂�fb
		}
#endif
		U4 nextSizeWithFlag = getSize( next );
		bool nextIsEmpty = ( ( nextSizeWithFlag & FLAG_EMPTY ) != 0 );
		U4 nextSize = ( nextSizeWithFlag & SIZE_MASK );

		if ( prevIsEmpty ){ //�O���󂢂Ă�B
			U4 prevSize = getPrevSize( addr );
			U4 prev = addr - prevSize;
			if ( nextIsEmpty ){ //�����󂢂Ă�
				//�O��Ƃ����O���āA
				removeFromList( prev );
				removeFromList( next );
				//�T�C�Y�Đݒ�
				prevSize += size + nextSize;
				setEmptySize( prev, prevSize );
				insertToList( prev, prevSize );
				//���̃u���b�N�ɋ󂢂Ă��邱�Ƃ�`����
				U4 nextNext = prev + prevSize;
				setPrevEmptyFlag( nextNext, prevSize );
#ifdef STRONG_DEBUG
check();
#endif
			}else{ //�O�����󂢂Ă�
				removeFromList( prev );
				//�T�C�Y�Đݒ�
				prevSize += size;
				setEmptySize( prev, prevSize );
				insertToList( prev, prevSize );
				//���̃u���b�N�ɋ󂢂Ă��邱�Ƃ�`����
				setPrevEmptyFlag( next, prevSize );
#ifdef STRONG_DEBUG
check();
#endif
			}
		}else{
			if ( nextIsEmpty ){ //�����󂢂Ă�
				removeFromList( next );
				//�T�C�Y�Đݒ�
				size += nextSize;
				setEmptySize( addr, size );
				insertToList( addr, size );
				//���̃u���b�N�ɋ󂢂Ă���T�C�Y��`����
				U4 nextNext = addr + size;
				setPrevSize( nextNext, size );
#ifdef STRONG_DEBUG
check();
#endif
			}else{ //�������ĂȂ��B
				if ( size >= VACANT_HEADER_SIZE ){ //���X�g�ɂȂ���󋵂ł����Ȃ��B����Ȃ��Ƃ��ׂ͗���������̂�҂B
					insertToList( addr, size );
				}
				setEmptyFlag( addr );
				//���̃u���b�N�ɋ󂢂Ă��邱�Ƃ�`����
				setPrevEmptyFlag( next, size );
#ifdef STRONG_DEBUG
check();
#endif
			}
		}
	}
	void check(){
		/*
		�����ōs���`�F�b�N�́A
		�P�D�A�h���X���̃`�F�b�N�B�O�T�C�Y�A�T�C�Y�A�u���b�N���E���X�̐�����
		�Q�D�����N�����̃`�F�b�N�B�󂢂Ă��邩�B�T�C�Y�͓K�����B
		�R�D�����N�~���̃`�F�b�N�B�󂢂Ă��邩�B�T�C�Y�͓K�����B
		*/
		U4 begin = align( sizeof( Heap ) + VACANT_HEADER_SIZE - 4, ALIGN ) - VACANT_HEADER_SIZE;
		U4 current = begin;
		U4 prevSizeCheck = 0;
		bool prevIsEmpty = false;
		U4 end = HEAP_REGION_SIZE - OCCUPYED_HEADER_SIZE; //�������O����B�łȂ��ƃw�b�_���͂����
		while ( current < end ){
			U4 sizeWithFlag = getSize( current );
			if ( sizeWithFlag & FLAG_PREV_EMPTY ){
				STRONG_ASSERT( prevIsEmpty );
				U4 prevSize = getPrevSize( current );
				STRONG_ASSERT( prevSize == prevSizeCheck );
			}
			prevIsEmpty = ( sizeWithFlag & FLAG_EMPTY ) ? true : false;
			U4 size = sizeWithFlag & SIZE_MASK;
			current += size;
			prevSizeCheck = size;
		}
		for ( U4 i = 0; i < TABLE_NUMBER; ++i ){
			U4 head = diff( mHeads[ i ], this );
			U4 current;
			//�����N�����`�F�b�N
			current = mHeads[ i ][ NEXT ];
			while ( current != head ){
				U4 sizeWithFlag = getSize( current );
				STRONG_ASSERT( sizeWithFlag & FLAG_EMPTY ); //�󂢂Ă�͂�����B
				U4 size = sizeWithFlag & SIZE_MASK;
				U4 idx = getHeadIndex( size - OCCUPYED_HEADER_SIZE + 4 );//�w�b�_�������āA4�o�C�g�̃u���b�N�ɐH�����ނ̂ŁA���̃T�C�Y�������B
				STRONG_ASSERT( idx == i );
				current = getNext( current );
			}
			//�����N�~���`�F�b�N
			current = mHeads[ i ][ PREV ];
			while ( current != head ){
				U4 sizeWithFlag = getSize( current );
				STRONG_ASSERT( sizeWithFlag & FLAG_EMPTY ); //�󂢂Ă�͂�����B
				U4 size = sizeWithFlag & SIZE_MASK;
				U4 idx = getHeadIndex( size - OCCUPYED_HEADER_SIZE + 4 );//�w�b�_�������āA4�o�C�g�̃u���b�N�ɐH�����ނ̂ŁA���̃T�C�Y�������B
				STRONG_ASSERT( idx == i );
				current = getPrev( current );
			}
		}
	}
	void write( FILE* fp ){
		char str[ 8192 ];
		U4 begin = align( sizeof( Heap ) + VACANT_HEADER_SIZE - 4, ALIGN ) - VACANT_HEADER_SIZE;
		U4 current = begin;
		U4 end = HEAP_REGION_SIZE - OCCUPYED_HEADER_SIZE; //�������O����B�łȂ��ƃw�b�_���͂����
		while ( current < end ){
			U4 sizeWithFlag = getSize( current );
			U4 size = sizeWithFlag & SIZE_MASK;
			if ( !( sizeWithFlag & FLAG_EMPTY ) ){
#ifdef USE_DEBUG_INFO
				U4 debugInfo = getDebugInfo( current );
				const char* filename = gFileNames[ debugInfo & 0xffff ];
				U4 line = debugInfo >> 16;
#else
				const char* filename = 0;
				U4 line = 0;
#endif
				if ( !filename ){
					filename = "unknown";
				}
				int l = sprintf_s( 
					str, 
					8192,
					"%p\t\t%d\t\t%s\t\t%d\n",
					ptr( this ) + current,
					size,
					filename,
					line );
				if ( fp ){
					fwrite( str, 1, l, fp );
				}else{
					::OutputDebugStringA( str );
				}
			}
			current += size;
		}
	}
	void lock(){
		mLock.lock();
	}
	bool tryLock(){
		return mLock.tryLock();
	}
	void unlock(){
		mLock.unlock();
	}
	enum Header{
		PREV_SIZE = 0, //�����͑O�̃u���b�N���󂢂Ă鎞���������B����ȊO�̎��͉󂳂�Ă���B
		SIZE = 1,
#ifdef USE_DEBUG_INFO
		DEBUG_INFO = 2, //���Ƌ��p�B�����͎g�p�������g��Ȃ��B
#endif
		NEXT = 2,
		PREV = 3,

		HEADER_MAX = 4,
	};
#ifdef USE_DEBUG_INFO
	static const U4 OCCUPYED_HEADER_SIZE = 12;
#else
	static const U4 OCCUPYED_HEADER_SIZE = 8;
#endif
	static const U4 VACANT_HEADER_SIZE = 16;
	//���܂��܂Ƃ����w�b�_����֐��Q
	void setNext( U4 addr, U4 target ){
		( cast< U4 >( this, addr ) )[ NEXT ] = target;
	}
	void setPrev( U4 addr, U4 target ){
		( cast< U4 >( this, addr ) )[ PREV ] = target;
	}
	void setSize( U4 addr, U4 size ){
		( cast< U4 >( this, addr ) )[ SIZE ] = size;
	}
	void setEmptySize( U4 addr, U4 size ){
		( cast< U4 >( this, addr ) )[ SIZE ] = size | FLAG_EMPTY;
	}
	void setPrevSize( U4 addr, U4 size ){
		( cast< U4 >( this, addr ) )[ PREV_SIZE ] = size;
	}
	void setPrevEmptyFlag( U4 addr, U4 prevSize ){
		U4* p4 = cast< U4 >( this, addr );
		p4[ SIZE ] |= FLAG_PREV_EMPTY;
		p4[ PREV_SIZE ] = prevSize;
	}
	void setEmptyFlag( U4 addr ){
		( cast< U4 >( this, addr ) )[ SIZE ] |= FLAG_EMPTY;
	}
	void resetEmptyFlag( U4 addr ){
		( cast< U4 >( this, addr ) )[ SIZE ] &= ~FLAG_EMPTY;
	}
	void resetPrevEmptyFlag( U4 addr ){
		( cast< U4 >( this, addr ) )[ SIZE ] &= ~FLAG_PREV_EMPTY;
	}
	U4 getSize( U4 addr ){
		return ( cast< U4 >( this, addr ) )[ SIZE ];
	}
	U4 getPrevSize( U4 addr ){
		return ( cast< U4 >( this, addr ) )[ PREV_SIZE ];
	}
	U4 getNext( U4 addr ){
		return ( cast< U4 >( this, addr ) )[ NEXT ];
	}
	U4 getPrev( U4 addr ){
		return ( cast< U4 >( this, addr ) )[ PREV ];
	}
#ifdef USE_DEBUG_INFO
	void setDebugInfo( U4 addr, U4 debugInfo ){
		( cast< U4 >( this, addr ) )[ DEBUG_INFO ] = debugInfo;
	}
	U4 getDebugInfo( U4 addr ){
		return ( cast< U4 >( this, addr ) )[ DEBUG_INFO ];
	}
#endif
	void* mReserved; //�\��̈�|�C���^
	Heap* mNext; //���̃q�[�v
	U4 mHeads[ TABLE_NUMBER ][ HEADER_MAX ]; //�T�C�Y���Ƃ̋󂫃��X�g
	Lock mLock;
};

class Impl{
public:
#ifdef USE_DEBUG_INFO
	void* allocate( size_t sizeOrig, U4 debugInfo = FILE_INDEX_UNKNOWN ){
#else
	void* allocate( size_t sizeOrig ){
#endif
#ifdef _WIN64
		STRONG_ASSERT( sizeoOrig <= 0xffffffff && "allocation over 4GB is forbidden" );
#endif
		U4 size = static_cast< U4 >( sizeOrig ); //4GB�ȏ��
		//�߂�l
		void* r = 0;
		//size�����E�𒴂��Ă���B��p�u���b�N��p��
		if ( size > MAX_NORMAL_BLOCK_SIZE ){
#ifdef USE_DEBUG_INFO
			r = allocateLb( size, debugInfo );
#else
			r = allocateLb( size );
#endif
		}else{
			//�q�[�v��allocate()������B�_���Ȃ玟�ցB
			Heap* current = mHead; //���b�N�s�v�B0�Ȃ�0�Ō��\�B0�ȊO�Ȃ�ǂ�ł����܂��B
			while ( current ){
				if ( current->tryLock() ){
#ifdef USE_DEBUG_INFO
					r = current->allocate( size, debugInfo );
#else
					r = current->allocate( size );
#endif
					current->unlock();
				}
				if ( r ){
					break;
				}
				current = current->mNext;
			}

			if ( !r ){
				//�󂫂��Ȃ��B�q�[�v�𑫂��Ȃ��Ƃ����܂���B
				Heap* newHeap = Heap::create();
#ifdef USE_DEBUG_INFO
				r = newHeap->allocate( size, debugInfo );
#else
				r = newHeap->allocate( size );
#endif
				//�擪�ɑ����܂�
				mHeapLock.lock();
				Heap* first = mHead;
				mHead = newHeap;
				newHeap->mNext = first;
				mTotalSize += HEAP_REGION_SIZE;
				mHeapLock.unlock();
			}
		}
STRONG_ASSERT( r && "BUG!" ); //�����͂��肦�Ȃ��B�o�O��
		return r;
	}
	void deallocate( void* p ){
		if ( !p ){ //0���
			return;
		}
		//mNext��0�Ȃ�A�ł����u���b�N�ł��B
		if ( isLb( p ) ){
			deallocateLb( p );
		}else{ //�����łȂ��ꍇ�A�܂�Area�̐擪������o���āAArea�ɍ폜������
			Heap* heap = getHeap( p );
			heap->lock();
			heap->deallocate( p );
			heap->unlock();
		}
	}
	void write( const char* filename ){
		//���̒���new���g���킯�ɂ͍s���Ȃ��B
		//����͑�����ofstream�����߂��Ƃ������Ƃ��Ӗ�����B����new���邩�炾�B
		//����āA���̃t�@�C���̒��ł�C�W�����C�u�����Ŏ����s���B
		FILE* fp = 0;
		if ( filename ){
			setlocale( LC_ALL, "" );
			errno_t e = fopen_s( &fp, filename, "w" );
			STRONG_ASSERT( ( e == 0 ) && "MemoryManager::write() : can't open output file." );
		}else{
			::OutputDebugStringA( "\n" );
		}
		char str[ 8192 ]; //��ߏ�Ɏ���B�����ŃP�`���Ă����̓����Ȃ�����ȁB�{��sprintf�̓_�����ȂƎv���B
		int l = 0;
		l += sprintf_s( str + l, 8192 - l, "[ MemoryManager::write() ]\n" );
		l += sprintf_s( str + l, 8192 - l, "totalSize = %d ( %x ) : %.2fMB\n", mTotalSize, mTotalSize, static_cast< float >( mTotalSize ) / 1024.f / 1024.f );
		l += sprintf_s( str + l, 8192 - l, "address\t\tsize\t\tfile\t\tline\n" );
		if ( fp ){
			fwrite( str, 1, l, fp );
		}else{
			::OutputDebugStringA( str );
		}

		mLbLock.lock();
		//�܂���u���b�N����
		if ( mLbHead ){
			void* begin = mLbHead;
			void* block = begin;
			do{
#ifdef USE_DEBUG_INFO
				U4 debugInfo = getLbDebugInfo( block );
				const char* filename = gFileNames[ debugInfo & 0xffff ];
				U4 line = debugInfo >> 16;
#else
				const char* filename = 0;
				U4 line = 0;
#endif
				if ( !filename ){
					filename = "unknown";
				}
				U4 size = getLbSize( block );
				l = sprintf_s( 
					str, 
					8192,
					"%p\t\t%d\t\t%s\t\t%d\n",
					block,
					size,
					filename,
					line );
				if ( fp ){
					fwrite( str, 1, l, fp );
				}else{
					::OutputDebugStringA( str );
				}
				block = getLbNext( block );
			} while ( begin != block );
		}
		mLbLock.unlock();
		//���q�[�v
		mHeapLock.lock();
		Heap* current = mHead;
		while ( current ){
			current->write( fp );
			current = current->mNext;
		}
		mHeapLock.unlock();
	}
	void check(){
		checkLb();
		checkHeap();
	}
	~Impl(){
#ifndef NDEBUG
		write( 0 );
#endif
	}
	int totalSize() const {
		return mTotalSize;
	}
private:
	void checkLb(){
		mLbLock.lock();
		//�܂���u���b�N����
		if ( mLbHead ){
			void* begin = mLbHead;
			void* block = begin;
			do{
				void* next = getLbNext( block );
				void* prev = getLbPrev( block );
				void* nextPrev = getLbPrev( next );
				void* prevNext = getLbNext( prev );
				STRONG_ASSERT( nextPrev == block );
				STRONG_ASSERT( prevNext == block );
				block = next;
			} while ( begin != block );
		}
		mLbLock.unlock();
	}
	void checkHeap(){
		Heap* h = mHead;
		while ( h ){
			h->lock();
			h->check();
			h->unlock();
		}
	}
	Heap* getHeap( void* p ){
		ptrdiff_t address = ptr( p ) - static_cast< char* >( 0 );
		address &= ~( HEAP_REGION_SIZE - 1 );
		return reinterpret_cast< Heap* >( address );
	}
#ifdef USE_DEBUG_INFO
	void* allocateLb( U4 size, U4 debugInfo ){
#else
	void* allocateLb( U4 size ){
#endif
#ifdef STRONG_DEBUG
checkLb();
#endif
		//�T�C�Y�̉���2�r�b�g��0�ɂ��邽�߂ɃA���C��
		size = align( size, 8 );
		//�w�b�_�T�C�Y�v�Z�B�|�C���^�O��U4���
		U4 alignedHeaderSize = align( LB_HEADER_SIZE, ALIGN );
		void* p = allocateMemory( size + alignedHeaderSize );

		//���[�U�ɕԂ��|�C���^
		void* r = ptr( p ) + alignedHeaderSize; 
		setLbSize( r, size );
#ifdef USE_DEBUG_INFO
		setLbDebugInfo( r, debugInfo );
#endif

		//�����N�����ւ�
		mLbLock.lock();
		void* prev;
		void* next;
		if ( mLbHead ){
			prev = mLbHead;
			next = getLbNext( mLbHead );
			setLbPrev( next, r );
			setLbNext( mLbHead, r );
		}else{
			prev = next = r;
		}
		setLbNext( r, next );
		setLbPrev( r, prev );
		mLbHead = r;
		mTotalSize += size;
		mLbLock.unlock();
#ifdef STRONG_DEBUG
checkLb();
#endif
		return r;
	}
	void deallocateLb( void* p ){
#ifdef STRONG_DEBUG
checkLb();
#endif

		//�����N����
		mLbLock.lock();
		void* prev = getLbPrev( p );
		void* next = getLbNext( p );
#ifndef NDEBUG
		void* nextPrev = getLbPrev( next );
		void* prevNext = getLbNext( prev );
		STRONG_ASSERT( nextPrev == p );
		STRONG_ASSERT( prevNext == p );
#endif
		setLbNext( prev, next );
		setLbPrev( next, prev );
		if ( mLbHead == p ){ //�擪�̍폜�������B
			mLbHead = next; //���̃|�C���^�ɕύX
			if ( mLbHead == p ){ //���ɂ��Ă��Ȃ������Ƃ������Ƃ͍Ō�̈���B
				mLbHead = 0; //���E
			}
		}
		mTotalSize -= getLbSize( p ) & SIZE_MASK;
		mLbLock.unlock();
#ifdef STRONG_DEBUG
checkLb();
#endif
		//���������
		char* origP = ptr( p ) - LB_HEADER_SIZE;
		U4 alignedHeaderSize = align( LB_HEADER_SIZE, ALIGN );
		origP -= alignedHeaderSize - LB_HEADER_SIZE; //�A���C���ł��炵���Ȃ�A�ԋp�A�h���X�����炷
		deallocateMemory( origP );
	}
/* //��u���b�N�̃��C�A�E�g
	void* next;
	void* prev;
	U4 size;
	U4 debugInfo; //���̒�������[�U�ɕԂ�
*/
#ifdef USE_DEBUG_INFO
	static const int DEBUG_INFO = sizeof( U4 );
	static const int SIZE = DEBUG_INFO + sizeof( U4 );
#else
	static const int SIZE = sizeof( U4 );
#endif
	static const int PREV = SIZE + sizeof( void* );
	static const int NEXT = PREV + sizeof( void* );
	static const int LB_HEADER_SIZE = NEXT;

	static bool isLb( void* p ){//�T�C�Y�͕K��4�o�C�g�O�ɂ���B
		U4* up = cast< U4 >( p, -SIZE );
		return ( *up & FLAG_LARGE_BLOCK );
	}
#ifdef USE_DEBUG_INFO
	static U4 getLbDebugInfo( void* p ){
		return *cast< U4 >( p, -DEBUG_INFO );
	}
	static void setLbDebugInfo( void* p, U4 di ){
		*cast< U4 >( p, -DEBUG_INFO ) = di;
	}
#endif
	static U4 getLbSize( void* p ){
		return *cast< U4 >( p, -SIZE );
	}
	static void* getLbPrev( void* p ){
		return *cast< void* >( p, -PREV );
	}
	static void* getLbNext( void* p ){
		return *cast< void* >( p, -NEXT );
	}
	static void setLbSize( void* p, U4 size ){
		*cast< U4 >( p, -SIZE ) = size | FLAG_LARGE_BLOCK;
	}
	static void setLbPrev( void* p, void* prev ){
		*cast< void* >( p, -PREV ) = prev;
	}
	static void setLbNext( void* p, void* next ){
		*cast< void* >( p, -NEXT ) = next;
	}

	//�q�[�v�擪
	Heap* mHead;
	//��u���b�N�擪
	void* mLbHead;
	//���b�N
	Lock mLbLock;
	Lock mHeapLock;
	//�S�T�C�Y
	int mTotalSize;
};
Impl gImpl;

} //namespace {}

MemoryManager MemoryManager::instance(){
	return MemoryManager();
}

void MemoryManager::write( const char* filename ){
	gImpl.write( filename );
}

int MemoryManager::totalSize() const {
	return gImpl.totalSize();
}

} //naemsapce GameLib

using namespace GameLib;

#ifdef USE_DEBUG_INFO
void* operator new( size_t size, const char* filename, int line ){
	unsigned debugInfo = ( line << 16 ) | getFileNameIndex( filename );
	return gImpl.allocate( size, debugInfo );
#else
void* operator new( size_t size, const char*, int ){
	return gImpl.allocate( size );
#endif
}

#ifdef USE_DEBUG_INFO
void* operator new[]( size_t size, const char* filename, int line ){
	unsigned debugInfo = ( line << 16 ) | getFileNameIndex( filename );
	return gImpl.allocate( size, debugInfo );
#else
void* operator new[]( size_t size, const char*, int ){
	return gImpl.allocate( size );
#endif
}

void operator delete( void* p, const char*, int ){
	gImpl.deallocate( p );
}

void operator delete[]( void* p, const char*, int ){
	gImpl.deallocate( p );
}

void* operator new( size_t size ){
	return gImpl.allocate( size );
}

void* operator new[]( size_t size ){
	return gImpl.allocate( size );
}

void operator delete( void* p ){
	gImpl.deallocate( p );
}

void operator delete[]( void* p ){
	gImpl.deallocate( p );
}
