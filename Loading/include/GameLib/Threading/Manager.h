#ifndef INCLUDED_GAMELIB_THREADING_MANAGER_H
#define INCLUDED_GAMELIB_THREADING_MANAGER_H

namespace GameLib{
namespace Threading{

class Manager{
public:
	Manager();
	static Manager instance();

	//�R�A���擾(�_���R�A���Ȃ̂ŁA���̂Ƃ͈Ⴄ�\��������)
	int getCoreNumber() const;

	//�ȉ����[�U���ĂԂ��Ƃ͂Ȃ�
	static void create( int additionalThreadNumber );
	static void destroy();
};

} //namespace Threading
} //namespace GameLib

#endif
