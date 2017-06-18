#ifndef Struct_h__
#define Struct_h__

typedef struct tagParticle
{
	D3DXVECTOR3 vPosition;
	D3DCOLOR	dxColor;
	float		fSize;
	static const DWORD FVF;
}PARTICLE;

typedef struct tagAttribute
{
	D3DXVECTOR3 vPosition;
	D3DXVECTOR3 vVelocity;
	D3DXVECTOR3 vAcceleration;
	float		fLifeTime;
	float		fAge;
	D3DXCOLOR	dxColor;
	D3DXCOLOR	dxColorFade;
	bool		IsAlive;
}ATTRIBUTE;

typedef struct tagBoundingBox
{
	D3DXVECTOR3 _min;
	D3DXVECTOR3 _max;
}BOUNDING_BOX;


typedef struct tagAttri
{
	D3DXVECTOR3 _position;
	D3DXVECTOR3 _velocity;
	D3DXVECTOR3 _acceleration;
	float _lifeTime;
	float _age ;
	D3DXCOLOR _color;
	D3DXCOLOR _colorFade;
	bool _isAlive;

}_Attribute;

#endif // Struct_h__
