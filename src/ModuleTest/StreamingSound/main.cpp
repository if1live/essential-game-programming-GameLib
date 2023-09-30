#include "GameLib/Framework.h"
#include "GameLib/Sound/Player.h"
#include "GameLib/FileIO/InFile.h"

using namespace GameLib;
using namespace GameLib::Sound;
using namespace GameLib::FileIO;

InFile gFile;
Player gPlayer;
const int BUFFER_SIZE = 16 * 1024; //����ȉ��͂����߂��Ȃ��B1/16�b���ȏ�Ȃ���Player�R���X�g���N�^�ł͂˂���B
const int BUFFER_HALFSIZE = BUFFER_SIZE / 2;

int gReadPos = 0;
int gWritePos = 0;
int gWaveSize;
int gWaveStart;

namespace GameLib{
	void Framework::configure( Configuration* ){}
	void Framework::update(){
		if ( !gFile ){
			gFile = InFile::create( "charara.wav" );
			while ( !gFile.isFinished() ){ ; } //���[�h�҂�

			//--------Wav�t�@�C������͂��Ă݂悤�B

			int channels = gFile.getShort( 22 ); //22�o�C�g�ڂ���2�o�C�g���`���l����
			int frequency = gFile.getInt( 24 ); //24�o�C�g�ڂ���4�o�C�g�����g��
			int bitsPerSample = gFile.getShort( 34 ); //34�o�C�g�ڂ���2�o�C�g���r�b�g��
			const char* data = gFile.data();
			int pos = 36; //36�o�C�g�ڂ���ǂ�ł���
			//����Ȃ����̂�ǂݔ�΂�
			while ( data[ pos+0 ] != 'd' || data[ pos+1 ] != 'a' || data[ pos+2 ] != 't' || data[ pos+3 ] != 'a' ){ //"data"�ƕ���ł�����̂�������܂Ői�߂�B
				pos += gFile.getInt( pos+4 ); //����Ȃ����̂̃T�C�Y�������ɓ����Ă���
			}
			//����ƃf�[�^����������
			gWaveSize = gFile.getInt( pos + 4 ); //�f�[�^�̗ʂ�"data"�̎�����4�o�C�g
			gWaveStart = pos + 8; //����Ɏ����炪�g
			if ( gWaveSize + gWaveStart > gFile.size() ){ //���܂ɃT�C�Y�ɃE�\�������Ă���t�@�C��������B
				gWaveSize = gFile.size() - gWaveStart; //�d�����Ȃ��̂Ŗ����܂ŁB
			}


			//�v���C���[����
			gPlayer = Player::create( bitsPerSample, BUFFER_SIZE, frequency, channels );

			//�ŏ��̕������[�U
			int restSize = gWaveSize - gReadPos; //�c��t�@�C���T�C�Y
			int writeSize = ( restSize >= BUFFER_HALFSIZE ) ? BUFFER_HALFSIZE : restSize; //�o�b�t�@�̔������傫����΂��ꂾ���B��������Ύc��S���B
			bool succeeded = gPlayer.write( gWritePos, gFile.data() + 44 + gReadPos, writeSize ); //�������݁B
			if ( !succeeded ){
				cout << "WRITING FAILED!" << endl;
			}
			gReadPos += writeSize;
			gWritePos += writeSize;
			//����Ȃ��f�[�^��0�𖄂߂�
			if ( writeSize < BUFFER_HALFSIZE ){
				gPlayer.fillSilence( gWritePos, BUFFER_HALFSIZE - writeSize );
				gWritePos += BUFFER_HALFSIZE - writeSize;
			}
			gPlayer.play(); //�X�g���[�~���O�Đ����͉����n���Ȃ��Ă�����Ƀ��[�v�Đ��ɂȂ�B�Ȃ��̂��̂����[�v�����鏈���͎��O�ł�邱�ƁB
		}
		///-----�ȉ����t���[��-----


		//�Đ����̏ꏊ�����āA
		int playPos = gPlayer.position();
		//�������݃|�C���^�Ƌ߂Â��Ă�����( �������݈ʒu���Đ��ʒu�����ŁA�������o�b�t�@�T�C�Y�ȉ��Ȃ� )
		if ( ( gWritePos > playPos ) && ( gWritePos - playPos < BUFFER_HALFSIZE ) ){
			cout << "write=" << gWritePos << "\tplay=" << playPos << "\tread=" << gReadPos << endl;
			//�����������݁B
			int restSize = gWaveSize - gReadPos;
			int writeSize = ( restSize >= BUFFER_HALFSIZE ) ? BUFFER_HALFSIZE : restSize;
			if ( gWritePos >= BUFFER_SIZE ){ //�Ō�܂ōs�����̂Ŋ����߂�
				gWritePos = 0;
			}
			bool succeeded = gPlayer.write( gWritePos, gFile.data() + 44 + gReadPos, writeSize );
			if ( !succeeded ){
				cout << "WRITING FAILED!" << endl;
			}
			gReadPos += writeSize;
			gWritePos += writeSize;
			//����Ȃ��f�[�^��0�𖄂߂�
			if ( writeSize < BUFFER_HALFSIZE ){
				gPlayer.fillSilence( gWritePos, BUFFER_HALFSIZE - writeSize );
				gWritePos += BUFFER_HALFSIZE - writeSize;
			}
		}
		if ( isEndRequested() ){
			gFile.release();
			gPlayer.release();
		}
	}
}
