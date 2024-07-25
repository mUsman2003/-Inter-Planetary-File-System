#include <Windows.h> 
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <string>
#include <random>
#include <sstream>
#include <vector>

#include "Circular_link_list.h"
#include "login.h"
#include "sha1.h"

using namespace std;

string hexToDecimal(const string& hex) 
{
    unsigned int decimalValue;
    std::stringstream ss;

    ss << std::hex << hex;
    ss >> decimalValue;

    return std::to_string(decimalValue);
}

BigInt hasher(BigInt& v, int capacity)
{
    int hashlength = v.value.length();
    cout << "length is : " << hashlength << endl;
    BigInt temp;
    temp.value = "";
    capacity /= 4;
    for (int i = 0; i < capacity; i++)
        temp.value += v.value[i];
    temp.value = hexToDecimal(temp.value);

    return temp;
}

int compare(const std::string& str1, const std::string& str2) {
    if (str1.length() < str2.length()) return -1;
    if (str1.length() > str2.length()) return 1;
    return str1.compare(str2);
}

// Function to subtract two strings representing integers
std::string subtract(const std::string& str1, const std::string& str2) {
    std::string result;

    int carry = 0;
    int len1 = str1.length(), len2 = str2.length();

    for (int i = 0; i < len1 || i < len2; ++i) {
        int digit1 = (i < len1) ? (str1[len1 - i - 1] - '0') : 0;
        int digit2 = (i < len2) ? (str2[len2 - i - 1] - '0') : 0;

        int diff = digit1 - digit2 - carry;

        if (diff < 0) {
            diff += 10;
            carry = 1;
        }
        else {
            carry = 0;
        }

        result = char(diff + '0') + result;
    }

    // Remove leading zeros
    size_t pos = result.find_first_not_of('0');
    return (pos != std::string::npos) ? result.substr(pos) : "0";
}

// Function to perform division on large integers
std::string divideLargeIntegers(const std::string& numerator, const std::string& denominator) {
    if (denominator == "0") {
        return "Error: Division by zero";
    }

    std::string result, current;
    int compareResult = compare(numerator, denominator);

    // If numerator is smaller than denominator, result is 0
    if (compareResult < 0) {
        return "0";
    }

    // If numerator is equal to denominator, result is 1
    if (compareResult == 0) {
        return "1";
    }

    // Perform long division
    for (char digit : numerator) {
        current += digit;
        int count = 0;
        while (compare(current, denominator) >= 0) {
            current = subtract(current, denominator);
            count++;
        }
        result += char(count + '0');
    }

    // Remove leading zeros
    size_t pos = result.find_first_not_of('0');
    return (pos != std::string::npos) ? result.substr(pos) : "0";
}

string SimpleHashFunction(const string& input_string) 
{
    unsigned int hash = 0;

    // Simple hash calculation based on the ASCII values of characters
    for (char ch : input_string) {
        hash = 31 * hash + static_cast<unsigned int>(ch);
    }

    // Convert the hash value to a string
    return to_string(hash);
}

void bubbleSort(string array[], int size) {
    for (int step = 0; step < size - 1; ++step) {
        // Add a flag to check if any swaps were made during a pass
        bool swapped = false;

        for (int i = 0; i < size - step - 1; ++i) {
            // Convert strings to integers for numerical comparison
            BigInt num1 = array[i];
            BigInt num2 = array[i + 1];

            if (num1 > num2) {
                // Swap the elements
                string temp = array[i];
                array[i] = array[i + 1];
                array[i + 1] = temp;

                // Set the swapped flag to true
                swapped = true;
            }
        }

        // If no swaps were made, the array is already sorted
        if (!swapped) {
            break;
        }
    }
}

void print_2_pow_n(int n, std::string& TotalCapacity) {
    int i, j, blen = n / 32 + 1, dlen = n / 29 + 1;
    unsigned int* bin = new unsigned int[blen]();
    unsigned int* dec = new unsigned int[dlen]();
    uint64_t num;

    bin[n / 32] = 1U << (n % 32);

    for (j = 0; blen > 0; ) {
        for (num = 0, i = blen; i-- > 0;) {
            num = (num << 32) | bin[i];
            bin[i] = num / 1000000000U;
            num = num % 1000000000U;
        }
        dec[j++] = static_cast<unsigned int>(num);
        while (blen > 0 && bin[blen - 1] == 0)
            blen--;
    }

    TotalCapacity = std::to_string(dec[--j]);
    while (j-- > 0)
        TotalCapacity += std::to_string(dec[j]);

    delete[] bin;
    delete[] dec;
}

bool copyFile(const std::string& sourcePathStr, const std::string& destinationDirStr) {
    // Convert std::string to LPCWSTR
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &sourcePathStr[0], (int)sourcePathStr.size(), NULL, 0);
    std::wstring sourcePathWstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &sourcePathStr[0], (int)sourcePathStr.size(), &sourcePathWstr[0], size_needed);
    LPCWSTR sourcePath = sourcePathWstr.c_str();

    size_needed = MultiByteToWideChar(CP_UTF8, 0, &destinationDirStr[0], (int)destinationDirStr.size(), NULL, 0);
    std::wstring destinationDirWstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &destinationDirStr[0], (int)destinationDirStr.size(), &destinationDirWstr[0], size_needed);
    LPCWSTR destinationDir = destinationDirWstr.c_str();

    // Append the source file name to the destination directory path
    std::string fileName = sourcePathStr.substr(sourcePathStr.find_last_of("\\/") + 1);
    std::string destinationPathStr = destinationDirStr;
    destinationPathStr+="\\";
    destinationPathStr+=fileName;

        size_needed = MultiByteToWideChar(CP_UTF8, 0, &destinationPathStr[0], (int)destinationPathStr.size(), NULL, 0);
    std::wstring destinationPathWstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &destinationPathStr[0], (int)destinationPathStr.size(), &destinationPathWstr[0], size_needed);
    LPCWSTR destinationPath = destinationPathWstr.c_str();

    if (CopyFile(sourcePath, destinationPath, FALSE)) {
        std::cout << "File copied successfully." << std::endl;
        return true;
    }
    else {
        std::cout << "Failed to copy file. Error: " << GetLastError() << std::endl;
        return false;
    }
}

void createfolder(const std::string& path) 
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &path[0], (int)path.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &path[0], (int)path.size(), &wstrTo[0], size_needed);

    if (CreateDirectory(wstrTo.c_str(), NULL)) {
        std::cout << "Folder created successfully." << std::endl;
    }
    else {
        std::cout << "Failed to create folder." << std::endl;
    }
}

void Admin_Functionalitise()
{
    cout << "\n   1).\tAdd a new Computer to Exixting System\n";
    cout << "   2).\tRemove a new Computer from Exixting System\n";
    cout << "   3).\tUpload content to Exixting System\n";
    cout << "   4).\tDisplay Routing Cycle \n";
    cout << "   5).\tSearch Content \n";
    cout << "   6).\tDisplay B-Tree\n";
    cout << "   7).\tRemove a element from B-Tree\n";
}

void User_Functionalitise()
{
    cout << "\n   1).\tUpload Content\n";
    cout << "   2).\tSearch Content\n";
}

void Guest_Functionalities()
{
    cout << "\n   1).\tSearch Content\n";
}

//--------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------(Main)-------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------

int main()
{
    int BTreeSize = 3;
    BigInt Two(2);
    int SizeIdentifier =5;     //  Default Size          
    int ActiveMachine = 4;      
    char Automatic = 'a';

    bool ActivateSystem=true;

    int option = 1;
    int GuestTries = 3;
    bool check = 1;

    bool AdminConrol = false;
    bool UserConrol = false;
    bool GuestConrol = false;

    Circular_Network Network;           //Creating utorrent


//-------------------------------------------------------------(Login)-------------------------------------------------------------
    bool GotoMainScreen = false;
    char GotoMainScreenChar = 'n';
    do
    {
        do
        {
            cout << "Do you want to login as \n";      
            cout << "   1).\tAdmin\n";
            cout << "   2).\tUser\n";
            cout << "   3).\tGuest\n";
            cin >> option;
        } while (!(option == 1 || option == 2 || option == 3));
    
        if (option == 1)
        {
            do
            {   } while (!AdminLoginPermission());
        
            cout << "\nEnter Size of Identifier Space : ";
            cin >> SizeIdentifier;
            cout << "\nTotal Capacity : 0 -> " << Two.power(SizeIdentifier) - 1;

            cout << "\n\nNumber of Machines you want to activate : ";
            cin >> ActiveMachine;

            cout << "\nWhat's the Degree you want to keep for the B-Tree : ";
            cin >> BTreeSize;
            cout << endl;
            AdminConrol = true;
        }
        else if (option == 2)
        {
            do
            {
            } while (!UserLogin());
            cout << "\nSize of Identifier Space : " << SizeIdentifier << endl;
            cout << "Total Capacity : 0->" << Two.power(SizeIdentifier) - 1;
            cout << "\nDegree of B-Tree Used : " << BTreeSize << endl;
            UserConrol = true;
        }
        else if (option == 3)
        {
            cout << "\nSize of Identifier Space : " << SizeIdentifier << endl;
            cout << "Total Capacity : 0->" << Two.power(SizeIdentifier) - 1;
            cout << "\nDegree of B-Tree Used : " << BTreeSize << endl;
            GuestConrol = true;
        }
    
    //---------------------------------------------------------(Creating Router)-------------------------------------------------------

        BigInt Capacity = Two.power(SizeIdentifier);
        BigInt Total_Capacity(Capacity);


        srand(time(0));

        string* MachineIdKeeper = new string[ActiveMachine];
        int Count_M = 0;
    
        while ((Count_M <ActiveMachine) && AdminConrol && ActivateSystem)
        {
            cout << "\nDo you want to add Machine " << Count_M + 1 << " Automatically or Manually? (A/M) : ";
            cin >> Automatic;

            if (Automatic == 'A' || Automatic == 'a')
            {
                bool CheckerforDuplicate = true;
                while (CheckerforDuplicate)
                {

                    BigInt NewRandom;
                    BigInt SimpleRandom(rand());
                    BigInt TempRand(SimpleRandom);
                    //cout << "\nRandom : " << SimpleRandom << endl;
                    int randcheck = rand() % 10;
                    int r = 0;
                    while (SimpleRandom < Capacity && r < randcheck)
                    {
                        SimpleRandom.value += TempRand.value;
                        r++;
                    }
                    //cout << "\nNew Random : " << SimpleRandom << endl;

                    CheckerforDuplicate = false;
                    BigInt Random_Big_Value(rand());
                    BigInt Randomvalue = Randomvalue.mod(SimpleRandom, Capacity);
                    //cout << "\nFinal Random : " << Randomvalue << endl;
                    if (Count_M == 0)
                    {
                        MachineIdKeeper[Count_M] = Randomvalue;
                        CheckerforDuplicate = false;
                        break;
                    }
                    for (int n = 0; n < Count_M; n++)
                        if (Randomvalue == MachineIdKeeper[n])
                        {
                            CheckerforDuplicate = true;
                            break;
                        }
                    if (CheckerforDuplicate)
                        continue;
                    else
                    {
                        MachineIdKeeper[Count_M] = Randomvalue.value;
                        CheckerforDuplicate = false;
                    }
                }
                cout << "System has Activated computer : " << MachineIdKeeper[Count_M] << endl;

            }
            else
            {
                string temp;
                cout << "Enter the computer which you want to activate : ";
                cin >> temp;
                MachineIdKeeper[Count_M] = temp;
            }

            string path = "Network/";
            string Computername = "System ";
            Computername += MachineIdKeeper[Count_M];
            path += Computername;
            createfolder(path);
            Count_M++;
        }

        while (Count_M < ActiveMachine && (UserConrol || GuestConrol) && ActivateSystem )
        {
            bool CheckerforDuplicate = true;
            while (CheckerforDuplicate)
            {
                BigInt NewRandom;
                BigInt SimpleRandom(rand());
                BigInt TempRand(SimpleRandom);
                //cout << "\nRandom : " << SimpleRandom << endl;
                int randcheck = rand() % 10;
                int r = 0;
                while (SimpleRandom < Capacity && r < randcheck)
                {
                    SimpleRandom.value += TempRand.value;
                    r++;
                }
                //cout << "\nNew Random : " << SimpleRandom << endl;

                CheckerforDuplicate = false;
                BigInt Random_Big_Value(rand());
                BigInt Randomvalue = Randomvalue.mod(SimpleRandom, Capacity);
                //cout << "\nFinal Random : " << Randomvalue << endl;

                if (Count_M == 0)
                {
                    MachineIdKeeper[Count_M] = Randomvalue;
                    CheckerforDuplicate = false;
                    break;
                }
                for (int n = 0; n < Count_M; n++)
                    if (Randomvalue == MachineIdKeeper[n])
                    {
                        CheckerforDuplicate = true;
                        break;
                    }
                if (CheckerforDuplicate)
                    continue;
                else
                {
                    MachineIdKeeper[Count_M] = Randomvalue;
                    CheckerforDuplicate = false;
                }
            }
            cout << "System has Activated computer : " << MachineIdKeeper[Count_M] << endl;

            string path = "Network/";
            string Computername = "System ";
            Computername += MachineIdKeeper[Count_M];
            path += Computername;
            createfolder(path);
            Count_M++;
        }
    
        bubbleSort(MachineIdKeeper, Count_M);

        for (int n = 0; n < Count_M && ActivateSystem; n++)
            Network.ActiateMachine(MachineIdKeeper[n],BTreeSize);

        cout << "\nCurrent Computer Cycle : ";
        Network.Display(ActiveMachine);

        for (int n = 0; n < Count_M && ActivateSystem; n++)
            Network.RoutingSystem(SizeIdentifier, ActiveMachine, MachineIdKeeper[n]);
        cout << "\n";

    //-------------------------------------------------------------(Functions)-------------------------------------------------------------
        bool FunctionEscape = false;
        do
        {
            
            if (AdminConrol)
            {
                Admin_Functionalitise();
                cin >> option;
           
            }
            else if (UserConrol)
            {
                User_Functionalitise();
                cin >> option;
            }
            else if (GuestConrol)
            {
                Guest_Functionalities();
                cin >> option;
            }
        

       

            //-------------------------------------------------------------(Add New Computer)-------------------------------------------------------------
            if (option == 1 && !GuestConrol && !UserConrol && AdminConrol)
            {
                Circular_Network TempNetwork = Network;
                cout << "Adding another system";
                ActiveMachine++;
                string* newTemp = new string[ActiveMachine];
                for (int n = 0; n < ActiveMachine - 1; n++)
                    newTemp[n] = MachineIdKeeper[n];
                cout << "\nCurrent Actie Machines : ";
                Network.Display(ActiveMachine - 1);
                string AdditionMachine;

                bool CheckerforDuplicate = true;
                while (CheckerforDuplicate)
                {
                    CheckerforDuplicate = false;
                    cout << "\nWhich Machine do you want to activate? : ";
                    cin >> AdditionMachine;
                
                    for (int n = 0; n < ActiveMachine-1; n++)
                        if (AdditionMachine == MachineIdKeeper[n])
                        {
                            CheckerforDuplicate = true;
                            cout << "\nMachine is already on";
                            break;
                        }
                    if (CheckerforDuplicate)
                        continue;
                    else
                    {
                        newTemp[ActiveMachine - 1] = AdditionMachine;
                        CheckerforDuplicate = false;
                    }
                }
                delete[] MachineIdKeeper;
                MachineIdKeeper = NULL;
                MachineIdKeeper = newTemp;
                cout << "System has Activated computer : " << AdditionMachine << endl;
                bubbleSort(MachineIdKeeper,ActiveMachine);
                Network.root = NULL;

                for (int n = 0; n < ActiveMachine; n++)
                    Network.ActiateMachine(MachineIdKeeper[n], BTreeSize);

                cout << "\nCurrent Computer Cycle : ";
                Network.Display(ActiveMachine);

                for (int n = 0; n < ActiveMachine; n++)
                    Network.RoutingSystem(SizeIdentifier, ActiveMachine, MachineIdKeeper[n]);
                cout << "\n";

                Network.Display(ActiveMachine);
                /*
                
                Circular_Network MaxComp;
                MaxComp.root = Network.root->prev;
                BigInt newHash=Network.root->ComputerID;
                for (int n = 0; n < ActiveMachine; n++)
                {

                }
                
                 Circular_Network temp = Network;

                int n = 0;
                while (n < ActiveMachine)
                {
                    if (NewHash > MaxComp.root->ComputerID)
                    {
                        temp.root = Network.root;
                        break;
                    }
                    else if (NewHash > temp.root->ComputerID)
                    {
                        temp.root = temp.root->next;
                        n--;
                    }
                    n++;
                }

                cout << "\nValue Will be inserted in Computer : " << temp.root->ComputerID << endl;

                FinalPosition += temp.root->ComputerID.value;
                FinalPosition += "/";
                copyFile(PathInputFolder, FinalPosition);
                FinalPosition += FileName;

                temp.root->Tree->insertion(NewHash, FinalPosition, FileName);
                temp.root->Tree->traverse();
                
                */


            }
            //-------------------------------------------------------------(Remove New Computer)-------------------------------------------------------------
            else if (option == 2 && !GuestConrol && !UserConrol && AdminConrol)
            {
                cout << "Removing a existing system";

                string* newTemp = new string[ActiveMachine-1];
            
                cout << "\nCurrent Actie Machines : ";
                Network.Display(ActiveMachine );
                string AdditionMachine;

                bool CheckerforDuplicate = true;
        
                cout << "\nWhich Machine do you want to remove? : ";
                cin >> AdditionMachine;
                int c1 = 0;
                for (int n = 0; n < ActiveMachine; n++)
                {
                    if (AdditionMachine == MachineIdKeeper[n])
                    {

                        continue;
                    }
                    else
                        newTemp[c1] = MachineIdKeeper[n];
                    c1++;
                }
                ActiveMachine--;
                delete[] MachineIdKeeper;
                MachineIdKeeper = NULL;
                MachineIdKeeper = newTemp;
                cout << "System has Removed computer : " << AdditionMachine << endl;
                bubbleSort(MachineIdKeeper, ActiveMachine);
                Network.root = NULL;

                for (int n = 0; n < ActiveMachine; n++)
                    Network.ActiateMachine(MachineIdKeeper[n], BTreeSize);

                cout << "\nCurrent Computer Cycle : ";
                Network.Display(ActiveMachine);

                for (int n = 0; n < ActiveMachine; n++)
                    Network.RoutingSystem(SizeIdentifier, ActiveMachine, MachineIdKeeper[n]);
                cout << "\n";

                Network.Display(ActiveMachine);
            }
            //-------------------------------------------------------------(File Control)-------------------------------------------------------------
            else if (option == 3 || UserConrol && !GuestConrol)
            {
                bool AnotherFile = false;

                do
                {
                string FileName;
                string PathInputFolder = "input/";
                string FileNameFromUser = "1.txt";
                cout << "\nDo you want to create a file or Find a existing file?\n1).\tCreate a new File.\n2).\tMove a Existing File.\n";
                cin >> option;

                if (option == 1)
                {
                    cout << "Enter New File Name : ";
                    cin >> FileName;
                    FileName += ".txt";
                    PathInputFolder += FileName;
                    fstream NewFile(PathInputFolder, ios::out);
                    cout << "Do you want to Enter any Data?";
                
                }
                else if (option == 2)
                {
                    FileName;
                    cout << "File Name with extension (.txt .pdf .mp3 etc) : ";
                    cin >> FileName;;
                    PathInputFolder += FileName;
                }
                cout << "Path of File (From) : " << PathInputFolder << endl;
                cout << "Current Machines Online : ";
                Network.Display(ActiveMachine);

                BigInt MachineInputFromUser;
                bool check2 = false;
                do
                {
                    cout << "\n\nIn which Machine do you want to upload the file : ";
                    cin >> MachineInputFromUser;
                    int counter2 = 0;
                    for (int n = 0; n < ActiveMachine; n++)
                    {
                        if (MachineInputFromUser == MachineIdKeeper[n])
                        {
                            cout << "Machine Found\n";
                            check2 = true;
                            break;
                        }
                    }
                    if (check2 == false)
                    {
                        cout << "Machine was not found (Try again)!!\n";
                    }
                } while (!check2);

                string FinalPosition = "Network/System ";

                string Hash = SimpleHashFunction(PathInputFolder);

                BigInt NewHash;
                BigInt SimpleHash(Hash);
                cout << "\nSimple Hash : " << Hash;

                int LengthofRequiredHash = Capacity.value.length();
                int LengthofSimpleHash = Hash.length() - 1;
                int HashInt = 0;
                for (int n = 0; n < LengthofSimpleHash; n++)
                {
                    char t = SimpleHash.value[n];
                    HashInt += atoi(&t);
                }
                NewHash = HashInt;
                BigInt TempHash(HashInt);

                cout << "\nNew Hash : " << NewHash;

                while (NewHash < Two.power(SizeIdentifier))
                {
                    NewHash.value += TempHash.value;
                }
                cout << "\nNew Hash : " << NewHash;
                cout << "\nCapacity : " << Two.power(SizeIdentifier);

                NewHash = NewHash.mod(NewHash, Capacity);
                cout << "\nNew Reduced Hash : " << NewHash;


                Circular_Network temp = Network;
                Circular_Network MaxComp;
                MaxComp.root = temp.root->prev;

                int n = 0;
                while (n < ActiveMachine)
                {
                    if (NewHash > MaxComp.root->ComputerID)
                    {
                        temp.root = Network.root;
                        break;
                    }
                    else if (NewHash > temp.root->ComputerID)
                    {
                        temp.root = temp.root->next;
                        n--;
                    }
                    n++;
                }

                cout << "\nValue Will be inserted in Computer : " << temp.root->ComputerID << endl;

                FinalPosition += temp.root->ComputerID.value;
                FinalPosition += "/";
                copyFile(PathInputFolder, FinalPosition);
                FinalPosition += FileName;

                temp.root->Tree->insertion(NewHash, FinalPosition, FileName);
                //temp.root->Tree->traverse();


                char checker3 = 'n';
                cout << "\nDo you want to Add Another File ? (Y/N) ";
                cin >> checker3;
                if (checker3 == 'Y' || checker3 == 'y')
                    AnotherFile = true;
                else
                    AnotherFile = false;
                option = 3;

                } while (AnotherFile);

            }
            //-------------------------------------------------------------(Routing Cycle)------------------------------------------------------------
            else if (option == 4 && AdminConrol)
            {
                string abcd;
                cout << "\of Which Computer do you want to see the Routing cycle : ";
                cin >> abcd;
                Network.DisplayRoutingCycle(ActiveMachine, abcd);
            }
            //-------------------------------------------------------------(Search)--------------------------------------------------------------------
            else if ((option == 5 && AdminConrol) || (option == 2 && UserConrol) || (option == 1 && GuestConrol))
            {
                Circular_Network Search = Network;
                
                string HashInput;
                cout << "Enter Hash : ";
                cin >> HashInput;
                bool found = Network.root->Tree->search(HashInput);
                if (!found)
                    cout << "No file has HASH : " << HashInput << endl;
               
            }
            //-------------------------------------------------------------(B-Tree Display)------------------------------------------------------------
            else if (option == 6 && AdminConrol)
            {
                string ab;
                Circular_Network temp = Network;
                do
                {
                    cout << "From which Computer do you want to see the B-Tree : ";
                    cin >> ab;
                    temp.root = temp.root->next;
                } while (!(temp.root->ComputerID == ab));

                temp.root->Tree->traverse();
            }
            //--------------------------------------------------------------(Deletion of element)------------------------------------------------------
            else if (option == 7 && AdminConrol)
            {
                string ab;
                Circular_Network temp = Network;
                do
                {
                    cout << "From which Computer do you want to remove an element : ";
                    cin >> ab;
                    temp.root = temp.root->next;
                } while (!(temp.root->ComputerID == ab));

                cout << "Current B-Tree : \n";
                temp.root->Tree->traverse();

                string name;
                string path = "Network/System ";
                path += temp.root->ComputerID;
                path += "/";

                BigInt hash1;
                cout << "Enter Hash : ";
                cin >> hash1;
                cout << "Enter File name :";
                cin >> name;
                path += name;
                //cout << "Enter Path : ";
                
                temp.root->Tree->deletion(hash1, path, name);

            }


            char dawd = 'n';
            FunctionEscape = false;
            cout << "\nDo you want to perform any other Operation? (Y/N) : ";
            cin >> dawd;
            if (dawd == 'y' || dawd == 'Y')
                FunctionEscape = true;

        } while (FunctionEscape);

        GotoMainScreen = false;
        cout << "\nDo you want to Logout?\n";
        cout << "\t  1). Press 'Y' to logout\n";
        cout << "\t  2). Press 'H' to go to HomePage\n";
        cin >> GotoMainScreenChar;
        if (GotoMainScreenChar == 'H' || GotoMainScreenChar == 'h')
        {
            ActivateSystem = false;
            GotoMainScreen = true;
            AdminConrol = false;
            UserConrol = false;
            GuestConrol = false;
        }




        delete[] MachineIdKeeper;

    } while (GotoMainScreen);

    //-----------------------------------------------------------------------

   

    //--------------------------------------------------

    cout << "\n\nEnd";

}

