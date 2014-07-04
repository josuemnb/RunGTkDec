#ifndef INTERPRETER_H
#define INTERPRETER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "XGui.h"

union VALUE
{
    int Number;
    char Char;
    char String[255];
    float Real;
	//int boolean:1;
};

struct FUNCTION
{
    int index;
    short pInt;
    short sCharLen;
    short ret;
    short n_param;
    char param[10];
    int * sInt;
    char * sChar;
}*FUNC;

struct VARIABLE
{
    unsigned short int type;
//	unsigned short int index;
    unsigned short int length;
    union VALUE data;
}*VAR;
//
struct _PROG
{
    int *stackIntVal;
    char *stackCharVal;
    short stackIntPos;
    short stackCharPos;
    short lastChar;
    short lastInt;
};

 struct _DEF
{
    short totalOfVars;
    short maxValueLength;
    short totalStackInt;
    short totalStackChar;
};

void MovePointer(POINTER);
void ConnectEvent(XWidget *w, char event);
int closeLog();

int evalIf();
int evalBlock();
int evaluate();
int evalElse();
int evalAttrib();
int evalConst();
int evalDebug();
int evalPuts();
int evalRead();
int evalSpin();
int evalFunction();
int evalLog();
int evalCompNumber(int n1,int n2, int cond);
int processSpin(int index, int init, int final, int len );

//funcoes do Gui
int evalWindow();
int evalButton(XWidget*);
int evalText(XWidget*);
int evalLabel(XWidget*);
int evalImage(XWidget*);
int evalWidget(XWidget*,int type);
int evalEvent(XWidget*);

#ifdef __cplusplus
}
#endif

#endif // EVALUATOR_H
