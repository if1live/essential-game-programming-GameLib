#include "Triangle.h"
#include "Library/Vector3.h"
#include <limits>
using namespace std;


void Triangle::setPosition( const Vector3& p0, const Vector3& p1, const Vector3& p2 ){
	mPosition[ 0 ] = p0;
	mPosition[ 1 ] = p1;
	mPosition[ 2 ] = p2;
}

double Triangle::getIntersectionTime( const Vector3& lineBegin, const Vector3& lineVector ) const {
	double inf = numeric_limits< double >::max(); //double�ŕ\�����ԑ傫�Ȑ��B������Ȃ���΂��ꂪ�Ԃ�
	//�{�Ɠ����L���ɒu���Ȃ���
	const Vector3& a = lineBegin;
	const Vector3& b = lineVector;
	const Vector3& c = mPosition[ 0 ];
	Vector3 d, e;
	d.setSub( mPosition[ 1 ], c );
	e.setSub( mPosition[ 2 ], c );
	Vector3 f;
	f.setSub( c, a );
	//�@�������߂悤�B
	Vector3 n;
	n.setCross( d, e );
	//t�����߂�B
	double nf = n.dot( f );
	double nb = n.dot( b );
	if ( nb == 0.f ){ //���s
	  return inf; //�͂���ɂ����Ⴆ�B
	}
	double t = nf / nb;
	if ( t < 0 || t > 1 ){ 
	  return inf;
	}
	///v�����߂�B
	Vector3 p;
	p.setMadd( b, t, a );
	Vector3 g;
	g.setSub( p, c );
	double gd = g.dot( d );
	double ge = g.dot( e );
	double dd = d.dot( d );
	double ee = e.dot( e );
	double de = d.dot( e );
	//de*de - dd*ee�̋t���͓��g���̂ŁA���ōς܂��č���������B
	double dedeEedd = de * de - ee * dd; //������0�ɂ͂Ȃ肦�Ȃ��̂Ń`�F�b�N�Ȃ��ŋt��������
	double rcpDedeEedd = 1.0 / dedeEedd; 

	double u = ( gd*de - ge*dd ) * rcpDedeEedd;
	if ( u < 0 || u > 1 ){
	  return inf;
	}
	double v = ( ge*de - gd*ee ) * rcpDedeEedd;
	if ( v < 0 || ( u + v > 1 ) ){
	  return inf;
	}
	return t; //t������
}

bool Triangle::isIntersect( const Vector3& lineBegin, const Vector3& lineVector ) const {
	double t = getIntersectionTime( lineBegin, lineVector );
	return ( t >= 0.0 && t <= 1.0 );
}

bool Triangle::restrictMove( Vector3* v, const Vector3& p ) const {
	//�܂�����
	bool r = isIntersect( p, *v );
	if ( !r ){
		return false; //������Ȃ��̂ł��̂܂܏I���
	}
	//���ē������Ă���Ȃ�΁A�x�N�^���C������B
	//���͖@��n���g���Ĉȉ��̂悤�ɏ�����
	//a = v - dot( n, v ) * n / |n|^2
	Vector3 d, e, n;
	d.setSub( mPosition[ 1 ], mPosition[ 0 ] );
	e.setSub( mPosition[ 2 ], mPosition[ 0 ] );
	n.setCross( d, e );

	n *= n.dot( *v ) / n.squareLength();
	*v -= n;
	return true;
}

void Triangle::modifyMove( Vector3* v ) const {
	Vector3 d, e, n;
	d.setSub( mPosition[ 1 ], mPosition[ 0 ] );
	e.setSub( mPosition[ 2 ], mPosition[ 0 ] );
	n.setCross( d, e );

	n *= n.dot( *v ) / n.squareLength();
	*v -= n;
}
