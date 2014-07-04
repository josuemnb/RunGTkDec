#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <time.h>
#include <ctype.h>

#include "Interpreter.h"
#include "Error.h"
#include "XGui.h"

#include <gtk/gtk.h>

#ifndef nil
#define nil "\0"
#endif

#define EQ      40
#define NE      41
#define LT      42
#define LE      43
#define GT      44
#define GE      45

extern _DEF DEF;
extern _PROG PROG;
extern char mainType;

XWidget *lastXWidget;

char version[4]="0.2";
char fName[20]="test.cod";

short totalOfFunctions=0;

int fSize;
bool memOk=false;
FILE *fp;

Erro ERRO;

clock_t start, stop;

void printAll();
void loadThisFile(const char *str);
void loadVarTable();
void loadFunctionTable();
void closeAll();

void closeAll()
{
    stop=clock();
    clock_t elapsed;
    elapsed = 1000 * (stop - start) / (CLOCKS_PER_SEC);
    printf("Demorou %ld ms a interpretar\n",elapsed);
    if(memOk)
    {
//        delete [] VAR;
//        delete [] PROG.stackCharVal;
//        delete [] PROG.stackIntVal;
    }
    closeLog();
}

void loadThisFile(const char *str)
{
    if((fp=fopen(str,"rb"))==NULL)
    {
        puts("ERRO: Ficheiro não existe");
        exit(-1);
    }
    fseek(fp,0,SEEK_END);
    fSize=ftell(fp);
    fseek(fp,0,SEEK_SET);
    if(fgetc(fp)!='R' || fgetc(fp)!='U' || fgetc(fp)!='N')
        showErr(ERRO.UNKNOW_FILE);
    if(fgetc(fp)>version[0] || fgetc(fp)!=version[1] || fgetc(fp)>version[2])
        showErr(ERRO.CODEC_SUPERIOR);
    fread(&DEF,1,sizeof(DEF),fp);
    if(fgetc(fp)!='\n')
        showErr(ERRO.BROKEN_FILE);
}

void loadVarTable()
{
    int i,l;
    int indexV;

//    printf("total vars: %d\n",DEF.totalOfVars);
    PROG.stackCharPos=0;
    PROG.stackIntPos=0;
    PROG.stackIntVal=new int[DEF.totalStackInt];
    PROG.stackCharVal=new char[DEF.totalStackChar];
    fread((int*)PROG.stackIntVal,sizeof(int),DEF.totalStackInt,fp);
    if(fgetc(fp)!='\n')
        showErr(ERRO.BROKEN_FILE);
    fread((char*)PROG.stackCharVal,sizeof(char),DEF.totalStackChar,fp);
    if(fgetc(fp)!='\n')
        showErr(ERRO.BROKEN_FILE);
    VAR=new VARIABLE[DEF.totalOfVars];

    memOk=true;
    for(i=0; i<DEF.totalOfVars; i++)
    {
        indexV=PROG.stackIntVal[PROG.stackIntPos++];
        VAR[indexV].type=PROG.stackIntVal[PROG.stackIntPos++];
        VAR[indexV].length=PROG.stackIntVal[PROG.stackIntPos++];
        if(VAR[indexV].length>0)
        {
            for(l=0; l<VAR[indexV].length; l++)
                VAR[indexV].data.String[l]=PROG.stackCharVal[PROG.stackCharPos++];
            VAR[indexV].data.String[++l]='\0';
            if(VAR[indexV].type==2)
                VAR[indexV].data.Number=atoi(VAR[indexV].data.String);
        }
        else
            VAR[indexV].data.String[0]='\0';
//        puts(VAR[indexV].data.String);
    }
    free(PROG.stackCharVal);
    free(PROG.stackIntVal);
    return;
}

void loadFunctionTable()
{
    int j;

    fread(&totalOfFunctions,sizeof(short),1,fp);
    FUNC=new FUNCTION[totalOfFunctions];
    for(j=0; j<totalOfFunctions; j++)
    {
        fread(&FUNC[j].index,sizeof(short),1,fp);
        fread(&FUNC[j].pInt,sizeof(short),1,fp);
        FUNC[j].sInt=new int[FUNC[j].pInt];
        memset((char*)FUNC[j].sInt,0,FUNC[j].pInt);
        fread(&FUNC[j].sCharLen,sizeof(short),1,fp);
        FUNC[j].sChar=new char[FUNC[j].sCharLen+1];
        memset((char*)FUNC[j].sChar,0,FUNC[j].sCharLen+1);
        fread(&FUNC[j].ret,sizeof(short),1,fp);
        fread(&FUNC[j].n_param,sizeof(short),1,fp);
        fread(FUNC[j].param,sizeof(char),10,fp);
        fread(FUNC[j].sInt,sizeof(int),FUNC[j].pInt,fp);
        fread(FUNC[j].sChar,sizeof(char),FUNC[j].sCharLen-1,fp);
        if(fgetc(fp)!='\n')
            showErr(ERRO.BROKEN_FILE);
    }
    return;
}

void loadMainCode()
{
    short sCharLen,sIntLen;
    mainType=fgetc(fp);
    fread(&PROG.lastChar,sizeof(short),1,fp);
    //printf("lastchar: %d\n",PROG.lastChar);
    fread(&PROG.lastInt,sizeof(short),1,fp);
    //printf("lastint: %d\n",PROG.lastInt);
    if(mainType=='G')
        //XBegin();
        gtk_init(NULL,NULL);
    //printf("main: %c\n",mainType);
    fread(&sCharLen,sizeof(short),1,fp);
//    printf("charLen: %d\n",sCharLen);
    fread(&sIntLen,sizeof(short),1,fp);
//    printf("intLen: %d\n",sIntLen);
    if(fgetc(fp)!='I')
        showErr(ERRO.BROKEN_FILE);
    PROG.stackCharVal=new char[sCharLen+1];
    PROG.stackIntVal=new int[sIntLen];
    PROG.stackCharPos=0;
    PROG.stackIntPos=0;
    memset((char*)PROG.stackCharVal,0,sCharLen+1);
    memset((int*)PROG.stackIntVal,0,sIntLen);
    fread(PROG.stackIntVal,sizeof(int),sIntLen,fp);
    if(fgetc(fp)!='C')
        showErr(ERRO.BROKEN_FILE);
    fread(PROG.stackCharVal,sizeof(char),sCharLen,fp);
    if(fgetc(fp)!='\n')
        showErr(ERRO.BROKEN_FILE);
    return;
}

int main()
{
    int tok=0;
    char ch;

    atexit(closeAll);
    loadThisFile(fName);
    start=clock();
    loadVarTable();
    ch=fgetc(fp);
    if(ch=='$')
    {
        loadFunctionTable();
        ch=fgetc(fp);
    }
    if(ch!='#')
        showErr(ERRO.BROKEN_FILE);
    loadMainCode();
    PROG.stackCharPos=PROG.lastChar;
    PROG.stackIntPos=PROG.lastInt;
    printf("RUN - Version %s\n\n", version);
    while(tok!=-1)
        tok=evaluate();
    puts("\nFim do programa");
    return 0;
}
