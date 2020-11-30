#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

char* stringDuplicator(char* string_to_duplicate, int times){
    assert(string_to_duplicate);
    assert(times > 0);
    int len = strlen(string_to_duplicate);
    
    char* out = malloc(len*times+1);
    assert(out);
    if (out == NULL) {
        return NULL;
    }

    for (int i=0; i<times; i++){
        strcpy(out + i*len,string_to_duplicate);
    }
    
    return out;
}

int main() {
    printf("%s",stringDuplicator("",3));
    printf("\n");
    return 0;
}