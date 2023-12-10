#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* getSection(char* text, int textIndex, char *section, int sectionIndex){
    if(textIndex+1 >= strlen(text)){
        section = "";
    }
    else if(text[textIndex] == '\t'){
        section = "";
    }else{
        while(text[textIndex] != '\t' && text[textIndex] != '\n'){
            section[sectionIndex] = text[textIndex];
            textIndex++;
            sectionIndex++;
        }
        section[sectionIndex] = '\0';
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


void printTAB(char* fileName){
    FILE* TAB;
    TAB = fopen(fileName,"r"); // read

    printf("\n  ------------THIS IS %s------------\n",fileName);
    char* tmp = malloc(100*sizeof(char));
    while(fgets(tmp,100,TAB) != NULL){
        printf("  %s\n",tmp);
    }
    printf("  -----------------------------------\n");

    fclose(TAB);
}


//==============================================================

int main()
{   
    printf("\n  START TOSHA'S ASSEMMMBLER ^O^\n");

    int LOCCTER;
    char sLOCCTER[20];
    int STARTADDRESS;


    //open file
    FILE* INPUTFILE;
    INPUTFILE = fopen("inputFile.txt", "r"); // read
    FILE* OPTAB;
    OPTAB = fopen("OPTAB.txt", "r"); // read
    FILE* SYMTAB;
    SYMTAB = fopen("SYMTAB.txt", "w+"); //overwrite or create
    FILE* RESULTFILE;
    RESULTFILE = fopen("resultFile.txt", "w+"); //overwrite or create
    FILE* MIDFILE;
    MIDFILE = fopen("MIDFILE.txt", "w+"); //overwrite or create


    char* text = malloc(100*sizeof(char));
    
    // 【start pass 1】
    while(fgets(text,100,INPUTFILE) != NULL){

        //printf("%s\n",text);
        char* LABEL = malloc(20*sizeof(char));
        char* OPCODE = malloc(20*sizeof(char));
        char* OPERAND = malloc(20*sizeof(char));

        //seperate the input line
        LABEL = getSection(text, 0, LABEL, 0);
        OPCODE = getSection(text, strlen(LABEL)+1, OPCODE, 0);
        OPERAND = getSection(text, strlen(LABEL)+strlen(OPCODE)+2, OPERAND, 0);
        //printf("LABEL = %s  |  ",LABEL);
        //printf("OPCODE = %s|  ",OPCODE);
        //printf("OPERAND = %s\n",OPERAND);

        //-------------make SYMTAB--------------------------

        // OPCODE is START
        if(strcmp(OPCODE, "START")==0){
            STARTADDRESS = atoi(OPERAND); // str->int
            LOCCTER = STARTADDRESS;
            itoa(LOCCTER, sLOCCTER, 16); // 10->16, int->str
            fprintf(MIDFILE, "%s\t%s\t%s\t%s\n", sLOCCTER, LABEL, OPCODE, OPERAND);
            continue;
        }

        itoa(LOCCTER, sLOCCTER, 16); // 10->16, int->str
        fprintf(MIDFILE, "%s\t%s\t%s\t%s\n", sLOCCTER, LABEL, OPCODE, OPERAND);

        // OPCODE is not END
        if(strcmp(OPCODE, "END") != 0){
           
            // save LABEL's LOCCTER 
            if(strcmp(LABEL, "") != 0){
                itoa(LOCCTER, sLOCCTER, 16); // 10->16, int->str
                fprintf(SYMTAB, "%s %s\n", LABEL, sLOCCTER);
            }
            
            //search OPCODE in OPTAB
            if(isInOPTAB(OPCODE,OPTAB) == 1){ //IN OPTAB
                //printf("%s IS IN OPTAB\n",OPCODE);
                if(OPCODE[0] == '+'){
                    LOCCTER = LOCCTER + 4; // format 4
                }else if(OPCODE[strlen(OPCODE)-1] == 'R'){
                    LOCCTER = LOCCTER + 2; // format 2
                }else{
                    LOCCTER = LOCCTER + 3; // format 3
                }
            }else{ //NOT IN OPTAB
                //printf("%s IS NOT IN OPTAB\n",OPCODE);
                if(strcmp(OPCODE, "WORD")==0){
                    LOCCTER = LOCCTER + 3;
                }else if(strcmp(OPCODE, "RESW")==0){
                    LOCCTER = LOCCTER + (atoi(OPERAND)*3);
                }else if(strcmp(OPCODE, "RESB")==0){
                    LOCCTER = LOCCTER + atoi(OPERAND);
                }else if(strcmp(OPCODE, "BYTE")==0){
                    if(OPERAND[0]=='X'){
                        LOCCTER = LOCCTER + 1;
                    }else{
                        LOCCTER = LOCCTER + (strlen(OPERAND)-3); // C''
                    }
                }
            }
            
            rewind(OPTAB);  // reread file

        }// OPCODE is not END
        
    }// 【end pass 1】
    
    fclose(SYMTAB); // save SYMTAB file
    fclose(MIDFILE); // save MIDFILE file

    printTAB("SYMTAB.txt");
    printTAB("MIDFILE.txt");

    //-------------finish SYMTAB--------------------------



    //----------------------------------------------------
    fclose(INPUTFILE);
    fclose(RESULTFILE);
    fclose(OPTAB);
    
    
    return(0);
}