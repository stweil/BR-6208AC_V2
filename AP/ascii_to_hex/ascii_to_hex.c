#include <stdio.h>
#include <string.h>

int main(int argc, char **argv){
    char word[17], outword[33];//17:16+1, 33:16*2+1
    int i, len;

    //printf("Intro word:");
    //fgets(word, sizeof(word), stdin);
    printf("%s\n", argv[0]);
strcpy(word,argv[1]);
    len = strlen(word);
    if(word[len-1]=='\n')
        word[--len] = '\0';

    for(i = 0; i<len; i++){
        sprintf(outword+i*2, "%02X", word[i]);
    }
    printf("%s\n", outword);
    return 0;
}
