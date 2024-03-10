#include"Graph.h"
#include<vector>
using namespace std;

// 用来查找引用数目不匹配的程序
void CheckOpen(DdNode* node, const char* file, int line) {
    Cudd_Ref(node);
    const char* open = "open";
    FILE* fp = fopen("../check/check.txt", "a");
    if (fp != NULL) {
        fprintf(fp, "%p %s %s %d\n", node, open, file, line);
        fclose(fp);
    }
}
void CheckClose(DdManager* manager, DdNode* node, const char* file, int line) {
    Cudd_RecursiveDeref(manager, node);
    const char* close = "close";
    FILE* fp = fopen("../check/check.txt", "a");
    if (fp != NULL) {
        fprintf(fp, "%p %s %s %d\n", node, close, file, line);
        fclose(fp);
    }
}
typedef struct {
    void* address;
    int open_count;
    int close_count;
} Entry;
int FindOrAddEntry(Entry* entries, int* count, void* address) {
    for (int i = 0; i < *count; i++) {
        if (entries[i].address == address) {
            return i;
        }
    }
    entries[*count].address = address;
    entries[*count].open_count = 0;
    entries[*count].close_count = 0;
    (*count)++;
    return *count - 1;
}
void CheckRef() {
    FILE* fp = fopen("../check/check.txt", "r");
    Entry entries[10000000]; // assume max 1000 distinct addresses, adjust as needed
    int entries_count = 0;

    char line[256];
    void* address;
    char type[10];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%p %s", &address, type);
        int index = FindOrAddEntry(entries, &entries_count, address);
        if (strcmp(type, "open") == 0) {
            entries[index].open_count++;
        }
        else if (strcmp(type, "close") == 0) {
            entries[index].close_count++;
        }
    }
    fclose(fp);

    FILE* fp_out = fopen("../check/check_result.txt", "w");
    fp = fopen("../check/check.txt", "r");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%p %s", &address, type);
        int index = FindOrAddEntry(entries, &entries_count, address);
        if ((strcmp(type, "open") == 0 && entries[index].open_count > entries[index].close_count) ||
            (strcmp(type, "close") == 0 && entries[index].close_count > entries[index].open_count)) {
            fprintf(fp_out, "%s", line);
            if (strcmp(type, "open") == 0) {
                entries[index].open_count--;
            }
            else if (strcmp(type, "close") == 0) {
                entries[index].close_count--;
            }
        }
    }

    fclose(fp);
    fclose(fp_out);
    //  if (remove("../check/check.txt") == 0) {
    //    printf("Deleted successfully\n");
    //  } else {
    //    printf("Unable to delete the file\n");
    //  }
    //return 0;
}


// 最后的事件集合 是被引用了
Str_Graph* ReadGraph(DdManager* manager, FILE* f, Str_Graph* G) {

    char c;
    unsigned int nodes, times, i, level = 0, evnum;
    unsigned int flag = 0;
    unsigned int su, sv, st, prest;
    unsigned int count = 0;

    DdNode* p, * q, * events, * zero;

    zero = Cudd_ReadZero(manager);
    Cudd_Ref(events = zero);

    while (1) {

        if (EOF_Test(f)) break;         // 判断是否是空文件

        c = ReadChar(f);

        switch (c) {

        // 读取对整个数据的描述 
        case 'p':

            // 读顶点数,事件，时刻数
            nodes = GetInt(f);
            evnum = GetInt(f);
            times = GetInt(f);

            // 顶点变量与时刻变量个数
            G->nips_v = (int)(ComputeFloorLog2((unsigned int)(nodes)));
            G->nips_t = (int)(ComputeFloorLog2((unsigned int)(times)));
            std::cout << "Total Nodes: " << nodes << "," << "Total Times: " << times << std::endl;

            // 为顶点变量数组 时刻变量数组分配内存
            G->x = ALLOC(DdNode*, G->nips_v);
            if (G->x == NULL) goto failure;

            G->y = ALLOC(DdNode*, G->nips_v);
            if (G->y == NULL) goto failure;

            G->t = ALLOC(DdNode*, G->nips_t);
            if (G->t == NULL) goto failure;

            // 创建t变量
            for (i = 0; i < G->nips_t; i++) {
                G->t[i] = Cudd_addIthVar(manager, level++);
                Cudd_Ref(G->t[i]);
            }
           
            // level = Cudd_ReadPerm(manager, t[i - 1]->index) + 1; /* 不明白为什么是这样给level赋值，perm是干嘛的*/
            // 创建x变量
            for (i = 0; i < G->nips_v; i++) {
                G->x[i] = Cudd_addIthVar(manager, level++);
                Cudd_Ref(G->x[i]);
            }

            //level = Cudd_ReadPerm(manager, x[i - 1]->index) + 1; /* 不明白为什么是这样给level赋值，perm是干嘛的*/
            //创建y变量
            for (i = 0; i < G->nips_v; i++) {
                G->y[i] = Cudd_addIthVar(manager, level++);
                Cudd_Ref(G->y[i]);
            }
            break;

            // 读取每一个事件，创建事件ADD
        case 'e':

            // 读取顶点 时刻
            su = GetInt(f);
            sv = GetInt(f);
            st = GetInt(f);

            //printf("this is node numbers \n");
            //// 将读取的时刻映射到0开始的连续数字
            //if (flag == 0) {     // 代表这是处理的第一个事件
            //    prest = st;
            //    st = count;
            //    flag = 1;
            //}
            //else {
            //    if (st == prest) st = count;
            //    else {
            //        prest = st;
            //        st = ++count;
            //    }
            //}

            // 创建每一条边对应的ADD
            p = CreateADD(manager, st, su, sv, G->nips_v, G->nips_t, G->x, G->y, G->t);
            Cudd_Ref(q = Cudd_addApply(manager, Cudd_addOr, events, p));
            Cudd_RecursiveDeref(manager, p);
            Cudd_RecursiveDeref(manager, events);
            events = q;

            break;

        default:
            Barf("Unexpected case in InputFlowGraph\n");
            break;
        }
        

    }

    G->Events = events;
    G->evnum = evnum;
    G->nodes = nodes;
    G->tinum = times;

    //Cudd_RecursiveDeref(manager, zero);
    return(G);

failure:
    return(NULL);
}


// 提取奇数系数小项的判定法则  搭配特殊的apply函数使用
DdNode* Extra_01add_new(DdManager* dd, DdNode* f)
{
    if (cuddIsConstant(f))
    {
        VALUE_TYPE Va = cuddV(f);

        CUDD_VALUE_TYPE value = (Va % 2 != 0) ? 1 : 0;
        DdNode* res = cuddUniqueConst(dd, value);
        return (res);
    }
    return (NULL);
}


DdNode* Make_times(DdManager* dd, DdNode* f,int flag){
    if (cuddIsConstant(f) && flag == 1) return f;

    if(cuddIsConstant(f) && flag == 2)
    {
        VALUE_TYPE Va = cuddV(f);

        if (Va == 1) return f;
        CUDD_VALUE_TYPE value = (Va == 0) ? 1 : 0;
        DdNode* res = cuddUniqueConst(dd, value);
        return (res);
    }
    return (NULL);
}


DdNode* cuddAddsigleApplyRecur(DdManager* dd, DD_MA op, DdNode* f, int flag){
    DdNode* res, * ft, * fe, * T, * E;
    unsigned int index;

    /* Check terminal cases. */
    statLine(dd);
    res = (*op)(dd, f, flag);
    if (res != NULL) return(res);

    /* Check cache. */
   // res = cuddCacheLookup1(dd,op,f);
    //if (res != NULL) return(res);

    /* Recursive step. */
    index = f->index;
    ft = cuddT(f);
    fe = cuddE(f);

    T = cuddAddsigleApplyRecur(dd, op, ft, 1);
    if (T == NULL) return(NULL);
    cuddRef(T);

    E = cuddAddsigleApplyRecur(dd, op, fe, 2);
    if (E == NULL) {
        Cudd_RecursiveDeref(dd, T);
        return(NULL);
    }
    cuddRef(E);

    res = (T == E) ? T : cuddUniqueInter(dd, (int)index, T, E);
    if (res == NULL) {
        Cudd_RecursiveDeref(dd, T);
        Cudd_RecursiveDeref(dd, E);
        return(NULL);
    }
    cuddDeref(T);
    cuddDeref(E);

    /* Store result. */
    //cuddCacheInsert1(dd,op,f,res);

    return(res);

} /* end of cuddAddMonadicApplyRecur */


DdNode* Cudd_addsigleApply(DdManager* dd, DD_MA op, DdNode* f){
    DdNode* res;

    do {
        dd->reordered = 0;
        res = cuddAddsigleApplyRecur(dd, op, f, 0);
    } while (dd->reordered == 1);
    return(res);

} /* end of Cudd_addMonadicApply */


// 最后的结点ADD  是被引用了
DdNode* CreateNode(DdManager* manager, unsigned int v, unsigned int npis, DdNode** x)
{
    DdNode* p, * one, * zero, * func;
    unsigned int i, j, pos;

    one = Cudd_ReadOne(manager);
    zero = Cudd_ReadZero(manager);

    i = npis - 1;
    Cudd_Ref(func = one);

    for (j = 0; j <= i; j++)
    {
        pos = v & (int)pow(2, j);
        if (pos == 0)
            Cudd_Ref(p = Cudd_addIte(manager, x[i - j], zero, func));
        else Cudd_Ref(p = Cudd_addIte(manager, func, x[i - j], zero));
        Cudd_RecursiveDeref(manager, func);
        func = p;
    }

    return (func);
}


// 最后的事件ADD  是被引用了
DdNode* CreateADD(DdManager* manager, unsigned int st,unsigned int su,unsigned int sv, unsigned int npis_v,unsigned int npis_t, DdNode** x, DdNode** y,DdNode** t)
{
    DdNode* xunc, * yunc, * E, * p, * tunc, * q;
    DdNode* zero;

    zero = Cudd_ReadZero(manager);
    tunc = CreateNode(manager, st, npis_t, t);
    xunc = CreateNode(manager, su, npis_v, x);
    yunc = CreateNode(manager, sv, npis_v, y);

    //Cudd_Ref(p = Cudd_addSwapVariables(manager, wfunc, x, y, npis_v));  //  总是有一点疑问，为什么要这样，直接在y上做不行吗？  变量序和perm有什么关系？？
    q = Cudd_addIte(manager, xunc, yunc, zero);
    //Cudd_RecursiveDeref(manager, p);
    Cudd_Ref(q);
    Cudd_RecursiveDeref(manager, xunc);
    Cudd_RecursiveDeref(manager, yunc);

    E = Cudd_addIte(manager, tunc, q, zero);
    Cudd_Ref(E);
    Cudd_RecursiveDeref(manager, q);
    Cudd_RecursiveDeref(manager, tunc);
   

    //CheckOpen(E, "D:\\vcr\\test\\ReadGraph.cpp", 263);
    return (E);
}


/*************** 对文件的一些比较琐碎的操作 ************************************************************/
void Barf(const char* s)
{
    fprintf(stderr, "%s\n", s);
    exit(-1);
}


char ReadChar(FILE* f){
    char c;

    do {
        c = getc(f);
    } while (isspace(c));
    return c;
}


int EOF_Test(FILE* f){
    char c;

    c = ReadChar(f);
    if (c == EOF)
        return TRUE;
    ungetc(c, f);
    return FALSE;
}


void SkipLine(FILE* f){
    char c;

    do
        c = getc(f);
    while (c != EOF && c != '\n');

}


void Skip(FILE* f)
//跳过空白字符
{
    char c;

    while (isspace(c = getc(f)))	//一直读入字符，直到读到非空白字符
        ;
    ungetc(c, f);
}


void GetString(FILE* f, char* buff)
//将非空白字符名称存入buff数组,直到遇到空白字符
{
    char c;

    Skip(f);
    while (!isspace(c = getc(f)))
        *buff++ = c;
    *buff = 0;
}


int GetInt(FILE* f)
//读到一个非空白字符，将其转化为int类型返回
{
    char c;
    int v, sign;

    c = ReadChar(f);
    sign = FALSE;
    v = 0;

    if (c == '-') {
        sign = TRUE;
        c = getc(f);
    }
    while (isdigit(c)) {
        v = 10 * v + (c - '0');
        c = getc(f);
    }
    if (sign)
        v = -1 * v;

    ungetc(c, f);
    return v;
}


int ComputeFloorLog2(unsigned int value)
//求二叉树的深度
{
    int floorLog = 0;
    unsigned int copyv;
    copyv = value;
    while (value > 1)
    {
        floorLog++;
        value >>= 1;
    }
    if (copyv == (unsigned int)pow(2, floorLog))
        return (floorLog);
    else return(floorLog + 1);
}


void ErrorReport()
{
    printf("memory alloc error!");
    exit(-1);
}


FILE* OpenFile(const char* c)
{
    FILE* f;

    if ((f = fopen(c, "r")) != NULL)
        return f;

}


/*********************** 基于自己的模型所实现的基本操作函数 ***************************************************/
// 接下来的操作函数 要用到这些来做提取操作
static DdNode* xcube, * ycube,*tcube; 


//  最后的顶点集合的ADD 还是被引用了  |    这里做了一些改进 调整了一些步骤顺序，以期望降低要操作的集合大小


//DdNode* PointDirectNeighbor(DdManager* dd, Str_Graph* G, unsigned int u, unsigned int t) {
//
//    DdNode* ts, * temp, * p, * q, * zero, * ans;
//    Cudd_Ref(zero = Cudd_ReadZero(dd));
//
//    // 得到<=t时刻的所有时刻点的ADD集合
//    Cudd_Ref(ts = zero);
//    /*for (int i = 0; i <= t; i++) {
//        p = CreateNode(dd, i, G->nips_t, G->t);
//        Cudd_Ref(q = Cudd_addApply(dd, Cudd_addOr, ts, p));
//        Cudd_RecursiveDeref(dd, p);
//        Cudd_RecursiveDeref(dd, ts);
//        ts = q;
//    }*/
//    p = CreateNode(dd, t, G->nips_t, G->t);
//    ts = Cudd_addsigleApply(dd, Make_times, p);
//    Cudd_RecursiveDeref(dd, p);
//
//   
//    // 时刻集合 and 事件集合 ->  时刻集合下的所有事件集合
//    DdNode* tes = Cudd_addIte(dd, ts, G->Events, zero);
//    Cudd_RecursiveDeref(dd, ts);
//    Cudd_Ref(tes);
//
//    // 提取所有时刻变量t -> 时刻集合下的所有事件ADD（中间会有相同事件的合并）
//    Cudd_Ref(tcube = Cudd_addComputeCube(dd, G->t, NULL, G->nips_t));
//    Cudd_Ref(q = Cudd_addExistAbstract(dd, tes, tcube));
//    Cudd_RecursiveDeref(dd, tes);
//
//    // 创建顶点u的ADD and q -> 以顶点u为起点的事件集合
//    p = CreateNode(dd, u, G->nips_v, G->x);
//    Cudd_Ref(temp = Cudd_addIte(dd, p, q, zero));
//    Cudd_RecursiveDeref(dd, q);
//    Cudd_RecursiveDeref(dd, p);
//
//    // 提取奇数系数小项  提取x变量 -> 所有邻居顶点对应的ADD
//    Cudd_Ref(q = Cudd_addMonadicApply(dd, Extra_01add_new, temp));
//    Cudd_Ref(xcube = Cudd_addComputeCube(dd, G->x, NULL, G->nips_v));
//    Cudd_Ref(ans = Cudd_addExistAbstract(dd, q, xcube));
//
//    Cudd_RecursiveDeref(dd, temp);
//    Cudd_RecursiveDeref(dd, tcube);
//    Cudd_RecursiveDeref(dd, zero);
//    Cudd_RecursiveDeref(dd, q);
//    Cudd_RecursiveDeref(dd, xcube);
//    return ans;
//}

DdNode* PointDirectNeighbor(DdManager* dd, Str_Graph* G, unsigned int u, unsigned int t) {

    DdNode* ts, * temp, * p, * q, * zero, * ans;
    Cudd_Ref(zero = Cudd_ReadZero(dd));

    // 得到<=t时刻的所有时刻点的ADD集合
    Cudd_Ref(ts = zero);
    /*for (int i = 0; i <= t; i++) {
        p = CreateNode(dd, i, G->nips_t, G->t);
        Cudd_Ref(q = Cudd_addApply(dd, Cudd_addOr, ts, p));
        Cudd_RecursiveDeref(dd, p);
        Cudd_RecursiveDeref(dd, ts);
        ts = q;
    }*/
    p = CreateNode(dd, t, G->nips_t, G->t);
    ts = Cudd_addsigleApply(dd, Make_times, p);
    Cudd_RecursiveDeref(dd, p);

    
    // 时刻集合 and 事件集合 ->  时刻集合下的所有事件集合
    DdNode* tes = Cudd_addIte(dd, ts, G->Events, zero);
    Cudd_Ref(tes);
    Cudd_RecursiveDeref(dd, ts);

    
    // 创建顶点u的ADD and q -> 以顶点u为起点的事件集合
    p = CreateNode(dd, u, G->nips_v, G->x);
    Cudd_Ref(temp = Cudd_addIte(dd, p, tes, zero));
    Cudd_RecursiveDeref(dd, p);
    Cudd_RecursiveDeref(dd, tes);

    
    // 提取所有时刻变量t -> 时刻集合下的所有事件ADD（中间会有相同事件的合并）
    Cudd_Ref(tcube = Cudd_addComputeCube(dd, G->t, NULL, G->nips_t));
    Cudd_Ref(q = Cudd_addExistAbstract(dd, temp, tcube));
    Cudd_RecursiveDeref(dd, temp);

    // 提取奇数系数小项  提取x变量 -> 所有邻居顶点对应的ADD
    Cudd_Ref(p = Cudd_addMonadicApply(dd, Extra_01add_new, q));
    Cudd_RecursiveDeref(dd, q);

    Cudd_Ref(xcube = Cudd_addComputeCube(dd, G->x, NULL, G->nips_v));
    Cudd_Ref(ans = Cudd_addExistAbstract(dd, p, xcube));
    Cudd_RecursiveDeref(dd, p);


    Cudd_RecursiveDeref(dd, tcube);
    Cudd_RecursiveDeref(dd, zero);
    Cudd_RecursiveDeref(dd, xcube);
    
    return ans;
}

//  最后的顶点集合的ADD 还是被引用了

//DdNode* PoDiNe(DdManager* dd, Str_Graph* G, unsigned int u, unsigned int t) {
//
//    DdNode* ts, * temp, * p, * q, * zero, * ans;
//    Cudd_Ref(zero = Cudd_ReadZero(dd));
//
//    // 创建时刻t的ADD
//    Cudd_Ref(ts = CreateNode(dd, t, G->nips_t, G->t));
//
//    // ts and 事件集合 -> 该时刻t下的所有事件集合
//    DdNode* tes = Cudd_addIte(dd, ts, G->Events, zero);
//    Cudd_RecursiveDeref(dd, ts);
//    Cudd_Ref(tes);
//
//    // 提取变量t -> t时刻下的事件集合
//    Cudd_Ref(tcube = Cudd_addComputeCube(dd, G->t, NULL, G->nips_t));
//    Cudd_Ref(q = Cudd_addExistAbstract(dd, tes, tcube));
//    Cudd_RecursiveDeref(dd, tes);
//
//    // 创建顶点u的Add图 p and q -> 以顶点u出发事件集合
//    p = CreateNode(dd, u, G->nips_v, G->x);
//    Cudd_Ref(temp = Cudd_addIte(dd, p, q, zero));
//    Cudd_RecursiveDeref(dd, q);
//    Cudd_RecursiveDeref(dd, p);
//
//    // 提取变量x -> 得到所有正向邻居顶点集合的ADD
//    Cudd_Ref(xcube = Cudd_addComputeCube(dd, G->x, NULL, G->nips_v));
//    Cudd_Ref(ans = Cudd_addExistAbstract(dd, temp, xcube));
//
//    Cudd_RecursiveDeref(dd, tcube);
//    Cudd_RecursiveDeref(dd, xcube);
//    Cudd_RecursiveDeref(dd, temp);
//    Cudd_RecursiveDeref(dd, zero);
//    return ans;
//}

DdNode* PoDiNe(DdManager* dd, Str_Graph* G, unsigned int u, unsigned int t) {

    DdNode* ts, * temp, * p, * q, * zero, * ans;
    Cudd_Ref(zero = Cudd_ReadZero(dd));

    // 创建时刻t的ADD
    Cudd_Ref(ts = CreateNode(dd, t, G->nips_t, G->t));

    
    // ts and 事件集合 -> 该时刻t下的所有事件集合
    DdNode* tes = Cudd_addIte(dd, ts, G->Events, zero);
    Cudd_Ref(tes);
    Cudd_RecursiveDeref(dd, ts);

    
    // 创建顶点u的Add图 p and q -> 以顶点u出发事件集合
    p = CreateNode(dd, u, G->nips_v, G->x);
    Cudd_Ref(temp = Cudd_addIte(dd, p, tes, zero));
    Cudd_RecursiveDeref(dd, p);
    Cudd_RecursiveDeref(dd, tes);

    
    // 提取变量t -> t时刻下的事件集合
    Cudd_Ref(tcube = Cudd_addComputeCube(dd, G->t, NULL, G->nips_t));
    Cudd_Ref(q = Cudd_addExistAbstract(dd, temp, tcube));
    Cudd_RecursiveDeref(dd, temp);

    // 提取变量x -> 得到所有正向邻居顶点集合的ADD
    Cudd_Ref(xcube = Cudd_addComputeCube(dd, G->x, NULL, G->nips_v));
    Cudd_Ref(ans = Cudd_addExistAbstract(dd, q, xcube));

    Cudd_RecursiveDeref(dd, tcube);
    Cudd_RecursiveDeref(dd, xcube);
    Cudd_RecursiveDeref(dd, q);
    Cudd_RecursiveDeref(dd, zero);
    
    return ans;
}


// 引用数目对应


//bool PointActiveEdge(DdManager* dd, Str_Graph* G, unsigned int u, unsigned int v, unsigned int t) {
//
//    DdNode* ts, * temp, * p, * q, * zero, * ans, * r;
//    Cudd_Ref(zero = Cudd_ReadZero(dd));
//
//    // 得到<=t时刻的所有时刻点的ADD集合
//    Cudd_Ref(ts = zero);
//    /*for (int i = 0; i <= t; i++) {
//        p = CreateNode(dd, i, G->nips_t, G->t);
//        Cudd_Ref(q = Cudd_addApply(dd, Cudd_addOr, ts, p));
//        Cudd_RecursiveDeref(dd, p);
//        Cudd_RecursiveDeref(dd, ts);
//        ts = q;
//    }*/
//    p = CreateNode(dd, t, G->nips_t, G->t);
//    ts = Cudd_addsigleApply(dd, Make_times, p);
//    Cudd_RecursiveDeref(dd, p);
//
//    // 时刻集合 and 事件集合 ->  时刻集合下的所有事件集合
//    DdNode* tes = Cudd_addIte(dd, ts, G->Events, zero);
//    Cudd_RecursiveDeref(dd, ts);
//    Cudd_Ref(tes);
//
//    // 提取所有时刻变量t -> 时刻集合下的所有事件ADD（中间会有相同事件的合并）
//    Cudd_Ref(tcube = Cudd_addComputeCube(dd, G->t, NULL, G->nips_t));
//    Cudd_Ref(q = Cudd_addExistAbstract(dd, tes, tcube));
//    Cudd_RecursiveDeref(dd, tes);
//
//    // 创建 边uv 的ADD
//    p = CreateNode(dd, u, G->nips_v, G->x);
//    r = CreateNode(dd, v, G->nips_v, G->y);
//    Cudd_Ref(temp = Cudd_addIte(dd, p, r, zero));
//    Cudd_RecursiveDeref(dd, p);
//    Cudd_RecursiveDeref(dd, r);
//
//    // temp and q 得到边uv 的所有事件
//    Cudd_Ref(p = Cudd_addIte(dd, temp, q, zero));
//    Cudd_RecursiveDeref(dd, temp);
//    Cudd_RecursiveDeref(dd, q);
//
//    // 提取奇数系数小项得到结果ADD 然后通过判断结果ADD是否为空得到状态结果
//    ans = Cudd_addMonadicApply(dd, Extra_01add_new, p);
//    Cudd_RecursiveDeref(dd, p);
//    Cudd_RecursiveDeref(dd, tcube);
//    Cudd_RecursiveDeref(dd, zero);
//    if (cuddIsConstant(ans)) return false;
//    return true;
//}
bool PointActiveEdge(DdManager* dd, Str_Graph* G, unsigned int u, unsigned int v, unsigned int t) {

    DdNode* ts, * temp, * p, * q, * zero, * ans, * r;
    Cudd_Ref(zero = Cudd_ReadZero(dd));

    // 得到<=t时刻的所有时刻点的ADD集合
    Cudd_Ref(ts = zero);
    /*for (int i = 0; i <= t; i++) {
        p = CreateNode(dd, i, G->nips_t, G->t);
        Cudd_Ref(q = Cudd_addApply(dd, Cudd_addOr, ts, p));
        Cudd_RecursiveDeref(dd, p);
        Cudd_RecursiveDeref(dd, ts);
        ts = q;
    }*/
    p = CreateNode(dd, t, G->nips_t, G->t);
    ts = Cudd_addsigleApply(dd, Make_times, p);
    Cudd_RecursiveDeref(dd, p);

    
    // 时刻集合 and 事件集合 ->  时刻集合下的所有事件集合
    DdNode* tes = Cudd_addIte(dd, ts, G->Events, zero);
    Cudd_Ref(tes);
    Cudd_RecursiveDeref(dd, ts);
    
    // 创建 边uv 的ADD
    p = CreateNode(dd, u, G->nips_v, G->x);
    r = CreateNode(dd, v, G->nips_v, G->y);
    Cudd_Ref(temp = Cudd_addIte(dd, p, r, zero));
    Cudd_RecursiveDeref(dd, p);
    Cudd_RecursiveDeref(dd, r);
    
    // temp and q 得到边uv 的所有事件
    Cudd_Ref(p = Cudd_addIte(dd, temp, tes, zero));
    Cudd_RecursiveDeref(dd, temp);
    Cudd_RecursiveDeref(dd, tes);

    
    // 提取所有时刻变量t -> 时刻集合下的所有事件ADD（中间会有相同事件的合并）
    Cudd_Ref(tcube = Cudd_addComputeCube(dd, G->t, NULL, G->nips_t));
    Cudd_Ref(q = Cudd_addExistAbstract(dd, p, tcube));
    Cudd_RecursiveDeref(dd, p);

    // 提取奇数系数小项得到结果ADD 然后通过判断结果ADD是否为空得到状态结果
    ans = Cudd_addMonadicApply(dd, Extra_01add_new, q);
    Cudd_RecursiveDeref(dd, q);
    Cudd_RecursiveDeref(dd, tcube);
    Cudd_RecursiveDeref(dd, zero);
    if (cuddIsConstant(ans)) return false;
    
    return true;
}

//bool PoAcEd(DdManager* dd, Str_Graph* G, unsigned int u, unsigned int v, unsigned int t) {
//
//    DdNode* ts, * temp, * p, * q, * zero, * ans, * r;
//    Cudd_Ref(zero = Cudd_ReadZero(dd));
//
//    // 创建时刻t的ADD
//    Cudd_Ref(ts = CreateNode(dd, t, G->nips_t, G->t));
//
//    // ts and 事件集合 -> 该时刻t下的所有事件集合
//    DdNode* tes = Cudd_addIte(dd, ts, G->Events, zero);
//    Cudd_RecursiveDeref(dd, ts);
//    Cudd_Ref(tes);
//
//    // 提取变量t -> t时刻下的事件集合
//    Cudd_Ref(tcube = Cudd_addComputeCube(dd, G->t, NULL, G->nips_t));
//    Cudd_Ref(q = Cudd_addExistAbstract(dd, tes, tcube));
//    Cudd_RecursiveDeref(dd, tes);
//
//    // 创建 边uv 的ADD
//    p = CreateNode(dd, u, G->nips_v, G->x);
//    r = CreateNode(dd, v, G->nips_v, G->y);
//    Cudd_Ref(temp = Cudd_addIte(dd, p, r, zero));
//    Cudd_RecursiveDeref(dd, p);
//    Cudd_RecursiveDeref(dd, r);
//
//    // temp and q 得到边uv 的所有事件
//    ans = Cudd_addIte(dd, temp, q, zero);
//    Cudd_RecursiveDeref(dd, q);
//    Cudd_RecursiveDeref(dd, temp);
//    Cudd_RecursiveDeref(dd, tcube);
//    Cudd_RecursiveDeref(dd, zero);
//
//    //判断结果ADD是否为空得到状态结果
//    if (cuddIsConstant(ans)) return false;
//    return true;
//
//}

bool PoAcEd(DdManager* dd, Str_Graph* G, unsigned int u, unsigned int v, unsigned int t) {

    DdNode* ts, * temp, * p, * q, * zero, * ans, * r;
    Cudd_Ref(zero = Cudd_ReadZero(dd));

    // 创建时刻t的ADD
    Cudd_Ref(ts = CreateNode(dd, t, G->nips_t, G->t));

    // ts and 事件集合 -> 该时刻t下的所有事件集合
    DdNode* tes = Cudd_addIte(dd, ts, G->Events, zero);
    Cudd_RecursiveDeref(dd, ts);
    Cudd_Ref(tes);

    //// 提取变量t -> t时刻下的事件集合
    //Cudd_Ref(tcube = Cudd_addComputeCube(dd, G->t, NULL, G->nips_t));
    //Cudd_Ref(q = Cudd_addExistAbstract(dd, tes, tcube));
    //Cudd_RecursiveDeref(dd, tes);

    // 创建 边uv 的ADD
    p = CreateNode(dd, u, G->nips_v, G->x);
    r = CreateNode(dd, v, G->nips_v, G->y);
    Cudd_Ref(temp = Cudd_addIte(dd, p, r, zero));
    Cudd_RecursiveDeref(dd, p);
    Cudd_RecursiveDeref(dd, r);

    // temp and q 得到边uv 的所有事件
    ans = Cudd_addIte(dd, temp, tes, zero);
    Cudd_RecursiveDeref(dd, tes);
    //Cudd_RecursiveDeref(dd, q);
    Cudd_RecursiveDeref(dd, temp);
   // Cudd_RecursiveDeref(dd, tcube);
    Cudd_RecursiveDeref(dd, zero);

    //判断结果ADD是否为空得到状态结果
    if (cuddIsConstant(ans)) return false;
    return true;

}

// 最后的边集合的ADD 还是被引用了

//DdNode* Snapshot(DdManager* dd, Str_Graph* G, unsigned int t) {
//
//    DdNode* ts, * temp, * p, * q, * zero, * ans;
//    Cudd_Ref(zero = Cudd_ReadZero(dd));
//
//    // 得到<=t时刻的所有时刻点的ADD集合
//    Cudd_Ref(ts = zero);
//   /* for (int i = 0; i <= t; i++) {
//        p = CreateNode(dd, i, G->nips_t, G->t);
//        Cudd_Ref(q = Cudd_addApply(dd, Cudd_addOr, ts, p));
//        Cudd_RecursiveDeref(dd, p);
//        Cudd_RecursiveDeref(dd, ts);
//        ts = q;
//    }*/
//    p = CreateNode(dd, t, G->nips_t, G->t);
//    ts = Cudd_addsigleApply(dd, Make_times, p);
//    Cudd_RecursiveDeref(dd, p);
//
//    // 时刻集合 and 事件集合 ->  时刻集合下的所有事件集合
//    DdNode* tes = Cudd_addIte(dd, ts, G->Events, zero);
//    Cudd_RecursiveDeref(dd, ts);
//    Cudd_Ref(tes);
//
//    // 提取所有时刻变量t -> 时刻集合下的所有事件ADD（中间会有相同事件的合并）
//    Cudd_Ref(tcube = Cudd_addComputeCube(dd, G->t, NULL, G->nips_t));
//    Cudd_Ref(q = Cudd_addExistAbstract(dd, tes, tcube));
//    Cudd_RecursiveDeref(dd, tes);
//
//    // 提取奇数系数小项 得到所有在t时刻存在的边集合
//    ans = Cudd_addMonadicApply(dd, Extra_01add_new, q);
//    Cudd_RecursiveDeref(dd, tcube);
//    Cudd_RecursiveDeref(dd, q);
//    Cudd_RecursiveDeref(dd, zero);
//    Cudd_Ref(ans);
//    return ans;
//}

DdNode* Snapshot(DdManager* dd, Str_Graph* G, unsigned int t) {

    DdNode* ts, * temp, * p, * q, * zero, * ans;
    Cudd_Ref(zero = Cudd_ReadZero(dd));

    // 得到<=t时刻的所有时刻点的ADD集合
    Cudd_Ref(ts = zero);
    /* for (int i = 0; i <= t; i++) {
         p = CreateNode(dd, i, G->nips_t, G->t);
         Cudd_Ref(q = Cudd_addApply(dd, Cudd_addOr, ts, p));
         Cudd_RecursiveDeref(dd, p);
         Cudd_RecursiveDeref(dd, ts);
         ts = q;
     }*/
    p = CreateNode(dd, t, G->nips_t, G->t);
    ts = Cudd_addsigleApply(dd, Make_times, p);
    Cudd_RecursiveDeref(dd, p);

    // 时刻集合 and 事件集合 ->  时刻集合下的所有事件集合
    DdNode* tes = Cudd_addIte(dd, ts, G->Events, zero);
    Cudd_Ref(tes);
    Cudd_RecursiveDeref(dd, ts);

    
    // 提取所有时刻变量t -> 时刻集合下的所有事件ADD（中间会有相同事件的合并）
    Cudd_Ref(tcube = Cudd_addComputeCube(dd, G->t, NULL, G->nips_t));
    Cudd_Ref(q = Cudd_addExistAbstract(dd, tes, tcube));
    Cudd_RecursiveDeref(dd, tes);

    // 提取奇数系数小项 得到所有在t时刻存在的边集合
    ans = Cudd_addMonadicApply(dd, Extra_01add_new, q);
    Cudd_Ref(ans);
    Cudd_RecursiveDeref(dd, tcube);
    Cudd_RecursiveDeref(dd, q);
    Cudd_RecursiveDeref(dd, zero);
   
    return ans;
}

// 最后的边集合的ADD 也是被引用了
DdNode* Sns(DdManager* dd, Str_Graph* G, unsigned int t) {
    DdNode* ts, * temp, * p, * q, * zero, * ans, * r;
    Cudd_Ref(zero = Cudd_ReadZero(dd));

    // 创建时刻t的ADD
    Cudd_Ref(ts = CreateNode(dd, t, G->nips_t, G->t));

    // ts and 事件集合 -> 该时刻t下的所有事件集合
    DdNode* tes = Cudd_addIte(dd, ts, G->Events, zero);
    Cudd_Ref(tes);
    Cudd_RecursiveDeref(dd, ts);
  
    // 提取变量t -> t时刻下的事件集合
    Cudd_Ref(tcube = Cudd_addComputeCube(dd, G->t, NULL, G->nips_t));
    Cudd_Ref(q = Cudd_addExistAbstract(dd, tes, tcube));
    Cudd_RecursiveDeref(dd, tes);
    Cudd_RecursiveDeref(dd, tcube);
    Cudd_RecursiveDeref(dd, zero);
    return q;
}


DdNode* ChangedEdgesPoint(DdManager* dd, Str_Graph* G, unsigned int t) {
    return Sns(dd, G, t);
}


DdNode* ChEdPo(DdManager* dd, Str_Graph* G, unsigned int t) {
    return Sns(dd, G, t);
}


/******************--新版本(把所有边聚集在每个时刻下)--******************************************************************************/
void Htob(int u, int npis, vector<int>& ans) {
    int temp = u;
    for (int i = npis - 1;i >= 0; i--) {
        ans[i] = temp % 2;
        temp = temp / 2;
    }
}

DdNode* Restrict(DdManager* dd, DdNode* f, int npis, vector<int>& an) {
    DdNode* ans = f;
    for (int i = 0; i < npis; i++) {
        if (an[i] == 0) ans = ans->type.kids.E;
        else ans = ans->type.kids.T;
    }
    Cudd_Ref(ans);
    return ans;
}

Str_Graph* ReadGraph_new(DdManager* manager, FILE* f, Str_Graph* G) {
    char c;
    unsigned int nodes, times, i, level = 0, evnum, counts = 0;
    unsigned int flag = 0;
    unsigned int su, sv, st, prest;
    unsigned int count = 0;

    DdNode* p, * q, * events, * zero, * logs;

    DdNode* xunc, * yunc, * E, * edgp, * tunc, * edgq, * tlogs;
    zero = Cudd_ReadZero(manager);
    events = zero;
    Cudd_Ref(events);
    logs = zero;
    Cudd_Ref(logs);
    clock_t start_time_1, end_time_1;
    start_time_1 = clock();
    while (1) {

        if (EOF_Test(f)) break;         // 判断是否是空文件

        c = ReadChar(f);

        switch (c) {

            // 读取对整个数据的描述 
        case 'p':

            // 读顶点数,事件，时刻数
            nodes = GetInt(f);
            evnum = GetInt(f);
            times = GetInt(f);

            // 顶点变量与时刻变量个数
            G->nips_v = (int)(ComputeFloorLog2((unsigned int)(nodes)));
            G->nips_t = (int)(ComputeFloorLog2((unsigned int)(times)));
            std::cout << "Total Nodes: " << nodes << "," << "Total Times: " << times << std::endl;

            // 为顶点变量数组 时刻变量数组分配内存
            G->x = ALLOC(DdNode*, G->nips_v);
            if (G->x == NULL) goto failure;

            G->y = ALLOC(DdNode*, G->nips_v);
            if (G->y == NULL) goto failure;

            G->t = ALLOC(DdNode*, G->nips_t);
            if (G->t == NULL) goto failure;

            // 创建t变量
            for (i = 0; i < G->nips_t; i++) {
                G->t[i] = Cudd_addIthVar(manager, level++);
                Cudd_Ref(G->t[i]);
            }

            // level = Cudd_ReadPerm(manager, t[i - 1]->index) + 1; /* 不明白为什么是这样给level赋值，perm是干嘛的*/
            // 创建x变量
            for (i = 0; i < G->nips_v; i++) {
                G->x[i] = Cudd_addIthVar(manager, level++);
                Cudd_Ref(G->x[i]);
            }

            //level = Cudd_ReadPerm(manager, x[i - 1]->index) + 1; /* 不明白为什么是这样给level赋值，perm是干嘛的*/
            //创建y变量
            for (i = 0; i < G->nips_v; i++) {
                G->y[i] = Cudd_addIthVar(manager, level++);
                Cudd_Ref(G->y[i]);
            }
            break;

            // 读取每一个事件，创建事件ADD
        case 'e':

            // 读取顶点 时刻
            su = GetInt(f);
            sv = GetInt(f);
            st = GetInt(f);
            counts++;
            if (counts % 100000 == 0) {
                end_time_1 = clock();
                cout << "test for 100000边 spend times: " << (end_time_1 - start_time_1) / 1000 << "s" << std::endl;
                cout << "100000 is done" << endl;
                start_time_1 = end_time_1;
            }
            // 创建每一条边对应的ADD


            tunc = CreateNode(manager, st, G->nips_t, G->t);
            xunc = CreateNode(manager, su, G->nips_v, G->x);
            yunc = CreateNode(manager, sv, G->nips_v, G->y);

            //Cudd_Ref(p = Cudd_addSwapVariables(manager, wfunc, x, y, npis_v));  //  总是有一点疑问，为什么要这样，直接在y上做不行吗？  变量序和perm有什么关系？？
            edgq = Cudd_addIte(manager, xunc, yunc, zero);
            Cudd_Ref(edgq);
            Cudd_RecursiveDeref(manager, xunc);
            Cudd_RecursiveDeref(manager, yunc);
            edgp = Cudd_addApply(manager, Cudd_addPlus, events, edgq);
            Cudd_Ref(edgp);
            Cudd_RecursiveDeref(manager, edgq);
            Cudd_RecursiveDeref(manager, events);

            events = edgp;


            E = Cudd_addIte(manager, events, tunc, zero);
            Cudd_Ref(E);
            Cudd_RecursiveDeref(manager, tunc);
            tlogs = Cudd_addApply(manager, Cudd_addOr, logs, E);
            Cudd_Ref(tlogs);
            Cudd_RecursiveDeref(manager, logs);
            Cudd_RecursiveDeref(manager, E);
            logs = tlogs;

            break;

        default:
            Barf("Unexpected case in InputFlowGraph\n");
            break;
        }


//    char c;
//    unsigned int nodes, times, i, level = 0, evnum;
//    unsigned int flag = 0;
//    unsigned int su, sv, st, prest;
//    unsigned int count = 0;
//
//    DdNode* p, * q, * events, * zero, * logs;
//
//    DdNode* xunc, * yunc, * E, * edgp, * tunc, * edgq, * tlogs;
//    /*zero = Cudd_ReadZero(manager);
//    Cudd_Ref(events = zero);
//    Cudd_Ref(logs = zero);*/
//    zero = Cudd_ReadZero(manager);
//    events = zero;
//    Cudd_Ref(events);
//    logs = zero;
//    Cudd_Ref(logs);
//    
//    while (1) {
//
//        if (EOF_Test(f)) break;         // 判断是否是空文件
//
//        c = ReadChar(f);
//
//        switch (c) {
//
//            // 读取对整个数据的描述 
//        case 'p':
//
//            // 读顶点数,事件，时刻数
//            nodes = GetInt(f);
//            evnum = GetInt(f);
//            times = GetInt(f);
//
//            // 顶点变量与时刻变量个数
//            G->nips_v = (int)(ComputeFloorLog2((unsigned int)(nodes)));
//            G->nips_t = (int)(ComputeFloorLog2((unsigned int)(times)));
//            std::cout << "Total Nodes: " << nodes << "," << "Total Times: " << times << std::endl;
//
//            // 为顶点变量数组 时刻变量数组分配内存
//            G->x = ALLOC(DdNode*, G->nips_v);
//            if (G->x == NULL) goto failure;
//
//            G->y = ALLOC(DdNode*, G->nips_v);
//            if (G->y == NULL) goto failure;
//
//            G->t = ALLOC(DdNode*, G->nips_t);
//            if (G->t == NULL) goto failure;
//
//            // 创建t变量
//            for (i = 0; i < G->nips_t; i++) {
//                G->t[i] = Cudd_addIthVar(manager, level++);
//                Cudd_Ref(G->t[i]);
//            }
//
//            // level = Cudd_ReadPerm(manager, t[i - 1]->index) + 1; /* 不明白为什么是这样给level赋值，perm是干嘛的*/
//            // 创建x变量
//            for (i = 0; i < G->nips_v; i++) {
//                G->x[i] = Cudd_addIthVar(manager, level++);
//                Cudd_Ref(G->x[i]);
//            }
//
//            //level = Cudd_ReadPerm(manager, x[i - 1]->index) + 1; /* 不明白为什么是这样给level赋值，perm是干嘛的*/
//            //创建y变量
//            for (i = 0; i < G->nips_v; i++) {
//                G->y[i] = Cudd_addIthVar(manager, level++);
//                Cudd_Ref(G->y[i]);
//            }
//            break;
//
//            // 读取每一个事件，创建事件ADD
//        case 'e':
//
//            // 读取顶点 时刻
//            su = GetInt(f);
//            sv = GetInt(f);
//            st = GetInt(f);
//
//            // 创建每一条边对应的ADD
//            
//
//            tunc = CreateNode(manager, st, G->nips_t, G->t);
//            xunc = CreateNode(manager, su, G->nips_v, G->x);
//            yunc = CreateNode(manager, sv, G->nips_v, G->y);
//
//            //Cudd_Ref(p = Cudd_addSwapVariables(manager, wfunc, x, y, npis_v));  //  总是有一点疑问，为什么要这样，直接在y上做不行吗？  变量序和perm有什么关系？？
//            edgq = Cudd_addIte(manager, xunc, yunc, zero);
//            Cudd_Ref(edgq);
//            Cudd_RecursiveDeref(manager, xunc);
//            Cudd_RecursiveDeref(manager,yunc);
//            edgp = Cudd_addApply(manager, Cudd_addPlus, events, edgq);
//            Cudd_RecursiveDeref(manager, edgq);
//            Cudd_RecursiveDeref(manager, events);
//            Cudd_Ref(edgp);
//            
//            events = edgp;
//
//
//            E = Cudd_addIte(manager, events, tunc, zero);
//            Cudd_Ref(E);
//            Cudd_RecursiveDeref(manager, tunc);
//
//            Cudd_Ref(tlogs = Cudd_addApply(manager, Cudd_addPlus, logs, E));
//            Cudd_RecursiveDeref(manager, logs);
//            //Cudd_RecursiveDeref(manager, E);
//            logs = tlogs;
//    
//            break;
//
//        default:
//            Barf("Unexpected case in InputFlowGraph\n");
//            break;
//        }
//
//
//    }
//
//    G->Events = logs;
//    G->evnum = evnum;
//    G->nodes = nodes;
//    G->tinum = times;
// 
//    
//    return(G);
//
//failure:
//    return(NULL);

}

    Cudd_RecursiveDeref(manager, events);
G->Events = logs;
G->evnum = evnum;
G->nodes = nodes;
G->tinum = times;

return(G);

failure:
return(NULL);
}

DdNode* PointDirectNeighbor_new(DdManager* dd, Str_Graph* G, unsigned int u, unsigned int t) {

    DdNode* ts, * temp, * p, * q, * zero, * ans;
    Cudd_Ref(zero = Cudd_ReadZero(dd));

    // 得到t时刻点的二进制表示
    vector<int> ant(G->nips_t);
    Htob(t, G->nips_t, ant);

    // t时刻下的所有事件集合
    DdNode* tes = Restrict(dd, G->Events, G->nips_t, ant);
    

    // 创建顶点u的ADD and q -> 以顶点u为起点的事件集合
    p = CreateNode(dd, u, G->nips_v, G->x);
    Cudd_Ref(temp = Cudd_addIte(dd, p, tes, zero));
    Cudd_RecursiveDeref(dd, p);
    Cudd_RecursiveDeref(dd, tes);

    if (cuddIsConstant(temp)) {
        return temp;
    }
    else {
        vector<int> anu(G->nips_v);
        Htob(u, G->nips_v, anu);
        p = Restrict(dd, temp, G->nips_v, anu);
        Cudd_RecursiveDeref(dd, temp);
    }
    // 提取奇数系数小项  提取x变量 -> 所有邻居顶点对应的ADD
    Cudd_Ref(ans = Cudd_addMonadicApply(dd, Extra_01add_new, p));
    Cudd_RecursiveDeref(dd, p);
    Cudd_RecursiveDeref(dd, zero);
    return ans;
}

bool PointActiveEdge_new(DdManager* dd, Str_Graph* G, unsigned int u, unsigned int v, unsigned int t) {
    DdNode* ts, * temp, * p, * q, * zero, * ans;
    Cudd_Ref(zero = Cudd_ReadZero(dd));

    // 得到t时刻点的二进制表示
    vector<int> ant(G->nips_t);
    Htob(t, G->nips_t, ant);

    // t时刻下的所有事件集合
    DdNode* tes = Restrict(dd, G->Events, G->nips_t, ant);

    // 创建 边uv 的ADD
    p = CreateNode(dd, u, G->nips_v, G->x);
    q = CreateNode(dd, v, G->nips_v, G->y);
    Cudd_Ref(ts = Cudd_addIte(dd, p, q, zero));
    Cudd_RecursiveDeref(dd, p);
    Cudd_RecursiveDeref(dd, q);

    Cudd_Ref(temp = Cudd_addIte(dd, ts, tes, zero));
    Cudd_RecursiveDeref(dd, ts);
    Cudd_RecursiveDeref(dd, tes);

    if (cuddIsConstant(temp)) {
        Cudd_RecursiveDeref(dd, temp);
        return false;
    }
    else {
        vector<int> anu(G->nips_v);
        Htob(u, G->nips_v, anu);
        p = Restrict(dd, temp, G->nips_v, anu);
        Cudd_RecursiveDeref(dd, temp);
    }


    // 提取奇数系数小项得到结果ADD 然后通过判断结果ADD是否为空得到状态结果
    ans = Cudd_addMonadicApply(dd, Extra_01add_new, p);
    Cudd_RecursiveDeref(dd, p);
    Cudd_RecursiveDeref(dd, zero);
    if (cuddIsConstant(ans)) {
        return false;
    }
    return true;

}

DdNode* Snapshot_new(DdManager* dd, Str_Graph* G, unsigned int t) {
    DdNode* ts, * temp, * p, * q, * zero, * ans;

    // 得到t时刻点二进制表示
    vector<int> ant(G->nips_t);
    Htob(t, G->nips_t, ant);

    // t时刻下的所有事件集合
    DdNode* tes = Restrict(dd, G->Events, G->nips_t, ant);

    // 提取奇数系数小项  提取x变量 -> 所有邻居顶点对应的ADD
    Cudd_Ref(ans = Cudd_addMonadicApply(dd, Extra_01add_new, tes));
    Cudd_RecursiveDeref(dd, tes);
    return ans;
}

DdNode* ChangedEdgesPoint_new(DdManager* dd, Str_Graph* G, unsigned int t) {
    DdNode* ts, * temp, * p, * q, * zero, * ans;

    // 得到t时刻点的二进制表示
    vector<int> ant(G->nips_t);
    Htob(t, G->nips_t, ant);

    // t时刻下的所有事件集合
    DdNode* tes = Restrict(dd, G->Events, G->nips_t, ant);

    // 得到t-1时刻点的二进制表示
    vector<int> antt(G->nips_t);
    Htob(t-1, G->nips_t, antt);

    // t-1时刻下的所有事件集合
    ts = Restrict(dd, G->Events, G->nips_t, antt);
    Cudd_Ref(temp = Cudd_addApply(dd, Cudd_addMinus, tes, ts));
    Cudd_RecursiveDeref(dd, tes);
    Cudd_RecursiveDeref(dd, ts);

    // 提取奇数系数小项  提取x变量 -> 所有邻居顶点对应的ADD
    Cudd_Ref(ans = Cudd_addMonadicApply(dd, Extra_01add_new, temp));
    Cudd_RecursiveDeref(dd, temp);
    return ans;
}



/******************--新版本的分块优化--******************************************************************************/

Str_Graph_new* ReadGraph_new_2(DdManager* manager, FILE* f, Str_Graph_new* G) {
    char c;
    unsigned int nodes, times, i, level = 0, evnum, counts = 0;
    unsigned int flag = 0;
    unsigned int su, sv, st, prest, knum = 0;
    unsigned int count = 0;

    DdNode* p, * q, * events, * zero, * logs;

    DdNode* xunc, * yunc, * E, * edgp, * tunc, * edgq, * tlogs;
    zero = Cudd_ReadZero(manager);
    events = zero;
    Cudd_Ref(events);
    logs = zero;
    Cudd_Ref(logs);
    clock_t start_time_1, end_time_1;
    start_time_1 = clock();
    while (1) {

        if (EOF_Test(f)) break;         // 判断是否是空文件

        c = ReadChar(f);

        switch (c) {

            // 读取对整个数据的描述 
        case 'p':

            // 读顶点数,事件，时刻数
            nodes = GetInt(f);
            evnum = GetInt(f);
            times = GetInt(f);

            // 顶点变量与时刻变量个数
            G->gaps = times / G->nums;
            if (times % G->nums != 0) G->gaps++;
            G->nips_v = (int)(ComputeFloorLog2((unsigned int)(nodes)));
            G->nips_t = (int)(ComputeFloorLog2((unsigned int)(times)));
            std::cout << "Total Nodes: " << nodes << "," << "Total Times: " << times << std::endl;

            // 为顶点变量数组 时刻变量数组分配内存
            G->x = ALLOC(DdNode*, G->nips_v);
            if (G->x == NULL) goto failure;

            G->y = ALLOC(DdNode*, G->nips_v);
            if (G->y == NULL) goto failure;

            G->t = ALLOC(DdNode*, G->nips_t);
            if (G->t == NULL) goto failure;

            // 创建t变量
            for (i = 0; i < G->nips_t; i++) {
                G->t[i] = Cudd_addIthVar(manager, level++);
                Cudd_Ref(G->t[i]);
            }

            // level = Cudd_ReadPerm(manager, t[i - 1]->index) + 1; /* 不明白为什么是这样给level赋值，perm是干嘛的*/
            // 创建x变量
            for (i = 0; i < G->nips_v; i++) {
                G->x[i] = Cudd_addIthVar(manager, level++);
                Cudd_Ref(G->x[i]);
            }

            //level = Cudd_ReadPerm(manager, x[i - 1]->index) + 1; /* 不明白为什么是这样给level赋值，perm是干嘛的*/
            //创建y变量
            for (i = 0; i < G->nips_v; i++) {
                G->y[i] = Cudd_addIthVar(manager, level++);
                Cudd_Ref(G->y[i]);
            }
            break;

            // 读取每一个事件，创建事件ADD
        case 'e':

            // 读取顶点 时刻
            su = GetInt(f);
            sv = GetInt(f);
            st = GetInt(f);

            if (flag == 0) {     // 代表这是处理的第一个事件
                prest = st;
                count++;
                flag = 1;
                tunc = CreateNode(manager, st, G->nips_t, G->t);
                xunc = CreateNode(manager, su, G->nips_v, G->x);
                yunc = CreateNode(manager, sv, G->nips_v, G->y);
            }
            else {
                if (st != prest) {
                    prest = st;
                    count++;
                    E = Cudd_addIte(manager, events, tunc, zero);
                    Cudd_Ref(E);
                    Cudd_RecursiveDeref(manager, tunc);
                    tlogs = Cudd_addApply(manager, Cudd_addOr, logs, E);
                    Cudd_Ref(tlogs);
                    Cudd_RecursiveDeref(manager, logs);
                    Cudd_RecursiveDeref(manager, E);
                    logs = tlogs;

                    tunc = CreateNode(manager, st, G->nips_t, G->t);
                    xunc = CreateNode(manager, su, G->nips_v, G->x);
                    yunc = CreateNode(manager, sv, G->nips_v, G->y);

                }
                else {
                    xunc = CreateNode(manager, su, G->nips_v, G->x);
                    yunc = CreateNode(manager, sv, G->nips_v, G->y);
                }
            }
            if (count == G->gaps + 1) {
                end_time_1 = clock();
                cout << "test for one block spend times: " << (end_time_1 - start_time_1) / 1000 << "s" << std::endl;
                cout << "one block is done" << endl;
                start_time_1 = end_time_1;
                count = 0;
                Cudd_RecursiveDeref(manager, events);
                G->Events[knum] = logs;
                knum++;

                events = zero;
                Cudd_Ref(events);
                logs = zero;
                Cudd_Ref(logs);

            }
            /*counts++;
            if (counts % 100000 == 0) {
                end_time_1 = clock();
                cout << "test for 100000边 spend times: " << (end_time_1 - start_time_1) / 1000 << "s" << std::endl;
                cout << "100000 is done" << endl;
                start_time_1 = end_time_1;
            }*/

            edgq = Cudd_addIte(manager, xunc, yunc, zero);
            Cudd_Ref(edgq);
            Cudd_RecursiveDeref(manager, xunc);
            Cudd_RecursiveDeref(manager, yunc);

            edgp = Cudd_addApply(manager, Cudd_addPlus, events, edgq);
            Cudd_Ref(edgp);
            Cudd_RecursiveDeref(manager, edgq);
            Cudd_RecursiveDeref(manager, events);
            events = edgp;

            // // 创建每一条边对应的ADD

            // tunc = CreateNode(manager, st, G->nips_t, G->t);
            // xunc = CreateNode(manager, su, G->nips_v, G->x);
            // yunc = CreateNode(manager, sv, G->nips_v, G->y);

            // //Cudd_Ref(p = Cudd_addSwapVariables(manager, wfunc, x, y, npis_v));  //  总是有一点疑问，为什么要这样，直接在y上做不行吗？  变量序和perm有什么关系？？

            // edgq = Cudd_addIte(manager, xunc, yunc, zero);
            // Cudd_Ref(edgq);
            // Cudd_RecursiveDeref(manager, xunc);
            // Cudd_RecursiveDeref(manager, yunc);
            // edgp = Cudd_addApply(manager, Cudd_addPlus, events, edgq);
            // Cudd_Ref(edgp);
            // Cudd_RecursiveDeref(manager, edgq);
            // Cudd_RecursiveDeref(manager, events);

            // events = edgp;


            // E = Cudd_addIte(manager, events, tunc, zero);
            // Cudd_Ref(E);
            // Cudd_RecursiveDeref(manager, tunc);
            // tlogs = Cudd_addApply(manager, Cudd_addOr, logs, E);
            // Cudd_Ref(tlogs);
            // Cudd_RecursiveDeref(manager, logs);
            // Cudd_RecursiveDeref(manager, E);
            // logs = tlogs;

            break;

        default:
            Barf("Unexpected case in InputFlowGraph\n");
            break;
        }


        //    char c;
        //    unsigned int nodes, times, i, level = 0, evnum;
        //    unsigned int flag = 0;
        //    unsigned int su, sv, st, prest;
        //    unsigned int count = 0;
        //
        //    DdNode* p, * q, * events, * zero, * logs;
        //
        //    DdNode* xunc, * yunc, * E, * edgp, * tunc, * edgq, * tlogs;
        //    /*zero = Cudd_ReadZero(manager);
        //    Cudd_Ref(events = zero);
        //    Cudd_Ref(logs = zero);*/
        //    zero = Cudd_ReadZero(manager);
        //    events = zero;
        //    Cudd_Ref(events);
        //    logs = zero;
        //    Cudd_Ref(logs);
        //    
        //    while (1) {
        //
        //        if (EOF_Test(f)) break;         // 判断是否是空文件
        //
        //        c = ReadChar(f);
        //
        //        switch (c) {
        //
        //            // 读取对整个数据的描述 
        //        case 'p':
        //
        //            // 读顶点数,事件，时刻数
        //            nodes = GetInt(f);
        //            evnum = GetInt(f);
        //            times = GetInt(f);
        //
        //            // 顶点变量与时刻变量个数
        //            G->nips_v = (int)(ComputeFloorLog2((unsigned int)(nodes)));
        //            G->nips_t = (int)(ComputeFloorLog2((unsigned int)(times)));
        //            std::cout << "Total Nodes: " << nodes << "," << "Total Times: " << times << std::endl;
        //
        //            // 为顶点变量数组 时刻变量数组分配内存
        //            G->x = ALLOC(DdNode*, G->nips_v);
        //            if (G->x == NULL) goto failure;
        //
        //            G->y = ALLOC(DdNode*, G->nips_v);
        //            if (G->y == NULL) goto failure;
        //
        //            G->t = ALLOC(DdNode*, G->nips_t);
        //            if (G->t == NULL) goto failure;
        //
        //            // 创建t变量
        //            for (i = 0; i < G->nips_t; i++) {
        //                G->t[i] = Cudd_addIthVar(manager, level++);
        //                Cudd_Ref(G->t[i]);
        //            }
        //
        //            // level = Cudd_ReadPerm(manager, t[i - 1]->index) + 1; /* 不明白为什么是这样给level赋值，perm是干嘛的*/
        //            // 创建x变量
        //            for (i = 0; i < G->nips_v; i++) {
        //                G->x[i] = Cudd_addIthVar(manager, level++);
        //                Cudd_Ref(G->x[i]);
        //            }
        //
        //            //level = Cudd_ReadPerm(manager, x[i - 1]->index) + 1; /* 不明白为什么是这样给level赋值，perm是干嘛的*/
        //            //创建y变量
        //            for (i = 0; i < G->nips_v; i++) {
        //                G->y[i] = Cudd_addIthVar(manager, level++);
        //                Cudd_Ref(G->y[i]);
        //            }
        //            break;
        //
        //            // 读取每一个事件，创建事件ADD
        //        case 'e':
        //
        //            // 读取顶点 时刻
        //            su = GetInt(f);
        //            sv = GetInt(f);
        //            st = GetInt(f);
        //
        //            // 创建每一条边对应的ADD
        //            
        //
        //            tunc = CreateNode(manager, st, G->nips_t, G->t);
        //            xunc = CreateNode(manager, su, G->nips_v, G->x);
        //            yunc = CreateNode(manager, sv, G->nips_v, G->y);
        //
        //            //Cudd_Ref(p = Cudd_addSwapVariables(manager, wfunc, x, y, npis_v));  //  总是有一点疑问，为什么要这样，直接在y上做不行吗？  变量序和perm有什么关系？？
        //            edgq = Cudd_addIte(manager, xunc, yunc, zero);
        //            Cudd_Ref(edgq);
        //            Cudd_RecursiveDeref(manager, xunc);
        //            Cudd_RecursiveDeref(manager,yunc);
        //            edgp = Cudd_addApply(manager, Cudd_addPlus, events, edgq);
        //            Cudd_RecursiveDeref(manager, edgq);
        //            Cudd_RecursiveDeref(manager, events);
        //            Cudd_Ref(edgp);
        //            
        //            events = edgp;
        //
        //
        //            E = Cudd_addIte(manager, events, tunc, zero);
        //            Cudd_Ref(E);
        //            Cudd_RecursiveDeref(manager, tunc);
        //
        //            Cudd_Ref(tlogs = Cudd_addApply(manager, Cudd_addPlus, logs, E));
        //            Cudd_RecursiveDeref(manager, logs);
        //            //Cudd_RecursiveDeref(manager, E);
        //            logs = tlogs;
        //    
        //            break;
        //
        //        default:
        //            Barf("Unexpected case in InputFlowGraph\n");
        //            break;
        //        }
        //
        //
        //    }
        //
        //    G->Events = logs;
        //    G->evnum = evnum;
        //    G->nodes = nodes;
        //    G->tinum = times;
        // 
        //    
        //    return(G);
        //
        //failure:
        //    return(NULL);

    }

    E = Cudd_addIte(manager, events, tunc, zero);
    Cudd_Ref(E);
    Cudd_RecursiveDeref(manager, tunc);
    tlogs = Cudd_addApply(manager, Cudd_addOr, logs, E);
    Cudd_Ref(tlogs);
    Cudd_RecursiveDeref(manager, logs);
    Cudd_RecursiveDeref(manager, E);
    logs = tlogs;

    Cudd_RecursiveDeref(manager, events);
    G->Events[G->nums - 1] = logs;
    G->evnum = evnum;
    G->nodes = nodes;
    G->tinum = times;

    return(G);

failure:
    return(NULL);
}
//Str_Graph_new* ReadGraph_new_2(DdManager* manager, FILE* f, Str_Graph_new *G) {
//    char c;
//    unsigned int nodes, times, i, level = 0, evnum, counts = 0;
//    unsigned int flag = 0;
//    unsigned int su, sv, st, prest, knum = 0;
//    unsigned int count = 0;
//
//    DdNode* p, * q, * events, * zero, * logs;
//
//    DdNode* xunc, * yunc, * E, * edgp, * tunc, * edgq, * tlogs;
//    zero = Cudd_ReadZero(manager);
//    events = zero;
//    Cudd_Ref(events);
//    logs = zero;
//    Cudd_Ref(logs);
//    clock_t start_time_1, end_time_1;
//    start_time_1 = clock();
//    while (1) {
//
//        if (EOF_Test(f)) break;         // 判断是否是空文件
//
//        c = ReadChar(f);
//
//        switch (c) {
//
//            // 读取对整个数据的描述 
//        case 'p':
//
//            // 读顶点数,事件，时刻数
//            nodes = GetInt(f);
//            evnum = GetInt(f);
//            times = GetInt(f);
//
//            // 顶点变量与时刻变量个数
//            G->gaps = times / G->nums;
//            if (times % G->nums != 0) G->gaps++;
//            G->nips_v = (int)(ComputeFloorLog2((unsigned int)(nodes)));
//            G->nips_t = (int)(ComputeFloorLog2((unsigned int)(times)));
//            std::cout << "Total Nodes: " << nodes << "," << "Total Times: " << times << std::endl;
//
//            // 为顶点变量数组 时刻变量数组分配内存
//            G->x = ALLOC(DdNode*, G->nips_v);
//            if (G->x == NULL) goto failure;
//
//            G->y = ALLOC(DdNode*, G->nips_v);
//            if (G->y == NULL) goto failure;
//
//            G->t = ALLOC(DdNode*, G->nips_t);
//            if (G->t == NULL) goto failure;
//
//            // 创建t变量
//            for (i = 0; i < G->nips_t; i++) {
//                G->t[i] = Cudd_addIthVar(manager, level++);
//                Cudd_Ref(G->t[i]);
//            }
//
//            // level = Cudd_ReadPerm(manager, t[i - 1]->index) + 1; /* 不明白为什么是这样给level赋值，perm是干嘛的*/
//            // 创建x变量
//            for (i = 0; i < G->nips_v; i++) {
//                G->x[i] = Cudd_addIthVar(manager, level++);
//                Cudd_Ref(G->x[i]);
//            }
//
//            //level = Cudd_ReadPerm(manager, x[i - 1]->index) + 1; /* 不明白为什么是这样给level赋值，perm是干嘛的*/
//            //创建y变量
//            for (i = 0; i < G->nips_v; i++) {
//                G->y[i] = Cudd_addIthVar(manager, level++);
//                Cudd_Ref(G->y[i]);
//            }
//            break;
//
//            // 读取每一个事件，创建事件ADD
//        case 'e':
//
//            // 读取顶点 时刻
//            su = GetInt(f);
//            sv = GetInt(f);
//            st = GetInt(f);
//
//            if (flag == 0) {     // 代表这是处理的第一个事件
//                prest = st;
//                count++;
//                flag = 1;
//            }
//            else {
//                if (st != prest) {
//                    prest = st;
//                    count++;
//                }
//            }
//            if (count == G->gaps+1) {
//                end_time_1 = clock();
//                cout << "test for one block spend times: " << (end_time_1 - start_time_1) / 1000 << "s" << std::endl;
//                cout << "one block is done" << endl;
//                start_time_1 = end_time_1;
//                count = 0;
//                Cudd_RecursiveDeref(manager, events);
//                G->Events[knum] = logs;
//                knum++;
//
//                events = zero;
//                Cudd_Ref(events);
//                logs = zero;
//                Cudd_Ref(logs);
//
//            }
//            /*counts++;
//            if (counts % 100000 == 0) {
//                end_time_1 = clock();
//                cout << "test for 100000边 spend times: " << (end_time_1 - start_time_1) / 1000 << "s" << std::endl;
//                cout << "100000 is done" << endl;
//                start_time_1 = end_time_1;
//            }*/
//
//            // 创建每一条边对应的ADD
//            tunc = CreateNode(manager, st, G->nips_t, G->t);
//            xunc = CreateNode(manager, su, G->nips_v, G->x);
//            yunc = CreateNode(manager, sv, G->nips_v, G->y);
//
//            //Cudd_Ref(p = Cudd_addSwapVariables(manager, wfunc, x, y, npis_v));  //  总是有一点疑问，为什么要这样，直接在y上做不行吗？  变量序和perm有什么关系？？
//            edgq = Cudd_addIte(manager, xunc, yunc, zero);
//            Cudd_Ref(edgq);
//            Cudd_RecursiveDeref(manager, xunc);
//            Cudd_RecursiveDeref(manager, yunc);
//            edgp = Cudd_addApply(manager, Cudd_addPlus, events, edgq);
//            Cudd_Ref(edgp);
//            Cudd_RecursiveDeref(manager, edgq);
//            Cudd_RecursiveDeref(manager, events);
//
//            events = edgp;
//
//
//            E = Cudd_addIte(manager, events, tunc, zero);
//            Cudd_Ref(E);
//            Cudd_RecursiveDeref(manager, tunc);
//            tlogs = Cudd_addApply(manager, Cudd_addOr, logs, E);
//            Cudd_Ref(tlogs);
//            Cudd_RecursiveDeref(manager, logs);
//            Cudd_RecursiveDeref(manager, E);
//            logs = tlogs;
//
//
//            
//            break;
//
//        default:
//            Barf("Unexpected case in InputFlowGraph\n");
//            break;
//        }
//
//
//        //    char c;
//        //    unsigned int nodes, times, i, level = 0, evnum;
//        //    unsigned int flag = 0;
//        //    unsigned int su, sv, st, prest;
//        //    unsigned int count = 0;
//        //
//        //    DdNode* p, * q, * events, * zero, * logs;
//        //
//        //    DdNode* xunc, * yunc, * E, * edgp, * tunc, * edgq, * tlogs;
//        //    /*zero = Cudd_ReadZero(manager);
//        //    Cudd_Ref(events = zero);
//        //    Cudd_Ref(logs = zero);*/
//        //    zero = Cudd_ReadZero(manager);
//        //    events = zero;
//        //    Cudd_Ref(events);
//        //    logs = zero;
//        //    Cudd_Ref(logs);
//        //    
//        //    while (1) {
//        //
//        //        if (EOF_Test(f)) break;         // 判断是否是空文件
//        //
//        //        c = ReadChar(f);
//        //
//        //        switch (c) {
//        //
//        //            // 读取对整个数据的描述 
//        //        case 'p':
//        //
//        //            // 读顶点数,事件，时刻数
//        //            nodes = GetInt(f);
//        //            evnum = GetInt(f);
//        //            times = GetInt(f);
//        //
//        //            // 顶点变量与时刻变量个数
//        //            G->nips_v = (int)(ComputeFloorLog2((unsigned int)(nodes)));
//        //            G->nips_t = (int)(ComputeFloorLog2((unsigned int)(times)));
//        //            std::cout << "Total Nodes: " << nodes << "," << "Total Times: " << times << std::endl;
//        //
//        //            // 为顶点变量数组 时刻变量数组分配内存
//        //            G->x = ALLOC(DdNode*, G->nips_v);
//        //            if (G->x == NULL) goto failure;
//        //
//        //            G->y = ALLOC(DdNode*, G->nips_v);
//        //            if (G->y == NULL) goto failure;
//        //
//        //            G->t = ALLOC(DdNode*, G->nips_t);
//        //            if (G->t == NULL) goto failure;
//        //
//        //            // 创建t变量
//        //            for (i = 0; i < G->nips_t; i++) {
//        //                G->t[i] = Cudd_addIthVar(manager, level++);
//        //                Cudd_Ref(G->t[i]);
//        //            }
//        //
//        //            // level = Cudd_ReadPerm(manager, t[i - 1]->index) + 1; /* 不明白为什么是这样给level赋值，perm是干嘛的*/
//        //            // 创建x变量
//        //            for (i = 0; i < G->nips_v; i++) {
//        //                G->x[i] = Cudd_addIthVar(manager, level++);
//        //                Cudd_Ref(G->x[i]);
//        //            }
//        //
//        //            //level = Cudd_ReadPerm(manager, x[i - 1]->index) + 1; /* 不明白为什么是这样给level赋值，perm是干嘛的*/
//        //            //创建y变量
//        //            for (i = 0; i < G->nips_v; i++) {
//        //                G->y[i] = Cudd_addIthVar(manager, level++);
//        //                Cudd_Ref(G->y[i]);
//        //            }
//        //            break;
//        //
//        //            // 读取每一个事件，创建事件ADD
//        //        case 'e':
//        //
//        //            // 读取顶点 时刻
//        //            su = GetInt(f);
//        //            sv = GetInt(f);
//        //            st = GetInt(f);
//        //
//        //            // 创建每一条边对应的ADD
//        //            
//        //
//        //            tunc = CreateNode(manager, st, G->nips_t, G->t);
//        //            xunc = CreateNode(manager, su, G->nips_v, G->x);
//        //            yunc = CreateNode(manager, sv, G->nips_v, G->y);
//        //
//        //            //Cudd_Ref(p = Cudd_addSwapVariables(manager, wfunc, x, y, npis_v));  //  总是有一点疑问，为什么要这样，直接在y上做不行吗？  变量序和perm有什么关系？？
//        //            edgq = Cudd_addIte(manager, xunc, yunc, zero);
//        //            Cudd_Ref(edgq);
//        //            Cudd_RecursiveDeref(manager, xunc);
//        //            Cudd_RecursiveDeref(manager,yunc);
//        //            edgp = Cudd_addApply(manager, Cudd_addPlus, events, edgq);
//        //            Cudd_RecursiveDeref(manager, edgq);
//        //            Cudd_RecursiveDeref(manager, events);
//        //            Cudd_Ref(edgp);
//        //            
//        //            events = edgp;
//        //
//        //
//        //            E = Cudd_addIte(manager, events, tunc, zero);
//        //            Cudd_Ref(E);
//        //            Cudd_RecursiveDeref(manager, tunc);
//        //
//        //            Cudd_Ref(tlogs = Cudd_addApply(manager, Cudd_addPlus, logs, E));
//        //            Cudd_RecursiveDeref(manager, logs);
//        //            //Cudd_RecursiveDeref(manager, E);
//        //            logs = tlogs;
//        //    
//        //            break;
//        //
//        //        default:
//        //            Barf("Unexpected case in InputFlowGraph\n");
//        //            break;
//        //        }
//        //
//        //
//        //    }
//        //
//        //    G->Events = logs;
//        //    G->evnum = evnum;
//        //    G->nodes = nodes;
//        //    G->tinum = times;
//        // 
//        //    
//        //    return(G);
//        //
//        //failure:
//        //    return(NULL);
//
//    }
//
//    Cudd_RecursiveDeref(manager, events);
//    G->Events[G->nums-1] = logs;
//    G->evnum = evnum;
//    G->nodes = nodes;
//    G->tinum = times;
//
//    return(G);
//
//failure:
//    return(NULL);
//}

DdNode* PointDirectNeighbor_new_1(DdManager* dd, Str_Graph_new* G, unsigned int u, unsigned int t) {

    DdNode* ts, * temp, * p, * q, * zero, * ans;
    Cudd_Ref(zero = Cudd_ReadZero(dd));
    //确定t时刻所在的块的位置
    int indexs = t / G->gaps;
    // 得到t时刻点的二进制表示
    vector<int> ant(G->nips_t);
    Htob(t, G->nips_t, ant);

    // t时刻下的所有事件集合
    DdNode* tes = Restrict(dd, G->Events[indexs], G->nips_t, ant);


    // 创建顶点u的ADD and q -> 以顶点u为起点的事件集合
    p = CreateNode(dd, u, G->nips_v, G->x);
    Cudd_Ref(temp = Cudd_addIte(dd, p, tes, zero));
    Cudd_RecursiveDeref(dd, p);
    Cudd_RecursiveDeref(dd, tes);

    if (cuddIsConstant(temp)) {
        return temp;
    }
    else {
        vector<int> anu(G->nips_v);
        Htob(u, G->nips_v, anu);
        p = Restrict(dd, temp, G->nips_v, anu);
        Cudd_RecursiveDeref(dd, temp);
    }
    // 提取奇数系数小项  提取x变量 -> 所有邻居顶点对应的ADD
    Cudd_Ref(ans = Cudd_addMonadicApply(dd, Extra_01add_new, p));
    Cudd_RecursiveDeref(dd, p);
    Cudd_RecursiveDeref(dd, zero);
    return ans;
}

DdNode* PointDirectNeighbor_new_2(DdManager* dd, Str_Graph_new* G, unsigned int u, unsigned int t) {

    DdNode* ts, * temp, * p, * q, * zero, * ans;
    zero = Cudd_ReadZero(dd);
    ts = zero;
    Cudd_Ref(ts);
    //确定t时刻所在的块的位置
    int indexs = t / G->gaps;

    //找出来所有事件块中的那些目标顶点0-1ADD，然后相加
    for (int ti = -1+G->gaps, k = 0; k <= indexs - 1; ti += G->gaps, k++) {
        temp = PointDirectNeighbor_new_1(dd, G, u, ti);
        if (!cuddIsConstant(temp)) {
            p = Cudd_addApply(dd, Cudd_addPlus, ts, temp);
            Cudd_Ref(p);
            Cudd_RecursiveDeref(dd, ts);
            Cudd_RecursiveDeref(dd, temp);
            ts = p;
        }
    }
    temp = PointDirectNeighbor_new_1(dd, G, u, t);
    if (!cuddIsConstant(temp)) {
        p = Cudd_addApply(dd, Cudd_addPlus, ts, temp);
        Cudd_Ref(p);
        Cudd_RecursiveDeref(dd, ts);
        Cudd_RecursiveDeref(dd, temp);
        ts = p;
    }

    // 提取奇数系数小项  得到最终的邻居顶点对应的0-1ADD
    Cudd_Ref(ans = Cudd_addMonadicApply(dd, Extra_01add_new, ts));
    Cudd_RecursiveDeref(dd, ts);
    return ans;
}

bool oper(bool bL, bool bR) {
    if (bL == false && bR == false) return false;
    else if (bL == false && bR == true) return true;
    else if (bL == true && bR == false) return true;
    else return false;
}

bool PointActiveEdge_new_1(DdManager* dd, Str_Graph_new* G, unsigned int u, unsigned int v, unsigned int t) {
    DdNode* ts, * temp, * p, * q, * zero, * ans;
    Cudd_Ref(zero = Cudd_ReadZero(dd));

    //确定t时刻所在的块的位置
    int indexs = t / G->gaps;
    
    // 得到t时刻点的二进制表示
    vector<int> ant(G->nips_t);
    Htob(t, G->nips_t, ant);

    // t时刻下的所有事件集合
    DdNode* tes = Restrict(dd, G->Events[indexs], G->nips_t, ant);

    // 创建 边uv 的ADD
    p = CreateNode(dd, u, G->nips_v, G->x);
    q = CreateNode(dd, v, G->nips_v, G->y);
    Cudd_Ref(ts = Cudd_addIte(dd, p, q, zero));
    Cudd_RecursiveDeref(dd, p);
    Cudd_RecursiveDeref(dd, q);

    Cudd_Ref(temp = Cudd_addIte(dd, ts, tes, zero));
    Cudd_RecursiveDeref(dd, ts);
    Cudd_RecursiveDeref(dd, tes);

    if (cuddIsConstant(temp)) {
        Cudd_RecursiveDeref(dd, temp);
        return false;
    }
    else {
        vector<int> anu(G->nips_v);
        Htob(u, G->nips_v, anu);
        p = Restrict(dd, temp, G->nips_v, anu);
        Cudd_RecursiveDeref(dd, temp);
    }


    // 提取奇数系数小项得到结果ADD 然后通过判断结果ADD是否为空得到状态结果
    ans = Cudd_addMonadicApply(dd, Extra_01add_new, p);
    Cudd_RecursiveDeref(dd, p);
    Cudd_RecursiveDeref(dd, zero);
    if (cuddIsConstant(ans)) {
        return false;
    }
    return true;

}

bool PointActiveEdge_new_2(DdManager* dd, Str_Graph_new* G, unsigned int u, unsigned int v, unsigned int t) {
    DdNode* ts,  * p, * q, * zero;
    bool ans = false, temp;
    Cudd_Ref(zero = Cudd_ReadZero(dd));

    //确定t时刻所在的块的位置
    int indexs = t / G->gaps;
   
    //找出来每个事件块中这条边是否存在，然后逐块结果进行操作得到最终结果。
    for (int ti = -1 + G->gaps, k = 0; k <= indexs - 1; ti += G->gaps, k++) {
        temp = PointActiveEdge_new_1(dd, G, u, v, ti);
        ans = oper(ans, temp);
    }
    temp = PointActiveEdge_new_1(dd, G, u,v ,t);
    ans = oper(ans, temp);

    return ans;
}

DdNode* Snapshot_new_1(DdManager* dd, Str_Graph_new* G, unsigned int t) {
    DdNode* ts, * temp, * p, * q, * zero, * ans;

    //确定t时刻所在的块的位置
    int indexs = t / G->gaps;

    // 得到t时刻点二进制表示
    vector<int> ant(G->nips_t);
    Htob(t, G->nips_t, ant);

    // t时刻下的所有事件集合
    DdNode* tes = Restrict(dd, G->Events[indexs], G->nips_t, ant);

    // 提取奇数系数小项  提取x变量 -> 所有邻居顶点对应的ADD
    Cudd_Ref(ans = Cudd_addMonadicApply(dd, Extra_01add_new, tes));
    Cudd_RecursiveDeref(dd, tes);
    return ans;
}

DdNode* Snapshot_new_2(DdManager* dd, Str_Graph_new* G, unsigned int t) {
    DdNode* ts, * temp, * p, * q, * zero, * ans;
    zero = Cudd_ReadZero(dd);
    ts = zero;
    Cudd_Ref(ts);
    //确定t时刻所在的块的位置
    int indexs = t / G->gaps;

    //找出来所有事件块中的边集合0-1ADD，然后相加
    for (int ti = -1 + G->gaps, k = 0; k <= indexs - 1; ti += G->gaps, k++) {
        temp = Snapshot_new_1(dd, G, ti);
        if (!cuddIsConstant(temp)) {
            p = Cudd_addApply(dd, Cudd_addPlus, ts, temp);
            Cudd_Ref(p);
            Cudd_RecursiveDeref(dd, ts);
            Cudd_RecursiveDeref(dd, temp);
            ts = p;
        }
    }
    temp = Snapshot_new_1(dd, G, t);
    if (!cuddIsConstant(temp)) {
        p = Cudd_addApply(dd, Cudd_addPlus, ts, temp);
        Cudd_Ref(p);
        Cudd_RecursiveDeref(dd, ts);
        Cudd_RecursiveDeref(dd, temp);
        ts = p;
    }

    // 提取奇数系数小项  得到最终的邻居顶点对应的0-1ADD
    Cudd_Ref(ans = Cudd_addMonadicApply(dd, Extra_01add_new, ts));
    Cudd_RecursiveDeref(dd, ts);
    return ans;
}

DdNode* ChangedEdgesPoint_new_2(DdManager* dd, Str_Graph_new* G, unsigned int t) {
    DdNode* ts, * temp, * p, * q, * zero, * ans;

    //确定t时刻所在的块的位置
    int indexss = t / G->gaps;
    int indexs = (t - 1) / G->gaps;
    // 得到t时刻点的二进制表示
    vector<int> ant(G->nips_t);
    Htob(t, G->nips_t, ant);

    // t时刻下的所有事件集合
    DdNode* tes = Restrict(dd, G->Events[indexss], G->nips_t, ant);

    // 得到t-1时刻点的二进制表示
    vector<int> antt(G->nips_t);
    Htob(t - 1, G->nips_t, antt);

    // t-1时刻下的所有事件集合
    ts = Restrict(dd, G->Events[indexs], G->nips_t, antt);
    Cudd_Ref(temp = Cudd_addApply(dd, Cudd_addMinus, tes, ts));
    Cudd_RecursiveDeref(dd, tes);
    Cudd_RecursiveDeref(dd, ts);
   

    // 提取奇数系数小项  提取x变量 -> 所有邻居顶点对应的ADD
    Cudd_Ref(ans = Cudd_addMonadicApply(dd, Extra_01add_new, temp));
    Cudd_RecursiveDeref(dd, temp);
    return ans;
}


/**********************补的所需操作***************************************************************************************/

DdNode* Extra_01add(DdManager* dd, DdNode* f)
{
    if (cuddIsConstant(f))
    {
        CUDD_VALUE_TYPE value = (cuddV(f) > 0) ? 1 : 0;
        DdNode* res = cuddUniqueConst(dd, value);
        return (res);
    }
    return (NULL);
}

DdNode* pointDirectNeighbor_inter(DdManager* dd, Str_Graph_new* G, unsigned int u, unsigned int t) {
  
    DdNode* ts, * temp, * p, * q, * zero, * ans;
    Cudd_Ref(zero = Cudd_ReadZero(dd));
    //确定t时刻所在的块的位置
    int indexs = t / G->gaps;
    // 得到t时刻点的二进制表示
    vector<int> ant(G->nips_t);
    Htob(t, G->nips_t, ant);

    // t时刻下的所有事件集合
    DdNode* tes = Restrict(dd, G->Events[indexs], G->nips_t, ant);


    // 创建顶点u的ADD and q -> 以顶点u为起点的事件集合
    p = CreateNode(dd, u, G->nips_v, G->x);
    Cudd_Ref(temp = Cudd_addIte(dd, p, tes, zero));
    Cudd_RecursiveDeref(dd, p);
    Cudd_RecursiveDeref(dd, tes);

    if (cuddIsConstant(temp)) {
        return temp;
    }
   
    // 提取奇数系数小项  提取x变量 -> 所有邻居顶点对应的ADD
    Cudd_Ref(ans = Cudd_addMonadicApply(dd, Extra_01add_new, temp));
    Cudd_RecursiveDeref(dd, temp);
    Cudd_RecursiveDeref(dd, zero);
    return ans;
   
}

DdNode* pointReverseNeighbor_inter(DdManager* dd, Str_Graph_new* G, unsigned int u, unsigned int t) {
    DdNode* ts, * temp, * p, * q, * zero, * ans;
    Cudd_Ref(zero = Cudd_ReadZero(dd));
    //确定t时刻所在的块的位置
    int indexs = t / G->gaps;
    // 得到t时刻点的二进制表示
    vector<int> ant(G->nips_t);
    Htob(t, G->nips_t, ant);

    // t时刻下的所有事件集合
    DdNode* tes = Restrict(dd, G->Events[indexs], G->nips_t, ant);

    // 创建顶点u的ADD and q -> 以顶点u为起点的事件集合
    p = CreateNode(dd, u, G->nips_v, G->y);
    Cudd_Ref(temp = Cudd_addIte(dd, p, tes, zero));
    Cudd_RecursiveDeref(dd, p);
    Cudd_RecursiveDeref(dd, tes);

    if (cuddIsConstant(temp)) {
        return temp;
    }

    // 提取奇数系数小项  提取x变量 -> 所有邻居顶点对应的ADD
    Cudd_Ref(p = Cudd_addMonadicApply(dd, Extra_01add_new, temp));
    Cudd_RecursiveDeref(dd, temp);
    return p;
}

DdNode* pointReverseNeighbor_new_2(DdManager* dd, Str_Graph_new* G, unsigned int u, unsigned int t) {
    DdNode* ts, * temp, * p, * q, * zero, * ans;
    Cudd_Ref(zero = Cudd_ReadZero(dd));
    //确定t时刻所在的块的位置
    int indexs = t / G->gaps;
    // 得到t时刻点的二进制表示
    vector<int> ant(G->nips_t);
    Htob(t, G->nips_t, ant);

    // t时刻下的所有事件集合
    DdNode* tes = Restrict(dd, G->Events[indexs], G->nips_t, ant);

    // 创建顶点u的ADD and q -> 以顶点u为起点的事件集合
    p = CreateNode(dd, u, G->nips_v, G->y);
    Cudd_Ref(temp = Cudd_addIte(dd, p, tes, zero));
    Cudd_RecursiveDeref(dd, p);
    Cudd_RecursiveDeref(dd, tes);

    if (cuddIsConstant(temp)) {
        return temp;
    }

    // 提取奇数系数小项  提取x变量 -> 所有邻居顶点对应的ADD
    Cudd_Ref(p = Cudd_addMonadicApply(dd, Extra_01add_new, temp));
    Cudd_RecursiveDeref(dd, temp);
    if (cuddIsConstant(p)) {
        return p;
    }
    Cudd_Ref(ycube = Cudd_addComputeCube(dd, G->y, NULL, G->nips_v));
    Cudd_Ref(ans = Cudd_addExistAbstract(dd, p, ycube));
    Cudd_RecursiveDeref(dd, p);
    Cudd_RecursiveDeref(dd, ycube);
    Cudd_RecursiveDeref(dd, zero);
    return ans;
}

DdNode* intervalDirectNeighbor(DdManager* dd, Str_Graph_new* G, unsigned int u, unsigned int t, unsigned ti) {
    DdNode* ts, * temp, * p, * q, * zero, * ans;
    Cudd_Ref(zero = Cudd_ReadZero(dd));
    //确定t时刻所在的块的位置
    int indexs = t / G->gaps;
    // 得到t时刻点的二进制表示
    vector<int> ant(G->nips_t);
    Htob(t, G->nips_t, ant);

    // t时刻下的所有事件集合
    DdNode* tes = Restrict(dd, G->Events[indexs], G->nips_t, ant);

    //确定t时刻所在的块的位置
    int ind = (ti-1) / G->gaps;
    // 得到t时刻点的二进制表示
    vector<int> anti(G->nips_t);
    Htob(ti-1, G->nips_t, anti);

    // t时刻下的所有事件集合
    DdNode* ties = Restrict(dd, G->Events[ind], G->nips_t, anti);



    DdNode* tactive = pointDirectNeighbor_inter(dd, G, u, t);
    if (cuddIsConstant(tactive)) {
        return tactive;
    }


    DdNode* diff = Cudd_addApply(dd, Cudd_addMinus, ties, tes);
    Cudd_Ref(diff);
    Cudd_RecursiveDeref(dd, tes);
    Cudd_RecursiveDeref(dd, ties);

    p = Cudd_addMonadicApply(dd, Extra_01add, diff);
    Cudd_Ref(p);
    Cudd_RecursiveDeref(dd, diff);

    Cudd_Ref(temp = Cudd_addIte(dd, tactive, p, zero));
    Cudd_RecursiveDeref(dd, p);

    Cudd_Ref(ans = Cudd_addApply(dd, Cudd_addMinus, tactive, temp));
    Cudd_RecursiveDeref(dd, tactive);
    Cudd_RecursiveDeref(dd, temp);
    if (cuddIsConstant(ans)) {
        return ans;
    }
    vector<int> anu(G->nips_v);
    Htob(u, G->nips_v, anu);
    p = Restrict(dd, ans, G->nips_v, anu);
    Cudd_RecursiveDeref(dd, ans);
    return p;
}

DdNode* intervalReverseNeighbor(DdManager* dd, Str_Graph_new* G, unsigned int u, unsigned int t, unsigned ti) {
    DdNode* ts, * temp, * p, * q, * zero, * ans;
    Cudd_Ref(zero = Cudd_ReadZero(dd));
    //确定t时刻所在的块的位置
    int indexs = t / G->gaps;
    // 得到t时刻点的二进制表示
    vector<int> ant(G->nips_t);
    Htob(t, G->nips_t, ant);

    // t时刻下的所有事件集合
    DdNode* tes = Restrict(dd, G->Events[indexs], G->nips_t, ant);

    //确定t时刻所在的块的位置
    int ind = (ti - 1) / G->gaps;
    // 得到t时刻点的二进制表示
    vector<int> anti(G->nips_t);
    Htob(ti - 1, G->nips_t, anti);

    // t时刻下的所有事件集合
    DdNode* ties = Restrict(dd, G->Events[ind], G->nips_t, anti);

    DdNode* tactive = pointReverseNeighbor_inter(dd, G, u, t);//这里应该是返回边集合，不是邻居集合！！！！
    if (cuddIsConstant(tactive)) {
        return tactive;
    }


    DdNode* diff = Cudd_addApply(dd, Cudd_addMinus, ties, tes);
    Cudd_Ref(diff);
    Cudd_RecursiveDeref(dd, tes);
    Cudd_RecursiveDeref(dd, ties);

    p = Cudd_addMonadicApply(dd, Extra_01add, diff);
    Cudd_Ref(p);
    Cudd_RecursiveDeref(dd, diff);

    Cudd_Ref(temp = Cudd_addIte(dd, tactive, p, zero));
    Cudd_RecursiveDeref(dd, p);

    Cudd_Ref(ans = Cudd_addApply(dd, Cudd_addMinus, tactive, temp));
    Cudd_RecursiveDeref(dd, tactive);
    Cudd_RecursiveDeref(dd, temp);
    if (cuddIsConstant(ans)) {
        return ans;
    }

    // 提取奇数系数小项  提取u变量 -> 所有邻居顶点对应的ADD
    
    Cudd_Ref(ycube = Cudd_addComputeCube(dd, G->y, NULL, G->nips_v));
    Cudd_Ref(p = Cudd_addExistAbstract(dd, ans, ycube));
    Cudd_RecursiveDeref(dd, ans);
    Cudd_RecursiveDeref(dd, ycube);
    Cudd_RecursiveDeref(dd, zero);
    return p;
}

bool intervalActiveEdge(DdManager* dd, Str_Graph_new* G, unsigned int u, unsigned int v, unsigned int t, unsigned int ti) {
    DdNode* ts, * temp, * p, * q, * zero, * ans;
    Cudd_Ref(zero = Cudd_ReadZero(dd));
    //确定t时刻所在的块的位置
    int indexs = t / G->gaps;
    // 得到t时刻点的二进制表示
    vector<int> ant(G->nips_t);
    Htob(t, G->nips_t, ant);

    // t时刻下的所有事件集合
    DdNode* tes = Restrict(dd, G->Events[indexs], G->nips_t, ant);

    //确定t时刻所在的块的位置
    int ind = (ti - 1) / G->gaps;
    // 得到t时刻点的二进制表示
    vector<int> anti(G->nips_t);
    Htob(ti - 1, G->nips_t, anti);

    // t时刻下的所有事件集合
    DdNode* ties = Restrict(dd, G->Events[ind], G->nips_t, anti);

    bool tactive = PointActiveEdge_new_1(dd, G, u,v, t);
    if (!tactive) return false;


    DdNode* diff = Cudd_addApply(dd, Cudd_addMinus, ties, tes);
    Cudd_Ref(diff);
    Cudd_RecursiveDeref(dd, tes);
    Cudd_RecursiveDeref(dd, ties);


    p = CreateNode(dd, u, G->nips_v, G->x);
    q = CreateNode(dd, v, G->nips_v, G->y);
    Cudd_Ref(ts = Cudd_addIte(dd, p, q, zero));
    Cudd_RecursiveDeref(dd, p);
    Cudd_RecursiveDeref(dd, q);

    Cudd_Ref(temp = Cudd_addIte(dd, ts, diff, zero));
    Cudd_RecursiveDeref(dd, ts);
    Cudd_RecursiveDeref(dd, diff);

    if (cuddIsConstant(temp)) return true;
    else return false;

    return true;
}

DdNode* changedEdgesInterval(DdManager* dd, Str_Graph_new* G, unsigned int t, unsigned int ti) {
    DdNode* ts, * temp, * p, * q, * zero, * ans;
    Cudd_Ref(zero = Cudd_ReadZero(dd));
    //确定t时刻所在的块的位置
    int indexs = (t-1) / G->gaps;
    // 得到t时刻点的二进制表示
    vector<int> ant(G->nips_t);
    Htob(t-1, G->nips_t, ant);

    // t时刻下的所有事件集合
    DdNode* tes = Restrict(dd, G->Events[indexs], G->nips_t, ant);

    //确定t时刻所在的块的位置
    int ind = (ti - 1) / G->gaps;
    // 得到t时刻点的二进制表示
    vector<int> anti(G->nips_t);
    Htob(ti - 1, G->nips_t, anti);

    // t时刻下的所有事件集合
    DdNode* ties = Restrict(dd, G->Events[ind], G->nips_t, anti);



    DdNode* diff = Cudd_addApply(dd, Cudd_addMinus, ties, tes);
    Cudd_Ref(diff);
    Cudd_RecursiveDeref(dd, tes);
    Cudd_RecursiveDeref(dd, ties);

    return diff; //懒了，不想改成0-1ADD 了。
}