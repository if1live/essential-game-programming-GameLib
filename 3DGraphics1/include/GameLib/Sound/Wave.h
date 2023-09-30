#ifndef INCLUDED_GAMELIB_SOUND_WAVE_H
#define INCLUDED_GAMELIB_SOUND_WAVE_H

namespace GameLib{
namespace Sound{

class Wave{
public:
	static Wave create( const char* filename );
	///�g�`������(8bit)
	static Wave create( const unsigned char* data, int size, int frequency, int channelNumber ); 
	///�g�`������(16bit)
	static Wave create( const short* data, int size, int frequency, int channelNumber );
	bool isReady(); //���[�h�I������H(const�łȂ��̂͒��ŏ������邽��)
	bool isError() const; //�G���[�N�����ĂȂ��H

	//�ȉ����[�U�͈ӎ����Ȃ��֐��Q
	Wave();
	Wave( const Wave& );
	~Wave();
	operator void*() const;
	Wave& operator=( const Wave& );
	void release();
	bool operator==( const Wave& ) const;
	bool operator!=( const Wave& ) const;
private:
	class Impl;
	friend class Player;
	Impl* mImpl;
};

} //namespace Sound
} //namespace GameLib

#endif
