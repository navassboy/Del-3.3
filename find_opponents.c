//Include this library to be able to make calls in shiva2.upc.es
//#include <my_global.h> 
#include <mysql.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	MYSQL *conn;
	int err;
	// Special structure for storing query results
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	// We create a connection to the MYSQL server
	conn = mysql_init(NULL);
	if (conn == NULL) {
		printf("Error creating connection: %u %s\n",
			   mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	// Initialize the connection
	conn = mysql_real_connect(conn, "localhost", "root", "mysql", "League", 0, NULL, 0);
	if (conn == NULL) {
		printf("Error initializing connection: %u %s\n",
			   mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	
	char playerName[40];
	// 	I ask for the name of the player
	printf("Enter the name of the player: \n");
	scanf("%s", playerName);
	
	char query[512];
	strcpy(query, "SELECT DISTINCT p2.Username FROM Player p2, Game_Record gr1, Game_Record gr2 WHERE p2.Identifier_P = gr2.Player ");
	strcat(query, "AND gr2.Game = gr1.Game AND gr1.Player = (");
	strcat(query, "SELECT Identifier_P FROM Player WHERE Username = '");
	strcat(query, playerName);
	strcat(query, "') AND p2.Identifier_P != (");
	strcat(query, "SELECT Identifier_P FROM Player WHERE Username = '");
	strcat(query, playerName);
	strcat(query, "')");
	
	
	err = mysql_query(conn, query);
	if (err != 0) {
		printf("Error while querying data from database: %u %s\n",
			   mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	// We collect the result of the query
	result = mysql_store_result(conn); // Store the result after executing the query
	if (result == NULL) {
		printf("No data was obtained in the query\n");
	} else {
		printf("Opponents of %s:\n", playerName);
		while ((row = mysql_fetch_row(result)) != NULL) {
			// Column 0 contains the opponent's name
			printf("%s\n", row[0]);
		}
		mysql_free_result(result); // Free result after processing
	}
		
		mysql_close(conn); 
		exit(0); 
}
	
