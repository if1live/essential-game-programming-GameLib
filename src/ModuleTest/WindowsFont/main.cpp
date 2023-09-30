#include "GameLib/Framework.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Matrix34.h"
#include "GameLib/Math/Matrix44.h"
#include "GameLib/Math/Functions.h"
#include "GameLib/Scene/StringRenderer.h"
#include "GameLib/Scene/Font.h"
#include <sstream>
using namespace std;
using namespace GameLib::Math;
using namespace GameLib::Graphics;
using namespace GameLib::Scene;

int gCount;
Font gFont0;
Font gFont1;
StringRenderer gStringRenderer;

namespace GameLib{
	void Framework::configure( Configuration* c ){
		c->enableVSync( true );
	}
	void Framework::update(){
		if ( gCount == 0 ){
			//Windows�t�H���g����(�A���`�G�C���A�X�t��)
			gFont0 = Font::create( "�l�r �S�V�b�N", 32, false, false, false, true ); //MS�͑S�p�A���̃X�y�[�X�͔��p�B�C�����悤�B
			//Windows�t�H���g����(�A���`�G�C���A�X�t��)
			gFont1 = Font::create( "�l�r ����", 32, false, false, false, true ); //MS�͑S�p�A���̃X�y�[�X�͔��p�B�C�����悤�B
			//�����`��N���X(�������Aadd�̌Ăяo����)
			gStringRenderer = StringRenderer::create( 2048, 128 );
		}
		//�`�悵�Ă݂�B
		ostringstream oss;
		oss << "�t���[�����[�g: " << frameRate();
		//true�͎����܂�Ԃ��@�\�Bfalse�ɂ���ΐ܂�Ԃ��Ȃ�
		gStringRenderer.setFont( gFont0 );
		gStringRenderer.add( gCount % 640, 0, oss.str().c_str(), 0xffffffff, true );
		oss.str( "" );
		oss << "�@�ؗ��i�����߁j���i����́j�����Γ��i���₫�ǂ��j�A���ɂ͂킴�ƐԊ~��p�Ђ�����i����łӁj���̒��Δ��ɑ΂ЂĘb���G�i�������j���Ȃ��B��l�A�����͗҂������ɍ��苏��O�\�O��̏��A�j�̂₤�ɗ��h�Ȕ��������i���j�|�Ђ�������鍭�̐ƁA�������o�ނׂ��J��̎R�̐F���ƁU�߂Đ��i�݂ǂ�j�̂Ј�g���ُ������A�@�؂�ƒʂ��K�L���R�Ə��A��Д�������^�_�ƍ��i�ނ��j���ہi�܂�j�߂Ĉ��􎆂�������ЂɈ�{�ρi���ۂ񂴂��j�ł����Ɨ��߂��h�����F�C���̗l�͂���ǁA";
		//true�͎����܂�Ԃ��@�\�Bfalse�ɂ���ΐ܂�Ԃ��Ȃ�
		gStringRenderer.setFont( gFont1 );
		gStringRenderer.add( gCount % 640, 100, oss.str().c_str(), 0xffffffff, true );

		gStringRenderer.draw();
		++gCount;

		if ( isEndRequested() ){
			gStringRenderer.release(); //�O���[�o���Ȃ̂ŊJ���������K�v
			gFont0.release();
			gFont1.release();
		}
	}
}
