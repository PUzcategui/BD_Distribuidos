using namespace std;
#include <iostream>
#include "postgresql/libpq-fe.h"

PGconn *cnn = NULL;
PGresult *result = NULL;

char *host = "localhost";
char *port = "5432";
char *dataBase = "prueba";
char *user = "postgres";
char *passwd = "12345";

int main(int argc, char * argv[])
{
    int i;

    cnn = PQsetdbLogin(host,port,NULL,NULL,dataBase,user,passwd);

    if (PQstatus(cnn) != CONNECTION_BAD) {
        cout << "Estamos conectados a PostgreSQL!" << endl;
        result = PQexec(cnn, "SELECT * FROM users");

        if (result != NULL) {
            int tuplas = PQntuples(result);
            int campos = PQnfields(result);
            cout << "No. Filas:" << tuplas << endl;
            cout << "No. Campos:" << campos << endl;

            cout << "Los nombres de los campos son:" << endl;

            for (i=0; i<campos; i++) {
                cout << PQfname(result,i) << " | ";
            }

            cout << endl << "Contenido de la tabla" << endl;

            for (i=0; i<tuplas; i++) {
                for (int j=0; j<campos; j++) {
                    cout << PQgetvalue(result,i,j) << " | ";
                }
                cout << endl;
            }
        }

        // Ahora nos toca liberar la memoria
        PQclear(result);

    } else {
        cout << "Error de conexion" << endl;
        return 0;
    }

    PQfinish(cnn);

    return 0;
}

/*#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "postgresql/libpq-fe.h"

int main(int argc, char *argv[]){
    const char *conninfo;
    PGconn *conn;

    conninfo = "dbname = postgres";

    conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK)
    {
        printf("La conexion con la base de datos ha fallado: %s", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    std::cout << "HostPort : " <<PQport(conn) << std::endl;
    std::cout << "ServPath : " <<PQhost(conn) << std::endl;
    std::cout << "Database : " <<PQdb(conn) << std::endl;
    std::cout << "Login ID : " <<PQuser(conn) << std::endl;
    std::cout << "Proc PID : " <<PQbackendPID(conn) << std::endl;

    int ver = PQserverVersion((const PGconn *)conn);
    int verMajor = int(ver/10000);
    int verMinor = (ver%10000);
    std::cout << "Version : " << verMajor << "." << verMinor << std::endl;
    
    PQfinish(conn);
    return 0;
}*/