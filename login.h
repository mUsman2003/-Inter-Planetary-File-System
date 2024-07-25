#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool AdminLoginPermission()
{
    string name;
    string password;

    string realName;
    string realPassword;

    cout << "\nEnter Username: ";
    cin >> name;
    cout << "Enter Password: ";
    cin >> password;

    ifstream admin("admin.txt");
    if (admin.is_open())
    {
        int t;
        admin >> realName >> realPassword;

        if (name == realName && password == realPassword)
        {
            cout << "Login successful!" <<  endl;
            admin.close(); // Close the file before returning
            return true;
            //cin >> t;
        }
        else
        {
            cout << "Login failed. Incorrect username or password." <<  endl;
            admin.close(); // Close the file before returning
            return false;
        }
    }
    else
    {
         cout << "Error opening the file." <<  endl;
    }

    return false;
}

bool UserLogin() 
{

    string name;
    string password;

    string realName;
    string realPassword;

    cout << "\nEnter Employee Username: ";
    cin >> name;
    cout << "Enter Employee Password: ";
    cin >> password;

    ifstream userFile("userinfo.txt");

    while (userFile.is_open() && !userFile.eof()) 
    {
        userFile >> realName >> realPassword;
        if (name == realName && password == realPassword) 
        {
            cout << "Login successful!" << endl;

            userFile.close();
            return true;
        }
    }
    cout << "Login failed. Incorrect username or password." << endl;
    userFile.close();
    return false;
}