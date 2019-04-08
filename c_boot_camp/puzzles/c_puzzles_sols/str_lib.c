#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* For all these problems, you may not use any 
 * string library functions; you may, however, 
 * define any helper functions that you may need */ 

int strlen_m(char* str) {
    int i = 0;  
    for (i = 0; str[i] != '\0'; i++);
    return i;  
}

/* Concatenates the src string to the end of the dest
 * string, returns the dest string
 *
 * You can assume that src/dest are '\0' terminated, 
 * that dest hold enough space to store src+dest and 
 * src/dest do not overlap in memory */
char* strcat_m(char* dest, char* src) { 
    // first, find the end of the dest string.  Then, 
    // append src character by character; remember to add a '\0'
    // character at the end!
    
    int dest_len = strlen_m(dest);
    int src_len = strlen_m(src); 

    // a very common mistake here is to include strlen_m(src)
    // as part of the loop condition.  This is bad since 
    // strlen_m(src) will be computed on every iteration of the loop, 
    // resulting in a quadratic runtime for what is otherwise a linear
    // time operation
    for (int i = 0; i < src_len; i++) {
        dest[i + dest_len] = src[i]; 
    } 
    dest[src_len+dest_len] = '\0'; 
    return dest; 
}

/* Reverses a string and returns the a new string 
 * containing the reversed string 
 *
 * You can assume that str is '\0' terminated */
char* strrev(char *str) {
    // we allocate space for a new string (remember to leave space 
    // for the '\0' character!) and then access
    // the characters in reverse.  Don't forget the '\0' 
    // character at the end!

    int str_len = strlen_m(str);
    char* rev_str = malloc(sizeof(char) * (str_len + 1)); 
    
    for (int i = str_len-1; i >= 0; i--) { 
        rev_str[str_len - i - 1] = str[i]; 
    }
    rev_str[str_len] = '\0'; 
    return rev_str; 
}

int main()
{
    char* str1 = malloc(sizeof(char) * 10); 
    strcpy(str1, "efgh");
    char* str2 = malloc(sizeof(char) * 10); 
    strcpy(str2, "abcd");
   
    strcat_m(str2, str1); 
    printf("%s\n", str2); // expect abcdefgh 
    char* rev_str1 = strrev(str1); 
    printf("%s\n", rev_str1); // expect hgfe
    
    free(str1); free(str2); free(rev_str1);  
    return 0;
}
