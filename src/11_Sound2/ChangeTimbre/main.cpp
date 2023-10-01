#include "GameLib/Framework.h"

namespace GameLib{
	void Framework::update(){
		sleep( 16 );
		static int a = 0;
		static int b = 0;
		if ( a++ % 120 == 0 ){
			const int N = 44100;
			short wave[ N ];
			if ( b % 3 == 0 ){
				//のこぎり波
				for ( int i = 0; i < N; ++i ){
					wave[ i ] = static_cast< short >( (i*200) % 20000 );
				}
			}else if ( b % 3 == 1 ){
				//四角波
				for ( int i = 0; i < N; ++i ){
					wave[ i ] = ( i % 100 < 50 ) ? 0 : 20000;
				}
			}else{
				//三角波
				for ( int i = 0; i < N; ++i ){
					int j = i % 100;
					int w = ( j < 50 ) ? ( j*400 ) : ( 40000 - j*400 );
					wave[ i ] = static_cast< short >( w );
				}
			}
			playSound( wave, N );
			++b;
		}
	}
}
