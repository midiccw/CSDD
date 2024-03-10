#pragma once
#include<math.h>
#include<stdlib.h>
#include<ctype.h>
#include<stdio.h>
#include<time.h>
#include<iostream>
#include"cuddInt.h"
#include"util.h"
#define TRUE 1
#define FALSE 0
#define VALUE_TYPE long int
typedef DdNode* (*DD_MA)(DdManager*, DdNode*, int);

typedef struct Str_Graph {
	unsigned int nips_v, nips_t;              // ���㣬ʱ�̱����ĸ���
	unsigned int evnum, tinum, nodes;        // �¼�,ʱ�̣��������
	DdNode** x;                             // �����������
	DdNode** y;          
	DdNode** t;                           // ʱ�̱�������
	DdNode* Events;                      // �¼�����
}Str_Graph;

// ��ͼ����  ��õ����ݶ�Ӧ��ͼG (������¼�����ADD�Ѿ���������)
extern Str_Graph* ReadGraph(DdManager* manager, FILE* f, Str_Graph* G); 

// �ᴴ������ or ʱ�̵� ��ADD ���Ѿ��������ˣ�
extern DdNode* CreateNode(DdManager* manager, unsigned int sv, unsigned int npis, DdNode** x);

// �ᴴ��һ���¼���ADD ���Ѿ��������ˣ�
extern DdNode* CreateADD(DdManager* manager, unsigned int st,unsigned int su,unsigned int sv, unsigned int npis_v,unsigned int npis_t, DdNode** x, DdNode** y,DdNode**t);

extern DdNode* Make_times(DdManager* dd, DdNode* f, int flag);

extern DdNode* cuddAddsigleApplyRecur(DdManager* dd, DD_MA op, DdNode* f, int flag);

extern DdNode* Cudd_addsigleApply(DdManager* dd, DD_MA op, DdNode* f);

extern void Barf(const char* s);
extern char ReadChar(FILE* f);
extern int EOF_Test(FILE* f);
extern void SkipLine(FILE* f);
extern void Skip(FILE* f);
extern void GetString(FILE* f, char* buff);
extern int GetInt(FILE* f);
extern int ComputeFloorLog2(unsigned int value);
extern FILE* OpenFile(const char* c);
extern void ErrorReport();


#define CHECK_OPEN(node) CheckOpen(node,__FILE__,__LINE__)
#define CHECK_CLOSE(manager, node) CheckClose(manager,node,__FILE__,__LINE__)
extern void CheckOpen(DdNode* node, const char* file, int line);
extern void CheckClose(DdManager* manager, DdNode* node, const char* file, int line);
extern void CheckRef();



/********************************************************************************/
// ��ȡ����ϵ��С����ж�����  ���������apply����ʹ��
extern DdNode* Extra_01add_new(DdManager* dd, DdNode* f);

// �����tʱ��״̬�����ı�ıߵĲ���
extern DdNode* ChEdPo(DdManager* dd, Str_Graph* G, unsigned int t);
extern DdNode* ChangedEdgesPoint(DdManager* dd, Str_Graph* G, unsigned int t);

// �����tʱ��ͼ��״̬�Ĳ���
extern DdNode* Sns(DdManager* dd, Str_Graph* G, unsigned int t);
extern DdNode* Snapshot(DdManager* dd, Str_Graph* G, unsigned int t);

// �ж�ĳ������tʱ���Ƿ���ڵĲ���
extern bool PoAcEd(DdManager* dd, Str_Graph* G, unsigned int u, unsigned int v, unsigned int t);
extern bool PointActiveEdge(DdManager* dd, Str_Graph* G, unsigned int u, unsigned int v, unsigned int t);

// ���ĳ��������tʱ�̵������ھӲ���
extern DdNode* PoDiNe(DdManager* dd, Str_Graph* G, unsigned int u, unsigned int t);
extern DdNode* PointDirectNeighbor(DdManager* dd, Str_Graph* G, unsigned int u, unsigned int t);


/*******************--�°汾--***************************************************************************************/

extern Str_Graph* ReadGraph_new(DdManager* manager, FILE* f, Str_Graph* G);


extern DdNode* PointDirectNeighbor_new(DdManager* dd, Str_Graph* G, unsigned int u, unsigned int t);

extern bool PointActiveEdge_new(DdManager* dd, Str_Graph* G, unsigned int u, unsigned int v, unsigned int t);

extern DdNode* Snapshot_new(DdManager* dd, Str_Graph* G, unsigned int t);

extern DdNode* ChangedEdgesPoint_new(DdManager* dd, Str_Graph* G, unsigned int t);


/******************--�°汾�ķֿ��Ż�--******************************************************************************/

typedef struct Str_Graph_new {
	enum {nums = 1};
	unsigned int nips_v, nips_t;               // ���㣬ʱ�̱����ĸ���
	unsigned int evnum, tinum, nodes;         // �¼�,ʱ�̣��������
	unsigned int gaps;                       // ÿ���������Ŀ
	DdNode** x;                             // �����������
	DdNode** y;
	DdNode** t;                           // ʱ�̱�������
	DdNode* Events[nums];                // ������
}Str_Graph_new;

extern Str_Graph_new* ReadGraph_new_2(DdManager* manager, FILE* f, Str_Graph_new* G);

extern DdNode* PointDirectNeighbor_new_2(DdManager* dd, Str_Graph_new* G, unsigned int u, unsigned int t);

extern DdNode* PointDirectNeighbor_new_1(DdManager* dd, Str_Graph_new* G, unsigned int u, unsigned int t);

extern bool PointActiveEdge_new_2(DdManager* dd, Str_Graph_new* G, unsigned int u, unsigned int v, unsigned int t);

extern bool PointActiveEdge_new_1(DdManager* dd, Str_Graph_new* G, unsigned int u, unsigned int v, unsigned int t);

extern DdNode* Snapshot_new_2(DdManager* dd, Str_Graph_new* G, unsigned int t);

extern DdNode* Snapshot_new_1(DdManager* dd, Str_Graph_new* G, unsigned int t);

extern DdNode* ChangedEdgesPoint_new_2(DdManager* dd, Str_Graph_new* G, unsigned int t);

extern DdNode* pointReverseNeighbor_new_2(DdManager* dd, Str_Graph_new* G, unsigned int u, unsigned int t);

extern DdNode* intervalDirectNeighbor(DdManager* dd, Str_Graph_new* G, unsigned int u, unsigned int t, unsigned ti);

extern DdNode* intervalReverseNeighbor(DdManager* dd, Str_Graph_new* G, unsigned int u, unsigned int t, unsigned ti);

extern bool intervalActiveEdge(DdManager* dd, Str_Graph_new* G, unsigned int u, unsigned int v, unsigned int t, unsigned int ti);

extern DdNode* changedEdgesInterval(DdManager* dd, Str_Graph_new* G, unsigned int t, unsigned int ti);

