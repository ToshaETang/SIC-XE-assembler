#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* getSection(char* text, int i, char *section, int j){
    if(i+1 >= strlen(text)){
        section = "";
    }
    else if(text[i] == '\t'){
        section = "";
    }else{
        while(text[i] != '\t' && text[i] != '\n'){
            section[j] = text[i];
            i++;
            j++;
        }
        section[j] = '\0';
    }
    return section;
}


//==============================================================


int main()
{   
    printf("START!!!!!!!\n");

    //read input file
    FILE* inputFile;
    inputFile = fopen ("inputFile.txt", "r"); 
    char* text = malloc(100*sizeof(char));
    
    while(fgets(text,100,inputFile) != NULL){
        //printf("text[1]=%c  len=%d\n", text[1], strlen(text));
        //printf("%s\n",text);
        char* text_1 = malloc(20*sizeof(char)); //eg. FIRST
        char* text_2 = malloc(20*sizeof(char)); //eg. STL
        char* text_3 = malloc(20*sizeof(char)); //eg. RETADR

        //seperate the line
        text_1 = getSection(text, 0, text_1, 0);
        //printf("text_1 = %s  |  ",text_1, strlen(text_1));
        text_2 = getSection(text, strlen(text_1)+1, text_2, 0);
        //printf("text_2 = %s  |  ",text_2, strlen(text_2));
        text_3 = getSection(text, strlen(text_1)+strlen(text_2)+2, text_3, 0);
        //printf("text_3 = %s\n",text_3);





    }//while(fgets(text,100,inputFile) != NULL)



    //open result file
    FILE* resultFile;
    resultFile = fopen ("resultFile.txt", "w"); 


    //----------------------------
    fclose(inputFile);
    fclose(resultFile);
    return(0);
}