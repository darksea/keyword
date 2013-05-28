#include<iostream>
#include<fstream>
#include<string>
#include<set>
#include<cstring>
using namespace std;
 
int main(int argc, char *argv[])
{
    if (argc < 2) {      //命令行中需要给定要处理的文件名
        cout << "Usage:command filename" << endl;
        return 1;
    }
 
    string arr_pos[] = { "/n",  //名词
        "/nr",      //人名
        "/nr1",     //汉语姓氏
        "/nr2",     //汉语名字
        "/nrj",     //日语人名
        "/ns",      //地名
        "/nt",      //机构团体名
        "/wj",      //句号
        "/nl",      //名词性惯用语
        "/ng",      //名词性语素
        "/v",       //动词
        "/vd",      //副动词
        "/vn",      //名动词
        "/vl",      //动词性惯用语
        "/vg",      //动词性语素
        "/a",       //形容词
        "/an",      //名形词
        "/ag",      //形容词性语素
        "/al",      //形容词性惯用语
        ""
    };
    set < string > set_pos;
    int i;
    for (i = 0; arr_pos[i] != ""; ++i)
        set_pos.insert(arr_pos[i]);
 
    string filename(argv[1]);
    string outfile = filename + "_pos";
 
    ifstream ifs(filename.c_str()); //打开输入文件
    ofstream ofs(outfile.c_str());  //打开输出文件
    if (!(ifs && ofs)) {
        cerr << "error:open file failed." << endl;
        return 1;
    }
 
    string word;
    while (ifs >> word) {
        bool flag = false;
        int length = word.find("/");
        //cout<<word<<"\t"<<length<<endl;
        if (length == 3 && strncmp(word.c_str(), "。", 3) != 0)  //过滤掉单个汉字，但是不过滤全角句号(因为一个汉字就3个字节)
            continue;
        string pos = word.substr(length);
        if (set_pos.find(pos) != set_pos.end())
            flag = true;
        if (flag)
            ofs << word << "\t";
    }
 
    ifs.close();
    ofs.close();
    return 0;
}