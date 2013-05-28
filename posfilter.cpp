#include<iostream>
#include<fstream>
#include<string>
#include<set>
#include<cstring>
using namespace std;
 
int main(int argc, char *argv[])
{
    if (argc < 2) {      //����������Ҫ����Ҫ������ļ���
        cout << "Usage:command filename" << endl;
        return 1;
    }
 
    string arr_pos[] = { "/n",  //����
        "/nr",      //����
        "/nr1",     //��������
        "/nr2",     //��������
        "/nrj",     //��������
        "/ns",      //����
        "/nt",      //����������
        "/wj",      //���
        "/nl",      //�����Թ�����
        "/ng",      //����������
        "/v",       //����
        "/vd",      //������
        "/vn",      //������
        "/vl",      //�����Թ�����
        "/vg",      //����������
        "/a",       //���ݴ�
        "/an",      //���δ�
        "/ag",      //���ݴ�������
        "/al",      //���ݴ��Թ�����
        ""
    };
    set < string > set_pos;
    int i;
    for (i = 0; arr_pos[i] != ""; ++i)
        set_pos.insert(arr_pos[i]);
 
    string filename(argv[1]);
    string outfile = filename + "_pos";
 
    ifstream ifs(filename.c_str()); //�������ļ�
    ofstream ofs(outfile.c_str());  //������ļ�
    if (!(ifs && ofs)) {
        cerr << "error:open file failed." << endl;
        return 1;
    }
 
    string word;
    while (ifs >> word) {
        bool flag = false;
        int length = word.find("/");
        //cout<<word<<"\t"<<length<<endl;
        if (length == 3 && strncmp(word.c_str(), "��", 3) != 0)  //���˵��������֣����ǲ�����ȫ�Ǿ��(��Ϊһ�����־�3���ֽ�)
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