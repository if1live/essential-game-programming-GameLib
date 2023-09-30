#ifndef INCLUDED_GAMELIB_SOUND_PLAYER_H
#define INCLUDED_GAMELIB_SOUND_PLAYER_H

namespace GameLib{
namespace Sound{

class Wave;

class Player{
public:
	static Player create( Wave );
	///�X�g���[�~���O�Đ��pPlayer
	static Player create( int bitsPerSample, int bufferSizeInByte, int frequency, int channelNumber );

	///�Đ��B�������[�v�Đ������������true��n��
	void play( bool looping = false );
	void stop();
	///�{�����[���ݒ�(100���ő�A0���ŏ�)
	void setVolume( int volume );
	bool isPlaying() const; //�Đ����ł����H

	///�X�g���[�~���O�o�b�t�@�ɏ������ށB�Đ��ʒu���߂Â������Ď��s�����ꍇ��false��Ԃ��B����т���B
	bool write( int position, const char* data, int size );
	///�X�g���[�~���O�o�b�t�@�ɖ������������ށB�Đ��ʒu���߂Â������Ď��s�����ꍇ��false��Ԃ��B����т���B
	bool fillSilence( int position, int size );
	///�Đ����̃o�b�t�@�ʒu���o�C�g�P�ʂŕԂ�
	int position() const;

	//�ȉ����[�U�͈ӎ����Ȃ��֐��Q
	Player();
	Player( const Player& );
	~Player();
	operator void*() const;
	Player& operator=( const Player& );
	void release();
	bool operator==( const Player& ) const;
	bool operator!=( const Player& ) const;
private:
	class Impl;
	Impl* mImpl;
};

} //namespace Sound
} //namespace GameLib

#endif
