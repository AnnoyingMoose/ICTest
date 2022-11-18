#pragma once

namespace ICTest
{
	/// <summary>
	/// A procedure to call in response to an error. The parameter should be a message relating the nature of the error.
	/// </summary>
	typedef void(*ICONECT_ERRORPROC)(const char*);

	/// <summary>
	/// Represents a connection to a specific Oracle database.
	/// </summary>
	class COracle
	{
	public:

		/// <summary>
		/// Constructor. Opens a connection to the Oracle database (where the table ‘testing’ is located).
		/// </summary>
		/// <param name="errorProc">The procedure to run in case of an error during construction or connection. Optional.</param>
		COracle(ICONECT_ERRORPROC errorProc = nullptr);

		/// <summary>
		/// Destructor. Frees handles, destroys any loaded data from a query including the statement, resets all markers, and disconnects from Oracle.
		/// </summary>
		~COracle();

		/// <summary>
		/// Runs the parameter query <paramref name="szQuery"/> and retrieves the resulting recordset.
		/// </summary>
		/// <param name="szQuery">The query to run.</param>
		/// <returns><see langword="true"/> if the query was successful and returned at least one record; otherwise <see langword="false"/>.</returns>
		bool Open(const char* szQuery);

		/// <summary>
		/// Returns the value of the field specified by <paramref name="nField"/>.
		/// Note: This function allocates the memory for the value returned by this function.
		/// </summary>
		/// <param name="nField">The index of the field to retrieve. The first field has index 0.</param>
		/// <returns></returns>
		char* GetFieldValue(int nField);

		/// <summary>
		/// Returns the name of the field specified by <paramref name="nField"/>.
		/// </summary>
		/// <param name="nField">The index of the field to retrieve. The first field has index 0.</param>
		/// <returns></returns>
		char* GetFieldName(int nField);

		/// <summary>
		/// Returns the number of fields in the active query.
		/// </summary>
		/// <returns></returns>
		int GetFieldCount(void);

		/// <summary>
		/// Moves the current recordset forward one step.
		/// </summary>
		/// <returns>
		/// <see langword="false"/> if no query has been processed, or the current query
		/// is already at the end of the recordset; otherwise <see langword="true"/>.
		/// </returns>
		bool MoveNext(void);

		/// <summary>
		/// Returns whether the current recordset has reached EOF.
		/// </summary>
		/// <returns></returns>
		bool IsEOF(void);
	};
}
