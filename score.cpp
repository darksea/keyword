
#include<fstream>
#include<iostream>
#include<cassert>
#include<sstream>
#include<cstdlib>
#include<queue>
#include<stack>
using namespace std;
 
class myclass {
 public:
    string word;
    double score;
     myclass(string w, double s):word(w), score(s) {
    };
    bool operator <(const myclass & rth) const {
        return score > rth.score;
}};
 
priority_queue < myclass > maxPQ;
int queue_size;         //控制大根堆的容量，即关键词的数目
 
const float vdw = 0.6;
const float tw = 0.4;
const float locw1 = 0.5;
const float locw2 = 0.3;
const float locw3 = 0.3;
const float lenw = 0.01;
const float posw = 0.5;
const float tfw = 0.8;
 
double getScore(int wordlen, string pos, int count, int title, int head,
        int tail, double bcdens)
{
    double score = 0.0;
    double pos_score;
    int len = pos.size();
    if (pos[len - 1] == 'g') {  //语素
        pos_score = 0.2;
    } else if (pos[0] == 'n'    //名词
           || pos[len - 1] == 'n'   //名动词或名形词
           || pos[len - 1] == 'l') {    //成语或习用语
        pos_score = 0.6;
    } else if (pos == "v") {    //动词
        pos_score = 0.3;
    } else if (pos == "vd") {   //副动词
        pos_score = 0.4;
    } else if (pos == "a") {    //形容词
        pos_score = 0.5;
    } else {
        cerr << "存在未知词性." << endl;
        pos_score = 0.0;
    }
    score =
        vdw * bcdens + tw * (title * locw1 + head * locw2 + tail * locw3 +
                 wordlen * lenw + pos_score * posw +
                 count * tfw);
    return score;
}
 
int main(int argc, char *argv[])
{
    if (argc < 3) {
        cout << "Usage:" << argv[0] << " filename num_of_keywords" <<
            endl;
        return 0;
    }
    string keyword = "";
    keyword+="firefox http://www.google.com.hk/search?q=";
    queue_size = atoi(argv[2]);
    string outfile = "kw_score";
    ifstream ifs(argv[1]);
    ofstream ofs(outfile.c_str());
    assert(ifs && ofs);
    string line;
    while (getline(ifs, line)) {
        string word, pos;
        int count, title, head, tail;
        double bcdens;
        istringstream stream(line);
        stream >> word >> pos >> count >> title >> head >> tail >>
            bcdens;
        double score =
            getScore(word.size() / 3, pos, count, title, head, tail,
                 bcdens);
        //ofs << word << "\t" << score << endl;
        myclass mc(word, score);
        if (maxPQ.size() == queue_size) {
            if (mc < maxPQ.top()) {
                maxPQ.pop();
                maxPQ.push(mc);
            }
        } else {
            maxPQ.push(mc);
        }
    }
    ifs.close();
    ofs.close();
    //cout << "关键词:" << endl;
    stack<myclass> st;
    while (!maxPQ.empty()) {
        myclass mc = maxPQ.top();
        st.push(mc);
        maxPQ.pop();
    }
    while(!st.empty()){
        myclass mc = st.top();
        //cout << mc.word << "\t" << mc.score << endl;
        keyword+=mc.word;
        keyword+="+";
        //cout<<mc.word<<endl;
        st.pop();
    }
    //cout<<keyword;
    system(keyword.c_str());
    return 0;
}
