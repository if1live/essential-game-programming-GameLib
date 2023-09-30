#ifndef INCLUDED_IMAGE_H
#define INCLUDED_IMAGE_H

class Image{
public:
	Image( const char* filename );
	~Image();
	int width() const;
	int height() const;
	void draw( 
		int dstX, 
		int dstY, 
		int srcX, 
		int srcY, 
		int width, 
		int height ) const;
	void drawWithFixedColor( 
		int dstX, 
		int dstY, 
		int srcX, 
		int srcY, 
		int width, 
		int height,
		unsigned color ) const;
	void draw() const;
private:
	int mWidth;
	int mHeight;
	unsigned* mData;
};

#endif
