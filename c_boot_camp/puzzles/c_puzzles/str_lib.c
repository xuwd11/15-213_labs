#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* For all these problems, you may not use any 
 * string library functions; you may, however, 
 * define any helper functions that you may need */ 

int strlen_m(char* s) {
    int i = 0;
    for (; s[i] != '\0'; i++);
    return i;
}

/* Concatenates the src string to the end of the dest
 * string, returns the dest string
 *
 * You can assume that src/dest are '\0' terminated, 
 * that dest hold enough space to store src+dest and 
 * src/dest do not overlap in memory */
char* strcat_m(char* dest, char* src) { 
    /* TODO: Implement this function */
    int dest_len = strlen_m(dest);
    int src_len = strlen_m(src);
    for (int i = 0; i < src_len; i++) {
        dest[dest_len + i] = src[i];
    }
    dest[dest_len + src_len] = '\0';
    return dest;
}

/* Reverses a string and returns the a new string 
 * containing the reversed string 
 *
 * You can assume that str is '\0' terminated */
char* strrev(char *str) {
    /* TODO: Implement this function */
    int str_len = strlen_m(str);
    char* rev = malloc(sizeof(char) * (str_len + 1));
    for (int i = 0; i < str_len; i++) {
        rev[i] = str[str_len - 1 - i];
    }
    rev[str_len] = '\0';
    return rev; 
}

int main()
{
    /* TODO: Implement test cases to check your implemenation
     * You MAY use string library functions to test your code */
    char* str1 = malloc(sizeof(char) * 10);
    char* str2 = malloc(sizeof(char) * 10);
    strcpy(str1, "abc");
    strcpy(str2, "defg");
    str1 = strcat_m(str1, str2);
    printf("%s\n", str1);
    char* str3 = strrev(str1);
    printf("%s\n", str3);
    free(str1); free(str2); free(str3);
    return 0;
}
