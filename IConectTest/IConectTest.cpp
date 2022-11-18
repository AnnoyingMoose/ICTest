// IConectTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <functional>
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
/// Copies the content of <paramref name="src"/> (up to and including the terminating null byte) into
/// <paramref name="dst"/> and returns the address of the null-terminator added to <paramref name="dst"/>.
/// Has undefined results if <paramref name="src"/> and <paramref name="dst"/> overlap.
/// </summary>
/// <param name="dst">The destination string.</param>
/// <param name="src">The source string.</param>
/// <returns>The address of the null-terminator ('\0') added to <paramref name="dst"/>.</returns>
/// <remarks>
/// This is a quick and inefficient pseudo-implementation of the function `stpcpy` defined in POSIX.1-2008.
/// </remarks>
inline char* stpcpy(char* dst, const char* src)
{
    return strcpy(dst, src) + strlen(src);
}

/// <summary>
/// Constructs a string consisting of all the values, comma-separated, returned by each call to
/// <paramref name="retriever"/> for all integers in the range [0, <paramref name="numFields"/>).<br/>
/// The string is allocated dynamically using the default allocator (<see langword="new"/>).
/// The caller is responsible for deallocating the returned string.<br/>
/// This function assumes the values returned by <paramref name="retriever"/>
/// have been allocated similarly, and will deallocate them accordingly.
/// </summary>
/// <param name="db">Object representing the relevant database connection.</param>
/// <returns>A comma-separated concatenation of each value returned by <paramref name="retriever"/>.</returns>
char* GetHeaderOrRecord(int numFields, function<char*(int)> retriever)
{
    // Variable for calculating and storing the length of the final string.
    // The `2` is the length of the substring ", ", which will separate each entry.
    // The `+1` is for the null-terminator.
    int stringLength = 2 * (numFields - 1) + 1;

    char** fieldNames = new char* [numFields];

    for (int i = 0; i < numFields; i++)
    {
        fieldNames[i] = retriever(i);
        stringLength += strlen(fieldNames[i]);
    }

    char* header = new char[stringLength];
    char* carat = stpcpy(header, fieldNames[0]);
    delete[] fieldNames[0];

    for (int i = 1; i < numFields; i++)
    {
        carat = stpcpy(carat, ", ");
        carat = stpcpy(carat, fieldNames[i]);
        delete[] fieldNames[i];
    }

    delete[] fieldNames;

    return header;
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
    return GetHeaderOrRecord(db->GetFieldCount(), [db](int nField) { return db->GetFieldName(nField); });
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
    char* record = GetHeaderOrRecord(db->GetFieldCount(), [db](int nField) { return db->GetFieldValue(nField); });
    db->MoveNext();
    return record;
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

    for (int i = 0; i <= numRecords; i++)
        cout << resultsBuffer[i] << endl;

    // Do something with the data here, maybe?

    // Clean up everything:

    for (int i = 0; i <= numRecords; i++)
        delete[] resultsBuffer[i];

    delete[] resultsBuffer;
    delete db;

    return 0;
}
