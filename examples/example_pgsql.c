#include <stdio.h>
#define _HOEL_PGSQL
#include "../src/hoel.h"

void print_result(struct _h_result result) {
  int col, row, i;
  char buf[64];
  printf("rows: %d, col: %d\n", result.nb_rows, result.nb_columns);
  for (row = 0; row<result.nb_rows; row++) {
    for (col=0; col<result.nb_columns; col++) {
      switch(result.data[row][col].type) {
        case HOEL_COL_TYPE_INT:
          printf("| %d ", ((struct _h_type_int *)result.data[row][col].t_data)->value);
          break;
        case HOEL_COL_TYPE_DOUBLE:
          printf("| %f ", ((struct _h_type_double *)result.data[row][col].t_data)->value);
          break;
        case HOEL_COL_TYPE_TEXT:
          printf("| %s ", ((struct _h_type_text *)result.data[row][col].t_data)->value);
          break;
        case HOEL_COL_TYPE_BLOB:
          for (i=0; i<((struct _h_type_blob *)result.data[row][col].t_data)->length; i++) {
            printf("%c", *((char*)(((struct _h_type_blob *)result.data[row][col].t_data)->value+i)));
            if (i%80 == 0 && i>0) {
              printf("\n");
            }
          }
          break;
        case HOEL_COL_TYPE_DATE:
          strftime(buf, 64, "%Y-%m-%d %H:%M:%S", &((struct _h_type_datetime *)result.data[row][col].t_data)->value);
          printf("| %s ", buf);
        case HOEL_COL_TYPE_NULL:
          printf("| [null] ");
          break;
      }
    }
    printf("|\n");
  }
}

int main(int argc, char ** argv) {
  struct _h_result result;
  struct _h_connection * conn;
  char * query = "select * from test", * connectionstring = "host=localhost dbname=test user=test password=test";
  int res;
  
  conn = h_connect_pgsql(connectionstring);
  
  res = h_execute_query(conn, query, &result);
  
  if (res == H_OK) {
    print_result(result);
    h_clean_result(&result);
  } else {
    printf("Error executing query: %d\n", res);
  }
  h_close_db(conn);
  return h_clean_connection(conn);
}
