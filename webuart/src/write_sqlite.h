#include "main.h"

#define MAX_BUFF    128
#define DELAY_WRITE_SQLITE_S     1800

static int int_temperature = 0;
static int ext_temperature = 0;
static unsigned long timestamp = 0;
//-----------------------------------------------------------------------------
static int parseTemperature(char *buffer);
///////////////////////////////////////////////////////////////////////////////
static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for(i=0; i<argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}
//*****************************************************************************
void WriteSQLITE(char * in_str)
{
    time_t seconds = time(NULL);

    if(seconds < timestamp)
    {
        return;
    }
    timestamp = time(NULL) + DELAY_WRITE_SQLITE_S;

    sqlite3 *db;
    char *zErrMsg = 0;
    int  rc;
    char *sql;

    /* Open database */
    rc = sqlite3_open("/mnt/usb/.data/main.db", &db);
    if( rc )
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else
    {
        fprintf(stdout, "Opened database successfully\n");
    }

    /* Create SQL statement */
    sql = "CREATE TABLE IF NOT EXISTS temperature(\
    ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\
    id_sensor INT NOT NULL,\
    t_data INT NOT NULL,\
    time_date INT NOT NULL)";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        char *ptr_sql_insert;
        char sql_insert[100];
        ptr_sql_insert = sql_insert;
        if(parseTemperature(in_str) == 0)
        {
            printf("Error parsing");
            return;
        }
        /* Create SQL statement */
        snprintf(ptr_sql_insert, sizeof sql_insert, "%s%d,%ld );", "INSERT INTO temperature" \
                 "(id_sensor,t_data,time_date)VALUES (1,", int_temperature, seconds);
        //printf("sql_1 = %s\n", ptr_sql_insert);
        rc = sqlite3_exec(db, ptr_sql_insert, callback, 0, &zErrMsg);

        if( rc != SQLITE_OK )
        {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
        else
        {
            snprintf(ptr_sql_insert, sizeof sql_insert, "%s%d,%ld );", "INSERT INTO temperature" \
                     "(id_sensor,t_data,time_date)VALUES (2,", ext_temperature, seconds);
            //printf("sql_2 = %s\n", ptr_sql_insert);
            printf("Запись в базу\n");
            rc = sqlite3_exec(db, ptr_sql_insert, callback, 0, &zErrMsg);
            if( rc != SQLITE_OK )
            {
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
        }
    }
    sqlite3_close(db);
}
//*****************************************************************************
static int parseTemperature(char *buffer)
{
    char *ptr;
    char temp_buf[MAX_BUFF-1];
    memmove(temp_buf, buffer, MAX_BUFF-1);
    ptr = temp_buf;
    ptr = strstr(ptr, "ti=");
    if(!ptr)
    {
        return 0;
    }
    ptr += 3;
    ptr = strtok(ptr, "&");
    if(!ptr)
    {
        return 0;
    }
    int_temperature = atoi(ptr);

    ptr = buffer;
    ptr = strstr(ptr, "te=");
    if(!ptr)
    {
        return 0;
    }
    ptr += 3;
    ptr = strtok(ptr, "&");
    if(!ptr)
    {
        return 0;
    }
    ext_temperature = atoi(ptr);
    return 1;
}
//*****************************************************************************

