#include "GameLib/Framework.h"
#include "GameLib/PseudoXml/Document.h"
#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"
using namespace GameLib;

PseudoXml::Document gDocument;
int gCount;


namespace GameLib{
	void Framework::configure( Configuration* ){}
	void Framework::update(){
		setFrameRate( 60 );
		if ( !gDocument ){
			//�t�@�C�����琶��
			gDocument = PseudoXml::Document::create( "data2.txt" );

			//1�������ēf���o���Ă݂�
			using namespace PseudoXml;
			Document doc = Document::create();
			Element root = doc.root();
			//�������Ɉ������
			root.setChildNumber( 1 );
			Element cameraData = root.child( 0 );
			cameraData.setName( "Data" );
			//�f�[�^�������
			cameraData.setChildNumber( 2 );

			Element e;
			//���
			e = cameraData.child( 0 );
			e.setName( "SomeElement0" );
			e.setAttributeNumber( 3 );
			e.attribute( 0 ).set( "name", "value" ); //������l
			e.attribute( 1 ).set( "intValue", 1 ); //int�l
			e.attribute( 2 ).set( "floatValue", 2.32f ); //float�l
			//���
			e = cameraData.child( 1 );
			e.setName( "SomeElement1" );
			e.setAttributeNumber( 2 );
			int ia[ 5 ];
			for ( int i = 0; i < 5; ++i ){
				ia[ i ] = i;
			}
			e.attribute( 0 ).set( "intArray", ia, 5 ); //int�z��
			float fa[ 3 ];
			for ( int i = 0; i < 3; ++i ){
				fa[ i ] = 1.f / static_cast< float >( i + 1 );
			}
			e.attribute( 1 ).set( "floatArray", fa, 3 ); //float�z��
			//cout�ɓf���Ă݂�
			string s;
			doc.convertToString( &s );
			cout << s.c_str() << endl;
			//�t�@�C���ɂ���������
			doc.write( "dataCreated.txt", false );
		}else if ( gDocument.isReady() ){ //�ǂݍ��ݏI��
			//�����񉻂���cout�ɏo���Ă݂�B
			string s;
			gDocument.convertToString( &s );
			cout << s.c_str() << endl;
			//���łɃt�@�C���ɂ��f���Ă݂�B�����̂��o�Ă邩�H
			gDocument.write( "dataOut.txt" );
			gDocument.release(); //�ǂݏI��������A����ˁB
			requestEnd(); //�I���B
		}
	}
}
