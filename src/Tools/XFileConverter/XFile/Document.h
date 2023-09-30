#ifndef INCLUDED_XFILE_DOCUMENT_H
#define INCLUDED_XFILE_DOCUMENT_H

#include <string>

namespace XFile{

class Parser;

//XFile�N���X
/*!
XFile����f�[�^�\�����\�z����B�������\�z�����f�[�^�\���ւ͒��ڃA�N�Z�X�͂ł����A
xml�ɂ��Ă��܂��Ă���xml�p�[�T�œǂݎ�邱�ƂɂȂ�B
����́AXFile�𒼐ڂ̃��f���t�H�[�}�b�g�Ƃ��Ďg�킸�A
�P�Ȃ钆�ԃt�H�[�}�b�g�Ƃ������Ă��Ȃ�����ł���B
*/
class Document{
public:
	Document( const char* data, int size );
	~Document();
	bool isValid() const;
	///xml������ɕϊ��B����ȊO�̂��Ƃ͂ł��Ȃ��B
	void convertToString( std::string* ) const;
private:
	Parser* mParser;
};

} //namespace XFile

#endif
