#include<utility>
#include<climits>
#include<vector>
#include<fstream>
#include<iostream>
#include<sstream>
#include<cassert>
#include<cstdlib>
#include<list>
#include"pairheap.h"
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
 
int vertextNum;         //������
double **adjMatrix;     //���ڽӾ������洢ͼ
vector < Table > dijkTable;   //dijkstraά���ı�
double *pairDependency;

vector <string> words;    //�洢�����г��ֵĴ�
 
/**�������еĴʣ�����vector**/
void initWords(string filename)
{
    ifstream ifs(filename.c_str());
    assert(ifs);
    string line;
    while (getline(ifs, line)) {
        istringstream stream(line);
        string word;
        stream >> word;   //����һ���е����д��Ｔ��
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
    getline(ifs, line); //������һ�У�����ž���Ĺ�ģ
    istringstream stream(line);
    stream >> scale;
    vertextNum = scale;
 
    adjMatrix = new double *[scale];    //��̬������ά����
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
        strm >> vari; //��һ�д洢���Ƕ����index
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
 
/*�ͷ��ڽӾ���*/
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

/*����DijkTable��ӡ���е����·��*/
void printSPathFromSource(int startindex){
    ofstream ofs("shortpath",ofstream::app);        //�����е����·��׷�ӷ�ʽд���ļ�
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
 
/*ָ����㣬���д�pairingheap��dijkstra�㷨*/
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
        pairDependency[ind] += base / (len * vertextNum);   //��ĸ�ϼ�һ��vertextNum,��Ϊ�˱����������ľӼ��̫��
        ancRat(pairDependency[ind], ind, start);
    }
}
 
/*����Ӷ���start��terminal�����·���ϵ����нڵ��pair-dependency*/
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
    initMatrix(filename);   //��ʼ���ڽӾ���
    pairDependency = new double[vertextNum];
    for (int i = 0; i < vertextNum; ++i)
        pairDependency[i] = 0;
    for (int i = 0; i < vertextNum; ++i) {
        dijkstra(i);
        printSPathFromSource(i);
        //printDijkTable();
        for (int j = 0; j < i; ++j) {    //��Ϊ������ͼ��������һ��͹���
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
