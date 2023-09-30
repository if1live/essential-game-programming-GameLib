#ifndef INCLUDED_GAMELIB_FILEIO_MANAGER_H
#define INCLUDED_GAMELIB_FILEIO_MANAGER_H

#include <string>

namespace GameLib{
namespace FileIO{

class Manager{
public:
	//�A�[�J�C�u�̎g����
	enum AccessMode{
		MODE_ARCHIVE_FIRST, //�A�[�J�C�u�D��B
		MODE_DIRECT_FIRST, //���ڃA�N�Z�X�D��B
		MODE_ARCHIVE_ONLY, //�A�[�J�C�u�̂݁B�z�鎞�ɂ͂���ɂ��Ă��������������B
	};
	///�C���X�^���X�𓾂�B�������A�P��Manager().foo()�Ə����Ă��ǂ��B
	static Manager instance();
	///�n���h�����󂯎�炸�ɏ������݂��s���B�������s�̕ʂ͂킩��Ȃ��B
	void write( const char* filename, const char* data, int size );
	///���[�h���Ă�����̂����X�g�ɂ��ēf���o���B�f�o�O�p�B�J���R��𒲂ׂ悤�B
	std::string createListString() const;
	///�e�ʌ��E�̎w��Bwarning�𒴂���ƌx�����Aerror�𒴂���ƃG���[�ɂ���B�W���l�͗���2GB
	void setLimit( int warning, int error );
	///�G���[���N���������Ɏ~�܂邩�ǂ����B�W����true�B�c�[�����~�܂��č���Ȃ�false������B
	void enableHaltOnError( bool );

	//�ȉ����[�U�͈ӎ����Ȃ�
	static void create( 
		const char** archiveNames = 0, 
		int archiveNumber = 0, 
		AccessMode = MODE_DIRECT_FIRST );
	static void destroy();
};

} //namespace Sound
} //namespace GameLib

#endif
