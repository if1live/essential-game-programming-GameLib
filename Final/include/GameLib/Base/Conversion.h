#ifndef INCLUDED_GAMELIB_CONVERSION_H
#define INCLUDED_GAMELIB_CONVERSION_H


//������ւ̕ϊ�
//char�Ȃ�ő�4�����Ashort�Ȃ�6�����Aint�Ȃ�11�����Afloat�Ȃ�16�Adouble�Ȃ�30�O��(�\��)��
//�T�C�Y�̃o�b�t�@��n���K�v������B
//�߂�l�͏������݃T�C�Y���B�͈͊O�ɂ��ӂ�Ă��Ȃ����`�F�b�N���悤�B
namespace GameLib{

int toString10( char* out, char ); //�����łȂ��P�Ȃ鐮���Ƃ��Ĉ���
int toString10( char* out, unsigned char );
int toString10( char* out, short );
int toString10( char* out, unsigned short );
int toString10( char* out, int );
int toString10( char* out, unsigned );

int toString16( char* out, char ); //�����łȂ��P�Ȃ鐮���Ƃ��Ĉ���
int toString16( char* out, unsigned char );
int toString16( char* out, short );
int toString16( char* out, unsigned short );
int toString16( char* out, int );
int toString16( char* out, unsigned );

int toString( char* out, float, int precision = 6 );
int toString( char* out, double, int precision = 6 );

} //namespace GameLib

#endif