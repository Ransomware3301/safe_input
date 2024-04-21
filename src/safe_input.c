/*
 *  Program:    "safe_input()" function that enables a user to acquire input of all basic C types in a
 *              safe way by using a fixed-size buffer (which can still be modified to fit specific needs)
 * 
 *  Made by:    Ransomware3301 (https://www.github.com/ransomware3301)
 *  Date:       18-03-2024             
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define STRING_BUFFER_SIZE 256
#define STRING_EMPTY_PLACEHOLDER " "
#define END_OF_STRING '\0'
#define NEWLINE_CHAR '\n'


/* ==== Type Definitions ==== */


/* 
 *  ALL types that can be read from user input 
 *  with the "safe_input()" function 
 */
typedef enum type 
{
    SHORT_INT,
    U_SHORT_INT,
    INT,
    U_INT,
    LONG_INT,
    U_LONG_INT,
    FLOAT,
    DOUBLE,
    CHAR,
    STRING
}
type_t;


/* Boolean Type Definition */
typedef enum bool
{
    false,
    true
}
bool_t;


/* ==== Function Declarations ==== */


/* I/O */
void * safe_input(const type_t, const unsigned long int, char*);


/* Miscellaneous */
char * filter(char*, char);


/* ==== Function Definitions ==== */


/*
 *  Safe input method that can get an user input in different 
 *  data types (see the "enum type" for the complete list) by
 *  first acquiring user input in a buffer and then formatting it
 *  as specified by the function parameters. Then the function
 *  returns a void pointer that needs to be formatted afterwards
 *  into the correct type
 */
void * safe_input(const type_t type_selector, const unsigned long int bufsize, char *message)
{
    void *result;
    char *buf, *format, *filtered_buf;
    bool_t acquired_input;


    if (bufsize > 0)
    {
        switch (type_selector)
        {
            case SHORT_INT:     format = "%hd";
                                result = (short*)malloc(sizeof(short int));
                                break;
            
            case U_SHORT_INT:   format = "%hu";
                                result = (unsigned short int*)malloc(sizeof(unsigned short int));
                                break;

            case INT:           format = "%d";
                                result = (int*)malloc(sizeof(int));
                                break;
            
            case U_INT:         format = "%u";
                                result = (unsigned int*)malloc(sizeof(unsigned int));
                                break;

            case LONG_INT:      format = "%ld";
                                result = (long int*)malloc(sizeof(long int));
                                break;
            
            case U_LONG_INT:    format = "%lu";
                                result = (unsigned long int*)malloc(sizeof(unsigned long int));
                                break;

            case FLOAT:         format = "%f";
                                result = (float*)malloc(sizeof(float));
                                break;

            case DOUBLE:        format = "%lf";
                                result = (double*)malloc(sizeof(double));
                                break;

            case CHAR:          format = "%c";
                                result = (char*)malloc(sizeof(char));
                                break;

            case STRING:        format = "%s";
                                result = (char*)malloc(sizeof(char) * (bufsize + 1));
                                break;

            default:            format = STRING_EMPTY_PLACEHOLDER;
                                result = NULL;
        }

        if (result)
        {
            if (( buf = (char*)malloc(sizeof(char) * (bufsize + 1)) ))
            {
                acquired_input = false;
                fflush(stdin);

                do
                {
                    if (message)
                    {
                        printf("%s", message);
                    }

                    /* 
                     *  NOTE: 
                     *  -   fgets() puts an END_OF_STRING ('\0') character at the 
                     *      end of the buffer if no errors occur during input
                     */
                    if (fgets(buf, bufsize + 1, stdin))
                    {
                        if (type_selector != STRING)
                        {
                            if (1 == sscanf(buf, format, result))
                            {
                                acquired_input = true;
                            }
                        }
                        else
                        {
                            filtered_buf = filter(buf, NEWLINE_CHAR);
                            memcpy(result, filtered_buf, strlen(filtered_buf));

                            acquired_input = true;
                        }
                    }
                }
                while ( !acquired_input );

                free(buf);
            }
            else
            {
                printf("[safe_input()] ERROR: Memory allocation was unsuccessful\n");
            }
        }
        else
        {
            if (strcmp(format, STRING_EMPTY_PLACEHOLDER) == 0)
            {
                printf("[safe_input()] ERROR: Memory allocation was unsuccessful\n");
            }
            else
            {
                printf("[safe_input()] ERROR: The selected type does not exist in the list of specified ones\n");
            }
        }
    }
    else
    {
        result = NULL;
        printf("[safe_input()] ERROR: The give buffer size can only be greater than zero\n");
    }

    return result;
}


/*
 *  Filters out all the given 'remove' characters found in str
 */
char * filter(char *str, char remove)
{
    char *filtered;
    int i, j, len;
   

    filtered = NULL;

    if (str)
    {
        len = strlen(str);
        i = 0;

        while ((str + i) && *(str + i) != END_OF_STRING)
        {   
            if ( *(str + i) == remove)
            {
                len--;
            }
            i++;
        }

        if (( filtered = (char*)malloc(sizeof(char) * (len + 1)) ))
        {
            i = 0;
            j = 0;

            while ((str + i) && *(str + i) != END_OF_STRING)
            {
                if (*(str + i) != remove)
                {
                    *(filtered + j) = *(str + i);
                    j++;
                }
                i++;
            }

            *(filtered + len) = END_OF_STRING;
        }
        else
        {
            printf("[filter()] ERROR: Memory allocation was unsuccessful\n");
        }
    }

    return filtered;
}
