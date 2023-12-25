#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//-------------------------------------------

void changeInputFile(){
    FILE* inputFile;
    inputFile = fopen("thisIsTheChangedInputFile.txt", "w+"); 
    char* fileName = malloc(100*sizeof(char));
    printf("  Please enter the file name:");
    scanf("%s",fileName);
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("File DNE!");
    }

    char line[256];  
    char* token1 = malloc(20*sizeof(char));
    char* token2 = malloc(20*sizeof(char));
    char* token3 = malloc(20*sizeof(char));

    while (fgets(line, sizeof(line), file) != NULL) {
        token1 = strtok(line, "\t\n");
        token2 = strtok(NULL, "\t\n");
        token3 = strtok(NULL, "\t\n");
        //printf("token1:%s| token2:%s| token3:%s| \n",token1,token2,token3);
        if(token3==NULL) fprintf(inputFile, """\t%s\t%s\n",token1,token2);
        else fprintf(inputFile, "%s\t%s\t%s\n",token1,token2,token3);
    }

    fclose(file);
    fclose(inputFile);
}

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

int isInSYMTAB(char* OPERAND2, FILE* SYMTAB){
    char* text = malloc(100*sizeof(char));
    while(fgets(text,100,SYMTAB) != NULL){
        char* tmp = malloc(20*sizeof(char));
        char* address = malloc(20*sizeof(char));
        tmp = getSection(text, 0, tmp, 0);
        //printf("tmp = %s  ",tmp);
        address = getSection(text, strlen(tmp)+1, address, 0);
        //printf("address = %s\n",address);
        if(strcmp(OPERAND2, tmp) == 0){
            int add = strtol(address, NULL, 10);
            return add;
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

    changeInputFile();

    int LOCCTER;
    char sLOCCTER[20];
    int STARTADDRESS;
    char sSTARTADDRESS[20];
    int PROGRAMLENGTH;
    char sPROGRAMLENGTH[20];


    //open file
    FILE* INPUTFILE;
    INPUTFILE = fopen("thisIsTheChangedInputFile.txt", "r"); // read
    FILE* OPTAB;
    OPTAB = fopen("OPTAB.txt", "r"); // read
    FILE* SYMTAB;
    SYMTAB = fopen("SYMTAB.txt", "w+"); //overwrite or create
    FILE* MIDFILE;
    MIDFILE = fopen("MIDFILE.txt", "w+"); //overwrite or create


    char* text = malloc(100*sizeof(char));
    
    printf("\n  ===========START PASS 1=========== \n");
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


        // OPCODE is START
        if(strcmp(OPCODE, "START")==0){
            STARTADDRESS = atoi(OPERAND); // str->int
            LOCCTER = STARTADDRESS;
            fprintf(MIDFILE, "%04x\t%s\t%s\t%s\n", LOCCTER, LABEL, OPCODE, OPERAND);
            continue;
        }

        fprintf(MIDFILE, "%04x\t%s\t%s\t%s\n", LOCCTER, LABEL, OPCODE, OPERAND);

        // OPCODE is not END
        if(strcmp(OPCODE, "END") != 0){
           
            // save LABEL's LOCCTER 
            if(strcmp(LABEL, "") != 0){
                fprintf(SYMTAB, "%s\t%04x\n", LABEL, LOCCTER);
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
    PROGRAMLENGTH = (LOCCTER) - STARTADDRESS;

    fclose(SYMTAB); // save SYMTAB file
    fclose(MIDFILE); // save MIDFILE file
    fclose(INPUTFILE);

    printTAB("SYMTAB.txt");
    printTAB("MIDFILE.txt");
    
    printf("  PROGRAMLENGTH = %d (%s)\n",PROGRAMLENGTH,itoa(LOCCTER, sLOCCTER, 16));

    printf("\n  ===========END PASS 1=========== \n");

    // 【start pass 2】
    printf("\n  ===========START PASS 2=========== \n");

    FILE* MIDFILE_2;
    MIDFILE_2 = fopen("MIDFILE.txt", "r"); //read
    FILE* SYMTAB_2;
    SYMTAB_2 = fopen("SYMTAB.txt", "r"); //read
    FILE* RESULTFILE;
    RESULTFILE = fopen("resultFile.txt", "w+"); //overwrite or create

    char* text2 = malloc(100*sizeof(char));

    while(fgets(text2,100,MIDFILE_2) != NULL){

        int OPADDRESS;

        char* LOC = malloc(20*sizeof(char));
        char* LABEL2 = malloc(20*sizeof(char));
        char* OPCODE2 = malloc(20*sizeof(char));
        char* OPERAND2 = malloc(20*sizeof(char));
        LOC = getSection(text2, 0, LOC, 0);
        LABEL2 = getSection(text2, strlen(LOC)+1, LABEL2, 0);
        OPCODE2 = getSection(text2, strlen(LOC)+strlen(LABEL2)+2, OPCODE2, 0);
        OPERAND2 = getSection(text2, strlen(LOC)+strlen(LABEL2)+strlen(OPCODE2)+3, OPERAND2, 0);

        printf("LOC = %s  |  ",LOC);
        printf("LABEL2 = %s  |  ",LABEL2);
        printf("OPCODE2 = %s|  ",OPCODE2);
        printf("OPERAND2 = %s\n",OPERAND2);

        if(strcmp(OPCODE2, "START")==0){
            fprintf(RESULTFILE, "H%s\t%06x%06x\n", LABEL2, STARTADDRESS, PROGRAMLENGTH);
        }
        else if(strcmp(OPCODE2, "END")!=0){ // NOT END

            //search OPCODE in OPTAB
            if(isInOPTAB(OPCODE2,OPTAB) == 1){ //IN OPTAB
                if(strcmp(OPERAND2, "")!=0){ // there is OPERAND
                    //printf("%s OPERAND ADDRESS = %d\n",OPERAND2, isInSYMTAB(OPERAND2, SYMTAB_2));
                    OPADDRESS = isInSYMTAB(OPERAND2, SYMTAB_2);
                }
            }else if(strcmp(OPCODE2, "BYTE")==0){ // do not need "WORD"

            }
            rewind(OPTAB);  // reread file
            rewind(SYMTAB);

        }else{ //END

        }

    }

    

    //printf("\n  ===========END PASS 2=========== \n");
    // 【end pass 2】
    //----------------------------------------------------


    fclose(RESULTFILE);
    fclose(OPTAB);
    fclose(SYMTAB);
    
    
    return(0);
}