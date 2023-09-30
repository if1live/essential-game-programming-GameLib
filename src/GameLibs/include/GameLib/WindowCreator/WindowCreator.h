#ifndef INCLUDED_GAMELIB_WINDOWCREATOR_H
#define INCLUDED_GAMELIB_WINDOWCREATOR_H

namespace GameLib{

class WindowCreator{
public:
	class Configuration{
	public:
		void setWidth( int width );
		void setHeight( int height );
		void setTitle( const char* title );
		void forbidFullScreen( bool = true ); //������Ă�ł�����alt-enter�����������
		void enableFullScreen( bool = true );
		void enableDragAndDrop( bool = true );
	};
	WindowCreator();
	//���[�U�����֐�
	void configure( Configuration* );
	void start( void* windowHandle );
	void update();

	const char* title() const;
	int titleLength() const;
	int width() const;
	int height() const;
	bool isFullScreen() const;
	bool isMinimized() const;
	bool isActive() const;
	void enableFullScreen( bool );
	///�R�}���h���C��������擾
	const char* commandLineString() const;
	///�h���b�O�A���h�h���b�v�͗L���ł����H
	bool isDragAndDropEnabled() const;
	///�h���b�O�A���h�h���b�v���擾
	int droppedItemNumber() const;
	///�h���b�O�A���h�h���b�v�擾
	const char* droppedItem( int index ) const;
	///�h���b�O�A���h�h���b�v�N���A
	void clearDroppedItem();
	///�~���b�P�ʎ�����Ԃ�
	unsigned time() const;

	static bool isMainThread();
	static WindowCreator instance();
	void requestEnd(); //�I����v������
	void end(); //�I����ʒm����B
	bool isEndRequested() const; //�I���͗v������Ă܂���
	int getAndResetMouseWheel();
};

} //namespace GameLib

#endif