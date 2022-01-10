#include <bits/stdc++.h>
#include <unistd.h>
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
    string currentUser = "";
    //Get current user
    if (getenv("SUDO_USER") != NULL) 
        currentUser = getenv("SUDO_USER"); 
    else 
        currentUser = getenv("USER");
    cout<<"Hello "<<currentUser<<endl;
    // cout<<currentUser<<endl;

    string oldPasswd = "";
    cout<<"Enter old password:";
    cin>>oldPasswd;
    // oldPasswd = crypt();
     // importantttttttt

    ifstream shadowFile("/etc/shadow");
    string line = "";
    vector<string> shadowLines;
    while (getline(shadowFile, line)){
        shadowLines.push_back(line+':');
    }
    bool changed = false;
    vector<vector<string>> original;
    for (int i = 0; i < shadowLines.size(); i++) {
        vector<string> tmp;
        tmp = split(shadowLines[i],':');
        if (tmp[0] == currentUser) {
            
            cout<<"Found user"<<endl;

            oldPasswd = crypt(oldPasswd.c_str(),("$6$" + split(tmp[1],'$')[2]).c_str());

            cout<<oldPasswd<<endl<<tmp[1]<<endl;

            if (oldPasswd == tmp[1]) {
                cout<<"Password correct, please enter new password:";
                string newPassword = "";
                cin>> newPassword;
                newPassword = crypt(newPassword.c_str(),("$6$" + split(tmp[1],'$')[2]).c_str());
                tmp[1]=newPassword;
                changed = true;
            }
        }
        original.push_back(tmp);
    }
    shadowFile.close();
    if (changed) {
        ofstream shadowFile("/etc/shadow");
        for (int i = 0; i < original.size(); i++) {
            for (int j = 0; j < original[i].size()-2; j++ ) {
                shadowFile<<original[i][j]<<":";
            }
            shadowFile<<original[i][original[i].size()-1]<<endl;
        }
    }
    shadowFile.close();
    return 0;
}
