#ifndef INCLUDED_TEXTURE_H
#define INCLUDED_TEXTURE_H

#include <string>
using namespace std;

namespace GameLib{
	namespace PseudoXml{
		class Element;
	}
	class Texture;
}

class Texture{
public:
	Texture( GameLib::PseudoXml::Element& );
	Texture( const char* filename );
	~Texture();
	void set() const; //�n�[�h�E�F�A�ɃZ�b�g
	const string* name() const;
private:
	GameLib::Texture* mTexture;
	string mName;
};

#endif
