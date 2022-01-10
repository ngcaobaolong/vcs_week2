#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

//split like python
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
    
    //Get current password from user input
    string oldPasswd = "";
    cout<<"Enter old password:";
    cin>>oldPasswd;

    //read shadow file to get password hash
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
        //if found user in shadow
        if (tmp[0] == currentUser) {
            
            cout<<"Found user"<<endl;
            //harsh the inputed password with salt from shadow
            oldPasswd = crypt(oldPasswd.c_str(),("$6$" + split(tmp[1],'$')[2]).c_str());
            //if newly hashed password is the same with password in shadow, proceed
            if (oldPasswd == tmp[1]) {
                cout<<"Password correct, please enter new password:";
                string newPassword = "";
                //get new password from user to hash
                cin>> newPassword;
                newPassword = crypt(newPassword.c_str(),("$6$" + split(tmp[1],'$')[2]).c_str());
                tmp[1]=newPassword;
                changed = true;
            }
        }
        original.push_back(tmp);
    }
    shadowFile.close();
    //if password changed, rewrite shadow file
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
