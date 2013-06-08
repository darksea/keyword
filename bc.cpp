#include <utility>
#include <climits>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cassert>
#include <cstdlib>
#include <list>
#include "pairheap.h"
using namespace std;
 
struct Table {
    bool known;
    double dv;
     vector < int >pv;
};

template <typename Printable>
struct Node{
    Printable data;
    Node *parent;

    Node(Printable d,Node *p=NULL)
        :data(d),parent(p){
    }

};
 
int vertextNum;         //顶点数
double **adjMatrix;     //用邻接矩阵来存储图
vector < Table > dijkTable;   //dijkstra维护的表
double *pairDependency;

vector <string> words;    //存储文章中出现的词
 
/**读出所有的词，存入vector**/
void initWords(string filename)
{
    ifstream ifs(filename.c_str());
    assert(ifs);
    string line;
    while (getline(ifs, line)) {
        istringstream stream(line);
        string word;
        stream >> word;   //读出一行中的首列词语即可
        words.push_back(word);
    }
    ifs.close();
}
 
void initMatrix(string filename)
{
    ifstream ifs(filename.c_str());
    assert(ifs);
    int scale;
    string line;
    getline(ifs, line); //读出第一行，存放着矩阵的规模
    istringstream stream(line);
    stream >> scale;
    vertextNum = scale;
 
    adjMatrix = new double *[scale];    //动态创建二维数组
    for (int i = 0; i < scale; ++i) {
        adjMatrix[i] = new double[scale];
        for (int j = 0; j < scale; ++j)
            adjMatrix[i][j] = 0;
    }
 
    int row, col;
    string word;
    int vari;
    while (getline(ifs, line)) {
        istringstream strm(line);
        strm >> vari; //第一列存储的是顶点的index
        row = vari;
        while (strm >> word) {
            int pos1 = word.find("(");
            int pos2 = word.find(")");
            int ind = atoi(word.substr(0, pos1).c_str());
            col = ind;
            double dis =
                atof(word.substr(pos1 + 1, pos2 - pos1 - 1).
                 c_str());
            adjMatrix[row][col] = adjMatrix[col][row] = dis;
        }
    }
 
    ifs.close();
}
 
/*释放邻接矩阵*/
void deleteMatrix()
{
    for (int i = 0; i < vertextNum; ++i)
        delete adjMatrix[i];
    delete[]adjMatrix;
}
 
void printMatrix()
{
    for (int i = 0; i < vertextNum; i++) {
        for (int j = 0; j < vertextNum; ++j) {
            cout << adjMatrix[i][j] << "\t";
        }
        cout << endl;
    }
}
 
void initDijkTable()
{
    dijkTable.clear();
    Table tb;
    tb.dv = INT_MAX;
    tb.known = false;
    for (int i = 0; i < vertextNum; ++i)
        dijkTable.push_back(tb);
}
 
void printDijkTable()
{
    for (int i = 0; i < vertextNum; ++i) {
        cout << i << "\t" << (dijkTable[i].
                      known ? "TRUE" : "FALSE") << "\t" <<
            dijkTable[i].dv << "\t";
        for (int j = 0; j < dijkTable[i].pv.size(); ++j) {
            cout << dijkTable[i].pv.at(j) << "\t";
        }
        cout << endl;
    }
}

void addNode(Node<string> *nodeP,int index,int startindex,list<Node<string> *> *leafNodes){
    if(index==startindex){
        leafNodes->push_back(nodeP);
        return;
    }
    for(int j=0;j<dijkTable[index].pv.size();++j){
        Node<string>* nodeC=new Node<string>(words[dijkTable[index].pv.at(j)]);
        nodeC->parent=nodeP;
        addNode(nodeC,dijkTable[index].pv.at(j),startindex,leafNodes);
    }
} 

/*根据DijkTable打印所有的最短路徑*/
void printSPathFromSource(int startindex){
    ofstream ofs("shortpath",ofstream::app);        //把所有的最短路径追加方式写入文件
    for (int endindex = 0; endindex < vertextNum; ++endindex) {
        list<Node<string> *> leafNodes;
        Node<string>* nodeP=new Node<string>(words[endindex]);
        addNode(nodeP,endindex,startindex,&leafNodes);
        
        list<Node<string> *>::iterator itr=leafNodes.begin();
        while(itr!=leafNodes.end()){
            Node<string>* down=*itr;
            while(down){
                ofs<<down->data<<"\t";
                down=down->parent;
            }
            ofs<<endl;
            itr++;
        }
    }
}
 
/*指定起点，运行带pairingheap的dijkstra算法*/
void dijkstra(int start)
{
    initDijkTable();
    dijkTable[start].dv = 0;
    PairNode *phroot = new PairNode(start, 0);
    for (int i = 0; i < vertextNum; ++i) {
        if (i == start)
            continue;
        insert(phroot, new PairNode(i, INT_MAX));
    }
    while (phroot != NULL) {
        int index = phroot->nodeindex;
        dijkTable[index].known = true;
        deleteMin(phroot);
        for (int i = 0; i < vertextNum; ++i) {
            if (adjMatrix[index][i] != 0
                && dijkTable[i].known == false) {
                double newdis =
                    dijkTable[index].dv + adjMatrix[index][i];
                double delta = dijkTable[i].dv - newdis;
                if (delta > 0) {
                    pair < int, double >pa;
                    pa.first = i;
                    pa.second = dijkTable[i].dv;
                    PairNode *fp = findNode(phroot, pa);
                    if (fp == NULL) {
                        cerr << "not found:" << pa.
                            first << "(" << pa.
                            second << ")" << endl;
                        cerr << "root=" << phroot->
                            nodeindex << endl;
                        exit(-1);
                    }
                    decreaseKey(phroot, fp, delta);
                    dijkTable[i].dv = newdis;
                    dijkTable[i].pv.clear();
                    dijkTable[i].pv.push_back(index);
                } else if (delta == 0) {
                    dijkTable[i].pv.push_back(index);
                }
            }
        }
    }
    deleteHeap(phroot);
}
 
void ancRat(double base, int index, int start)
{
    if (index == start)
        return;
    int len = dijkTable[index].pv.size();
    for (int i = 0; i < len; ++i) {
        int ind = dijkTable[index].pv.at(i);
        if (ind == start)
            continue;
        pairDependency[ind] += base / (len * vertextNum);   //分母上加一项vertextNum,是为了避免计算出来的居间度太大
        ancRat(pairDependency[ind], ind, start);
    }
}
 
/*计算从顶点start到terminal的最短路径上的所有节点的pair-dependency*/
void pairDepend(int start, int terminal)
{
    ancRat(1.0, terminal, start);
}
 
int main(int argc, char *argv[])
{
    if (argc < 3) {
        cerr << "Usage:command sim_matrix_file tuple_file." << endl;
        return -1;
    }
    string filename(argv[1]);
    initWords(argv[2]);
    initMatrix(filename);   //初始华邻接矩阵
    pairDependency = new double[vertextNum];
    for (int i = 0; i < vertextNum; ++i)
        pairDependency[i] = 0;
    for (int i = 0; i < vertextNum; ++i) {
        dijkstra(i);
        printSPathFromSource(i);
        //printDijkTable();
        for (int j = 0; j < i; ++j) {    //因为是无向图，所以算一半就够了
            pairDepend(i, j);
        }
    }
    deleteMatrix();
 
    ofstream ofs("BetweenCencility");
    ofs << vertextNum << endl;
    for (int i = 0; i < vertextNum; ++i)
        ofs << pairDependency[i] << endl;
    ofs.close();
 
    return 0;
}
