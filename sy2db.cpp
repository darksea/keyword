#include<gdbm.h>
#include<iostream>
#include<fstream>
#include<sys/stat.h>
#include<sstream>
using namespace std;
 
int main(int argc, char *argv[])
{
    string infile("a.mdb");
    ifstream ifs(infile.c_str());
    if (!ifs) {
        cerr << "open file failed!" << endl;
        return -1;
    }
 
    GDBM_FILE dbm_ptr;
    dbm_ptr = gdbm_open("sydb", 0, GDBM_WRCREAT, S_IRUSR | S_IWUSR, NULL);
    datum key, data;
 
    string line, word;
    while (getline(ifs, line)) {
        istringstream stream(line);
        stream >> word;   //ȡ��ÿ�еĵ�һ����Ϊkey
        key.dptr = (char *)word.c_str();
        key.dsize = word.size() + 1;
        data.dptr = (char *)line.c_str();
        data.dsize = line.size() + 1;
        gdbm_store(dbm_ptr, key, data, GDBM_REPLACE);
    }
 
    ifs.close();
    gdbm_close(dbm_ptr);
    return 0;
}
