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


int isInOPTAB(char* OPCODE, FILE* OPTAB){
    char* text = malloc(100*sizeof(char));
    while(fgets(text,100,OPTAB) != NULL){
        char* tmp = malloc(20*sizeof(char));
        tmp = getSection(text, 0, tmp, 0);
        if(strcmp(OPCODE, tmp) == 0){
            return 1;
        }
    }
    return 0;
}


//==============================================================

int main()
{   
    printf("START!!!!!!!\n");

    int LOCCTER;
    int STARTADDRESS;


    //open file
    FILE* inputFile;
    inputFile = fopen ("inputFile.txt", "r"); // read
    FILE* OPTAB;
    OPTAB = fopen ("OPTAB.txt", "r"); // read
    FILE* SYMTAB;
    SYMTAB = fopen ("SYMTAB.txt", "w+"); //overwrite or create
    FILE* resultFile;
    resultFile = fopen ("resultFile.txt", "w+"); //overwrite or create
    FILE* interMediateFile;
    interMediateFile = fopen ("interMediateFile.txt", "w+"); //overwrite or create


    char* text = malloc(100*sizeof(char));
    

    while(fgets(text,100,inputFile) != NULL){

        //printf("%s\n",text);
        char* LABEL = malloc(20*sizeof(char)); //eg. FIRST
        char* OPCODE = malloc(20*sizeof(char)); //eg. STL
        char* OPERAND = malloc(20*sizeof(char)); //eg. RETADR

        //seperate the input line
        LABEL = getSection(text, 0, LABEL, 0);
        OPCODE = getSection(text, strlen(LABEL)+1, OPCODE, 0);
        OPERAND = getSection(text, strlen(LABEL)+strlen(OPCODE)+2, OPERAND, 0);
        //printf("LABEL = %s  |  ",LABEL);
        //printf("OPCODE =%s|  ",OPCODE);
        //printf("OPERAND = %s\n",OPERAND);

        //-------------

        // OPCODE is START
        if(strcmp(OPCODE, "START")==0){
            STARTADDRESS = atoi(OPERAND); // str->int
            LOCCTER = STARTADDRESS;
            continue;
        }else{
            LOCCTER = 0;
        }


        //-------------
        // OPCODE is not END
        if(strcmp(OPCODE, "END") != 0){
            if(strcmp(LABEL, "") != 0){
                char sLOCCTER[20];
                itoa(LOCCTER, sLOCCTER, 16); // 10->16  int->str
                fprintf(SYMTAB, "%s %s\n", LABEL, sLOCCTER);
            }
            
            //search OPTAB
            if(isInOPTAB(OPCODE,OPTAB) == 1){ //IN OPTAB
                //printf("%s IS IN OPTAB\n",OPCODE);

            }else{ //NOT IN OPTAB
                //printf("%s IS NOT IN OPTAB\n",OPCODE);
            }
            rewind(OPTAB);
            //strcmp(OPCODE, "RESW")==0 || strcmp(OPCODE, "RESB")==0 || strcmp(OPCODE, "BYTE")==0
            
        }//if(strcmp(OPCODE, "END") != 0)





    }//while(fgets(text,100,inputFile) != NULL)

     


    //----------------------------
    fclose(inputFile);
    fclose(resultFile);
    fclose(SYMTAB);
    fclose(OPTAB);
    fclose(interMediateFile);
    
    return(0);
}