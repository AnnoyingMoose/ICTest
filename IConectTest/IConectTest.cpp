// IConectTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Oracle.h"

using namespace std;
using namespace ICTest;

/// <summary>
/// Procedure to call in case connecting to the Oracle database fails.
/// </summary>
/// <param name="message">The failure message.</param>
void ErrorProc(const char* message)
{
    cerr << "Failed to connect to the Oracle database. Message from COracle follows:" << endl
         << message << endl;

    exit(1);
}

/// <summary>
/// Constructs a string consisting of all the field names, comma-separated,
/// for the specified database connection's current query.<br/>
/// The string is allocated dynamically using the default allocator (<see langword="new"/>).
/// The caller is responsible for deallocating the returned string.
/// </summary>
/// <param name="db">Object representing the relevant database connection.</param>
/// <returns>A string representation of the query's field names, comma-separated.</returns>
char* GetHeader(COracle* db)
{
    // TODO
    return nullptr;
}

/// <summary>
/// Constructs a string consisting of all the field values, comma-separated, for the
/// specified database connection's current record, and advances to the next record.<br/>
/// The string is allocated dynamically using the default allocator (<see langword="new"/>).
/// The caller is responsible for deallocating the returned string.
/// </summary>
/// <param name="db">Object representing the relevant database connection.</param>
/// <returns>A string representation of the values in the record, comma-separated.</returns>
char* GetRecordAndAdvance(COracle* db)
{
    // TODO
    return nullptr;
}

int main()
{
#define IC_QUERY_TEMPLATE(s) "SELECT " s " FROM testing WHERE AUTHOR = 'iconect\\london' OR OBJECT_ID >= 5"
    const char* QUERY_COUNT = IC_QUERY_TEMPLATE("COUNT(*)");
    const char* QUERY_DATA = IC_QUERY_TEMPLATE("OBJECT_ID, AUTHOR, TITLE");
#undef IC_QUERY_TEMPLATE

    COracle* db = new COracle(ErrorProc);

    // Get the size of the result set first:
    db->Open(QUERY_COUNT);
    char* fieldBuffer = db->GetFieldValue(0);
    int numRecords = atoi(fieldBuffer);
    delete[] fieldBuffer;

    // Allocate the outer layer of the buffer:
    char** resultsBuffer = new char* [numRecords + 1];

    db->Open(QUERY_DATA);

    // The first element of the array is the header:
    resultsBuffer[0] = GetHeader(db);

    for (int i = 1; i <= numRecords; i++)
        resultsBuffer[i] = GetRecordAndAdvance(db);

    for (int i = 0; i < numRecords; i++)
        cout << resultsBuffer[i] << endl;

    // Do something with the data here, maybe?

    // Clean up everything:

    for (int i = 0; i < numRecords; i++)
        delete[] resultsBuffer[i];

    delete[] resultsBuffer;
    delete db;

    return 0;
}
