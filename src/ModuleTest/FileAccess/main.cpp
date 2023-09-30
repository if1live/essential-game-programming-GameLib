#include "GameLib/Framework.h"
#include "GameLib/FileIO/Manager.h"
#include "GameLib/FileIO/InFile.h"
#include "GameLib/FileIO/OutFile.h"
#include "GameLib/Math/Random.h"
using namespace GameLib;
#include <sstream>
using namespace std;

static const int N = 30;
FileIO::InFile gIn[ N ];
FileIO::OutFile gOut[ N ];
int gSize[ N ];
Math::Random gRandom;

namespace GameLib{
	void Framework::configure( Configuration* c ){
		//�A�[�J�C�u�A�N�Z�X�B�����A�[�J�C�u�̃e�X�g
		c->setArchiveNumber( 2 );
		c->setArchiveName( 0, "data1.bin" );
		c->setArchiveName( 1, "data2.bin" ); //���݂��Ă��Ȃ��Ă����܂�Ȃ��B
		c->setLoadMode( LOAD_ARCHIVE_FIRST );
	}
	void Framework::update(){
		if ( !gRandom ){
			gRandom = Math::Random::create();
			setFrameRate( 60 );
		}
		FileIO::Manager m = FileIO::Manager::instance();
		for ( int i = 0; i < N; ++i ){
			if ( gIn[ i ] ){
				if ( gIn[ i ].isFinished() ){
					for ( int j = 0; j < ( i % 10 ); ++j ){
						ASSERT( gIn[ i ].data()[ j ] == '0'+ ( i % 10 ) );
					}
					gIn[ i ].release();
				}
			}else{
				ostringstream oss;
				oss << "data\\" << i << ".txt";
				gIn[ i ] = FileIO::InFile::create( oss.str().c_str() );
				cout << "READ:" << oss.str().c_str() << endl;
			}
			//�o�̓n���h���������Ă���ΏI�����`�F�b�N�B�I����Ă���΍폜
			if ( gOut[ i ] ){
				if ( gOut[ i ].isFinished() ){
					gOut[ i ].release();
				}
			}else{
				ostringstream oss;
				oss << "wrote" << i << ".txt";
				gSize[ i ] = gRandom.getInt( 0, N );
				char* buffer = NEW char[ gSize[ i ] ];
				for ( int j = 0; j < gSize[ i ]; ++j ){
					buffer[ j ] = '0' + static_cast< char >( i );
				}
				gOut[ i ] = FileIO::OutFile::create( oss.str().c_str(), buffer, gSize[ i ] );
				delete[] buffer;
				cout << "WRITE: " << oss.str().c_str() << endl;
			}
		}
		//���܂Ƀn���h���Ȃ���������
		if ( gRandom.getInt( 0, 1000 ) == 0 ){
			int size = gRandom.getInt( 0, N );
			char* buffer = NEW char[ size ];
			for ( int j = 0; j < size; ++j ){
				buffer[ j ] = '0' + static_cast< char >( j );
			}
			m.write( "handleLessWrite.txt", buffer, size );
			cout << "write request without handle." << endl;
			delete[] buffer;
		}
		//�I������(���������[�N�A�j�󓙂𒲂ׂ悤)
		if ( isEndRequested() ){
			for ( int i = 0; i < N; ++i ){
				gIn[ i ].release();
				while ( gOut[ i ] && !gOut[ i ].isFinished() ){
					; //�������ݑ҂�
				}
				gOut[ i ].release();
			}
			gRandom.release();
		}
	}
}
