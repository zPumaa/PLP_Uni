#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h> /* for ENOSYS */
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int matrix_allocate(matrix_t *m, int rows, int columns) {
    /* implement the function here ... */
    if (rows < 0) {
        return 9;
    }
    if (columns < 0) {
        return 9;
    }
    (*m).rows = rows;
    (*m).columns = columns;
    (*m).content = malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++)
    {
        (*m).content[i] = malloc(columns * sizeof(int));
    }
    if ((*m).content == NULL)
    {
        return 9;
    }
    else {
        return 0;
    }
   
}

int matrix_init_identity(matrix_t *m){
    /* implement the function here ... */
    if (m->rows == m->columns){
        for (int i = 0; i < m->rows; i++)
        {
            for (int j = 0; j < m->columns; j++)
            {
                if (i == j)
                {
                    m->content[i][j] = 1;
                }
                else
                {
                    m->content[i][j] = 0;
                }
            }
        }
        return 0;    
    }
    else
    {
        return 1;;
    }
}

void matrix_init_zeros(matrix_t *m) {
    /* implement the function here ... */
    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->columns; j++)
        {
            m->content[i][j] = 0;
        }
    }
}

void matrix_free(matrix_t *m) {
    /* implement the function here ... */
    free(m->content);
}


void matrix_init_rand(matrix_t *m) {
    /* implement the function here ... */
    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->columns; j++)
        {
            m->content[i][j] = (rand() % 200) - 100;
        }
    }
}

int matrix_equal(matrix_t *m1, matrix_t *m2) {
    /* implement the function here ... */

    if (m1->rows == m2->rows && m1->columns == m2->columns)
    {
        for (int i = 0; i < m1->rows; i++)
        {
            for (int j = 0; j < m1->columns; j++)
            {
                if (m1->content[i][j] != m2->content[i][j])
                {
                    return 0;
                }
                else {
                    return 1;
                }
            }
        }
    }
    else {
        return 0;       
    }
}

int matrix_sum(matrix_t *m1, matrix_t *m2, matrix_t *result) {
    /* implement the function here ... */
    matrix_allocate(result, m1->rows, m1->columns);
    if (m1->rows == m2->rows && m1->columns == m2->columns)
    {
        for (int i = 0; i < m1->rows; i++)
        {
            for (int j = 0; j < m1->columns; j++)
            {
                result->content[i][j] = m1->content[i][j] + m2->content[i][j];
            }
        }
        return 0;
    }
    else {
        return 1;
    }   
}

int matrix_scalar_product(matrix_t *m, int scalar, matrix_t *result) {
    /* implement the function here ... */
    matrix_allocate(result, m->rows, m->columns);
    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->columns; j++)
        {
            result->content[i][j] = m->content[i][j] * scalar;
            return 0;
        }
    }
    return 1;

}

int matrix_transposition(matrix_t *m, matrix_t *result) {
    /* implement the function here ... */
    
    matrix_allocate(result, m->columns, m->rows);
    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->columns; j++)
        {
            result->content[j][i] = m->content[i][j];
        }
    }
    
    if (result->content[1][2] == m->content[2][1])
    {
        return 0;
    }
    else {
        return 1;
    }
}

int matrix_product(matrix_t *m1, matrix_t *m2, matrix_t *result) {
    /* implement the function here ... */
    matrix_allocate(result, m1->rows, m2->columns);
    if (m1->columns != m2->rows)
    {
        return 1;
    }    

    for (int i = 0; i < m1->rows; i++)
    {
        for (int j = 0; j < m2->columns; j++)
        {
            result->content[i][j] = 0;
            for (int k = 0; k < m1->columns; k++)
            {
                result->content[i][j] += m1->content[i][k] * m2->content[k][j];
            }
        }
    }
    return 0;
}    


int matrix_dump_file(matrix_t *m, char *output_file) {
    /* implement the function here ... */
    FILE *f = fopen(output_file, "w");
    if (f == NULL) {
        return 7;
    }
    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->columns; j++)
        {
            fprintf(f, "%d ", m->content[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
    return 0; 

}

int remover(char *aString, int *a){
    char space[] = " ,";char *token = strtok(aString, space);    

    int z = 0;
    while (token != NULL) {
        if (a != NULL) {
            a[z] = atoi(token);
        }
        z = z + 1;
        token = strtok(NULL, space);
        
    }

    return z;
}


int matrix_allocate_and_init_file(matrix_t *m, char *input_file) {
    int c = 0; char myBuffer[2048]; int r = 0; int z = 0;

    FILE *testfile;
    testfile = fopen(input_file, "r");
    if (testfile == NULL) {
        return 7;
    }
    
    while (feof(testfile) == 0){        
        char *str = fgets(myBuffer, sizeof(myBuffer)-1, testfile);
        int integers = remover(str, NULL);
        if (integers == 0) {
            continue;
        }
        if (c == 0) {
            c = integers;
        }    
        else if (integers != c) {
            return 7;
            fclose(testfile);            
        }
        r = r + 1;
    }
    
    int alloc = matrix_allocate(m, r, c);
    if (alloc != 0){
        fclose(testfile);
        return 7;
    }

    fseek(testfile, 0, SEEK_SET);
    
    
    while (feof(testfile) == 0){
        char *str = fgets(myBuffer, sizeof(myBuffer)-1, testfile);
        int individualNo = remover(str, m->content[z]);
        z = z + 1;
        if (individualNo == 0) {
            continue;
        }    
        
    }

    fclose(testfile);
    return 0;
}


