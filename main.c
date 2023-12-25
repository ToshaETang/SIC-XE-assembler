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


int getCompareAdresss(char* OPCODE){
    FILE* OPTAB = fopen("OPTAB.txt","r");
    char* text = malloc(100*sizeof(char));
    
    while(fgets(text,100,OPTAB) != NULL){
        char* label = malloc(20*sizeof(char));
        label = getSection(text, 0, label, 0);
        if(strcmp(OPCODE, label) == 0){
            char* comadd = malloc(20*sizeof(char));
            int comaddress = strtol(getSection(text, strlen(label)+1, comadd, 0),NULL,16);
            return comaddress;
        }
    }
    fclose(OPTAB);
    return 0;
}

int getCompareSymbolValue(char* OPERAND){
    FILE* SYMTAB = fopen("SYMTAB.txt","r");
    char* text = malloc(100*sizeof(char));
    
    while(fgets(text,100,SYMTAB) != NULL){
        char* label = malloc(20*sizeof(char));
        label = getSection(text, 0, label, 0);
        //printf("%s\n",label);
        if(strcmp(OPERAND, label) == 0){
            char* comsym = malloc(20*sizeof(char));
            int comsymbolvalue = strtol(getSection(text, strlen(label)+1, comsym, 0),NULL,16);
            //printf("%x %d\n",comsymbol,comsymbol);
            return comsymbolvalue;
        }
    }
    fclose(SYMTAB);
    return 0;
}


int getPC(char* OPCODE){
    FILE* MIDFILE = fopen("MIDFILE.txt","r");
    char* text = malloc(100*sizeof(char));
    
    while(fgets(text,100,MIDFILE) != NULL){
        char* opcode = malloc(20*sizeof(char));
        char* pc = malloc(20*sizeof(char));
        pc = getSection(text, 0, pc, 0);
        opcode = getSection(text, strlen(pc)+1, opcode, 0);
        //printf("%s\n",label);
        if(strcmp(OPCODE, opcode) == 0){
            return strtol(pc,NULL,16);
        }
    }
    fclose(MIDFILE);
    return 0;

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

    int N,I,X,B,P,E,format,invalidopcode;

    char* text2 = malloc(100*sizeof(char));

    while(fgets(text2,100,MIDFILE_2) != NULL){

        int OPADDRESS,PC,textRecordLength=0;
        char* mncode = malloc(20*sizeof(char));
        char* LOC = malloc(20*sizeof(char));
        char* LABEL2 = malloc(20*sizeof(char));
        char* OPCODE2 = malloc(20*sizeof(char));
        char* OPERAND2 = malloc(20*sizeof(char));
        LOC = getSection(text2, 0, LOC, 0);
        LABEL2 = getSection(text2, strlen(LOC)+1, LABEL2, 0);
        OPCODE2 = getSection(text2, strlen(LOC)+strlen(LABEL2)+2, OPCODE2, 0);
        OPERAND2 = getSection(text2, strlen(LOC)+strlen(LABEL2)+strlen(OPCODE2)+3, OPERAND2, 0);

        //printf("LOC = %s  |  ",LOC);
        //printf("LABEL2 = %s  |  ",LABEL2);
        //printf("OPCODE2 = %s|  ",OPCODE2);
        //printf("OPERAND2 = %s\n",OPERAND2);

        if(strcmp(OPCODE2, "START")==0){
            fprintf(RESULTFILE, "H%s\t%06x%06x\n", LABEL2, STARTADDRESS, PROGRAMLENGTH);
            PC = STARTADDRESS;
        }
        else if(strcmp(OPCODE2, "END")!=0){ // NOT END
            invalidopcode = 1;
            PC = getPC(OPCODE2);
            //printf("%s:%x\n",OPCODE2,PC);
            //search OPCODE in OPTAB
            if(isInOPTAB(OPCODE2,OPTAB) == 1){ //IN OPTAB
                if(strcmp(OPERAND2, "")!=0){ // there is OPERAND
                    //printf("%s OPERAND ADDRESS = %d\n",OPERAND2, isInSYMTAB(OPERAND2, SYMTAB_2));
                    OPADDRESS = isInSYMTAB(OPERAND2, SYMTAB_2);
                    //printf("%s : %d\n",OPERAND2,OPADDRESS);
                    //printf("%s\n",OPCODE2);
                    
                    //assemble OPCODE2
                    if(OPCODE2[strlen(OPCODE2)-1]=='R'){
                        //printf("%s is FORMAT 2\n",OPCODE2);
                        format = 2;
                        N = 0;
                        I = 0;
                        X = 0;
                        B = 0;
                        P = 0;
                        E = 0;
                    }
                    else{ // not format 2
                        if(OPCODE2[0]=='+'){
                            format = 4;
                            E = 1;
                        }
                        else{
                            E = 0;
                            format = 3;
                        }

                        if(OPERAND2[0]=='#'){
                            N=0;
                            I=1;
                            X=0;
                            B=0;
                            P=0;
                            for(int i=0; OPERAND2[i]!='\0'; i++){
                                OPERAND2[i]=OPERAND2[i+1];
                            }
                            //printf("  %s new:%s\n",OPCODE2,OPERAND2);
                        }
                        else if(OPERAND2[0]=='@'){
                            N=1;
                            I=0;
                            X=0;
                            B=0;
                            P=0;
                            for(int i=0; OPERAND2[i]!='\0'; i++){
                                OPERAND2[i]=OPERAND2[i+1];
                            }
                        }
                        else if(OPERAND2[strlen(OPERAND2)-1]=='X' && OPERAND2[strlen(OPERAND2)-2]==','){
                            N=1;
                            I=1;
                            X=1;
                            B=0;
                            P=1;
                            OPERAND2[strlen(OPERAND2)-2]='\0';
                            //printf("  %s new:%s\n",OPCODE2,OPERAND2);
                        }
                        else{
                            N=1;
                            I=1;
                            X=0;
                            B=0;
                            P=1;
                        }
                    }
                }
                // generate
                //printf("%x\n",getCompareAdresss(OPCODE2));
                //int compareAdresss = getCompareAdresss(OPCODE2);
                invalidopcode = 0;
                int compareAdresss = getCompareAdresss(OPCODE2);
                int obj =  compareAdresss/16;
                //printf("%x\n",obj);
                char sobj[10];
                itoa(obj,sobj,16);
                strcat(mncode,sobj);
                compareAdresss = compareAdresss%16;

                if(N == 1) compareAdresss+=2;
                if(I == 1) compareAdresss+=1;
                itoa(compareAdresss,sobj,16);
                strcat(mncode,sobj);
                //printf("%s mncode:%s\n",OPCODE2,mncode);

                if(format == 2){
                    switch(OPERAND2[0])
                    {
                        case 'X' : strcat(mncode,"1");
                                 break;
                        case 'A' : strcat(mncode,"0");
                                 break;
                        case 'S' : strcat(mncode,"4");
                                 break;
                        case 'T' : strcat(mncode,"5");
                                  break;
                    }
                    //printf("%s mncode:%s\n",OPCODE2,mncode);
                }else{
                    compareAdresss = 0;
                    if(X==1)
                        compareAdresss+=8;
                    if(B==1)
                        compareAdresss+=4;
                    if(P==1)
                        compareAdresss+=2;
                    if(E==1)
                        compareAdresss+=1;
                    itoa(compareAdresss,sobj,16);
                    strcat(mncode,sobj);
                    //printf("%s mncode:%s\n",OPCODE2,mncode);
                    if(E==1){
                        if(N==1 && I==1){
                            int compareSymbolValue = getCompareSymbolValue(OPERAND2);
                            //printf("%s compareSymbol:%x\n",OPERAND2, compareSymbolValue);
                            //strcat(mncode,"_0");
                            itoa(compareAdresss,sobj,16);
                            strcat(mncode,sobj);
                        }
                        //printf("%s mncode:%s\n",OPCODE2,mncode); !!!!!
                    }
                }

                //printf("%s mncode:%s\n",OPCODE2,mncode);
            }else if(strcmp(OPCODE2, "BYTE")==0){ // do not need "WORD"
                format = 1;
                if(OPERAND2[0]=='X'){
                    for(int i =0 ; OPERAND2[i]!='\0';i++){
                        OPERAND2[i]=OPERAND2[i+2];
                    }
                    OPERAND2[strlen(OPERAND2)-1]='\0';
                    strcpy(mncode,OPERAND2);
                    //printf("%s mncode:%s\n",OPCODE2,mncode);
                }else{
                    char ele[2];
                    char buffer[20]="";
                    for(int i = 2;i<strlen(OPERAND2)-1;i++){
                        sprintf(ele,"%x",OPERAND2[i]);
                        strcat(buffer,ele);
                    }
                    strcat(mncode,buffer);
                    //printf("%s mncode:%s\n",OPCODE2,mncode);
                }
            }
            printf("%s objectCode:%s\n",OPCODE2,mncode);


            rewind(OPTAB);  // reread file
            rewind(SYMTAB);

        }else{ //END

        }

    }

    

    printf("\n  ===========END PASS 2=========== \n");
    // 【end pass 2】
    //----------------------------------------------------


    fclose(RESULTFILE);
    fclose(OPTAB);
    fclose(SYMTAB);
    
    
    return(0);
}