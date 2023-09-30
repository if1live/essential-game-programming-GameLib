float3 gAmbient : register( c0 );
float4 gLightPosX : register( c1 );
float4 gLightPosY : register( c2 );
float4 gLightPosZ : register( c3 );
float3 gEyePosition : register( c4 );
float4 gSpecularColor : register( c5 );
float3 gLightColor0 : register( c6 );
float3 gLightColor1 : register( c7 );
float3 gLightColor2 : register( c8 );
float3 gLightColor3 : register( c9 );
float3 gEmissionColor : register( c10 );

sampler2D gSampler : register( s0 );

struct FromVertex{
	float4 color : COLOR0; //���_�J���[
	float2 texCoord : TEXCOORD0;
	float3 position : TEXCOORD1;
	float3 normal : TEXCOORD2;
};

float4 main( FromVertex input ) : COLOR {
	//���˃x�N�^�̐���(�����������x�N�^�ɑ΂���)
	float3 ev = gEyePosition - input.position;
	float3 nv = normalize( input.normal );
	float3 rv = 2.0 * nv * dot( nv, ev ) - ev;
	//���C�g�x�N�^�̐���
	float4 lvX, lvY, lvZ;
	lvX = gLightPosX - input.position.xxxx;
	lvY = gLightPosY - input.position.yyyy;
	lvZ = gLightPosZ - input.position.zzzz;
	//���C�g�x�N�^�Ɣ��˃x�N�^�̓���
	float4 s;
	s = lvX * rv.xxxx;
	s += lvY * rv.yyyy;
	s += lvZ * rv.zzzz;
	//�w���֐������O�� |L||R|�ŏ����K�v����B
	float4 lvL2 = lvX * lvX; //���C�g�x�N�^��撷��
	lvL2 += lvY * lvY;
	lvL2 += lvZ * lvZ;
	lvL2 += 0.0001; //0����h�~
	float4 rcpLvL = rsqrt( lvL2 ); //�����̋t��
	float4 rvL2 = dot( rv, rv );
	s *= rsqrt( lvL2 * rvL2 );
	s = max( 0, s );
	s = pow( s, gSpecularColor.w );
	//����Ƀ��C�g�����ŏ���
	s *= rcpLvL;
	//�f�B�t���[�Y�����v�Z����B
	float4 d = lvX * nv.xxxx;
	d += lvY * nv.yyyy;
	d += lvZ * nv.zzzz; 
	d *= rcpLvL * rcpLvL; //�����|L|^2�ŏ�����B
	d = max( 0, d );

	float3 dc = gAmbient;
	dc += gLightColor0 * d.x;
	dc += gLightColor1 * d.y;
	dc += gLightColor2 * d.z;
	dc += gLightColor3 * d.w;
	float3 sc = 0;
	sc += gLightColor0 * s.x;
	sc += gLightColor1 * s.y;
	sc += gLightColor2 * s.z;
	sc += gLightColor3 * s.w;

	float4 c = input.color * tex2D( gSampler, input.texCoord );
	c.xyz *= dc.xyz;
	c.xyz += sc * gSpecularColor.xyz;
	c.xyz += gEmissionColor.xyz;
	c.w = c.w;
	return c;
}

