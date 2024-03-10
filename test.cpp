#include"Graph.h"
#include"cudd.h"
#include "util.h"
#include "cuddInt.h"
using namespace std;

int main(){
	FILE* fp;
	DdManager* dd;
	//Str_Graph* G, * NG;
	Str_Graph_new* G, * NG;
	clock_t start_time, end_time;
	clock_t start_time_1, end_time_1;
	clock_t start_time_2, end_time_2;
	clock_t start_time_3, end_time_3;
	clock_t start_time_4, end_time_4;


	if ((fp = OpenFile("D:/old_data/data/Transfor/test_super_0.txt")) == NULL) {
		Barf("Could not open input file");
	}
	dd = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
	if (dd == NULL)  exit(1);
	G = ALLOC(Str_Graph_new, 1);


	start_time_1 = clock();
	NG = ReadGraph_new_2(dd, fp, G);
	//printf("this is node numbers \n");
	//Cudd_RecursiveDeref(dd, NG->Events);
	//CheckClose(dd, NG->Events, "D:\\vcr\\test\\test.cpp", 29);
	//Cudd_PrintDebug(dd, NG->Events, NG->nips_t + 2 * NG->nips_v, 1);
	//CheckRef();
	//Cudd_PrintMinterm(dd, NG->Events);
	end_time_1 = clock();
	std::cout << "read graph spend times: " << (end_time_1 - start_time_1) /1000 << "s" << std::endl;
	//DdNode* ans5 = PointDirectNeighbor_new_2(dd, NG, 300, 15510);
/**************************--test_data--用来验证正确性的数据集--*************************************************************************************/

	
	//DdNode* ans1 = intervalDirectNeighbor(dd, NG, 3, 1,3);
	//DdNode* ans2 = intervalDirectNeighbor(dd, NG, 1, 1,3);
	//DdNode* ans3 = intervalReverseNeighbor(dd, NG, 4, 1,3);
	//DdNode* ans4 = intervalReverseNeighbor(dd, NG, 3, 1,3);
	//bool ans5 = intervalActiveEdge(dd, NG, 1,2, 1,3);
	//bool ans6 = intervalActiveEdge(dd, NG, 1,3, 1,3);
	//bool and1 = PoAcEd(dd, NG, 1, 2, 1);
	//bool and2 = PoAcEd(dd, NG, 2, 3, 1);
	//bo0l and3 = PoAcEd(dd, NG, 1, 2, 0);
	//bool and4 = PointActiveEdge_new_2(dd, NG, 1, 2, 1);
	//bool and5 = PointActiveEdge_new_2(dd, NG, 2, 3, 1);
	//bool and6 = PointActiveEdge_new_2(dd, NG, 1, 2, 0);
	//
	//DdNode* ans7 = changedEdgesInterval(dd, NG, 1,3);
	//cout << "SS" << endl;
	//DdNode* ans8 = changedEdgesInterval(dd, NG, 1);
	//DdNode* ans9 = ChangedEdgesPoint_new_2(dd, NG, 1);
	//std::cout << "Done of test for correctness" << std::endl;

///***************************--test_CM_0--用来比较两种算法的时间性能的数据集---**************************************************************************************************/
	//test for 1
//	/*start_time_1 = clock();
	//for (int i = 1; i <= 365; i++) {
	//	DdNode* ans1 = PointDirectNeighbor_new_2(dd, NG, 1, 10);
	//	DdNode* ans2 = PointDirectNeighbor_new_2(dd, NG, 10, 100);
	//	DdNode* ans3 = PointDirectNeighbor_new_2(dd, NG, 100, 500);
//
	//	DdNode* ans4 = PointDirectNeighbor_new_2(dd, NG, 200, 1000);
	//	DdNode* ans5 = PointDirectNeighbor_new_2(dd, NG, 300, 15500);
	//	DdNode* ans6 = PointDirectNeighbor_new_2(dd, NG, 401, 20000);
//
	//	DdNode* ans7 = PointDirectNeighbor_new_2(dd, NG, 500, 30050);
	//	DdNode* ans8 = PointDirectNeighbor_new_2(dd, NG, 600, 40050);
	//	DdNode* ans9 = PointDirectNeighbor_new_2(dd, NG, 700, 50020);
//
	//	DdNode* ans10 = PointDirectNeighbor_new_2(dd, NG, 800, 35000);
	//	DdNode* ans11 = PointDirectNeighbor_new_2(dd, NG, 900, 45000);
	//	DdNode* ans12 = PointDirectNeighbor_new_2(dd, NG, 1010, 58000);*/
//
//		/*DdNode* ans1 = PointDirectNeighbor(dd, NG, 1, 10);
//		DdNode* ans2 = PointDirectNeighbor(dd, NG, 10, 100);
//		DdNode* ans3 = PointDirectNeighbor(dd, NG, 100, 500);
//
//		DdNode* ans4 = PointDirectNeighbor(dd, NG, 200, 1000);
//		DdNode* ans5 = PointDirectNeighbor(dd, NG, 300, 15500);
//		DdNode* ans6 = PointDirectNeighbor(dd, NG, 401, 20000);
//
//		DdNode* ans7 = PointDirectNeighbor(dd, NG, 500, 30050);
//		DdNode* ans8 = PointDirectNeighbor(dd, NG, 600, 40050);
//		DdNode* ans9 = PointDirectNeighbor(dd, NG, 700, 50020);
//
//		DdNode* ans10 = PointDirectNeighbor(dd, NG, 800, 35000);
//		DdNode* ans11 = PointDirectNeighbor(dd, NG, 900, 45000);
//		DdNode* ans12 = PointDirectNeighbor(dd, NG, 1010, 58000);*/
//	/*}
//	end_time_1 = clock();
//	std::cout << "test_Interval for 1 spend times: " << (end_time_1 - start_time_1) * 1000 << "us" << std::endl;*/
//
//	/*start_time_1 = clock();
//	for (int i = 1; i <= 365; i++) {
//		DdNode* ans1 = PoDiNe(dd, NG, 1, 10);
//		DdNode* ans2 = PoDiNe(dd, NG, 10, 100);
//		DdNode* ans3 = PoDiNe(dd, NG, 100, 500);
//
//		DdNode* ans4 = PoDiNe(dd, NG, 200, 1000);
//		DdNode* ans5 = PoDiNe(dd, NG, 300, 15500);
//		DdNode* ans6 = PoDiNe(dd, NG, 401, 20000);
//
//		DdNode* ans7 = PoDiNe(dd, NG, 500, 30050);
//		DdNode* ans8 = PoDiNe(dd, NG, 600, 40050);
//		DdNode* ans9 = PoDiNe(dd, NG, 700, 50020);
//
//		DdNode* ans10 = PoDiNe(dd, NG, 800, 35000);
//		DdNode* ans11 = PoDiNe(dd, NG, 900, 45000);
//		DdNode* ans12 = PoDiNe(dd, NG, 1010, 58000);
//
//	}
//	end_time_1 = clock();
//	std::cout << "test_point for 1 spend times: " << (end_time_1 - start_time_1) * 1000 << "us" << std::endl;*/
//
	 //test for 2
	/*start_time_2 = clock();
	for (int i = 1; i <= 365; i++) {
		bool ans1 = PoAcEd(dd, NG, 1, 2, 10);
		bool ans2 = PoAcEd(dd, NG, 10, 20, 100);
		bool ans3 = PoAcEd(dd, NG, 100, 52, 500);

		bool ans4 = PoAcEd(dd, NG, 200, 206, 1000);
		bool ans5 = PoAcEd(dd, NG, 370, 373, 15500);
		bool ans6 = PoAcEd(dd, NG, 401, 420, 20000);

		bool ans7 = PoAcEd(dd, NG, 510, 520, 30050);
		bool ans8 = PoAcEd(dd, NG, 610, 620, 40050);
		bool ans9 = PoAcEd(dd, NG, 750, 700, 50020);

		bool ans10 = PoAcEd(dd, NG, 80, 806, 35020);
		bool ans11 = PoAcEd(dd, NG, 942, 973, 46030);
		bool ans12 = PoAcEd(dd, NG, 1010, 1007, 55000);
	}
	end_time_2 = clock();
	std::cout << "test_point for 2 spend times: " << (end_time_2 - start_time_2) * 1000 << "us" << std::endl;*/

	/*start_time_2 = clock();
	for (int i = 1; i <= 365; i++) {
		bool ans1 = PointActiveEdge_new_2(dd, NG, 1, 2, 10);
		bool ans2 = PointActiveEdge_new_2(dd, NG, 10, 20, 100);
		bool ans3 = PointActiveEdge_new_2(dd, NG, 100, 52, 500);

		bool ans4 = PointActiveEdge_new_2(dd, NG, 200, 206, 1000);
		bool ans5 = PointActiveEdge_new_2(dd, NG, 370, 373, 15500);
		bool ans6 = PointActiveEdge_new_2(dd, NG, 401, 420, 20000);

		bool ans7 = PointActiveEdge_new_2(dd, NG, 510, 520, 30050);
		bool ans8 = PointActiveEdge_new_2(dd, NG, 610, 620, 40050);
		bool ans9 = PointActiveEdge_new_2(dd, NG, 750, 700, 50020);

		bool ans10 = PointActiveEdge_new_2(dd, NG, 80, 806, 35020);
		bool ans11 = PointActiveEdge_new_2(dd, NG, 942, 973, 46030);
		bool ans12 = PointActiveEdge_new_2(dd, NG, 1010, 1007, 55000);*/
//
//		/*bool ans1 = PointActiveEdge(dd, NG, 1, 2, 10);
//		bool ans2 = PointActiveEdge(dd, NG, 10, 20, 100);
//		bool ans3 = PointActiveEdge(dd, NG, 100, 52, 500);
//
//		bool ans4 = PointActiveEdge(dd, NG, 200, 206, 1000);
//		bool ans5 = PointActiveEdge(dd, NG, 370, 373, 15500);
//		bool ans6 = PointActiveEdge(dd, NG, 401, 420, 20000);
//
//		bool ans7 = PointActiveEdge(dd, NG, 510, 520, 30050);
//		bool ans8 = PointActiveEdge(dd, NG, 610, 620, 40050);
//		bool ans9 = PointActiveEdge(dd, NG, 750, 700, 50020);
//
//		bool ans10 = PointActiveEdge(dd, NG, 80, 806, 35020);
//		bool ans11 = PointActiveEdge(dd, NG, 942, 973, 46030);
//		bool ans12 = PointActiveEdge(dd, NG, 1010, 1007, 55000);*/
//	
	/*}
	end_time_2 = clock();
	std::cout << "test_Interval for 2 spend times: " << (end_time_2 - start_time_2) * 1000 << "us" << std::endl;*/

	//test for 3
	/*start_time_3 = clock();
	for (int i = 1; i <= 365; i++) {
		Snapshot_new_2(dd, NG, 10);
		Snapshot_new_2(dd, NG, 100);
		Snapshot_new_2(dd, NG, 500);

		Snapshot_new_2(dd, NG, 1000);
		Snapshot_new_2(dd, NG, 15500);
		Snapshot_new_2(dd, NG, 20000);

		Snapshot_new_2(dd, NG, 30050);
		Snapshot_new_2(dd, NG, 40050);
		Snapshot_new_2(dd, NG, 50020);

		Snapshot_new_2(dd, NG, 35020);
		Snapshot_new_2(dd, NG, 46030);
		Snapshot_new_2(dd, NG, 55000);*/

//
//		/*Snapshot(dd, NG, 10);
//		Snapshot(dd, NG, 100);
//		Snapshot(dd, NG, 500);
//
//		Snapshot(dd, NG, 1000);
//		Snapshot(dd, NG, 15500);
//		Snapshot(dd, NG, 20000);
//
//		Snapshot(dd, NG, 30050);
//		Snapshot(dd, NG, 40050);
//		Snapshot(dd, NG, 50020);
//
//		Snapshot(dd, NG, 35020);
//		Snapshot(dd, NG, 46030);
//		Snapshot(dd, NG, 55000);*/
//
	/*}
	end_time_3 = clock();
	std::cout << "test_Interval for 3 spend times: " << (end_time_3 - start_time_3) * 1000 << "us" << std::endl;*/

//
//	/*start_time_3 = clock();
//	for (int i = 1; i <= 365; i++) {
//		Sns(dd, NG, 10);
//		Sns(dd, NG, 100);
//		Sns(dd, NG, 500);
//
//		Sns(dd, NG, 1000);
//		Sns(dd, NG, 15500);
//		Sns(dd, NG, 20000);
//
//		Sns(dd, NG, 30050);
//		Sns(dd, NG, 40050);
//		Sns(dd, NG, 50020);
//
//		Sns(dd, NG, 35020);
//		Sns(dd, NG, 46030);
//		Sns(dd, NG, 55000);
//
//	}
//	end_time_3 = clock();
//	std::cout << "test_point for 3 spend times: " << (end_time_3 - start_time_3) * 1000 << "us" << std::endl;*/
//
	// test for 4
	/*start_time_4 = clock();
	for (int i = 1; i <= 365; i++) {
		ChangedEdgesPoint_new_2(dd, NG, 10);
		ChangedEdgesPoint_new_2(dd, NG, 100);
		ChangedEdgesPoint_new_2(dd, NG, 500);

		ChangedEdgesPoint_new_2(dd, NG, 1000);
		ChangedEdgesPoint_new_2(dd, NG, 15500);
		ChangedEdgesPoint_new_2(dd, NG, 20000);

		ChangedEdgesPoint_new_2(dd, NG, 30050);
		ChangedEdgesPoint_new_2(dd, NG, 40050);
		ChangedEdgesPoint_new_2(dd, NG, 50020);

		ChangedEdgesPoint_new_2(dd, NG, 35020);
		ChangedEdgesPoint_new_2(dd, NG, 46030);
		ChangedEdgesPoint_new_2(dd, NG, 55000);*/

//		/*ChangedEdgesPoint(dd, NG, 10);
//		ChangedEdgesPoint(dd, NG, 100);
//		ChangedEdgesPoint(dd, NG, 500);
//
//		ChangedEdgesPoint(dd, NG, 1000);
//		ChangedEdgesPoint(dd, NG, 15500);
//		ChangedEdgesPoint(dd, NG, 20000);
//
//		ChangedEdgesPoint(dd, NG, 30050);
//		ChangedEdgesPoint(dd, NG, 40050);
//		ChangedEdgesPoint(dd, NG, 50020);
//
//		ChangedEdgesPoint(dd, NG, 35020);
//		ChangedEdgesPoint(dd, NG, 46030);
//		ChangedEdgesPoint(dd, NG, 55000);*/
	/*}
	end_time_4 = clock();
	std::cout << "test_Interval for 4 spend times: " << (end_time_4 - start_time_4) * 1000 << "us" << std::endl;*/



/********************--test_Witate_0--用来比较算法时间性能的数据集**********************************************************************/
	
 //   //test for 1
	//start_time_1 = clock();
	//for (int i = 1; i <= 10; i++) {
	//	DdNode* ans1 = PointDirectNeighbor_new_2(dd, NG, 1, 10000);
	//	DdNode* ans2 = PointDirectNeighbor_new_2(dd, NG, 10000, 100000);
	//	DdNode* ans3 = PointDirectNeighbor_new_2(dd, NG, 100000, 500000);
	//
	//	DdNode* ans4 = PointDirectNeighbor_new_2(dd, NG, 200000, 1000000);
	//	DdNode* ans5 = PointDirectNeighbor_new_2(dd, NG, 300702, 1550050);
	//	DdNode* ans6 = PointDirectNeighbor_new_2(dd, NG, 401000, 2000000);
	//
	//	DdNode* ans7 = PointDirectNeighbor_new_2(dd, NG, 500010, 3005000);
	//	DdNode* ans8 = PointDirectNeighbor_new_2(dd, NG, 600010, 4005000);
	//	DdNode* ans9 = PointDirectNeighbor_new_2(dd, NG, 700050, 5002000);
	//
	//	DdNode* ans10 = PointDirectNeighbor_new_2(dd, NG, 800660, 6005020);
	//	DdNode* ans11 = PointDirectNeighbor_new_2(dd, NG, 900402, 7006030);
	//	DdNode* ans12 = PointDirectNeighbor_new_2(dd, NG, 101000, 7055000);
	//}
	//end_time_1 = clock();
	//std::cout << "test_Interval for 1 spend times: " << (end_time_1 - start_time_1) * 1000 << "us" << std::endl;
	//
	////start_time_1 = clock();
	////for (int i = 1; i <= 10; i++) {
	////	DdNode* ans1 = PoDiNe(dd, NG, 1, 10000);
	////	DdNode* ans2 = PoDiNe(dd, NG, 10000, 100000);
	////	DdNode* ans3 = PoDiNe(dd, NG, 100000, 500000);
	////
	////	DdNode* ans4 = PoDiNe(dd, NG, 200000, 1000000);
	////	DdNode* ans5 = PoDiNe(dd, NG, 300702, 1550050);
	////	DdNode* ans6 = PoDiNe(dd, NG, 401000, 2000000);
	////
	////	DdNode* ans7 = PoDiNe(dd, NG, 500010, 3005000);
	////	DdNode* ans8 = PoDiNe(dd, NG, 600010, 4005000);
	////	DdNode* ans9 = PoDiNe(dd, NG, 700050, 5002000);
	////
	////	DdNode* ans10 = PoDiNe(dd, NG, 800660, 6005020);
	////	DdNode* ans11 = PoDiNe(dd, NG, 900402, 7006030);
	////	DdNode* ans12 = PoDiNe(dd, NG, 101000, 7055000);
	////	
	////}
	////end_time_1 = clock();
	////std::cout << "test_point for 1 spend times: " << (end_time_1 - start_time_1) * 1000 << "us" << std::endl;
	////
	////
	////
 //   // test for 2
	////start_time_2 = clock();
	////for (int i = 1; i <= 10; i++){
	////	
	////	bool ans1 = PoAcEd(dd, NG, 1,2, 10000);
	////	bool ans2 = PoAcEd(dd, NG, 10000,10120, 100000);
	////	bool ans3 = PoAcEd(dd, NG, 100000, 100520,500000);
	////
	////	bool ans4 = PoAcEd(dd, NG, 200000, 200620,1000000);
	////	bool ans5 = PoAcEd(dd, NG, 300702,300730, 1550050);
	////	bool ans6 = PoAcEd(dd, NG, 401000,401200, 2000000);
	////
	////	bool ans7 = PoAcEd(dd, NG, 500010, 500020,3005000);
	////	bool ans8 = PoAcEd(dd, NG, 600010,600120, 4005000);
	////	bool ans9 = PoAcEd(dd, NG, 700050,700520 ,5002000);
	////
	////	bool ans10 = PoAcEd(dd, NG, 800660, 800620,6005020);
	////	bool ans11 = PoAcEd(dd, NG, 900402, 900730,7006030);
	////	bool ans12 = PoAcEd(dd, NG, 101000, 100700,7055000);
	////}
	////end_time_2 = clock();
	////std::cout << "test_point for 2 spend times: " << (end_time_2 - start_time_2) * 1000 << "us" << std::endl;
	////
	//start_time_2 = clock();
	//for (int i = 1; i <= 10; i++) {
	//	bool ans1 = PointActiveEdge_new_2(dd, NG, 1, 2, 10000);
	//	bool ans2 = PointActiveEdge_new_2(dd, NG, 10000, 10120, 100000);
	//	bool ans3 = PointActiveEdge_new_2(dd, NG, 100000, 100520, 500000);
 //   
	//	bool ans4 = PointActiveEdge_new_2(dd, NG, 200000, 200620, 1000000);
	//	bool ans5 = PointActiveEdge_new_2(dd, NG, 300702, 300730, 1550050);
	//	bool ans6 = PointActiveEdge_new_2(dd, NG, 401000, 401200, 2000000);
 //   
	//	bool ans7 = PointActiveEdge_new_2(dd, NG, 500010, 500020, 3005000);
	//	bool ans8 = PointActiveEdge_new_2(dd, NG, 600010, 600120, 4005000);
	//	bool ans9 = PointActiveEdge_new_2(dd, NG, 700050, 700520, 5002000);
 //    
	//	bool ans10 = PointActiveEdge_new_2(dd, NG, 800660, 800620, 6005020);
	//	bool ans11 = PointActiveEdge_new_2(dd, NG, 900402, 900730, 7006030);
	//	bool ans12 = PointActiveEdge_new_2(dd, NG, 101000, 100700, 7055000);

	//}
	//end_time_2 = clock();
	//std::cout << "test_Interval for 2 spend times: " << (end_time_2 - start_time_2) * 1000 << "us" << std::endl;

	//

	////test for 3
	//start_time_3 = clock();
	//for (int i = 1; i <= 1; i++) {
	//	Snapshot_new_2(dd, NG, 10000);
	//	Snapshot_new_2(dd, NG, 100000);
	//	Snapshot_new_2(dd, NG, 500000);
//
	//	Snapshot_new_2(dd, NG, 1000000);
	//	Snapshot_new_2(dd, NG, 1550050);
	//	Snapshot_new_2(dd, NG, 2000000);
//
	//	Snapshot_new_2(dd, NG, 3005000);
	//	Snapshot_new_2(dd, NG, 4005000);
	//	Snapshot_new_2(dd, NG, 5002000);
//
	//	Snapshot_new_2(dd, NG, 6005020);
	//	Snapshot_new_2(dd, NG, 7006030);
	//	Snapshot_new_2(dd, NG, 7055000);
//
	//}
	//end_time_3 = clock();
	//std::cout << "test_Interval for 3 spend times: " << (end_time_3 - start_time_3) * 1000 << "us" << std::endl;
//
//
	//start_time_3 = clock();
	//for (int i = 1; i <= 10; i++) {
	//	Sns(dd, NG, 10000);
	//	Sns(dd, NG, 100000);
	//	Sns(dd, NG, 500000);
//
	//	Sns(dd, NG, 1000000);
	//	Sns(dd, NG, 1550050);
	//	Sns(dd, NG, 2000000);
//
	//	Sns(dd, NG, 3005000);
	//	Sns(dd, NG, 4005000);
	//	Sns(dd, NG, 5002000);
//
	//	Sns(dd, NG, 6005020);
	//	Sns(dd, NG, 7006030);
	//	Sns(dd, NG, 7055000);
//
	//}
	//end_time_3 = clock();
	//std::cout << "test_point for 3 spend times: " << (end_time_3 - start_time_3) * 1000 << "us" << std::endl;
//
	// test for 4
//
	/*start_time_4 = clock();
	for (int i = 1; i <= 10; i++) {
		ChangedEdgesPoint_new_2(dd, NG, 10000);
		ChangedEdgesPoint_new_2(dd, NG, 100000);
		ChangedEdgesPoint_new_2(dd, NG, 500000);

		ChangedEdgesPoint_new_2(dd, NG, 1000000);
		ChangedEdgesPoint_new_2(dd, NG, 1550050);
		ChangedEdgesPoint_new_2(dd, NG, 2000000);

		ChangedEdgesPoint_new_2(dd, NG, 3005000);
		ChangedEdgesPoint_new_2(dd, NG, 4005000);
		ChangedEdgesPoint_new_2(dd, NG, 5002000);

		ChangedEdgesPoint_new_2(dd, NG, 6005020);
		ChangedEdgesPoint_new_2(dd, NG, 7006030);
		ChangedEdgesPoint_new_2(dd, NG, 7055000);
	}
	
	end_time_4 = clock();
	std::cout << "test_point for 4 spend times: " << (end_time_4 - start_time_4) * 1000 << "us" << std::endl;
		
	*/
	//DdManager* dd;
	//dd = Cudd_Init(0, 0, 256, CUDD_CACHE_SLOTS, 0);
	//DdNode** t = ALLOC(DdNode*, 2);
	//int level = 0;
	//// 创建t变量
	//for (int i = 0; i < 2; i++) {
	//	t[i] = Cudd_addIthVar(dd, level++);
	//	Cudd_Ref(t[i]);
	//}
	//for (int i = 1; i < 8; i++) {
	//	DdNode* ts = CreateNode(dd, i, 3, t);
	//	DdNode* ans = Cudd_addsigleApply(dd, Make_times, ts);
	//	int j = 0;
	//}
	//DdNode* zero = Cudd_ReadZero(dd);
	//DdNode* one = Cudd_ReadOne(dd);
	//DdNode* x = Cudd_addIthVar(dd, 0); 
	//DdNode* y = Cudd_addIthVar(dd, 1);
	//DdNode* tt_11 = CreateNode(dd, 3, 2, t);
	//DdNode* tt_00 = CreateNode(dd, 0, 2, t);
	//
	//DdNode* ans = Cudd_addRestrict(dd, x, one);
	//DdNode* an = Cudd_addConstrain(dd, xy, ans);
	//DdNode* test = Cudd_addConst(dd, 1);
	//DdNode* vy = Cudd_addApply(dd, Cudd_addPlus, tt_11,tt_00 );
	//DdNode* vyv = cuddAddRestrictRecur(dd, vy, test);
	//vyv->type.kids.T;
	//DdNode* vyy = Cudd_addApply(dd, Cudd_addPlus, vy, tt_11);
	//DdNode* vz = Cudd_addApply(dd, Cudd_addOr, x, y);
	//DdNode* vx = Cudd_addApply(dd, Cudd_addPlus, vy, x);
	//DdNode* p = Cudd_addMonadicApply(dd, Extra_01add_new, vyy);
	//if (cuddIsConstant(p)) std::cout << "I'm p" << std::endl;
	//return 0;


/********************--test_Witate_0--用来比较算法时间性能的数据集**********************************************************************/
    /*test for 1
	start_time_1 = clock();
	for (int i = 1; i <= 10; i++) {
		DdNode* ans1 = intervalDirectNeighbor(dd, NG, 1,1, 10000);
		DdNode* ans2 = intervalDirectNeighbor(dd, NG, 10000, 1, 100000);
		DdNode* ans3 = intervalDirectNeighbor(dd, NG, 100000, 1, 500000);
	
		DdNode* ans4 = intervalDirectNeighbor(dd, NG, 200000, 1, 1000000);
		DdNode* ans5 = intervalDirectNeighbor(dd, NG, 300702, 1, 1550050);
		DdNode* ans6 = intervalDirectNeighbor(dd, NG, 401000, 1, 2000000);
	
		DdNode* ans7 = intervalDirectNeighbor(dd, NG, 500010, 1, 3005000);
		DdNode* ans8 = intervalDirectNeighbor(dd, NG, 600010, 1, 4005000);
		DdNode* ans9 = intervalDirectNeighbor(dd, NG, 700050, 1, 5002000);
	
		DdNode* ans10 = intervalDirectNeighbor(dd, NG, 800660, 1, 6005020);
		DdNode* ans11 = intervalDirectNeighbor(dd, NG, 900402, 1, 7006030);
		DdNode* ans12 = intervalDirectNeighbor(dd, NG, 101000, 1, 7055000);
	}
	end_time_1 = clock();
	std::cout << "test_Interval for 1 spend times: " << (end_time_1 - start_time_1) * 1000 << "us" << std::endl;
	
	test for 2
	start_time_2 = clock();
	for (int i = 1; i <= 10; i++) {
		bool ans1 = intervalActiveEdge(dd, NG, 1, 2, 1, 10000);
		bool ans2 = intervalActiveEdge(dd, NG, 10000, 10120, 1, 100000);
		bool ans3 = intervalActiveEdge(dd, NG, 100000, 100520, 1, 500000);
    
		bool ans4 = intervalActiveEdge(dd, NG, 200000, 200620, 1, 1000000);
		bool ans5 = intervalActiveEdge(dd, NG, 300702, 300730, 1, 1550050);
		bool ans6 = intervalActiveEdge(dd, NG, 401000, 401200, 1, 2000000);
    
		bool ans7 = intervalActiveEdge(dd, NG, 500010, 500020, 1, 3005000);
		bool ans8 = intervalActiveEdge(dd, NG, 600010, 600120, 1, 4005000);
		bool ans9 = intervalActiveEdge(dd, NG, 700050, 700520, 1, 5002000);
     
		bool ans10 = intervalActiveEdge(dd, NG, 800660, 800620, 1, 6005020);
		bool ans11 = intervalActiveEdge(dd, NG, 900402, 900730, 1, 7006030);
		bool ans12 = intervalActiveEdge(dd, NG, 101000, 100700, 1, 7055000);

	}
	end_time_2 = clock();
	std::cout << "test_Interval for 2 spend times: " << (end_time_2 - start_time_2) * 1000 << "us" << std::endl;

	 test for 4

	start_time_4 = clock();
	for (int i = 1; i <= 10; i++) {
		changedEdgesInterval(dd, NG, 1, 10000);
		changedEdgesInterval(dd, NG, 1, 100000);
		changedEdgesInterval(dd, NG, 1, 500000);

		changedEdgesInterval(dd, NG, 1, 1000000);
		changedEdgesInterval(dd, NG, 1, 1550050);
		changedEdgesInterval(dd, NG, 1, 2000000);

		changedEdgesInterval(dd, NG, 1, 3005000);
		changedEdgesInterval(dd, NG, 1, 4005000);
		changedEdgesInterval(dd, NG, 1, 5002000);

		changedEdgesInterval(dd, NG, 1, 6005020);
		changedEdgesInterval(dd, NG, 1, 7006030);
		changedEdgesInterval(dd, NG, 1, 7055000);
	}

	end_time_4 = clock();
	std::cout << "test_point for 4 spend times: " << (end_time_4 - start_time_4) * 1000 << "us" << std::endl;*/

/********************--test_Witate_0--点的位置影响*******************************************************************************************************/

	////test for 3
	//start_time_3 = clock();
	//for (int i = 1; i <= 1; i++) {
	//	
	//	Snapshot_new_2(dd, NG, 1760000);

	//	
	//	Snapshot_new_2(dd, NG, 3530000);

	//	
	//	Snapshot_new_2(dd, NG, 5290000);

	//	
	//	Snapshot_new_2(dd, NG, 7055000);

	//}
	//end_time_3 = clock();
	//std::cout << "test_Interval for 3 spend times: " << (end_time_3 - start_time_3) * 1000 << "us" << std::endl;




/*********************--test_super_0--用来比较算法时间性能的数据集合**************************************************************************************************************************/



 //   //test for 1
	//start_time_1 = clock();
	//for (int i = 1; i <= 365; i++) {
	//	DdNode* ans1 = PointDirectNeighbor_new_1(dd, NG, 1, 50000);
	//	DdNode* ans2 = PointDirectNeighbor_new_1(dd, NG, 10000, 100000);
	//	DdNode* ans3 = PointDirectNeighbor_new_1(dd, NG, 100000, 250000);
	//
	//	DdNode* ans4 = PointDirectNeighbor_new_1(dd, NG, 200000, 400000);
	//	DdNode* ans5 = PointDirectNeighbor_new_1(dd, NG, 300702, 550050);
	//	DdNode* ans6 = PointDirectNeighbor_new_1(dd, NG, 401000, 600000);
	//
	//	DdNode* ans7 = PointDirectNeighbor_new_1(dd, NG, 500010, 750500);
	//	DdNode* ans8 = PointDirectNeighbor_new_1(dd, NG, 600010, 900500);
	//	DdNode* ans9 = PointDirectNeighbor_new_1(dd, NG, 700050, 1050200);
	//
	//	DdNode* ans10 = PointDirectNeighbor_new_1(dd, NG, 800660, 1205020);
	//	DdNode* ans11 = PointDirectNeighbor_new_1(dd, NG, 900402, 1356030);
	//	DdNode* ans12 = PointDirectNeighbor_new_1(dd, NG, 101000, 1405000);
	//}
	//end_time_1 = clock();
	//std::cout << "test_Interval for 1 spend times: " << (end_time_1 - start_time_1) * 1000 << "us" << std::endl;

	//// test for 2
	//start_time_2 = clock();
	//for (int i = 1; i <= 10; i++) {
	//	bool ans1 = PointActiveEdge_new_1(dd, NG, 1, 2, 50000);
	//	bool ans2 = PointActiveEdge_new_1(dd, NG, 10000, 10120, 100000);
	//	bool ans3 = PointActiveEdge_new_1(dd, NG, 100000, 100520, 250000);
 //   
	//	bool ans4 = PointActiveEdge_new_1(dd, NG, 200000, 200620, 400000);
	//	bool ans5 = PointActiveEdge_new_1(dd, NG, 300702, 300730, 550050);
	//	bool ans6 = PointActiveEdge_new_1(dd, NG, 401000, 401200, 600000);
 //   
	//	bool ans7 = PointActiveEdge_new_1(dd, NG, 500010, 500020, 750500);
	//	bool ans8 = PointActiveEdge_new_1(dd, NG, 600010, 600120, 900500);
	//	bool ans9 = PointActiveEdge_new_1(dd, NG, 700050, 700520, 1052000);
 //    
	//	bool ans10 = PointActiveEdge_new_1(dd, NG, 800660, 800620, 1205020);
	//	bool ans11 = PointActiveEdge_new_1(dd, NG, 900402, 900730, 1356030);
	//	bool ans12 = PointActiveEdge_new_1(dd, NG, 101000, 100700, 1405000);

	//}
	//end_time_2 = clock();
	//std::cout << "test_Interval for 2 spend times: " << (end_time_2 - start_time_2) * 1000 << "us" << std::endl;


	////test for 3
	//start_time_3 = clock();
	//for (int i = 1; i <= 1; i++) {
	//	Snapshot_new_1(dd, NG, 50000);
	//	Snapshot_new_1(dd, NG, 100000);
	//	Snapshot_new_1(dd, NG, 250000);

	//	Snapshot_new_1(dd, NG, 400000);
	//	Snapshot_new_1(dd, NG, 550050);
	//	Snapshot_new_1(dd, NG, 600000);

	//	Snapshot_new_1(dd, NG, 750500);
	//	Snapshot_new_1(dd, NG, 900500);
	//	Snapshot_new_1(dd, NG, 1052000);

	//	Snapshot_new_1(dd, NG, 1205020);
	//	Snapshot_new_1(dd, NG, 1356030);
	//	Snapshot_new_1(dd, NG, 1405000);

	//}
	//end_time_3 = clock();
	//std::cout << "test_Interval for 3 spend times: " << (end_time_3 - start_time_3) * 1000 << "us" << std::endl;

	////test for 4
	//start_time_4 = clock();
	//for (int i = 1; i <= 365; i++) {
	//	ChangedEdgesPoint_new_2(dd, NG, 50000);
	//	ChangedEdgesPoint_new_2(dd, NG, 100000);
	//	ChangedEdgesPoint_new_2(dd, NG, 250000);

	//	ChangedEdgesPoint_new_2(dd, NG, 400000);
	//	ChangedEdgesPoint_new_2(dd, NG, 550050);
	//	ChangedEdgesPoint_new_2(dd, NG, 600000);

	//	ChangedEdgesPoint_new_2(dd, NG, 755000);
	//	ChangedEdgesPoint_new_2(dd, NG, 905000);
	//	ChangedEdgesPoint_new_2(dd, NG, 1052000);

	//	ChangedEdgesPoint_new_2(dd, NG, 1255020);
	//	ChangedEdgesPoint_new_2(dd, NG, 1356030);
	//	ChangedEdgesPoint_new_2(dd, NG, 1405000);
	//}

	//end_time_4 = clock();
	//std::cout << "test_point for 4 spend times: " << (end_time_4 - start_time_4) * 1000 << "us" << std::endl;

/*********************--test_math_0--用来比较算法时间性能的数据集合**************************************************************************************************************************/

	//test for 1
	//start_time_1 = clock();
	//for (int i = 1; i <= 365; i++) {
	//	DdNode* ans1 = PointDirectNeighbor_new_1(dd, NG, 1100, 1000);
	//	DdNode* ans2 = PointDirectNeighbor_new_1(dd, NG, 3000, 10000);
	//	DdNode* ans3 = PointDirectNeighbor_new_1(dd, NG, 4000, 50000);

	//	DdNode* ans4 = PointDirectNeighbor_new_1(dd, NG, 5000, 100000);
	//	DdNode* ans5 = PointDirectNeighbor_new_1(dd, NG, 7702, 150050);
	//	DdNode* ans6 = PointDirectNeighbor_new_1(dd, NG, 8000, 200000);

	//	DdNode* ans7 = PointDirectNeighbor_new_1(dd, NG, 10010, 255000);
	//	DdNode* ans8 = PointDirectNeighbor_new_1(dd, NG, 11010, 305000);
	//	DdNode* ans9 = PointDirectNeighbor_new_1(dd, NG, 15050, 352000);

	//	DdNode* ans10 = PointDirectNeighbor_new_1(dd, NG, 17660, 405020);
	//	DdNode* ans11 = PointDirectNeighbor_new_1(dd, NG, 20402, 456030);
	//	DdNode* ans12 = PointDirectNeighbor_new_1(dd, NG, 24000, 505000);
	//}
	//end_time_1 = clock();
	//std::cout << "test_Interval for 1 spend times: " << (end_time_1 - start_time_1) * 1000 << "us" << std::endl;

	//// test for 2
	//start_time_2 = clock();
	//for (int i = 1; i <= 365; i++) {
	//	bool ans1 = PointActiveEdge_new_1(dd, NG, 1, 2, 1000);
	//	bool ans2 = PointActiveEdge_new_1(dd, NG, 3000, 3110, 10000);
	//	bool ans3 = PointActiveEdge_new_1(dd, NG, 4000, 4520, 50000);

	//	bool ans4 = PointActiveEdge_new_1(dd, NG, 5000, 5620, 100000);
	//	bool ans5 = PointActiveEdge_new_1(dd, NG, 7702, 7730, 150050);
	//	bool ans6 = PointActiveEdge_new_1(dd, NG, 8000, 8200, 200000);

	//	bool ans7 = PointActiveEdge_new_1(dd, NG, 10010, 10020, 250500);
	//	bool ans8 = PointActiveEdge_new_1(dd, NG, 11010, 11020, 305000);
	//	bool ans9 = PointActiveEdge_new_1(dd, NG, 15050, 15520, 352000);

	//	bool ans10 = PointActiveEdge_new_1(dd, NG, 17660, 17620, 405020);
	//	bool ans11 = PointActiveEdge_new_1(dd, NG, 20402, 20730, 456030);
	//	bool ans12 = PointActiveEdge_new_1(dd, NG, 24000, 24700, 505000);

	//}
	//end_time_2 = clock();
	//std::cout << "test_Interval for 2 spend times: " << (end_time_2 - start_time_2) * 1000 << "us" << std::endl;


	////test for 3
	//start_time_3 = clock();
	//for (int i = 1; i <= 1; i++) {
	//	Snapshot_new_1(dd, NG, 1000);
	//	Snapshot_new_1(dd, NG, 10000);
	//	Snapshot_new_1(dd, NG, 50000);

	//	Snapshot_new_1(dd, NG, 100000);
	//	Snapshot_new_1(dd, NG, 150050);
	//	Snapshot_new_1(dd, NG, 200000);

	//	Snapshot_new_1(dd, NG, 255000);
	//	Snapshot_new_1(dd, NG, 305000);
	//	Snapshot_new_1(dd, NG, 352000);

	//	Snapshot_new_1(dd, NG, 405020);
	//	Snapshot_new_1(dd, NG, 456030);
	//	Snapshot_new_1(dd, NG, 505000);

	//}
	//end_time_3 = clock();
	//std::cout << "test_Interval for 3 spend times: " << (end_time_3 - start_time_3) * 1000 << "us" << std::endl;

	////test for 4
	//start_time_4 = clock();
	//for (int i = 1; i <= 365; i++) {
	//	ChangedEdgesPoint_new_2(dd, NG, 1000);
	//	ChangedEdgesPoint_new_2(dd, NG, 10000);
	//	ChangedEdgesPoint_new_2(dd, NG, 50000);

	//	ChangedEdgesPoint_new_2(dd, NG, 100000);
	//	ChangedEdgesPoint_new_2(dd, NG, 155050);
	//	ChangedEdgesPoint_new_2(dd, NG, 200000);

	//	ChangedEdgesPoint_new_2(dd, NG, 255000);
	//	ChangedEdgesPoint_new_2(dd, NG, 305000);
	//	ChangedEdgesPoint_new_2(dd, NG, 352000);

	//	ChangedEdgesPoint_new_2(dd, NG, 405020);
	//	ChangedEdgesPoint_new_2(dd, NG, 456030);
	//	ChangedEdgesPoint_new_2(dd, NG, 505000);
	//}

	//end_time_4 = clock();
	//std::cout << "test_point for 4 spend times: " << (end_time_4 - start_time_4) * 1000 << "us" << std::endl;


/*********************--test_ask_0--用来比较算法时间性能的数据集合**************************************************************************************************************************/

	////test for 1
	//start_time_1 = clock();
	//for (int i = 1; i <= 10; i++) {
	//	DdNode* ans1 = PointDirectNeighbor_new_1(dd, NG, 1, 1000);
	//	DdNode* ans2 = PointDirectNeighbor_new_1(dd, NG, 10000, 10000);
	//	DdNode* ans3 = PointDirectNeighbor_new_1(dd, NG, 10000, 50000);

	//	DdNode* ans4 = PointDirectNeighbor_new_1(dd, NG, 20000, 100000);
	//	DdNode* ans5 = PointDirectNeighbor_new_1(dd, NG, 30702, 155050);
	//	DdNode* ans6 = PointDirectNeighbor_new_1(dd, NG, 40000, 200000);

	//	DdNode* ans7 = PointDirectNeighbor_new_1(dd, NG, 50000, 300500);
	//	DdNode* ans8 = PointDirectNeighbor_new_1(dd, NG, 60000, 400500);
	//	DdNode* ans9 = PointDirectNeighbor_new_1(dd, NG, 70050, 502000);

	//	DdNode* ans10 = PointDirectNeighbor_new_1(dd, NG, 80600, 655020);
	//	DdNode* ans11 = PointDirectNeighbor_new_1(dd, NG, 90402, 806030);
	//	DdNode* ans12 = PointDirectNeighbor_new_1(dd, NG, 101000, 955000);
	//}
	//end_time_1 = clock();
	//std::cout << "test_Interval for 1 spend times: " << (end_time_1 - start_time_1) * 1000 << "us" << std::endl;

	//// test for 2
	//start_time_2 = clock();
	//for (int i = 1; i <= 10; i++) {
	//	bool ans1 = PointActiveEdge_new_1(dd, NG, 1, 2, 1000);
	//	bool ans2 = PointActiveEdge_new_1(dd, NG, 1000, 1120, 10000);
	//	bool ans3 = PointActiveEdge_new_1(dd, NG, 10000, 10520, 50000);

	//	bool ans4 = PointActiveEdge_new_1(dd, NG, 20000, 20620, 100000);
	//	bool ans5 = PointActiveEdge_new_1(dd, NG, 30072, 30730, 155050);
	//	bool ans6 = PointActiveEdge_new_1(dd, NG, 40100, 41200, 200000);

	//	bool ans7 = PointActiveEdge_new_1(dd, NG, 50010, 50020, 300500);
	//	bool ans8 = PointActiveEdge_new_1(dd, NG, 60010, 60120, 400500);
	//	bool ans9 = PointActiveEdge_new_1(dd, NG, 70050, 70520, 500200);

	//	bool ans10 = PointActiveEdge_new_1(dd, NG, 8060, 80620, 655020);
	//	bool ans11 = PointActiveEdge_new_1(dd, NG, 9002, 90073, 800600);
	//	bool ans12 = PointActiveEdge_new_1(dd, NG, 101000, 100070, 955000);

	//}
	//end_time_2 = clock();
	//std::cout << "test_Interval for 2 spend times: " << (end_time_2 - start_time_2) * 1000 << "us" << std::endl;


	////test for 3
	//start_time_3 = clock();
	//for (int i = 1; i <= 1; i++) {
	//	Snapshot_new_1(dd, NG, 1000);
	//	Snapshot_new_1(dd, NG, 10000);
	//	Snapshot_new_1(dd, NG, 50000);

	//	Snapshot_new_1(dd, NG, 100000);
	//	Snapshot_new_1(dd, NG, 155050);
	//	Snapshot_new_1(dd, NG, 200000);

	//	Snapshot_new_1(dd, NG, 300500);
	//	Snapshot_new_1(dd, NG, 400500);
	//	Snapshot_new_1(dd, NG, 500200);

	//	Snapshot_new_1(dd, NG, 650500);
	//	Snapshot_new_1(dd, NG, 800630);
	//	Snapshot_new_1(dd, NG, 955000);

	//}
	//end_time_3 = clock();
	//std::cout << "test_Interval for 3 spend times: " << (end_time_3 - start_time_3) * 1000 << "us" << std::endl;

	////test for 4
	//start_time_4 = clock();
	//for (int i = 1; i <= 10; i++) {
	//	ChangedEdgesPoint_new_2(dd, NG, 1000);
	//	ChangedEdgesPoint_new_2(dd, NG, 10000);
	//	ChangedEdgesPoint_new_2(dd, NG, 50000);

	//	ChangedEdgesPoint_new_2(dd, NG, 100000);
	//	ChangedEdgesPoint_new_2(dd, NG, 155000);
	//	ChangedEdgesPoint_new_2(dd, NG, 200000);

	//	ChangedEdgesPoint_new_2(dd, NG, 300500);
	//	ChangedEdgesPoint_new_2(dd, NG, 400500);
	//	ChangedEdgesPoint_new_2(dd, NG, 500200);

	//	ChangedEdgesPoint_new_2(dd, NG, 650200);
	//	ChangedEdgesPoint_new_2(dd, NG, 800630);
	//	ChangedEdgesPoint_new_2(dd, NG, 955000);
	//}

	//end_time_4 = clock();
	//std::cout << "test_point for 4 spend times: " << (end_time_4 - start_time_4) * 1000 << "us" << std::endl;

/*********************--test_super_0--间隔版本---用来比较算法时间性能的数据集合**************************************************************************************************************************/

	////test for 1
	//start_time_1 = clock();
	//for (int i = 1; i <= 365; i++) {
	//	DdNode* ans1 = intervalDirectNeighbor(dd, NG, 1, 1,50000);
	//	DdNode* ans2 = intervalDirectNeighbor(dd, NG, 10000, 1,100000);
	//	DdNode* ans3 = intervalDirectNeighbor(dd, NG, 100000, 1,250000);

	//	DdNode* ans4 = intervalDirectNeighbor(dd, NG, 200000, 1,400000);
	//	DdNode* ans5 = intervalDirectNeighbor(dd, NG, 300702, 1,550050);
	//	DdNode* ans6 = intervalDirectNeighbor(dd, NG, 401000,1, 600000);

	//	DdNode* ans7 = intervalDirectNeighbor(dd, NG, 500010, 1,750000);
	//	DdNode* ans8 = intervalDirectNeighbor(dd, NG, 600010, 1,900500);
	//	DdNode* ans9 = intervalDirectNeighbor(dd, NG, 700050, 1,1052000);

	//	DdNode* ans10 = intervalDirectNeighbor(dd, NG, 800660,1, 1205020);
	//	DdNode* ans11 = intervalDirectNeighbor(dd, NG, 900402,1, 1356030);
	//	DdNode* ans12 = intervalDirectNeighbor(dd, NG, 101000, 1,1405000);
	//}
	//end_time_1 = clock();
	//std::cout << "test_Interval for 1 spend times: " << (end_time_1 - start_time_1) * 1000 << "us" << std::endl;

	//// test for 2
	//start_time_2 = clock();
	//for (int i = 1; i <= 365; i++) {
	//	bool ans1 = intervalActiveEdge(dd, NG, 1, 2, 1,50000);
	//	bool ans2 = intervalActiveEdge(dd, NG, 10000, 10120, 1, 100000);
	//	bool ans3 = intervalActiveEdge(dd, NG, 100000, 100520, 1, 250000);

	//	bool ans4 = intervalActiveEdge(dd, NG, 200000, 200620, 1, 400000);
	//	bool ans5 = intervalActiveEdge(dd, NG, 300702, 300730, 1, 455050);
	//	bool ans6 = intervalActiveEdge(dd, NG, 401000, 401200, 1, 600000);

	//	bool ans7 = intervalActiveEdge(dd, NG, 500010, 500020, 1, 755000);
	//	bool ans8 = intervalActiveEdge(dd, NG, 600010, 600120, 1, 905000);
	//	bool ans9 = intervalActiveEdge(dd, NG, 700050, 700520, 1, 1052000);

	//	bool ans10 = intervalActiveEdge(dd, NG, 800660, 800620, 1, 1205020);
	//	bool ans11 = intervalActiveEdge(dd, NG, 900402, 900730, 1, 1356030);
	//	bool ans12 = intervalActiveEdge(dd, NG, 101000, 100700, 1, 1405000);

	//}
	//end_time_2 = clock();
	//std::cout << "test_Interval for 2 spend times: " << (end_time_2 - start_time_2) * 1000 << "us" << std::endl;


	////test for 4
	//start_time_4 = clock();
	//for (int i = 1; i <= 365; i++) {
	//	changedEdgesInterval(dd, NG, 1,50000);
	//	changedEdgesInterval(dd, NG, 1, 100000);
	//	changedEdgesInterval(dd, NG, 1, 250000);

	//	changedEdgesInterval(dd, NG, 1, 400000);
	//	changedEdgesInterval(dd, NG, 1, 455050);
	//	changedEdgesInterval(dd, NG, 1, 600000);

	//	changedEdgesInterval(dd, NG, 1, 755000);
	//	changedEdgesInterval(dd, NG, 1, 905000);
	//	changedEdgesInterval(dd, NG, 1, 1052000);

	//	changedEdgesInterval(dd, NG, 1, 1205020);
	//	changedEdgesInterval(dd, NG, 1, 1356030);
	//	changedEdgesInterval(dd, NG, 1, 1405000);
	//}

	//end_time_4 = clock();
	//std::cout << "test_point for 4 spend times: " << (end_time_4 - start_time_4) * 1000 << "us" << std::endl;


/*********************--test_math_0--间隔版本---用来比较算法时间性能的数据集合**************************************************************************************************************************/
	
 //   //test for 1
	//start_time_1 = clock();
	//for (int i = 1; i <= 365; i++) {
	//	DdNode* ans1 = intervalDirectNeighbor(dd, NG, 1100, 1,1000);
	//	DdNode* ans2 = intervalDirectNeighbor(dd, NG, 3000, 1, 10000);
	//	DdNode* ans3 = intervalDirectNeighbor(dd, NG, 4000, 1, 50000);

	//	DdNode* ans4 = intervalDirectNeighbor(dd, NG, 5000, 1, 100000);
	//	DdNode* ans5 = intervalDirectNeighbor(dd, NG, 7702, 1, 150050);
	//	DdNode* ans6 = intervalDirectNeighbor(dd, NG, 8000, 1, 200000);

	//	DdNode* ans7 = intervalDirectNeighbor(dd, NG, 10010, 1, 255000);
	//	DdNode* ans8 = intervalDirectNeighbor(dd, NG, 11010, 1, 305000);
	//	DdNode* ans9 = intervalDirectNeighbor(dd, NG, 15050, 1, 352000);

	//	DdNode* ans10 = intervalDirectNeighbor(dd, NG, 17660, 1, 405020);
	//	DdNode* ans11 = intervalDirectNeighbor(dd, NG, 20402, 1, 456030);
	//	DdNode* ans12 = intervalDirectNeighbor(dd, NG, 24000, 1, 505000);
	//}
	//end_time_1 = clock();
	//std::cout << "test_Interval for 1 spend times: " << (end_time_1 - start_time_1) * 1000 << "us" << std::endl;

	//// test for 2
	//start_time_2 = clock();
	//for (int i = 1; i <= 365; i++) {
	//	bool ans1 = intervalActiveEdge(dd, NG, 1, 2,1, 1000);
	//	bool ans2 = intervalActiveEdge(dd, NG, 3000, 3110, 1, 10000);
	//	bool ans3 = intervalActiveEdge(dd, NG, 4000, 4520, 1, 50000);

	//	bool ans4 = intervalActiveEdge(dd, NG, 5000, 5620, 1, 100000);
	//	bool ans5 = intervalActiveEdge(dd, NG, 7702, 7730, 1, 150050);
	//	bool ans6 = intervalActiveEdge(dd, NG, 8000, 8200, 1, 200000);

	//	bool ans7 = intervalActiveEdge(dd, NG, 10010, 10020, 1, 250500);
	//	bool ans8 = intervalActiveEdge(dd, NG, 11010, 11020, 1, 305000);
	//	bool ans9 = intervalActiveEdge(dd, NG, 15050, 15520, 1, 352000);

	//	bool ans10 = intervalActiveEdge(dd, NG, 17660, 17620, 1, 405020);
	//	bool ans11 = intervalActiveEdge(dd, NG, 20402, 20730, 1, 456030);
	//	bool ans12 = intervalActiveEdge(dd, NG, 24000, 24700, 1, 505000);

	//}
	//end_time_2 = clock();
	//std::cout << "test_Interval for 2 spend times: " << (end_time_2 - start_time_2) * 1000 << "us" << std::endl;


	//
	////test for 4
	//start_time_4 = clock();
	//for (int i = 1; i <= 365; i++) {
	//	changedEdgesInterval(dd, NG,1, 1000);
	//	changedEdgesInterval(dd, NG,1, 10000);
	//	changedEdgesInterval(dd, NG, 1, 50000);

	//	changedEdgesInterval(dd, NG, 1, 100000);
	//	changedEdgesInterval(dd, NG, 1, 155050);
	//	changedEdgesInterval(dd, NG, 1, 200000);

	//	changedEdgesInterval(dd, NG, 1, 255000);
	//	changedEdgesInterval(dd, NG, 1, 305000);
	//	changedEdgesInterval(dd, NG, 1, 352000);

	//	changedEdgesInterval(dd, NG, 1, 405020);
	//	changedEdgesInterval(dd, NG, 1, 456030);
	//	changedEdgesInterval(dd, NG, 1, 505000);
	//}

	//end_time_4 = clock();
	//std::cout << "test_point for 4 spend times: " << (end_time_4 - start_time_4) * 1000 << "us" << std::endl;



/*********************--test_ask_0--间隔版本---用来比较算法时间性能的数据集合**************************************************************************************************************************/
	
    //test for 1
	//start_time_1 = clock();
	//for (int i = 1; i <= 365; i++) {
	//	DdNode* ans1 = intervalDirectNeighbor(dd, NG, 1, 1,1000);
	//	DdNode* ans2 = intervalDirectNeighbor(dd, NG, 10000, 1, 10000);
	//	DdNode* ans3 = intervalDirectNeighbor(dd, NG, 10000, 1, 50000);

	//	DdNode* ans4 = intervalDirectNeighbor(dd, NG, 20000, 1, 100000);
	//	DdNode* ans5 = intervalDirectNeighbor(dd, NG, 30702, 1, 155050);
	//	DdNode* ans6 = intervalDirectNeighbor(dd, NG, 40000, 1, 200000);

	//	DdNode* ans7 = intervalDirectNeighbor(dd, NG, 50000, 1, 300500);
	//	DdNode* ans8 = intervalDirectNeighbor(dd, NG, 60000, 1, 400500);
	//	DdNode* ans9 = intervalDirectNeighbor(dd, NG, 70050, 1, 502000);

	//	DdNode* ans10 = intervalDirectNeighbor(dd, NG, 80600, 1, 655020);
	//	DdNode* ans11 = intervalDirectNeighbor(dd, NG, 90402, 1, 806030);
	//	DdNode* ans12 = intervalDirectNeighbor(dd, NG, 101000, 1, 955000);
	//}
	//end_time_1 = clock();
	//std::cout << "test_Interval for 1 spend times: " << (end_time_1 - start_time_1) * 1000 << "us" << std::endl;

	//// test for 2
	//start_time_2 = clock();
	//for (int i = 1; i <= 365; i++) {
	//	bool ans1 = intervalActiveEdge(dd, NG, 1, 2, 1,1000);
	//	bool ans2 = intervalActiveEdge(dd, NG, 1000, 1120, 1, 10000);
	//	bool ans3 = intervalActiveEdge(dd, NG, 10000, 10520, 1, 50000);

	//	bool ans4 = intervalActiveEdge(dd, NG, 20000, 20620, 1, 100000);
	//	bool ans5 = intervalActiveEdge(dd, NG, 30072, 30730, 1, 155050);
	//	bool ans6 = intervalActiveEdge(dd, NG, 40100, 41200, 1, 200000);

	//	bool ans7 = intervalActiveEdge(dd, NG, 50010, 50020, 1, 300500);
	//	bool ans8 = intervalActiveEdge(dd, NG, 60010, 60120, 1, 400500);
	//	bool ans9 = intervalActiveEdge(dd, NG, 70050, 70520, 1, 500200);

	//	bool ans10 = intervalActiveEdge(dd, NG, 8060, 80620, 1, 655020);
	//	bool ans11 = intervalActiveEdge(dd, NG, 9002, 90073, 1, 800600);
	//	bool ans12 = intervalActiveEdge(dd, NG, 101000, 100070, 1, 955000);

	//}
	//end_time_2 = clock();
	//std::cout << "test_Interval for 2 spend times: " << (end_time_2 - start_time_2) * 1000 << "us" << std::endl;


	//
	////test for 4
	//start_time_4 = clock();
	//for (int i = 1; i <= 365; i++) {
	//	changedEdgesInterval(dd, NG,1, 1000);
	//	changedEdgesInterval(dd, NG, 1, 10000);
	//	changedEdgesInterval(dd, NG, 1, 50000);

	//	changedEdgesInterval(dd, NG, 1, 100000);
	//	changedEdgesInterval(dd, NG, 1, 155000);
	//	changedEdgesInterval(dd, NG, 1, 200000);

	//	changedEdgesInterval(dd, NG, 1, 300500);
	//	changedEdgesInterval(dd, NG, 1, 400500);
	//	changedEdgesInterval(dd, NG, 1, 500200);

	//	changedEdgesInterval(dd, NG, 1, 650200);
	//	changedEdgesInterval(dd, NG, 1, 800630);
	//	changedEdgesInterval(dd, NG, 1, 955000);
	//}

	//end_time_4 = clock();
	//std::cout << "test_point for 4 spend times: " << (end_time_4 - start_time_4) * 1000 << "us" << std::endl;

/*********************--test_super_0--间隔长度的影响-----用来比较算法时间性能的数据集合**************************************************************************************************************************/
	////test for 1
	//start_time_1 = clock();
	//for (int i = 1; i <= 10; i++) {
	//	
	//	DdNode* ans3 = intervalDirectNeighbor(dd, NG, 100000, 1, 350000);

	//	
	//	DdNode* ans6 = intervalDirectNeighbor(dd, NG, 100000, 1, 700000);

	//
	//	DdNode* ans9 = intervalDirectNeighbor(dd, NG, 100000, 1, 1052000);

	//	
	//	DdNode* ans12 = intervalDirectNeighbor(dd, NG, 100000, 1, 1405000);
	//}
	//end_time_1 = clock();
	//std::cout << "test_Interval for 1 spend times: " << (end_time_1 - start_time_1) * 1000 << "us" << std::endl;

	//// test for 2
	//start_time_2 = clock();
	//for (int i = 1; i <= 10; i++) {
	//
	//	bool ans3 = intervalActiveEdge(dd, NG, 100000, 100520, 1, 350000);
	//	
	//	bool ans6 = intervalActiveEdge(dd, NG, 100000, 100520, 1, 700000);
	//
	//	bool ans9 = intervalActiveEdge(dd, NG, 100000, 100520, 1, 1052000);

	//	bool ans12 = intervalActiveEdge(dd, NG, 100000, 100520, 1, 1405000);
	//}
	//end_time_2 = clock();
	//std::cout << "test_Interval for 2 spend times: " << (end_time_2 - start_time_2) * 1000 << "us" << std::endl;


	//test for 3
	//start_time_3 = clock();
	/*for (int i = 1; i <= 1; i++) {
		start_time_3 = clock();
		Snapshot_new_2(dd, NG, 350000);
		end_time_3 = clock();
		std::cout << "test for 25% spend times: " << (end_time_3 - start_time_3) * 1000 << "us" << std::endl;

		start_time_3 = clock();
		Snapshot_new_2(dd, NG,  700000);
		end_time_3 = clock();
		std::cout << "test for 50%  spend times: " << (end_time_3 - start_time_3) * 1000 << "us" << std::endl;

		start_time_3 = clock();
		Snapshot_new_2(dd, NG, 1052000);
		end_time_3 = clock();
		std::cout << "test for 75% spend times: " << (end_time_3 - start_time_3) * 1000 << "us" << std::endl;
		
		start_time_3 = clock();
		Snapshot_new_2(dd, NG, 1405000);
		end_time_3 = clock();
		std::cout << "test for 100% spend times: " << (end_time_3 - start_time_3) * 1000 << "us" << std::endl;
	}*/
	//end_time_3 = clock();
	//std::cout << "test_Interval for 3 spend times: " << (end_time_3 - start_time_3) * 1000 << "us" << std::endl;

	//test for 4
	//start_time_4 = clock();
	for (int i = 1; i <= 1; i++) {
		start_time_4 = clock();
		changedEdgesInterval(dd, NG, 1, 350000);
		end_time_4 = clock();
		std::cout << "test_point for 4 spend times: " << (end_time_4 - start_time_4) * 1000 << "us" << std::endl;

		start_time_4 = clock();
		changedEdgesInterval(dd, NG, 1, 700000);
		end_time_4 = clock();
		std::cout << "test_point for 4 spend times: " << (end_time_4 - start_time_4) * 1000 << "us" << std::endl;

		start_time_4 = clock();
		changedEdgesInterval(dd, NG, 1, 1052000);
		end_time_4 = clock();
		std::cout << "test_point for 4 spend times: " << (end_time_4 - start_time_4) * 1000 << "us" << std::endl;

		start_time_4 = clock();
		changedEdgesInterval(dd, NG, 1, 1405000);
		end_time_4 = clock();
		std::cout << "test_point for 4 spend times: " << (end_time_4 - start_time_4) * 1000 << "us" << std::endl;
	}

	//end_time_4 = clock();
	//std::cout << "test_point for 4 spend times: " << (end_time_4 - start_time_4) * 1000 << "us" << std::endl;


/*********************--test_math_0--间隔版本---用来比较算法时间性能的数据集合**************************************************************************************************************************/
	////test for 1
	//start_time_1 = clock();
	//for (int i = 1; i <= 10; i++) {
	//	
	//	DdNode* ans3 = intervalDirectNeighbor(dd, NG, 4000, 1, 125000);

	//	
	//	DdNode* ans6 = intervalDirectNeighbor(dd, NG, 4000, 1, 250000);

	//	
	//	DdNode* ans9 = intervalDirectNeighbor(dd, NG, 4000, 1, 372000);

	//	
	//	DdNode* ans12 = intervalDirectNeighbor(dd, NG, 4000, 1, 505000);
	//}
	//end_time_1 = clock();
	//std::cout << "test_Interval for 1 spend times: " << (end_time_1 - start_time_1) * 1000 << "us" << std::endl;

	//// test for 2
	//start_time_2 = clock();
	//for (int i = 1; i <= 10; i++) {
	//	bool ans3 = intervalActiveEdge(dd, NG, 4000, 4520, 1, 125000);

	//	bool ans6 = intervalActiveEdge(dd, NG, 4000, 4520, 1, 250000);

	//
	//	bool ans9 = intervalActiveEdge(dd, NG, 4000, 4520, 1, 372000);

	//	bool ans12 = intervalActiveEdge(dd, NG, 4000, 4520, 1, 505000);

	//}
	//end_time_2 = clock();
	//std::cout << "test_Interval for 2 spend times: " << (end_time_2 - start_time_2) * 1000 << "us" << std::endl;


	////test for 3
	//start_time_3 = clock();
	//for (int i = 1; i <= 1; i++) {
	//	
	//	Snapshot_new_2(dd, NG, 125000);

	//	
	//	Snapshot_new_2(dd, NG, 250000);

	//	
	//	Snapshot_new_2(dd, NG, 372000);

	//	
	//	Snapshot_new_2(dd, NG, 505000);

	//}
	//end_time_3 = clock();
	//std::cout << "test_Interval for 3 spend times: " << (end_time_3 - start_time_3) * 1000 << "us" << std::endl;

	////test for 4
	//start_time_4 = clock();
	//for (int i = 1; i <= 10; i++) {
	//	changedEdgesInterval(dd, NG, 1, 125000);

	//	
	//	changedEdgesInterval(dd, NG, 1, 250000);

	//	
	//	changedEdgesInterval(dd, NG, 1, 372000);

	//	
	//	changedEdgesInterval(dd, NG, 1, 505000);
	//}

	//end_time_4 = clock();
	//std::cout << "test_point for 4 spend times: " << (end_time_4 - start_time_4) * 1000 << "us" << std::endl;


/*********************--test_ask_0--间隔版本---用来比较算法时间性能的数据集合**************************************************************************************************************************/
	////test for 1
	//start_time_1 = clock();
	//for (int i = 1; i <= 10; i++) {
	//	
	//	DdNode* ans3 = intervalDirectNeighbor(dd, NG, 10000, 1, 237000);

	//	DdNode* ans6 = intervalDirectNeighbor(dd, NG, 10000, 1, 475000);

	//	
	//	DdNode* ans9 = intervalDirectNeighbor(dd, NG, 10000, 1, 712000);

	//	
	//	DdNode* ans12 = intervalDirectNeighbor(dd, NG, 10000, 1, 955000);
	//}
	//end_time_1 = clock();
	//std::cout << "test_Interval for 1 spend times: " << (end_time_1 - start_time_1) * 1000 << "us" << std::endl;

	//// test for 2
	//start_time_2 = clock();
	//for (int i = 1; i <= 10; i++) {
	//	bool ans3 = intervalActiveEdge(dd, NG, 10000, 10520, 1, 237000);

	//
	//	bool ans6 = intervalActiveEdge(dd, NG, 10000, 10520, 1, 475000);

	//	
	//	bool ans9 = intervalActiveEdge(dd, NG, 10000, 10520, 1, 712000);

	//
	//	bool ans12 = intervalActiveEdge(dd, NG, 10000, 10520, 1, 955000);

	//}
	//end_time_2 = clock();
	//std::cout << "test_Interval for 2 spend times: " << (end_time_2 - start_time_2) * 1000 << "us" << std::endl;


	////test for 3
	//start_time_3 = clock();
	//for (int i = 1; i <= 1; i++) {
	//	Snapshot_new_2(dd, NG, 237000);
	//
	//	Snapshot_new_2(dd, NG, 475000);

	//	Snapshot_new_2(dd, NG, 712000);

	//	Snapshot_new_2(dd, NG, 955000);

	//}
	//end_time_3 = clock();
	//std::cout << "test_Interval for 3 spend times: " << (end_time_3 - start_time_3) * 1000 << "us" << std::endl;

	//////test for 4
	////start_time_4 = clock();
	////for (int i = 1; i <= 10; i++) {
	////	
	////	changedEdgesInterval(dd, NG, 1, 237000);

	////	changedEdgesInterval(dd, NG, 1, 475000);

	////	
	////	changedEdgesInterval(dd, NG, 1, 712000);

	////	
	////	changedEdgesInterval(dd, NG, 1, 955000);
	////}

	////end_time_4 = clock();
	////std::cout << "test_point for 4 spend times: " << (end_time_4 - start_time_4) * 1000 << "us" << std::endl;


	Cudd_Quit(dd);
	return 0;
	
}


//#include <iostream>
//#include "util.h"
//#include "cuddInt.h"
//
//using namespace std;
//
//int main()
//{
//		cout << "hello world" << endl;
//		DdManager * manager;
//		DdNode * f, * var, * tmp;
//		int i;
//	
//		manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
//	
//		f = Cudd_ReadOne(manager);
//		Cudd_Ref(f);
//		for (i = 3; i >= 0; i--) {
//				var = Cudd_bddIthVar(manager, i);
//				tmp = Cudd_bddAnd(manager, Cudd_Not(var), f);
//				Cudd_Ref(tmp);
//				Cudd_RecursiveDeref(manager, f);
//				f = tmp;
//		
//	    }
//	
//		cout << "index of manager is : " << Cudd_ReadSize(manager) << endl;
//		cout << "cnt of node is : " << Cudd_ReadNodeCount(manager) << endl;
//		return 0;
//}
