#pragma once

// CollisionManager Ŭ������ �浹�׽�Ʈ�� �ϰ������� ó���Ѵ�.
// �浹�׽�Ʈ�� ������Ʈ�� 2���� �׷����� ������, �� �׷���
// Ʈ�� �ڷᱸ�� ���´�. �� �θ𳢸� �浹�׽�Ʈ�� �����ϸ� �ڽ����� ������ �浹�׽�Ʈ
// �ϰ�, ��������� �浹�׽�Ʈ�� �� ���� ������ �浹�׽�Ʈ�� �����ϸ� �浹�Ǿ��ٰ� �˷��ش�.
// �� ������Ʈ�� nTestNum�� ������ �ٸ� �׷쳢�� �浹�׽�Ʈ�� ���� nTestNum������ �浹�׽�Ʈ�Ѵ�.
// �浹�׽�Ʈ�� ������Ʈ�� ICollisionable �������̽��� ��ӹ޾ƾ� �Ѵ�.


namespace graphic
{
	class cCollisionManager
	{
	public:
		cCollisionManager();
		virtual ~cCollisionManager();
		enum { MAX_COLLISION_TABLE = 64 };
		ICollisionable* m_checkTable[ MAX_COLLISION_TABLE][ 2];		// [table size][ src/dst]


		// �浹�׽�Ʈ�� ������Ʈ�� �����ϴ� Ʈ��
		struct sCollisionNode : cNode
		{
			int testnum; // �׽�Ʈ��ȣ (0���� ������, ���� �׽�Ʈ��ȣ���� ���Ѵ�.)
			int boundingType;	// 0: box, 1: sphere
			ICollisionable *pobj;
			//Sphere *psphere;
			cBoundingBox *box;
			sCollisionNode( int id ) : cNode(id) 
			{}
		};


		void InsertObject( int group, ICollisionable *obj, int testNum=0 ); // �浹�׽�Ʈ�� ������Ʈ �߰�
		void InsertObject( ICollisionable *parent, ICollisionable *obj, int testNum=0 ); // �浹�׽�Ʈ�� ������Ʈ �߰�
		void RemoveObject( ICollisionable *obj ); // ����Ʈ���� ������Ʈ �߰�
		void UpdateCollisionBox(); // �浹�ڽ��� ��ǥ�� �����Ѵ�.
		int CollisionTest( int testNum ); // �浹�׽�Ʈ


	protected:
		void InsertObject( sCollisionNode *parent, ICollisionable *obj, int testNum );
		void UpdateCollisionBoxRec( sCollisionNode *obj );
		bool CheckNodeCollision(sCollisionNode *node1, sCollisionNode *node2, int testNum );
		int CollisionTestRec( sCollisionNode *node1, sCollisionNode *node2, int testNum ); // �浹�׽�Ʈ


	private:
		sCollisionNode *m_group1;
		sCollisionNode *m_group2;
	};

}