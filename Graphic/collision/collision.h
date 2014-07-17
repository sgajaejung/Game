#pragma once


namespace graphic
{
	// interface ICollisionable
	// �浹�׽�Ʈ�� ������Ʈ�� ICollisionable �������̽��� ��ӹ޾� �����ؾ� �Ѵ�.
	struct ICollisionable
	{
		// Collision Test���� ����
		virtual BOOL IsTest( int testNum )=0;
		virtual int GetId()=0;

		// �浹�׽�Ʈ �ϱ����� �浹�ڽ�, �浹���� ��ǥ�� ������Ʈ �Ѵ�.
		// UpdateCollisionBox() �Լ��� �浹�׽�Ʈ�ϱ� ������ ȣ���ϸ� �ȴ�.
		virtual void UpdateCollisionBox()=0;

		// ���� ���� �浹�ڽ��� ����Ʈ�� �������� �����Ѵ�. 
		virtual cBoundingBox* GetCollisionBox()=0;

		// �浹���� �浹�� ����Ų ��ü���� ȣ���
		// TestNum: �׽�Ʈ ��ȣ
		// model : �浹�� ��ü
		virtual void Collision( int TestNum, ICollisionable *obj )=0;
	};

}