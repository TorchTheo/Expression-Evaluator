#include<iostream>
#include<regex>
using namespace std;
int main() {
    regex r("Func\\s+(.*?)\\((.*?)\\)\\s*=\\s*([0-9|a-z|A-Z|\\s*+\\s*|\\s*\\-\\s*|\\s*\\*\\s*|\\s*/\\s*]+)");
    string line;
    getline(cin, line);
    smatch m;
    regex_match(line, m, r);
    cout<<m[0]<<endl<<m[1]<<endl<<m[2]<<endl<<m[3]<<endl;
//     cout<<regex_match(line, r)<<endl;
    return 0;
}
