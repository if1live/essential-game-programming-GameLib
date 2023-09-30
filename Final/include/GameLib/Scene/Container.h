#ifndef INCLUDED_GAMELIB_SCENE_CONTAINER_H
#define INCLUDED_GAMELIB_SCENE_CONTAINER_H

namespace GameLib{

namespace PseudoXml{
	class ConstElement;
};

namespace Scene{

class Batch;
class Model;
class Tree;
class Animation;

///�`��f�[�^������R���e�i�B
/*!
createModel()���āAModel::draw()����΂����B2�s�ŊG���o��B
*/
class Container{
public:
	///�j�Zxml���琶���B�ǉ��f�[�^�p�X�͏ȗ�������exe�̏ꏊ�ɂȂ�
	static Container create( PseudoXml::ConstElement, const char* extraDataPath = 0 );
	///�t�@�C�����璼�ڐ����B�ǉ��f�[�^�p�X���Ȃ���΃t�@�C���Ɠ����ꏊ���g���B
	static Container create( const char* filename, const char* extraDataPath = 0 );
	///�o�b�`�����w�肵�ă��f�������
	Model createModel( const char* batchName ) const;
	///�o�b�`�ԍ����w�肵�ă��f�������
	Model createModel( int batchId ) const;
	///�c���[�����w�肵�ăc���[�����
	Tree createTree( const char* treeName ) const;
	///�c���[�̔ԍ����w�肵�ăc���[�����
	Tree createTree( int treeId ) const;

	///���[�h�I�����m�B�K���Ă��true���Ԃ邱�Ƃ��m�F���Ă���g�����ƁB
	bool isReady();

	Batch batch( const char* batchName ) const;
	Batch batch( int batchIndex ) const;
	Animation animation( const char* animationName ) const;
	Animation animation( int animationIndex ) const;
	//Batch�����킩��B�����Ă���S���̂��̂��o���������ɂ͂����getBatch(int)�𕹗p����Ηǂ��B
	int batchNumber() const;
	//Tree�����킩��B�����Ă���S���̂��̂��o���������ɂ͂����createTree(int)�𕹗p����Ηǂ��B
	int treeNumber() const;
	//�A�j���[�V���������킩��B
	int animationNumber() const;

	//�ȉ����[�U�͈ӎ����Ȃ��Ă���
	Container();
	~Container();
	Container( const Container& );
	operator void*() const;
	Container& operator=( const Container& );
	void release();
	bool operator==( const Container& ) const;
	bool operator!=( const Container& ) const;

	class Impl;
private:
	explicit Container( Impl* );
	Impl* mImpl;
};

} //namespace Scene
} //namespace GameLib

#endif
