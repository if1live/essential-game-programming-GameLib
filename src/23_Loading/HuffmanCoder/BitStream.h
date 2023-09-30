#ifndef INCLUDED_BITSTREAM_H
#define INCLUDED_BITSTREAM_H

//�ǂݍ��ݐ�p
class IBitStream{
public:
	//�̈���Z�b�g���ăR���X�g���N�g
	explicit IBitStream( const unsigned char* buffer ); //explicit�͈�������̃R���X�g���N�^�ɂ͕K�����邨�܂��Ȃ��B������������X�y�[�X�͂Ȃ����A�m��Ȃ��Ƃ܂����̂Œ��ׂĂ������ƁB
	void read( unsigned char* data, int sizeInBit ); //�w��̃o�C�g���ǂݍ���
	bool read(); //1�r�b�g�ǂݍ���ŁA1�Ȃ�true��Ԃ��B
	unsigned readU4(); //unsigned int�ǂݍ���
	int readS4(); //signed int�ǂݍ���
	int positionInByte() const; //�o�C�g�ňʒu��Ԃ��B
private:
	const unsigned char* mBuffer;
	int mPosition; //�r�b�g�P�ʂł��B
};

//�������ݐ�p
class OBitStream{
public:
	//�̈���Z�b�g���ăR���X�g���N�g
	explicit OBitStream( unsigned char* buffer ); //explicit�͈�������̃R���X�g���N�^�ɂ͕K�����邨�܂��Ȃ��B������������X�y�[�X�͂Ȃ����A�m��Ȃ��Ƃ܂����̂Œ��ׂĂ������ƁB
	void write( const unsigned char* data, int sizeInBit );//�w��̃r�b�g����������
	void write( bool ); //1�r�b�g��������
	void write( unsigned ); //unsigned�ǂݍ���
	void write( int ); //int�ǂݍ���
	int sizeInByte() const; //�o�C�g�ŃT�C�Y��Ԃ��B
private:
	unsigned char* mBuffer;
	int mPosition; //�r�b�g�P�ʂł��B
};

#endif

