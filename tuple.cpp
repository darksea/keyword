#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<map>
using namespace std;
 
struct Tuple {
    string word;        //词语
    string pos;     //词性
    int occurs;     //出现次数
    short local1;       //标题中出现
    short local2;       //段首出现
    short local3;       //段尾出现
 
    //构造函数
     Tuple() {
    };          //由于Tuple要作为map的second，所以必须提供空参数构造函数
    Tuple(string w, string p, int o = 1, short l1 = 0, short l2 =
          0, short l3 = 0)
 :      word(w), pos(p), occurs(o), local1(l1), local2(l2), local3(l3) {
    };
 
};
 
int main(int argc, char *argv[])
{
    if (argc < 2) {
        cout << "Usage:command filename." << endl;
        return 0;
    }
 
    string infile(argv[1]);
    string outfile = infile + "_tuple";
    ifstream ifs(infile.c_str());
    ofstream ofs(outfile.c_str());
    if (!(ifs && ofs)) {
        cerr << "Open file failed." << endl;
        return -1;
    }
 
    map < string, Tuple > tmap;
    map < string, Tuple >::const_iterator itr;
    string line;
    for (int i = 0; i < 7, getline(ifs, line); ++i) {
        istringstream stream(line);
        string word;
        while (stream >> word) {
            int index = word.find("/"); //斜杠之前是词语，斜杠之后是词性
            string front = word.substr(0, index);
            itr = tmap.find(front);
            if (itr == tmap.end()) {    //到目前为止没有出现过
                string post = word.substr(index + 1);
                if (post == "wj")
                    continue;
                Tuple tuple(front, post);
                switch (i) {
                case 0:
                    tuple.local1 = 1;
                    break;
                case 2:
                    tuple.local2 = 1;
                    break;
                case 6:
                    tuple.local3 = 1;
                    break;
                default:
                    break;
                }
                tmap[front] = tuple;
            } else {    //词语曾出现过
                Tuple tuple = tmap[front];
                tuple.occurs++;
                switch (i) {
                case 0:
                    tuple.local1 = 1;
                    break;
                case 2:
                    tuple.local2 = 1;
                    break;
                case 6:
                    tuple.local3 = 1;
                    break;
                default:
                    break;
                }
                tmap[front] = tuple;
            }
        }
    }
 
    for (itr = tmap.begin(); itr != tmap.end(); ++itr) {
        //将（词语，词性，次数，位置）写入文件
        ofs << itr->second.word << "\t" << itr->
            second.pos << "\t" << itr->second.
            occurs << "\t" << itr->second.local1 << "\t" << itr->second.
            local2 << "\t" << itr->second.local3 << endl;
    }
 
    ifs.close();
    ofs.close();
    return 0;
}
