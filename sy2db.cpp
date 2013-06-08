#include<gdbm.h>
#include<iostream>
#include<fstream>
#include<sys/stat.h>
#include<sstream>
using namespace std;
 
int main(int argc, char *argv[])
{
    string infile("synonyms.txt");
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
        stream >> word;   //取出每行的第一列作为key
        key.dptr = (char *)word.c_str();
        key.dsize = word.size() + 1;
        data.dptr = (char *)line.c_str();
        data.dsize = line.size() + 1;
        gdbm_store(dbm_ptr, key, data, GDBM_REPLACE);
        cout<<line.c_str()<<endl;
        cout<<word.c_str()<<endl;
        cout<<endl;
    }
 
    ifs.close();
    gdbm_close(dbm_ptr);
    return 0;
}
