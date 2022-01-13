#pragma once
#include <DirectXMath.h>

using namespace DirectX;

//XMFLOAT3�����Z�q�I�[�o�[���[�h������RVector3�^
class RVector3 : public XMFLOAT3
{
public:
	//�R���X�g���N�^
	RVector3(float x, float y, float z) : XMFLOAT3(x, y, z) {};
	RVector3() :XMFLOAT3(0, 0, 0) {};

	RVector3 operator+(const RVector3 &other)const { return RVector3(x + other.x, y + other.y, z + other.z); }
	RVector3 operator-(const RVector3 &other)const { return RVector3(x - other.x, y - other.y, z - other.z); }
	RVector3 operator*(const RVector3 &other)const { return RVector3(x * other.x, y * other.y, z * other.z); }
	RVector3 operator/(const RVector3 &other)const { return RVector3(x / other.x, y / other.y, z / other.z); }
	RVector3 &operator+=(const RVector3 &other) { this->x += other.x; this->y += other.y; this->z += other.z; return *this; }
	RVector3 &operator-=(const RVector3 &other) { this->x -= other.x; this->y -= other.y; this->z -= other.z; return *this; }
	RVector3 &operator*=(const RVector3 &other) { this->x *= other.x; this->y *= other.y; this->z *= other.z; return *this; }
	RVector3 &operator/=(const RVector3 &other) { this->x /= other.x; this->y /= other.y; this->z /= other.z; return *this; }

	bool operator==(const RVector3 &other) { return this->x == other.x && this->y == other.y && this->z == other.z; }
	bool operator!=(const RVector3 &other) { return !(*this == other); }

	RVector3 operator*(const float &other) { return RVector3(x * other, y * other, z * other); }
	RVector3 operator/(const float &other) { return RVector3(x / other, y / other, z / other); }
	RVector3 operator*=(const float &other) { this->x *= other; this->y *= other; return *this; }
	RVector3 operator/=(const float &other) { this->x /= other; this->y /= other; return *this; }

	float length() { return sqrtf(x * x + y * y + z * z); }
	RVector3 norm() { return RVector3(x / length(), y / length(), z / length()); }
	float distance(const RVector3 &v) { return sqrtf(pow(v.x - x, 2) + pow(v.y - y, 2) + pow(v.z - z, 2)); }

	void zero() { x = 0, y = 0, z = 0; }

	RVector3 operator-() const { return RVector3(-x, -y, -z); }
};

inline float dot(const RVector3 &a, const RVector3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline RVector3 cross(const RVector3 &a, const RVector3 &b) { return RVector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }

inline float distance(const RVector3 &v1,const RVector3 &v2){return sqrtf(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2) + pow(v1.z - v2.z, 2));}

inline RVector3 operator*(const float &sum, const RVector3 &sum2) { return RVector3(sum2.x * sum, sum2.y * sum, sum2.z * sum); }
inline RVector3 operator/(const float &sum, const RVector3 &sum2) { return RVector3(sum2.x / sum, sum2.y / sum, sum2.z / sum); }

//RVector3�^���g�p�����Փ˔���v���~�e�B�u
namespace RV3Colider {

	///�����s�o�E���f�B���O�{�b�N�X AABB
	struct Rv3AABB {

		RVector3 min;	//����T�C�Y
		RVector3 max;	//����T�C�Y
		RVector3 oldPos;//1F�O�̍��W

		//�R���X�g���N�^�A�f�X�g���N�^
		Rv3AABB()	= default;
		~Rv3AABB()	= default;

		//�l�w��R���X�g���N�^
		Rv3AABB(RVector3 min, RVector3 max, RVector3 pos) {
			this->min = min;
			this->max = max;
			this->oldPos = pos;
		}

		inline void Set(RVector3 pos, RVector3 min, RVector3 max) {
			this->min = min;
			this->max = max;
			this->oldPos = pos;
		}

		inline void Update(RVector3 actpos) {
			RVector3 diff = actpos - oldPos;
			min += diff;
			max += diff;
			oldPos = actpos;
		}
	};

	/// <summary>
	/// AABB���m�̔���
	/// </summary>
	/// <param name="box1">�{�b�N�X1</param>
	/// <param name="box2">�{�b�N�X2</param>
	/// <returns>�Փ˔���</returns>
	inline bool ColisionAABB(const Rv3AABB &box1, const Rv3AABB &box2) {
		//��Փ�
		if (box1.min.x > box2.max.x) { return false; }
		if (box1.max.x < box2.min.x) { return false; }
		if (box1.min.y > box2.max.y) { return false; }
		if (box1.max.y < box2.min.y) { return false; }
		if (box1.min.z > box2.max.z) { return false; }
		if (box1.max.z < box2.min.z) { return false; }
		//�Փ�
		return true;
	}

	//��
	struct Sphere {
		//���S���W
		RVector3 center;
		//���a
		float rad;
		//�f�t�H���g�R���X�g���N�^
		Sphere() {
			center = RVector3(0, 0, 0);
			rad = 0;
		}
		//�l�w��R���X�g���N�^
		Sphere(RVector3 pos, float r) {
			center = pos;
			rad = r;
		}
	};

	/// <summary>
	/// �����m�̔���
	/// </summary>
	/// <param name="a">��1</param>
	/// <param name="b">��2</param>
	/// <returns>�Փ˔���</returns>
	inline bool Colision2Sphere(Sphere a, Sphere b) {
		if (a.rad + b.rad < distance(a.center, b.center)) {
			return false;
		}
		//�Փ�
		return true;
	}

	//����
	struct Plane
	{
		//�@���x�N�g��
		RVector3 normal;
		//���_����̋���
		float distance;
		//�f�t�H���g
		Plane() {
			normal.zero();
			distance = 0;
		}
		//�l�w��
		Plane(RVector3 n, float d) {
			normal = n;
			distance = d;
		}
	};

	/// <summary>
	/// ���ƕ��ʂ̏Փ˔���
	/// </summary>
	/// <param name="sphere">���̃v���~�e�B�u</param>
	/// <param name="plane">���ʃv���~�e�B�u</param>
	/// <param name="coliPos">�Փ˓_��Ԃ��ϐ�</param>
	/// <returns>�Փ˔���</returns>
	bool ColisionSphereToPlane(const Sphere &sphere, const Plane &plane, RVector3 *coliPos = nullptr);

	//�O�p�`
	struct Triangle {
		//���_���W
		RVector3 p0, p1, p2;
		//�@��
		RVector3 normal;

	};

};
