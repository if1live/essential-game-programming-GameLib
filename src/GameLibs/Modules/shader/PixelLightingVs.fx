//�s�N�Z���ɏ��������邾���̃V�F�[�_
float4 gTransform[ 4 ] : register( c0 );
float4 gWorldMatrix[ 3 ] : register( c4 );
float4 gInvTransposedWorldMatrix[ 3 ] : register( c7 );
float4 gDiffuseColor : register( c10 );

struct ToPixel{
	float4 screenPosition : POSITION;
	float4 color : COLOR0; //���_�J���[
	float2 texCoord : TEXCOORD0; //�e�N�X�`�����W
	float3 position : TEXCOORD1; //���[�J�����W�ʒu
	float3 normal : TEXCOORD2; //���[�J�����W�@��
};

ToPixel main( 
float4 aPosition : POSITION0,
float3 aNormal : NORMAL0,
float4 aColor : COLOR0,
float2 aTexCoord : TEXCOORD0 ){
	ToPixel o;
	o.screenPosition.x = dot( gTransform[ 0 ], aPosition );
	o.screenPosition.y = dot( gTransform[ 1 ], aPosition );
	o.screenPosition.z = dot( gTransform[ 2 ], aPosition );
	o.screenPosition.w = dot( gTransform[ 3 ], aPosition );
	float3 wp; //���[���h�ʒu
	wp.x = dot( gWorldMatrix[ 0 ], aPosition );
	wp.y = dot( gWorldMatrix[ 1 ], aPosition );
	wp.z = dot( gWorldMatrix[ 2 ], aPosition );
	float3 wn; //���[���h�@��
	wn.x = dot( gInvTransposedWorldMatrix[ 0 ].xyz, aNormal );
	wn.y = dot( gInvTransposedWorldMatrix[ 1 ].xyz, aNormal );
	wn.z = dot( gInvTransposedWorldMatrix[ 2 ].xyz, aNormal );
#ifdef LINEAR_ZBUFFER
	o.screenPosition.z *= o.screenPosition.w;
#endif
	o.color = aColor.zyxw * gDiffuseColor;
	o.texCoord = aTexCoord;
	o.position = wp;
	o.normal = wn;
	return o;
}

