#ifndef INCLUDED_GAMELIB_REFSTRING_H
#define INCLUDED_GAMELIB_REFSTRING_H

//#include <string>

namespace GameLib{
//using namespace std;

//�Q�ƌ^������N���X�B
/*!
�Q�Ƃ��������Ȃ����߁A���̃f�[�^���j�������Δ��Ɋ댯�Ȃ��ƂɂȂ�B
const char*�݂̂ł͕s�ւȏꍇ�ANULL�I�[���s�\�ȏꍇ�ŁA
�����\���C�ɂȂ�ꍇ�Ɍ����Ďg�����Ƃ𐄏�����B
*/
class RefString{
public:
	RefString();
	RefString( const char*, int size );
	explicit RefString( const char* );
	~RefString();
	void set( const char*, int size );
	void operator=( const char* );
//	void operator=( const string& );
	char operator[]( int i ) const;
	const char* get() const;
	int size() const;
	void setSize( int );
	///last�͍Ō�̕������̂��́B+1����ȁBlast==-1���Ǝn�_�����ς���
	void clamp( int first, int last = -1 ); 
	int find( char ) const; //�O���當����T���Ĉʒu��Ԃ��B������Ȃ���-1�B
	int rfind( char ) const; //�ォ�當����T���Ĉʒu��Ԃ��B������Ȃ���-1�B
	///�Q�Ɛ敶������R�s�[���ă|�C���^��Ԃ��B���O��delete���邱�ƁB
	char* copyOriginal() const;

	bool operator==( const RefString& ) const;
	bool operator==( const char* ) const;
//	bool operator==( const string& ) const;
	bool operator!=( const RefString& ) const;
	bool operator!=( const char* ) const;
//	bool operator!=( const string& ) const;
	bool operator<( const RefString& ) const;
private:
	const char* mString;
	int mSize;
};

} //namespace GameLib

#endif