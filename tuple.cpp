#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<map>
using namespace std;
 
struct Tuple {
    string word;        //����
    string pos;     //����
    int occurs;     //���ִ���
    short local1;       //�����г���
    short local2;       //���׳���
    short local3;       //��β����
 
    //���캯��
     Tuple() {
    };          //����TupleҪ��Ϊmap��second�����Ա����ṩ�ղ������캯��
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
            int index = word.find("/"); //б��֮ǰ�Ǵ��б��֮���Ǵ���
            string front = word.substr(0, index);
            itr = tmap.find(front);
            if (itr == tmap.end()) {    //��ĿǰΪֹû�г��ֹ�
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
            } else {    //���������ֹ�
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
        //����������ԣ�������λ�ã�д���ļ�
        ofs << itr->second.word << "\t" << itr->
            second.pos << "\t" << itr->second.
            occurs << "\t" << itr->second.local1 << "\t" << itr->second.
            local2 << "\t" << itr->second.local3 << endl;
    }
 
    ifs.close();
    ofs.close();
    return 0;
}