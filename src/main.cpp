/*/////////////////////////////////////////////////////////////////////////
                           Employee-Management-System
Author     : Guan-Wei Huang
Email      : gwhuang24@gmail.com

Copyright 2024 Guan-Wei Huang
All rights reserved.

This project, including all source code and associated documentation files,
is the intellectual property of Lynn Lin. Unauthorized copying, distribution,
modification, or use of any part of this software, in whole or in part, without
explicit written permission from the copyright owner is strictly prohibited.

This software is provided "as is" without warranty of any kind, express or implied,
including but not limited to the warranties of merchantability, fitness for a
particular purpose, and noninfringement. In no event shall the copyright holder be
liable for any claim, damages, or other liability, whether in an action of contract,
tort, or otherwise, arising from, out of, or in connection with the software or the
use or other dealings in the software.
/////////////////////////////////////////////////////////////////////////*/

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <occi.h>
#include <cstring> 
#include "utility.h"
using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;

int main() {
    Environment* env = nullptr;
    Connection* conn = nullptr;
    try {
        // Modify below connection information
        string user = "Oracle UserName";
        string pass = "Oracle Password";
        string constr = "HostName:Port/ServiceName";
        //
        env = Environment::createEnvironment(Environment::DEFAULT);
        conn = env->createConnection(user, pass, constr);
        int userOption;
        do {
            userOption = menu(); 
            switch (userOption) {
            case 1: {
                int empNumber;
                cout << "Enter Employee Number: ";
                cin >> empNumber;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                Employee emp;
                if (findEmployee(conn, empNumber, &emp)) {
                    displayEmployee(conn, emp);
                }
                else {
                    cout << "Employee " << empNumber << " does not exist.\n" << endl;
                }
                break;
            }
            case 2:
                displayAllEmployees(conn);
                break;
            case 3:
                Employee emp;
                getEmployee(&emp);
                insertEmployee(conn, emp);
                break;
            case 4:
                int empNumber;
                cout << "Enter Employee Number: ";
                cin >> empNumber;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                updateEmployee(conn, empNumber);
                break;
            case 5:
                cout << "Enter Employee Number: ";
                cin >> empNumber;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                deleteEmployee(conn, empNumber);
                break;
            case 0:
                cout << "Exiting the program." << endl;
                break;
            default:
                cout << "Invalid option. Please try again." << endl;
                break;
            }
        } while (userOption != 0);
        env->terminateConnection(conn);
        Environment::terminateEnvironment(env);
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }
    return 0;
}
