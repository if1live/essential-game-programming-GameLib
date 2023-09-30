float4 gTransform[ 4 ] : register( c0 );
float4 gDiffuseColor : register( c10 );

struct ToPixel{
	float4 screenPosition : POSITION;
	float4 color : COLOR0; //���_�J���[
	float2 texCoord : TEXCOORD0; //�e�N�X�`�����W
};

ToPixel main( 
float4 aPosition : POSITION0,
float4 aColor : COLOR0,
float2 aTexCoord : TEXCOORD0 ){
	ToPixel o;
	o.screenPosition.x = dot( gTransform[ 0 ], aPosition );
	o.screenPosition.y = dot( gTransform[ 1 ], aPosition );
	o.screenPosition.z = dot( gTransform[ 2 ], aPosition );
	o.screenPosition.w = dot( gTransform[ 3 ], aPosition );
#ifdef LINEAR_ZBUFFER
	o.screenPosition.z *= o.screenPosition.w;
#endif
	o.color.xyzw = aColor.zyxw * gDiffuseColor; //BGRA�ŗ���̂ŏ��ԕύX
	o.texCoord = aTexCoord;
	return o;
}
