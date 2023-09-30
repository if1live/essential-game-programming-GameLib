#include "conversion.h"
#include "GameLib/GameLib.h"
#include "GameLib/PseudoXml/Document.h"
#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"
#include "GameLib/Math/Matrix34.h"
#include "GameLib/Math/Vector4.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/Base/RefString.h"
#include "GameLib/Base/Vector.h"
#include "GameLib/Base/Tank.h"
#include "GameLib/Base/OStringStream.h"
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

using namespace GameLib;
using namespace GameLib::PseudoXml;
using namespace GameLib::Math;
using namespace std;
//���������ʖ��͊��}����Ȃ����A�g�p�p�x����������̂Ŏd���Ȃ�
typedef Element E;
typedef Attribute A;
typedef ConstElement CE;
typedef ConstAttribute CA;
//�N���X���X�g
class XFile;
class Frame;
class FrameTransformMatrix;
class Mesh;
class MeshNormals;
class MeshMaterialList;

//�f�[�^�^
//typedef Vector< int > Face;
struct Face{
	int& operator[]( int i ){ return mIndices[ i ]; }
	int mIndices[ 3 ];
	int mFaceId;
};
struct Triangle{
	int& operator[]( int i ){ return mIndices[ i ]; }
	int mIndices[ 3 ];
};
struct IndexPair{
	bool operator<( const IndexPair& a ) const {
		if ( mVertex < a.mVertex ){
			return true;
		}else if ( mVertex > a.mVertex ){
			return false;
		}else if ( mNormal < a.mNormal ){
			return true;
		}else if ( mNormal > a.mNormal ){
			return false;
		}else{
			return ( mPosition < a.mPosition );
		}
	}
	int mPosition;
	int mVertex;
	int mNormal;
};

struct IndexColor{
	int mIndex;
	Vector4 mColor;
};
struct Material{
	Vector4 mFaceColor;
	float mPower;
	Vector3 mSpecularColor;
	Vector3 mEmissiveColor;
	string mTextureFilename;
	string mName;
};
struct Vertex{
	Vertex() : mPosition( 0.f ), mNormal( 0.f ), mUv( 0.f ), mColor( 1.f ){}
	Vector3 mPosition;
	Vector3 mNormal;
	Vector2 mUv;
	Vector4 mColor;
};
struct MaterialIndex{
	bool operator<( const MaterialIndex& a ) const {  //�}�e���A���ԍ��Ń\�[�g���邽�߂�<
		return ( mMaterial < a.mMaterial ); 
	}
	int mMaterial;
	int mFace;
};

//�֗��֐�
void getVectorArray( Array< Vector3 >* dst, CE e );
void getCoord2dArray( Array< Vector2 >* dst, CE e );
void getIndexColorArray( Array< IndexColor >* dst, CE e );
void getMeshFaceArray( Array< Face >* dst, CE e );
void dieIf( bool f );
int getInt( CE e );
float getFloat( CE e );
const char* getString( CE e );
void getName( RefString*, CE e );
void getMaterial( Material*, CE e );
void getMatrix( Matrix34*, CE e );
void getIndexColor( IndexColor*, CE e );

template< class T > void release( Vector< T* >* a ){
	for ( int i = 0; i < a->size(); ++i ){
		SAFE_DELETE( ( *a )[ i ] );
	}
}

//��O�N���X
class DataStructureException{
public:
	DataStructureException(){ ASSERT( false ); }
};

//�R�[�h�{��
class MeshMaterialList{
public:
	//�f�t�H���g�̃_�~�[�����܂��B
	MeshMaterialList( int vn ){
		mIndices.setSize( vn );
		for ( int i = 0; i < vn; ++i ){
			mIndices[ i ].mMaterial = 0;
			mIndices[ i ].mFace = i;
		}
		mMaterials.setSize( 1 );
		mMaterials[ 0 ].mFaceColor.set( 1.f, 1.f, 1.f, 1.f );
		mMaterials[ 0 ].mPower = 1.f;
		mMaterials[ 0 ].mSpecularColor.set( 1.f, 1.f, 1.f );
		mMaterials[ 0 ].mEmissiveColor.set( 1.f, 1.f, 1.f );
		mGroupSizes.setSize( 1 );
		mGroupSizes[ 0 ] = vn;
	}
	MeshMaterialList( CE e ){
		for ( int i = 0; i < e.childNumber(); ++i ){
			CE c = e.child( i );
			if ( RefString( "Material" ) == c.name() ){
				Material m;
				getMaterial( &m, c );
				mMaterials.push( m );
			}else{
				RefString name;
				getName( &name, c );
				if ( name == "faceIndexes" ){ //���_�擾
					dieIf( mIndices.size() > 0 ); //��ڂ͂Ȃ�
					mIndices.setSize( c.childNumber() );
					for ( int j = 0; j < c.childNumber(); ++j ){
						CE index = c.child( j );
						mIndices[ j ].mFace = j;
						mIndices[ j ].mMaterial = getInt( index );
					}
				}
			}
		}
		//�}�e���A���C���f�N�X���}�e���A���ԍ����Ƀ\�[�g
		sort( &mIndices[ 0 ], &mIndices[ 0 ] + mIndices.size() );
		//�O���[�v�̑傫���ƃI�t�Z�b�g���v�Z���ċL�^
		int matN = mMaterials.size();
		mGroupSizes.setSize( matN );
		for ( int i = 0;i < mGroupSizes.size(); ++i ){
			mGroupSizes[ i ] = 0;
		}
		int prev = mIndices[ 0 ].mMaterial;
		int count = 1;
		for ( int i = 1; i < mIndices.size(); ++i ){
			if ( prev != mIndices[ i ].mMaterial ){
				mGroupSizes[ prev ] = count;
				count = 0;
				prev = mIndices[ i ].mMaterial;
			}
			++count;
		}
		mGroupSizes[ prev ] = count;
	}
	Array< MaterialIndex > mIndices;
	Vector< Material > mMaterials;
	Array< int > mGroupSizes;
};

class MeshNormals{
public:
	MeshNormals( CE e ){
		for ( int i = 0; i < e.childNumber(); ++i ){
			CE c = e.child( i );
			RefString name;
			getName( &name, c );
			if ( name == "normals" ){ //�@���擾
				dieIf( mNormals.size() > 0 ); //��͂˂�
				getVectorArray( &mNormals, c );
			}else if ( name == "faceNormals" ){ //�O�p�`�擾
				dieIf( mFaces.size() > 0 ); //��͂˂�
				getMeshFaceArray( &mFaces, c );
			}
		}
	}
	Array< Vector3 > mNormals;
	Array< Face > mFaces;
};

class Mesh{
public:
	Mesh( CE e ) :
	mMeshNormals( 0 ),
	mMeshMaterialList( 0 ){
		for ( int i = 0; i < e.attributeNumber(); ++i ){
			CA a = e.attribute( i );
			if ( RefString( "name" ) == a.name() ){
				mName = a.value();
			}
		}
		for ( int i = 0; i < e.childNumber(); ++i ){
			CE c = e.child( i );
			if ( RefString( "MeshNormals" ) == c.name() ){
				dieIf( mMeshNormals != 0 );
				mMeshNormals = NEW MeshNormals( c );
			}else if ( RefString( "MeshTextureCoords" ) == c.name() ){
				dieIf( mUVs.size() > 0 );
				for ( int j = 0; j < c.childNumber(); ++j ){
					CE uvs = c.child( j );
					RefString name;
					getName( &name, uvs );
					if ( name == "textureCoords" ){ //UV�擾
						getCoord2dArray( &mUVs, uvs );
					}
				}
			}else if ( RefString( "MeshVertexColors" ) == c.name() ){
				dieIf( mColors.size() > 0 );
				for ( int j = 0; j < c.childNumber(); ++j ){
					CE colors = c.child( j );
					RefString name;
					getName( &name, colors );
					if ( name == "vertexColors" ){ //�F�擾
						getIndexColorArray( &mColors, colors );
					}
				}
			}else if ( RefString( "MeshMaterialList" ) == c.name() ){
				dieIf( mMeshMaterialList != 0 );
				mMeshMaterialList = NEW MeshMaterialList( c );
			}else if ( RefString( "VertexDuplicationIndices" ) == c.name() ){
				; //����
			}else if ( RefString( "XSkinMeshHeader" ) == c.name() ){
				; //����
			}else if ( RefString( "SkinWeights" ) == c.name() ){
				; //����
			}else{
				RefString name;
				getName( &name, c );
				if ( name == "vertices" ){ //���_�擾
					dieIf( mVertices.size() > 0 ); //��ڂ͂Ȃ�
					getVectorArray( &mVertices, c );
				}else if ( name == "faces" ){ //�O�p�`�擾
					dieIf( mFaces.size() > 0 ); //��ڂ͂Ȃ�
					getMeshFaceArray( &mFaces, c );
				}else if ( name == "nFaces" ){
					mOldFaceNumber = getInt( c ); //3�p�`�������O�̖ʐ�
				}
			}
		}
		if ( !mMeshMaterialList ){ //�}�e���A���˂��I����͍���̂Ŗ�������
			mMeshMaterialList = NEW MeshMaterialList( mVertices.size() );
		}
	}
	~Mesh(){
		SAFE_DELETE( mMeshNormals );
		SAFE_DELETE( mMeshMaterialList );
	}
	int countElement(){
		int matN = mMeshMaterialList->mMaterials.size(); //�}�e���A���������o�b�`���ł���
		int textureN = 0;
		for ( int i = 0; i < matN; ++i ){
			if ( mMeshMaterialList->mMaterials[ i ].mTextureFilename.size() > 0 ){
				++textureN; //���O�̓t�@�C�������琶������
			}
		}
		return matN + textureN + 1 + 1; //vertexBuffer, IndexBuffer��+2
	}
	int toXml( E e, int uniqueId, multimap< Mesh*, string >* batchNameTable, int* elementPos ){
		OStringStream oss; //�e��ϊ��pstream
		string meshName;
		if ( mName.size() > 0 ){
			meshName = mName;
		}else{
			oss << "batch" << uniqueId;
			++uniqueId;
			oss.get( &meshName );
			oss.clear();
		}

		//���_�ꏏ�����ǖ@���Ⴄ�A�݂����Ȃ̂�����̂ŁA���X�ʓ|�Ȏ菇�𓥂�Œ��_�𕡐�����B
		//1.�C���f�N�X�z���p��
		int newIn = mFaces.size() * 3; //���C���f�N�X��
		Array< IndexPair > indexPairs( newIn );
		//���_���Ɩ@�����̃C���f�N�X������������ŉ��
		for ( int i = 0; i < mFaces.size(); ++i ){
			for ( int j = 0; j < 3; ++j ){
				int t = i * 3 + j;
				IndexPair& ip = indexPairs[ t ];
				ip.mVertex = mFaces[ i ][ j ];
				ip.mNormal = ( mMeshNormals ) ? mMeshNormals->mFaces[ i ][ j ] : -1;
				ip.mPosition = t;
			}
		}
		//���_�J���[�̑Ή��\���ɍ���Ă��܂��B
		int oldVn = mVertices.size();
		Array< int > oldVertexToColors( oldVn, -1 );
		int colorN = mColors.size();
		for ( int i = 0; i < colorN; ++i ){
			oldVertexToColors[ mColors[ i ].mIndex ] = i;
		}
		//�\�[�g�����܂�
		sort( &indexPairs[ 0 ], &indexPairs[ 0 ] + newIn );
		//�V�C���f�N�X->�V���_�ԍ��e�[�u��
		Array< int > newIndexToNewVertices( newIn ); 
		Array< Vertex > newVertices( newIn ); //���_�z��͉ߏ�ɗp�ӂ��A���resize����
		int prevV = -1;
		int prevN = -1;
		int newVertexPos = -1;
		for ( int i = 0; i < newIn; ++i ){
			const IndexPair& ip = indexPairs[ i ];
			if ( ( prevV != ip.mVertex ) || ( prevN != ip.mNormal ) ){
				++newVertexPos;
				prevV = ip.mVertex;
				prevN = ip.mNormal;
				//���_�i�[���܂��B
				Vertex& v = newVertices[ newVertexPos ];
				v.mPosition = mVertices[ ip.mVertex ];
				if ( mMeshNormals ){
					v.mNormal = mMeshNormals->mNormals[ ip.mNormal ];
				}
				if ( mUVs.size() > ip.mVertex ){
					v.mUv = mUVs[ ip.mVertex ]; //�ʒu�ƓY�����͓���
				}
				int colorIndex = oldVertexToColors[ ip.mVertex ];
				if ( colorIndex != -1 ){
					v.mColor = mColors[ colorIndex ].mColor; //���_�ԍ�����F�ԍ��ւƕϊ����K�v
				}
			}
			newIndexToNewVertices[ ip.mPosition ] = newVertexPos;
		}
		int newVn = newVertexPos + 1;
		newVertices.setSize( newVn );

		//.x���̖ʔԍ����獡��O�p�`�������ʔԍ��ւ̕ϊ��e�[�u���𐶐�����B
		int oldFaceN = mOldFaceNumber;
		int newFaceN = mFaces.size();
		//���̖ʂ̐��������z���p��
		Array< int > oldToNewFaceCounts( oldFaceN, 0 ); //���̖ʂ̐��͂���
		for ( int i = 0; i < newFaceN; ++i ){
			++oldToNewFaceCounts[ mFaces[ i ].mFaceId ];
		}
		//�I�t�Z�b�g�ϊ�
		int offset = 0;
		Array< int > oldToNewFaceOffsets( oldFaceN ); //���̖ʂ̐��͂���
		for ( int i = 0; i < oldFaceN; ++i ){
			oldToNewFaceOffsets[ i ] = offset;
			offset += oldToNewFaceCounts[ i ];
			oldToNewFaceCounts[ i ] = 0; //�ʒu�Ɏg���̂ŏ�����
		}
		//�z��ɂԂ�����ł܂��
		Array< int > oldToNewFace( newFaceN );
		for ( int i = 0; i < newFaceN; ++i ){
			int t = mFaces[ i ].mFaceId;
			int o = oldToNewFaceOffsets[ t ];
			oldToNewFace[ o + oldToNewFaceCounts[ t ] ] = i;
			++oldToNewFaceCounts[ t ];
		}
		//����Ŗʔԍ��ϊ��e�[�u�����o����

		//�C���f�N�X�o�b�t�@�𐶐�����
		int matN = mMeshMaterialList->mMaterials.size(); //�}�e���A���������o�b�`���ł���
		Array< Tank< Triangle > > ib( matN ); //�g���������̂Œ���Tank

		offset = 0;
		int ibSizeTotal = 0;
		for ( int i = 0; i < matN; ++i ){
			int n = mMeshMaterialList->mGroupSizes[ i ];
			int start = offset;
			offset += n;
			for ( int j = 0; j < n; ++j ){
				int oldFaceId = mMeshMaterialList->mIndices[ start + j ].mFace;
				int newFaceIdOffset = oldToNewFaceOffsets[ oldFaceId ];
				int newFaceIdCount = oldToNewFaceCounts[ oldFaceId ];
				for ( int k = 0; k < newFaceIdCount; ++k ){
					Triangle tri;
					for ( int l = 0; l < 3; ++l ){
						tri[ l ] = newIndexToNewVertices[ ( k + newFaceIdOffset ) * 3 + l ];
					}
					ib[ i ].add( tri );
					++ibSizeTotal;
				}
			}
		}
		//�o����̐���������
		//��ӂȖ��O������
		int textureN = 0;
		int batchN = 0;
		for ( int i = 0; i < matN; ++i ){
			//�o�b�`
			if ( ib[ i ].size() > 0 ){ //�g���Ă�O�p�`���Ȃ���Ζ��p
				++batchN;
				//�e�N�X�`���B�o�b�`���Ȃ��Ȃ��͂肢��Ȃ��B
				if ( mMeshMaterialList->mMaterials[ i ].mTextureFilename.size() > 0 ){
					++textureN; //���O�̓t�@�C�������琶������
				}
			}
		}
		//���_�o�b�t�@
		Element vbE = e.child( *elementPos );
		++*elementPos;
		vbE.setName( "VertexBuffer" );
		vbE.setChildNumber( newVn );
		vbE.setAttributeNumber( 1 );
		
		string vbName;
		oss << meshName << "_vb";
		oss.get( &vbName );
		oss.clear();
		vbE.attribute( 0 ).set( "name", vbName.c_str() );
		for ( int i = 0; i < newVn; ++i ){
			Element vE = vbE.child( i );
			vE.setName( "Vertex" );
			//�S���_�o�����̂��ᖡ����B
			const Vertex& v = newVertices[ i ];
			//�@��
			bool writeNormal = ( ( v.mNormal.x != 0.f ) || ( v.mNormal.y != 0.f ) || ( v.mNormal.z != 0.f ) );
			bool writeUv = ( ( v.mUv.x != 0.f ) || ( v.mUv.y != 0.f ) );
			bool writeColor = ( ( v.mColor.x != 1.f ) || ( v.mColor.y != 1.f ) || ( v.mColor.z != 1.f ) || ( v.mColor.w != 1.f ) );
			int attrN = 1;
			attrN += ( writeNormal ) ? 1 : 0;
			attrN += ( writeUv ) ? 1 : 0;
			attrN += ( writeColor ) ? 1 : 0;
			vE.setAttributeNumber( attrN );
			int aPos = 0;
			vE.attribute( aPos ).set( "position", &v.mPosition.x, 3 );
			++aPos;
			if ( writeNormal ){
				vE.attribute( aPos ).set( "normal", &v.mNormal.x, 3 );
				++aPos;
			}
			if ( writeUv ){
				vE.attribute( aPos ).set( "uv", &v.mUv.x, 2 );
				++aPos;
			}
			if ( writeColor ){
				vE.attribute( aPos ).set( "color", &v.mColor.x, 4 );
				++aPos;
			}
		}
		//�C���f�N�X�o�b�t�@
		Element ibE = e.child( *elementPos );
		++*elementPos;
		ibE.setName( "IndexBuffer" );
		ibE.setChildNumber( ibSizeTotal );
		ibE.setAttributeNumber( 1 );
		string ibName;
		oss << meshName << "_ib";
		oss.get( &ibName );
		oss.clear();
		ibE.attribute( 0 ).set( "name", ibName.c_str() );
		int t = 0;
		for ( int i = 0; i < matN; ++i ){
			while ( !ib[ i ].isEnd() ){
				Element iE = ibE.child( t );
				iE.setName( "Triangle" );
				++t;
				iE.setAttributeNumber( 1 );
				iE.attribute( 0 ).set( "indices", ib[ i ].get()->mIndices, 3 );
				ib[ i ].toNext();
			}
		}
		//�o�b�`�s���܂�
		t = 0;
		for ( int i = 0; i < matN; ++i ){
			if ( ib[ i ].size() == 0 ){ //����O�p�`���Ȃ��̂ł��̃o�b�`�͏o���Ȃ��B
				continue;
			}
			Element bE = e.child( *elementPos );
			++*elementPos;
			bE.setName( "Batch" );

			const Material& mat = mMeshMaterialList->mMaterials[ i ];

			//�o�b�`������
			string batchName;
			oss << meshName << '_';
			//�}�e���A�������O�������Ă���΂������������
			if ( mat.mName.size() > 0 ){
				oss << mat.mName;
			}else{ //�Ȃ���ΓY�����ł�����Ƃ�
				oss << i;
			}
			oss.get( &batchName );
			oss.clear();
			//��̓e�N�X�`��
			const string& texFilename = mat.mTextureFilename;
			if ( texFilename.size() > 0 ){ //name,vb,ib,ibOffset,triangleNumber,diffuseColor,specularColor,phongExponent,texture
				//�e�N�X�`���s���܂�
				Element tE = e.child( *elementPos );
				++*elementPos;
				tE.setName( "Texture" );
				tE.setAttributeNumber( 2 );
				//���O�̓t�@�C��������g���q�ƃp�X�����������̂��Z�b�g
				RefString texName( texFilename.c_str() );
				int periodPos = texName.rfind( '.' );
				int slashPos = texName.rfind( '/' );
				int backSlashPos = texName.rfind( '\\' );
				int last = ( periodPos == -1 ) ? ( texName.size() - 1 ) : ( periodPos - 1 );
				int first = ( slashPos > backSlashPos ) ? ( slashPos + 1 ) : ( backSlashPos + 1 );
				if ( first == -1 ){
					first = 0;
				}
				texName.clamp( first, last );
				tE.attribute( 0 ).set( "name", sizeof( "name" ), texName.get(), texName.size() );
				tE.attribute( 1 ).set( "filename", texFilename.c_str() );
				//�o�b�`�̃e�N�X�`���Q�Ɩ����Z�b�g
				bE.setAttributeNumber( 11 );
				bE.attribute( 10 ).set( "texture", sizeof( "texture" ), texName.get(), texName.size() );
			}else{
				bE.setAttributeNumber( 10 );
			}
			batchNameTable->insert( make_pair( this, batchName ) ); //�o�b�`���O�\�ɂԂ�����
			bE.attribute( 0 ).set( "name", batchName.c_str() );
			bE.attribute( 1 ).set( "vertexBuffer", vbName.c_str() );
			bE.attribute( 2 ).set( "indexBuffer", ibName.c_str() );
			bE.attribute( 3 ).set( "bufferOffset", t * 3 ); //�O�p�`�Ȃ̂Ł~3
			bE.attribute( 4 ).set( "primitiveNumber", ib[ i ].size() );
			bE.attribute( 5 ).set( "diffuseColor", &mat.mFaceColor.x, 3 );
			bE.attribute( 6 ).set( "transparency", mat.mFaceColor.w );
			bE.attribute( 7 ).set( "specularColor", &mat.mSpecularColor.x, 3 );
			bE.attribute( 8 ).set( "specularSharpness", mat.mPower );
			bE.attribute( 9 ).set( "emissionColor", &mat.mEmissiveColor.x, 3 );

			t += ib[ i ].size();
		}
		return uniqueId;
	}
	Array< Vector3 > mVertices;
	Array< Face > mFaces;
	int mOldFaceNumber;
	MeshNormals* mMeshNormals;
	MeshMaterialList* mMeshMaterialList;
	Array< Vector2 > mUVs;
	Array< IndexColor > mColors;
	string mName;
};

class Frame{
public:
	Frame( CE e ){
		mMatrix.setIdentity();
		//���O�擾
		for ( int i = 0; i < e.attributeNumber(); ++i ){
			CA a = e.attribute( i );
			if ( RefString( "name" ) == a.name() ){
				mName = a.value();
			}
		}
		//�q���ɂ���
		for ( int i = 0; i < e.childNumber(); ++i ){
			CE c = e.child( i );
			if ( RefString( "FrameTransformMatrix" ) == c.name() ){
				dieIf( c.childNumber() == 0 );
				getMatrix( &mMatrix, c.child( 0 ) ); //Matrix4x4��n��
			}else if ( RefString( "Mesh" ) == c.name() ){
				mMeshes.push( NEW Mesh( c ) );
			}else if ( RefString( "Frame" ) == c.name() ){
				mFrames.push( NEW Frame( c ) );
			}
		}
	}
	~Frame(){
		release( &mFrames );
		release( &mMeshes );
	}
	void collectMesh( Vector< Mesh* >* meshes ){
		for ( int i = 0; i < mMeshes.size(); ++i ){
			meshes->push( mMeshes[ i ] );
		}
		for ( int i = 0; i < mFrames.size(); ++i ){
			mFrames[ i ]->collectMesh( meshes );
		}
	}
	int toXml( E e, int nodeId, multimap< Mesh*, string >* batchNameTable ){
		typedef multimap< Mesh*, string > Table;
		typedef Table::iterator It;
		//�����������b�V���ɑ�����o�b�`��������Ă���
		vector< string > batchNames;
		for ( int i = 0; i < mMeshes.size(); ++i ){
			pair< It, It > range = batchNameTable->equal_range( mMeshes[ i ] );
			for ( It j = range.first; j != range.second; ++j ){
				batchNames.push_back( j->second );
			}
		}
		//�܂������̏����Z�b�g
		e.setName( "Node" );
		Matrix34 identity;
		identity.setIdentity();
		bool writeMatrix = ( identity != mMatrix );
		int childNumber = ( writeMatrix ) ? 2 : 1;
		int batchN = static_cast< int >( batchNames.size() );
		childNumber += batchN; //�o�b�`���𑫂�

		e.setAttributeNumber( childNumber ); //name�ƁA�����transform

		int aPos = 0;
		if ( mName.size() > 0 ){
			e.attribute( aPos ).set( "name", mName.c_str() );
			++aPos;
		}else{
			e.attribute( aPos ).set( "name", nodeId );
			++aPos;
			++nodeId;
		}
		if ( writeMatrix ){
			e.attribute( aPos ).set( "transform", &mMatrix.m00, 12 );
			++aPos;
		}
		for ( int i = 0; i < batchN; ++i ){
			e.attribute( aPos ).set( "batch", batchNames[ i ].c_str() );
			++aPos;
		}
		//�q�̏���
		e.setChildNumber( mFrames.size() ) ;
		for ( int i = 0; i < mFrames.size(); ++i ){
			E c = e.child( i );
			nodeId = mFrames[ i ]->toXml( c, nodeId, batchNameTable );
		}
		return nodeId;
	}
	Matrix34 mMatrix;
	Vector< Mesh* > mMeshes;
	Vector< Frame* > mFrames;
	string mName;
};


class XFile{
public:
	XFile( CE e ){
		for ( int i = 0; i < e.childNumber(); ++i ){
			CE c = e.child( i );
			if ( RefString( "Frame" ) == c.name() ){
				mFrames.push( NEW Frame( c ) );
			}else if ( RefString( "Mesh" ) == c.name() ){
				mMeshes.push( NEW Mesh( c ) );
			}
		}
	}
	Document toXml() const {
		Vector< Mesh* > meshes;
		for ( int i = 0; i < mMeshes.size(); ++i ){
			meshes.push( mMeshes[ i ] );
		}
		for ( int i = 0; i < mFrames.size(); ++i ){
			mFrames[ i ]->collectMesh( &meshes );
		}
		//�h�L�������g�[
		Document d = Document::create();
		Element root = d.root();
		root.setChildNumber( 1 );
		Element graphicsContainer = root.child( 0 );
		graphicsContainer.setName( "Container" );
		//���b�V���|�C���^->�o�b�`����multimap�����
		multimap< Mesh*, string > batchNameTable;
		//�G�������g�m��
		int elementN = 0;
		for ( int i = 0; i < meshes.size(); ++i ){
			elementN += meshes[ i ]->countElement();
		}
		elementN += mFrames.size();
		graphicsContainer.setChildNumber( elementN );

		//���b�V����XML�ϊ�
		int elementPos = 0;
		int id = 0;
		for ( int i = 0; i < meshes.size(); ++i ){
			id = meshes[ i ]->toXml( graphicsContainer, id, &batchNameTable, &elementPos );
		}
		//���B�c���[�o���܂��B
		//������Frame��Tree�ɂ���B
		for ( int i = 0; i < mFrames.size(); ++i ){
			E tree = graphicsContainer.child( elementPos );
			++elementPos;
			tree.setName( "Tree" );
			tree.setAttributeNumber( 1 );
			tree.attribute( 0 ).set( "name", mFrames[ i ]->mName.c_str() );
			tree.setChildNumber( 1 ); //���[�g�m�[�h
			E rootNode = tree.child( 0 );
			mFrames[ i ]->toXml( rootNode, 0, &batchNameTable );
		}
		
		//TODO:
		return d;
	}
	~XFile(){
		release( &mFrames );
		release( &mMeshes );
	}
	Vector< Frame* > mFrames;
	Vector< Mesh* > mMeshes;
};

//�֗��֐�
void getVectorArray( Array< Vector3 >* dst, CE e ){
	dst->setSize( e.childNumber() );
	for ( int i = 0; i < e.childNumber(); ++i ){
		CE v = e.child( i ); //Vector
		Vector3& newV = ( *dst )[ i ];

		bool xOk, yOk, zOk;
		xOk = yOk = zOk = false;
		for ( int j = 0; j < v.childNumber(); ++j ){
			CE f = v.child( j );
			RefString name;
			getName( &name, f );
			float value = getFloat( f ); 
			if ( name == "x" ){
				newV.x = value;
				xOk = true;
			}else if ( name == "y" ){
				newV.y = value;
				yOk = true;
			}else if ( name == "z" ){
				newV.z = value;
				zOk = true;
			}
		}
		dieIf( !xOk || !yOk || !zOk );
	}
}

void getCoord2dArray( Array< Vector2 >* dst, CE e ){
	dst->setSize( e.childNumber() );
	for ( int i = 0; i < e.childNumber(); ++i ){
		CE v = e.child( i ); //Vector

		Vector2& newV = ( *dst )[ i ];
		bool uOk, vOk;
		uOk = vOk = false;
		for ( int j = 0; j < v.childNumber(); ++j ){
			CE f = v.child( j );
			RefString name;
			getName( &name, f );
			float value = getFloat( f );
			if ( name == "u" ){
				newV.x = value;
				uOk = true;
			}else if ( name == "v" ){
				newV.y = value;
				vOk = true;
			}
		}
		dieIf( !uOk || !vOk );
	}
}

void getMeshFaceArray( Array< Face >* dst, CE e ){
	//���^���N�ɓ���܂�
	Tank< Face > tank;
	Face face; //�e���|����
	for ( int i = 0; i < e.childNumber(); ++i ){
		CE meshFaceE = e.child( i );
		for ( int j = 0; j < meshFaceE.childNumber(); ++j ){
			CE indicesE = meshFaceE.child( j );
			if ( RefString( "Array" ) == indicesE.name() ){ //�z��Ȃ炱�����B
				face.mFaceId = i;
				face.mIndices[ 0 ] = getInt( indicesE.child( 0 ) );
				face.mIndices[ 1 ] = getInt( indicesE.child( 1 ) );
				for ( int k = 2; k < indicesE.childNumber(); ++k ){
					face.mIndices[ 2 ] = getInt( indicesE.child( k ) );
					tank.add( face );
					face.mIndices[ 1 ] = face.mIndices[ 2 ]; //���炵�Ď���
				}
			}
		}
	}
	//�z��փR�s�[
	if ( tank.size() > 0 ){
		dst->setSize( tank.size() );
		tank.copyTo( &( *dst )[ 0 ] );
	}
}

void getIndexColorArray( Array< IndexColor >* dst, CE e ){
	dst->setSize( e.childNumber() );
	for ( int i = 0; i < e.childNumber(); ++i ){
		CE t = e.child( i );
		getIndexColor( &( ( *dst )[ i ] ), t );
	}
}

void getColor4( Vector4* dst, CE e ){
	bool rOk, gOk, bOk, aOk;
	rOk = gOk = bOk = aOk = false;
	for ( int i = 0; i < e.childNumber(); ++i ){
		CE c = e.child( i );
		RefString name;
		getName( &name, c );
		float value = getFloat( c );

		if ( name == "red" ){
			dst->x = value;
			rOk = true;
		}else if ( name == "green" ){
			dst->y = value;
			gOk = true;
		}else if ( name == "blue" ){
			dst->z = value;
			bOk = true;
		}else if ( name == "alpha" ){
			dst->w = value;
			aOk = true;
		}
	}
	dieIf( !rOk || !gOk || !bOk || !aOk );
}

void getColor3( Vector3* dst, CE e ){
	bool rOk, gOk, bOk;
	rOk = gOk = bOk = false;
	for ( int i = 0; i < e.childNumber(); ++i ){
		CE c = e.child( i );
		RefString name;
		getName( &name, c );
		float value = getFloat( c );

		if ( name == "red" ){
			dst->x = value;
			rOk = true;
		}else if ( name == "green" ){
			dst->y = value;
			gOk = true;
		}else if ( name == "blue" ){
			dst->z = value;
			bOk = true;
		}
	}
	dieIf( !rOk || !gOk || !bOk );
}


void getIndexColor( IndexColor* dst, CE e ){
	bool indexOk;
	indexOk = false;
	for ( int i = 0; i < e.childNumber(); ++i ){
		CE c = e.child( i );
		if ( RefString( "DWORD" ) == c.name() ){
			dst->mIndex = getInt( c );
			indexOk = true;
		}else{
			getColor4( &dst->mColor, c );
		}
	}
	dieIf( !indexOk );
}

void getMaterial( Material* dst, CE e ){
	//���O�擾
	for ( int i = 0; i < e.attributeNumber(); ++i ){
		CA a = e.attribute( i );
		if ( RefString( "name" ) == a.name() ){
			dst->mName = a.value();
		}
	}
	//�܂�����ĂȂ��L��
	dst->mFaceColor.x = -1e10f;
	dst->mFaceColor.y = -1e10f;
	dst->mFaceColor.z = -1e10f;
	dst->mFaceColor.w = -1e10f;
	dst->mPower = -1e10f;
	dst->mSpecularColor.x = -1e10f;
	dst->mSpecularColor.y = -1e10f;
	dst->mSpecularColor.z = -1e10f;
	dst->mEmissiveColor.x = -1e10f;
	dst->mEmissiveColor.y = -1e10f;
	dst->mEmissiveColor.z = -1e10f;
	for ( int i = 0; i < e.childNumber(); ++i ){
		CE c = e.child( i );
		if ( RefString( "TextureFilename" ) == c.name() ){
			dieIf( c.childNumber() < 1 );
			dst->mTextureFilename = getString( c.child( 0 ) );
		}else{
			RefString name;
			getName( &name, c );
			if ( name == "faceColor" ){
				getColor4( &dst->mFaceColor, c );
			}else if ( name == "power" ){
				dst->mPower = getFloat( c );
			}else if ( name == "specularColor" ){
				getColor3( &dst->mSpecularColor, c );
			}else if ( name == "emissiveColor" ){
				getColor3( &dst->mEmissiveColor, c );
			}
		}
	}
	//�s���l�Ή�
	if ( dst->mPower <= 0.f ){ //0�ȉ��͕s���ł��B1�ɂ��āA�X�y�L�����F��0�ɂ��܂��B
		dst->mPower = 1.f;
		dst->mSpecularColor.set( 0.f, 0.f, 0.f );
	}
	dieIf(
		( dst->mFaceColor.x == -1e10f ) ||
		( dst->mFaceColor.y == -1e10f ) ||
		( dst->mFaceColor.z == -1e10f ) ||
		( dst->mFaceColor.w == -1e10f ) ||
		( dst->mPower == -1e10f ) ||
		( dst->mSpecularColor.x == -1e10f ) ||
		( dst->mSpecularColor.y == -1e10f ) ||
		( dst->mSpecularColor.z == -1e10f ) ||
		( dst->mEmissiveColor.x == -1e10f ) ||
		( dst->mEmissiveColor.y == -1e10f ) ||
		( dst->mEmissiveColor.z == -1e10f ) );
}

//��O�����܂��Bassert�ƈ����true�Ȃ瓊����
void dieIf( bool f ){
	if ( f ){
		throw DataStructureException();
	}
}

//value�l��int�Ŏ���Ă���
int getInt( CE e ){
	int r = 0;
	bool found = false;
	for ( int i = 0; i < e.attributeNumber(); ++i ){
		CA a = e.attribute( i );
		if ( RefString( "value" ) == a.name() ){
			r = a.getIntValue();
			found = true;
		}
	}
	dieIf( !found );
	return r;
}

//value�l��float�Ŏ���Ă���
float getFloat( CE e ){
	float r = 0;
	bool found = false;
	for ( int i = 0; i < e.attributeNumber(); ++i ){
		CA a = e.attribute( i );
		if ( RefString( "value" ) == a.name() ){
			r = a.getFloatValue();
			found = true;
		}
	}
	dieIf( !found );
	return r;
}

const char* getString( CE e ){
	const char* r = 0;
	bool found = false;
	for ( int i = 0; i < e.attributeNumber(); ++i ){
		CA a = e.attribute( i );
		if ( RefString( "value" ) == a.name() ){
			r = a.value();
			found = true;
		}
	}
	dieIf( !found );
	return r;
}

//name�l���擾
void getName( RefString* dst, CE e ){
	bool found = false;
	for ( int i = 0; i < e.attributeNumber(); ++i ){
		CA a = e.attribute( i );
		if ( RefString( "name" ) == a.name() ){
			*dst = a.value();
			found = true;
		}
	}
	dieIf( !found );
}

void getMatrix( Matrix34* dst, CE e ){
	dieIf( e.childNumber() == 0 ); 
	e = e.child( 0 ); //Array���o�Ă���

	dieIf( e.childNumber() < 16 );
	for ( int i = 0; i < 4; ++i ){ //�]�u���Ċi�[
		for ( int j = 0; j < 3; ++j ){
			CE c = e.child( i * 4 + j ); //j==3��0�Ȃ̂ŃX�L�b�v
			float t = getFloat( c );
			if ( -t == 0.f ){ //-0��+0�ɂ���B
				t = 0.f;
			}
			( *dst )( j, i ) = t; 
		}
	}
	//�O�̂��߃`�F�b�N
	dieIf( getFloat( e.child( 3 ) ) != 0.f );
	dieIf( getFloat( e.child( 7 ) ) != 0.f );
	dieIf( getFloat( e.child( 11 ) ) != 0.f );
	dieIf( getFloat( e.child( 15 ) ) != 1.f );
}

//�����̊֐�
//#include <fstream>
Document convert( ConstDocument src ){
#if 0
	string s;
	src.convertToString( &s );
	setlocale( LC_ALL, "" ); //���ꂪ�Ȃ��Ɠ��{��t�@�C�������󂯕t���Ȃ�
	ofstream out( "converted.txt", ofstream::binary );
	out.write( s.c_str(), s.size() );
	out.close();
#endif
	XFile xFile( src.child( 0 ) ); //XFile
	return xFile.toXml();
}
