#include <bits/stdc++.h>
using namespace std;

vector<string> split(string str, char del){
    string temp = "";   
    vector<string> result;
    str = str+':';
    for(int i=0; i<(int)str.size(); i++){
        if(str[i] != del){
            temp += str[i];
        }
        else {
            result.push_back(temp);
            temp = "";
        }
    }
    return result;
}
int main() {
    string username = "";
    cout<<"Enter username:";
    cin>>username;

    ifstream passwdFile("/etc/passwd");
    ifstream groupFile("/etc/group");
    bool found = false;
    string line = "";

    while (getline (passwdFile, line)) {
        vector<string> result;
        result = split(line,':');
        if (result[0] == username) {
            cout<<"id: "<<result[2]<<endl;
            cout<<"username: "<<result[0]<<endl;
            cout<<"home: "<<result[5]<<endl;
            cout<<"group: ";
            found=true;
        }
    }
    if (!found) {
        cout<<"Not found"<<endl;
        return 0;
    }
    
    line = "";
    vector<string> groupList;
    while (getline (groupFile, line)) {
        vector<string> result;
        result = split(line,':');
        
        if(find(result.begin(), result.end(), username) != result.end()) {
            groupList.push_back(result[0]);
        }
    }
    for (int i =0;i<groupList.size();i++) {
        cout<<groupList[i]<<" ";
    }
    cout<<endl;
    passwdFile.close();
    groupFile.close();
    return 0;
}