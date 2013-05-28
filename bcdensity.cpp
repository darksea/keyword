#include<vector>
#include<functional>
#include<algorithm>
#include<fstream>
#include<sstream>
#include<iostream>
#include<climits>
#include<cstdlib>
#include<cassert>
 
using namespace std;
 
class myclass {
 public:
    myclass(int a, double b):first(a), second(b) {
    } int first;
    double second;
    //����<������������1�����ݳ�Ա����
    bool operator <(const myclass & m) const {
        return first < m.first;
}};
//�Զ���º���������2�����ݳ�Ա����
bool less_second(const myclass & m1, const myclass & m2)
{
    return m1.second < m2.second;
}
 
vector < myclass > bc_vec;
vector < myclass > bcdens_vec;
int vec_len;
int s = 2;
const int c = 5;
const float d = 0.8;
int maxloop = 6;
 
//��ʼ���Ӽ�ȣ����ļ��ж�ȡ��
void init_bcvec(string filename)
{
    ifstream ifs(filename.c_str());
    assert(ifs);
    string line;
    getline(ifs, line); //�ӵ�һ����ȡ��ȫ�����������
    stringstream strstm(line);
    strstm >> vec_len;
    int index = 0;
    while (getline(ifs, line)) {
        stringstream strstm(line);
        double bc;
        strstm >> bc;
        myclass mc(index, bc);
        bc_vec.push_back(mc);
        ++index;
    }
    sort(bc_vec.begin(), bc_vec.end(), less_second);    //����second��Ա��vector����
}
 
//����Ӽ���ܶ�
void density()
{
    int loop = 0;
    double maxratio = INT_MAX;
    double *IntervalDensity = NULL;
    double block;
    vector < myclass >::const_iterator itr;
    while (maxratio >= d && loop++ <= maxloop) {
        s *= c;
        //�ѾӼ��ƽ����Ϊs������
        block = (bc_vec[vec_len - 1].second - bc_vec[0].second) / s;
        if (IntervalDensity != NULL)
            delete[]IntervalDensity;
        IntervalDensity = new double[s];
        itr = bc_vec.begin();
        //�������������Ԫ����Ŀռ��Ԫ�ص���Ŀ
        for (int i = 0; i < s; ++i) {
            while (itr != bc_vec.end()
                   && itr->second <= block * (i + 1)) {
                IntervalDensity[i]++;
                itr++;
            }
            IntervalDensity[i] /= vec_len;
 
        }
 
        maxratio = *max_element(IntervalDensity, IntervalDensity + s);
    }
    //����ÿ�������Ӧ�ľӼ���ܶ�
    itr = bc_vec.begin();
    for (int i = 0; i < s; ++i) {
        while (itr != bc_vec.end() && itr->second <= block * (i + 1) + 1e-8) {    //����������������Լ������1e-8
            int index = itr->first;
            double dens = IntervalDensity[i];
            myclass mc(index, dens);
            bcdens_vec.push_back(mc);
            itr++;
        }
    }
    delete[]IntervalDensity;
}
 
//�ѾӼ���ܶ�������ļ�
void printDensity(string infile, string outfile)
{
    //Vd��first����
    sort(bcdens_vec.begin(), bcdens_vec.end());
    ifstream ifs(infile.c_str());
    ofstream ofs(outfile.c_str());
    assert(ifs && ofs);
    vector < myclass >::const_iterator itr = bcdens_vec.begin();
    string line;
    while (getline(ifs, line) && itr != bcdens_vec.end()) {
        ofs << line << "\t" << (itr++)->second << endl;
    }
    ifs.close();
    ofs.close();
}
 
int main(int argc, char *argv[])
{
    if (argc < 4) {
        cout << "Usage:" << argv[0] << " tuplefile bcfile mergefile" <<
            endl;
        return 0;
    }
    init_bcvec(argv[2]);
    density();
    printDensity(argv[1], argv[3]);
    return 0;
}