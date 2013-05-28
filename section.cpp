#include<iostream>
#include<cassert>
#include<string>
#include<fstream>
using namespace std;
 
int main(int argc, char *argv[])
{
    if (argc < 2) {      //����������Ҫ����Ҫ������ļ���
        cout << "Usage:command filename" << endl;
        return 1;
    }
    string filename(argv[1]);   //�����ļ���
    string outfile = filename + "_part";    //����ļ���
    ifstream ifs(filename.c_str()); //�������ļ�
    ofstream ofs(outfile.c_str());  //������ļ�
    if (!(ifs && ofs)) {
        cerr << "error:open file failed." << endl;
        return 1;
    }
    string content, word;
    while (ifs >> word)   //���ļ�������ȫ������content�У�ȥ���˻��з����൱�ڰ���ƪ�ĵ��ϲ�����һ��
        content.append(word + "\t");
 
    string period = "��/wj";
    string::size_type front = content.find(period); //Ѱ�ҵ�һ��������ڵ�λ��
    assert(front != string::npos);
    content.replace(front, 6, "\t\t\t\t\n\n");
 
    front = content.find(period, front + 1);    //Ѱ�ҵڶ���������ڵ�λ��
    front = content.find(period, front + 1);    //Ѱ�ҵ�����������ڵ�λ��
    content.replace(front, 6, "\t\t\t\t\n\n");
 
    string::size_type back = content.rfind(period); //���һ�����
    assert(back != string::npos);
    content.replace(back, 3, ".");
    back = content.rfind(period);   //�����ڶ������
    content.replace(back, 3, ".");
    back = content.rfind(period);   //�������������
    content.replace(back, 6, "\t\t\t\t\n\n");
 
    ofs << content;
    ifs.close();
    ofs.close();
    return 0;
}