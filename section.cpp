#include<iostream>
#include<cassert>
#include<string>
#include<fstream>
using namespace std;
 
int main(int argc, char *argv[])
{
    if (argc < 2) {      //命令行中需要给定要处理的文件名
        cout << "Usage:command filename" << endl;
        return 1;
    }
    string filename(argv[1]);   //输入文件名
    string outfile = filename + "_part";    //输出文件名
    ifstream ifs(filename.c_str()); //打开输入文件
    ofstream ofs(outfile.c_str());  //打开输出文件
    if (!(ifs && ofs)) {
        cerr << "error:open file failed." << endl;
        return 1;
    }
    string content, word;
    while (ifs >> word)   //把文件的内容全部读到content中，去除了换行符，相当于把整篇文档合并成了一行
        content.append(word + "\t");
 
    string period = "。/wj";
    string::size_type front = content.find(period); //寻找第一个句号所在的位置
    assert(front != string::npos);
    content.replace(front, 6, "\t\t\t\t\n\n");
 
    front = content.find(period, front + 1);    //寻找第二个句号所在的位置
    front = content.find(period, front + 1);    //寻找第三个句号所在的位置
    content.replace(front, 6, "\t\t\t\t\n\n");
 
    string::size_type back = content.rfind(period); //最后一个句号
    assert(back != string::npos);
    content.replace(back, 3, ".");
    back = content.rfind(period);   //倒数第二个句号
    content.replace(back, 3, ".");
    back = content.rfind(period);   //倒数第三个句号
    content.replace(back, 6, "\t\t\t\t\n\n");
 
    ofs << content;
    ifs.close();
    ofs.close();
    return 0;
}