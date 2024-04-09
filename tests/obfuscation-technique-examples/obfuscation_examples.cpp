#include <iostream>
using namespace std;

string decrypt(string encrypted, int key){

    for (int i = 0; i < encrypted.length(); i++){
        encrypted[i] = encrypted[i] ^ key;
    }
    return encrypted;

}

int main(){

    string correct_password = "|m{|9:;<";   //test1234
    string wrong_password = "abcdefgh";
    string password;
    cout << "Enter passphrase: ";
    cin >> password;

    if((password.length() * 256) % 2 == 0){
        if(password == decrypt(correct_password, 8))
            cout << "Correct password!" << endl;
        }
    else{
        if(password == decrypt(wrong_password, 8))
            cout << "Correct password!" << endl;
    }
    
    return 0;

}