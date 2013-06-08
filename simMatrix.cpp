#include<iostream>
#include<fstream>
#include<sstream>
#include<gdbm.h>
#include<sys/stat.h>
#include<climits>
#include<cassert>
#include<vector>
 
using namespace std;
 
/**相似度计算相关参数设置**/
const double init_dist = 10;
const double alpha = 5.0;
const double beta = 0.66;
const double weight[6] = { 1.0, 0.5, 0.25, 0.125, 0.06, 0.03 };
 
GDBM_FILE dbm_ptr;      //数据库句柄
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
 
/**计算两个编码(编码分为code位和标志位)的距离**/
double calDist(string code1, string code2)
{
    if (code1[7] == '@' || code2[7] == '@') /*词语自我封闭、独立，在同义词词林中既没有同义词，也没有相关词 */
        return init_dist;
    double dist = -10;  //初始距离给一个负数
    int eqi = 0;        //两个code相同的倍数
    int i;
    for (i = 0; i < 7; ++i) {
        if (code1[i] != code2[i])
            break;
        eqi++;
    }
    if (i < 7) {     //code位不同
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
    } else {        //code位相同
        if (code1[i] == code2[i]) { //标志位相同
            if (code1[i] == '=')    //同义
                dist = 0;
            else if (code1[i] == '#')   //同类
                dist = weight[5] * init_dist;
        } else {    //只有code位相同，标志位就一定相同，所以else的情况不会了生
            cout << code1 << "和" << code2 <<
                "code位相同，标志位居然不相同！" <<
                endl;
            return -1;
        }
    }
    return dist;
}
 
/**计算两个词的相似度**/
double calSim(string word1, string word2)
{
    if (word1 == word2) //如果是同一个词，则相似度为1
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
 
    if (size1 != 0 && size2 != 0) { //两个词都在词林中找得到
        int i, j;
        string word1;
        vector <string> vec1, vec2;
        string buffer1(data1.dptr);
        istringstream stream1(buffer1);
        stream1 >> word1;
        stream1 >> word1; //路过前两列
        while (stream1 >> word1) {
            vec1.push_back(word1);  //把词对应的编码都存入vector中 
        }
        string word2;
        string buffer2(data2.dptr);
        istringstream stream2(buffer2);
        stream2 >> word2;
        stream2 >> word2; //路过前两列
        while (stream2 >> word2) {
            vec2.push_back(word2);
        }
        double minDist = INT_MAX;   //初始距离为无穷大
        for (int i = 0; i != vec1.size(); ++i) {
            for (int j = 0; j != vec2.size(); ++j) {
                //cout<<vec1[i]<<"和"<<vec2[j]<<"的距离"<<endl;
                double dist = calDist(vec1[i], vec2[j]);
                if (dist < minDist)
                    minDist = dist; //两个词的距离是所有编码组合中距离的最小值
            }
        }
        return alpha / (alpha + minDist);   //从距离到相似度的转换
    } else          //只要有一个词不在词林中，则返回相似度为0
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
    ofs << words.size() << endl;    //把邻接矩阵的规模写入文件首行
    for (int i = 0; i != words.size(); ++i) {
        ofs << i << "\t";
        for (int j = 0; j < i; ++j) {    //把顶点之间的相似度存入下三角矩阵
            double sim = calSim(words[i], words[j]);
            if (sim > beta)  //相似度大于阈值时才认为两个顶点之间有边
                ofs << j << "(" << sim << ")" << "\t";
        }
        ofs << endl;
    }
    ofs.close();
    gdbm_close(dbm_ptr);
    return 0;
}
