#include<iostream>
#include<fstream>
#include<sstream>
#include<gdbm.h>
#include<sys/stat.h>
#include<climits>
#include<cassert>
#include<vector>
 
using namespace std;
 
/**���ƶȼ�����ز�������**/
const double init_dist = 10;
const double alpha = 5.0;
const double beta = 0.66;
const double weight[6] = { 1.0, 0.5, 0.25, 0.125, 0.06, 0.03 };
 
GDBM_FILE dbm_ptr;      //���ݿ���
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
 
/**������������(�����Ϊcodeλ�ͱ�־λ)�ľ���**/
double calDist(string code1, string code2)
{
    if (code1[7] == '@' || code2[7] == '@') /*�������ҷ�ա���������ͬ��ʴ����м�û��ͬ��ʣ�Ҳû����ش� */
        return init_dist;
    double dist = -10;  //��ʼ�����һ������
    int eqi = 0;        //����code��ͬ�ı���
    int i;
    for (i = 0; i < 7; ++i) {
        if (code1[i] != code2[i])
            break;
        eqi++;
    }
    if (i < 7) {     //codeλ��ͬ
        switch (eqi) {
        case 0:
        case 1:
        case 2:
            dist = weight[eqi] * init_dist;
            break;
        case 3:
        case 4:
        case 5:
            dist = weight[eqi - 1] * init_dist;
            break;
        case 6:
            dist = weight[eqi - 2] * init_dist;
            break;
        default:
            break;
        }
    } else {        //codeλ��ͬ
        if (code1[i] == code2[i]) { //��־λ��ͬ
            if (code1[i] == '=')    //ͬ��
                dist = 0;
            else if (code1[i] == '#')   //ͬ��
                dist = weight[5] * init_dist;
        } else {    //ֻ��codeλ��ͬ����־λ��һ����ͬ������else�������������
            cout << code1 << "��" << code2 <<
                "codeλ��ͬ����־λ��Ȼ����ͬ��" <<
                endl;
            return -1;
        }
    }
    return dist;
}
 
/**���������ʵ����ƶ�**/
double calSim(string word1, string word2)
{
    if (word1 == word2) //�����ͬһ���ʣ������ƶ�Ϊ1
        return 1;
    datum key1, data1, key2, data2;
    key1.dptr = (char *)word1.c_str();
    key1.dsize = word1.size() + 1;
    data1 = gdbm_fetch(dbm_ptr, key1);
    int size1 = data1.dsize;
    key2.dptr = (char *)word2.c_str();
    key2.dsize = word2.size() + 1;
    data2 = gdbm_fetch(dbm_ptr, key2);
    int size2 = data2.dsize;
 
    if (size1 != 0 && size2 != 0) { //�����ʶ��ڴ������ҵõ�
        int i, j;
        string word1;
        vector <string> vec1, vec2;
        string buffer1(data1.dptr);
        istringstream stream1(buffer1);
        stream1 >> word1;
        stream1 >> word1; //·��ǰ����
        while (stream1 >> word1) {
            vec1.push_back(word1);  //�Ѵʶ�Ӧ�ı��붼����vector�� 
        }
        string word2;
        string buffer2(data2.dptr);
        istringstream stream2(buffer2);
        stream2 >> word2;
        stream2 >> word2; //·��ǰ����
        while (stream2 >> word2) {
            vec2.push_back(word2);
        }
        double minDist = INT_MAX;   //��ʼ����Ϊ�����
        for (int i = 0; i != vec1.size(); ++i) {
            for (int j = 0; j != vec2.size(); ++j) {
                //cout<<vec1[i]<<"��"<<vec2[j]<<"�ľ���"<<endl;
                double dist = calDist(vec1[i], vec2[j]);
                if (dist < minDist)
                    minDist = dist; //�����ʵľ��������б�������о������Сֵ
            }
        }
        return alpha / (alpha + minDist);   //�Ӿ��뵽���ƶȵ�ת��
    } else          //ֻҪ��һ���ʲ��ڴ����У��򷵻����ƶ�Ϊ0
        return 0;
}
 
int main(int argc, char *argv[])
{
    if (argc < 2) {
        cout << "Usage:command filename." << endl;
        return 0;
    }
    string infile(argv[1]);
    initWords(infile);
    dbm_ptr = gdbm_open("sydb", 0, GDBM_READER, S_IRUSR | S_IWUSR, NULL);
    ofstream ofs("simadj");
    ofs << words.size() << endl;    //���ڽӾ���Ĺ�ģд���ļ�����
    for (int i = 0; i != words.size(); ++i) {
        ofs << i << "\t";
        for (int j = 0; j < i; ++j) {    //�Ѷ���֮������ƶȴ��������Ǿ���
            double sim = calSim(words[i], words[j]);
            if (sim > beta)  //���ƶȴ�����ֵʱ����Ϊ��������֮���б�
                ofs << j << "(" << sim << ")" << "\t";
        }
        ofs << endl;
    }
    ofs.close();
    gdbm_close(dbm_ptr);
    return 0;
}