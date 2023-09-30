#ifndef INCLUDED_GAMELIB_LIST_H
#define INCLUDED_GAMELIB_LIST_H

class List{
public:
	explicit List( int capacity );
	~List();
	//�����ǉ��B�ǂ��ɑ���������Ԃ��B
	int addTail( int value );
	//�w��ꏊ���폜
	void remove( int position );
	///�l�擾
	int value( int position ) const;
	//���[���ǂ�����isEnd()�Ŋm�F����
	int next( int position ) const;
	//�擪�擾
	int first() const;
	//position�����[������B
	bool isEnd( int position ) const;
private:
	struct Node{
		int mNext;
		int mPrev;
		int mValue;
	};
	List( const List& ); //�R�s�[�R���X�g���N�^�֎~
	void operator=( const List& ); //����֎~

	Node* mNodes;
	int* mEmptyStack; //����łǂ����󂢂Ă��邩��O(1)�ł킩��
	int mStackTop;
	const int mCapacity; //�R���X�g���N�g�����炢����Ȃ��̂�const
};

//cpp�����̂��ʓ|�Ȃ̂ł����ɏ����Ă��܂��B���x���オ�邵�������낤�B

//�擪�Ɩ��������ʈ�������Ȃ��悤�ɂ�����ƍH�v�����Ă���B
//mNodes[ mCapacity ]�̓_�~�[�m�[�h���B�擪�Ɩ����ɂ��̃_�~�[�����邨�����ł��낢��y�Ȃ̂ł���B
inline List::List( int capacity ) : 
mNodes( 0 ),
mEmptyStack( 0 ),
mCapacity( capacity ){
	mNodes = new Node[ mCapacity + 1 ]; //�Ō�Ƀ_�~�[�m�[�h�B������+1
	mEmptyStack = new int[ mCapacity ];
	for ( int i = 0; i < mCapacity; ++i ){
		mEmptyStack[ i ] = i; //�󂢂Ă�ԍ����i�[�B�ŏ�������S���ȁB
	}
	mStackTop = mCapacity - 1; //�ŏ��Ɏ��o���͖̂���
	//�_�~�[�m�[�h�ݒ�B
	mNodes[ mCapacity ].mNext = mNodes[ mCapacity ].mPrev = mCapacity;
}

inline List::~List(){
	SAFE_DELETE_ARRAY( mNodes );
	SAFE_DELETE_ARRAY( mEmptyStack );
}

inline int List::addTail( int v ){
	//�󂫏ꏊ���擾
	int newPos = mEmptyStack[ mStackTop ];
	--mStackTop;

	mNodes[ newPos ].mValue = v; //�l�����āA
	mNodes[ newPos ].mNext = mCapacity; //���̓_�~�[�m�[�h
	int oldTail = mNodes[ mCapacity ].mPrev; //�O�̖����̓_�~�[�̑O�ɂ���
	mNodes[ newPos ].mPrev = oldTail; //�V���������̑O�ɌÂ�����������

	mNodes[ oldTail ].mNext = newPos; //�Â������̎�������
	mNodes[ mCapacity ].mPrev = newPos; //�_�~�[�̑O������
	return newPos; //�������ꏊ��Ԃ�
}

inline void List::remove( int position ){
	Node* cur = &mNodes[ position ]; //����
	Node* nxt = &mNodes[ cur->mNext ]; //��
	Node* prev = &mNodes[ cur->mPrev ]; //�O
	//�O�̓z�̎����A���ɂ���B
	prev->mNext = cur->mNext;
	//���̓z�̑O���A�O�ɂ���B
	nxt->mPrev = cur->mPrev;

	//�󂫔ԍ����X�g�ɒǉ�
	++mStackTop;
	mEmptyStack[ mStackTop ] = position;
}

inline int List::value( int position ) const {
	return mNodes[ position ].mValue;
}

inline int List::next( int position ) const {
	return mNodes[ position ].mNext;
}

inline int List::first() const {
	//�_�~�[�̎����擪�Ȃ̂���
	return mNodes[ mCapacity ].mNext;
}

inline bool List::isEnd( int position ) const {
	//�Ō�܂ōs����mCapacity�A�܂�_�~�[�ɂȂ�B
	return ( position == mCapacity );
}

#endif

