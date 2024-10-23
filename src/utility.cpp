#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <occi.h>
#include <cstring> 
#include "utility.h"
using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;

int menu(void) {
    int userOption;
    bool flag = true;
    do {
        cout << "********************* HR Menu *********************" << endl;
        cout << "1) Find Employee\n"
            << "2) Employees Report\n"
            << "3) Add Employee\n"
            << "4) Update Employee\n"
            << "5) Remove Employee\n"
            << "0) Exit\n"
            << "Please enter your selection (0-5): ";
        cin >> userOption;
        if (cin.fail() || userOption < 0 || userOption > 5) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input, please enter a number between 0 and 5.\n" << endl;
            flag = true;
        }
        else {
            flag = false;
        }
    } while (flag);
    return userOption;
}

int findEmployee(Connection* conn, int employeeNumber, struct Employee* emp) {
    try {
        string query = "SELECT employeenumber, lastname, firstname, extension, email, officecode, reportsto, jobtitle "
            "FROM dbs211_employees WHERE employeenumber = :1";
        Statement* stmt = conn->createStatement();
        stmt->setSQL(query);
        stmt->setInt(1, employeeNumber);
        ResultSet* rs = stmt->executeQuery();
        if (rs->next()) {
            emp->employeeNumber = rs->getInt(1);
            strcpy(emp->lastName, rs->getString(2).c_str());
            strcpy(emp->firstName, rs->getString(3).c_str());
            strcpy(emp->extension, rs->getString(4).c_str());
            strcpy(emp->email, rs->getString(5).c_str());
            strcpy(emp->officeCode, rs->getString(6).c_str());
            emp->reportsTo = rs->getInt(7);
            strcpy(emp->jobTitle, rs->getString(8).c_str());
            conn->terminateStatement(stmt);
            return 1;
        }
        else {
            conn->terminateStatement(stmt);
            return 0;
        }
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
        return 0;
    }
}

void displayEmployee(Connection* conn, struct Employee emp) {
    cout << "\n-------------- Employee Information -------------" << endl;
    cout << "Employee Number: " << emp.employeeNumber << endl;
    cout << "Last Name: " << emp.lastName << endl;
    cout << "First Name: " << emp.firstName << endl;
    cout << "Extension: " << emp.extension << endl;
    cout << "Email: " << emp.email << endl;
    cout << "Office Code: " << emp.officeCode << endl;
    cout << "Manager ID: " << emp.reportsTo << endl;
    cout << "Job Title: " << emp.jobTitle << endl << endl;
}

void displayAllEmployees(Connection* conn) {
    try {
        string query = "SELECT a.employeenumber, "
            "a.firstname || ' ' || a.lastname AS employee_name, "
            "a.email, "
            "o.phone, "
            "a.extension, "
            "b.firstname || ' ' || b.lastname AS manager_name "
            "FROM dbs211_employees a "
            "LEFT JOIN dbs211_employees b ON a.reportsto = b.employeenumber "
            "JOIN dbs211_offices o ON a.officecode = o.officecode "
            "ORDER BY a.employeenumber";
        Statement* stmt = conn->createStatement();
        stmt->setSQL(query);
        ResultSet* rs = stmt->executeQuery();
        if (!rs->next()) {
            cout << "There is no employees' information to be displayed." << endl;
            conn->terminateStatement(stmt);
        }
        else {
            cout << "\n------   ---------------   ---------------------------------  ----------------  ---------  -----------------" << endl;
            cout.setf(ios::left);
            cout.width(9);
            cout << "ID";
            cout.width(18);
            cout << "Employee Name";
            cout.width(35);
            cout << "Email";
            cout.width(18);
            cout << "Phone";
            cout.width(11);
            cout << "Extension";
            cout << "Manager Name" << endl;
            cout << "------   ---------------   ---------------------------------  ----------------  ---------  -----------------" << endl;
            do {
                cout.setf(ios::left);
                cout.width(9);
                cout << rs->getInt(1);
                cout.width(18);
                cout << rs->getString(2);
                cout.width(35);
                cout << rs->getString(3);
                cout.width(18);
                cout << rs->getString(4);
                cout.width(11);
                cout << rs->getString(5);
                cout << rs->getString(6) << endl;
            } while (rs->next());
            cout << endl;
            conn->terminateStatement(stmt);
        }
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }
}

void getEmployee(struct Employee* emp) {
    cout << "\n-------------- New Employee Information -------------" << endl;
    cout << "Employee Number: ";
    cin >> emp->employeeNumber;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Last Name: ";
    cin.getline(emp->lastName, 50);

    cout << "First Name: ";
    cin.getline(emp->firstName, 50);

    cout << "Extension: ";
    cin.getline(emp->extension, 10);

    cout << "Email: ";
    cin.getline(emp->email, 100);

    cout << "Office Code: ";
    cin.getline(emp->officeCode, 10);

    cout << "Manager ID: ";
    cin >> emp->reportsTo;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Job Title: ";
    cin.getline(emp->jobTitle, 50);
}

void insertEmployee(Connection* conn, struct Employee emp) {
    try {
        if (findEmployee(conn, emp.employeeNumber, &emp)) {
            cout << "\nAn employee with the same employee number exists.\n" << endl;
        }
        else {
            string insert = "INSERT INTO dbs211_employees "
                "(employeenumber, lastname, firstname, extension, email, officecode, reportsto, jobtitle)"
                "VALUES "
                "(:1, :2, :3, :4, :5, :6, :7, :8)";
            Statement* stmt = conn->createStatement();
            stmt->setSQL(insert);
            stmt->setInt(1, emp.employeeNumber);
            stmt->setString(2, emp.lastName);
            stmt->setString(3, emp.firstName);
            stmt->setString(4, emp.extension);
            stmt->setString(5, emp.email);
            stmt->setString(6, emp.officeCode);
            stmt->setInt(7, emp.reportsTo);
            stmt->setString(8, emp.jobTitle);
            stmt->executeUpdate();
            cout << "\nThe new employee is added successfully.\n" << endl;
            conn->terminateStatement(stmt);
        }
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }
}

void updateEmployee(Connection* conn, int employeeNumber) {
    try {
        Employee emp;
        if (findEmployee(conn, employeeNumber, &emp)) {
            cout << "Last Name: " << emp.lastName << endl;
            cout << "First Name: " << emp.firstName << endl;
            cout << "Extension: ";
            cin >> emp.extension;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            string update = "UPDATE dbs211_employees SET "
                "extension = :1 "
                "WHERE employeenumber = :2 AND lastname = :3 AND firstname = :4";
            Statement* stmt = conn->createStatement();
            stmt->setSQL(update);
            stmt->setString(1, emp.extension);
            stmt->setInt(2, emp.employeeNumber);
            stmt->setString(3, emp.lastName);
            stmt->setString(4, emp.firstName);
            stmt->executeUpdate();
            cout << "The employee's extension is updated successfully.\n" << endl;
            conn->terminateStatement(stmt);
        }
        else {
            cout << "The employee with ID " << employeeNumber << " does not exist.\n" << endl;
        }
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }
}
void deleteEmployee(Connection* conn, int employeeNumber) {
    try {
        Employee emp;
        if (findEmployee(conn, employeeNumber, &emp)) {
            string sqlQuery = "DELETE FROM dbs211_employees WHERE employeenumber = :1";
            Statement* stmt = conn->createStatement(sqlQuery);
            stmt->setInt(1, employeeNumber);
            int rowsAffected = stmt->executeUpdate();
            conn->commit();
            cout << "The employee with ID " << employeeNumber << " is deleted successfully.\n" << endl;
            conn->terminateStatement(stmt);
        }
        else {
            cout << "The employee with ID " << employeeNumber << " does not exist.\n" << endl;
        }
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }
}