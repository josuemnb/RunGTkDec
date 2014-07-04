#include "Interpreter.h"
#include "XGui.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>

char mainType;
short guiLevel=0;
int insideEvent=0;
int LogOpened=0;
FILE *fp;//este file para o log


struct _DEF DEF;
struct _PROG PROG;

int closeLog()
{
	if(LogOpened)
		fclose(fp);
}

void MovePointer(POINTER ptr)
{
	int tok=1;
	insideEvent=1;
	//printf("inicia evento com posições nos caracteres: %d e nos inteiros: %d\n",ptr.pChar,ptr.pInt);
    PROG.lastChar=PROG.stackCharPos;
    PROG.lastInt=PROG.stackIntPos;
    PROG.stackCharPos=ptr.pChar;
    PROG.stackIntPos=ptr.pInt;
	while(tok!=3)
		tok=evalBlock();
	insideEvent=0;
	PROG.stackCharPos=PROG.lastChar;
	PROG.stackIntPos=PROG.lastInt;
}

int evalWindow()
{
    char ch;
    int level=guiLevel;
    int indexW;
    char temp;

    if(PROG.stackCharVal[PROG.stackCharPos++]!='+')
        return 0;
	XWidget *w;
	w=XWindowNew();
//aumenta um nivel aos elementos de gui
    guiLevel++;
    while(level<guiLevel)
    {
        ch=PROG.stackCharVal[PROG.stackCharPos++];
        switch(ch)
        {
        case 'T':evalWidget(w,TEXTBOX);break;
        case 'L':evalWidget(w,LABEL);break;
        case 'I':evalWidget(w,IMAGE);break;
        case 'B':evalWidget(w,BUTTON);break;
        case 'C':evalWidget(w,CHECKBOX);break;
        case 1:indexW=PROG.stackIntVal[PROG.stackIntPos++];
            XWidgetSetCaption(w,VAR[indexW].data.String);break;
        case 2:break;//id
        case 3:XWidgetSetWidth(w,PROG.stackIntVal[PROG.stackIntPos++]);break;
        case 4:XWidgetSetHeight(w,PROG.stackIntVal[PROG.stackIntPos++]);break;
        case 5:XWidgetSetTop(w,PROG.stackIntVal[PROG.stackIntPos++]);break;
        case 6:XWidgetSetLeft(w,PROG.stackIntVal[PROG.stackIntPos++]);break;
        case 7:temp=PROG.stackCharVal[PROG.stackCharPos++];
			if(temp==1)XWidgetShow(w);else XWidgetHide(w);break;
        case 8:PROG.stackIntVal[PROG.stackIntPos++];break;//state
        case 9:evalEvent(w);break;
        case 45:guiLevel--;break;
        case 43:guiLevel++;break;
        default:return 0;
        }
    }
    gtk_main();
    return 1;
}

int evalEvent(XWidget *w)
{
	char event;
	short pchar;
	short pint;
    event=PROG.stackCharVal[PROG.stackCharPos++];//tipo de evento
    if(PROG.stackCharVal[PROG.stackCharPos++]!='F')
		return 0;//showErr(ERRO.BROKEN_FILE);
	pchar=PROG.stackIntVal[PROG.stackIntPos++];
    pint=PROG.stackIntVal[PROG.stackIntPos++];//posição do stack integer para onde vai
    switch(event)
    {
    case CLICK:w->event.Click.pInt=pint;w->event.Click.pChar=pchar;break;
    case DOWN:w->event.Down.pInt=pint;w->event.Down.pChar=pchar;break;
    case UP:w->event.Up.pInt=pint;w->event.Up.pChar=pchar;break;
    case ENTER:w->event.Enter.pInt=pint;w->event.Enter.pChar=pchar;break;
    case LEAVE:w->event.Leave.pInt=pint;w->event.Leave.pChar=pchar;break;
    case MOVE:w->event.Move.pInt=pint;w->event.Move.pChar=pchar;break;
    case RESIZE:w->event.Resize.pInt=pint;w->event.Resize.pChar=pchar;break;
    case SHOW:w->event.Show.pInt=pint;w->event.Show.pChar=pchar;break;
    case HIDE:w->event.Hide.pInt=pint;w->event.Hide.pChar=pchar;break;
    case PAINT:w->event.Paint.pInt=pint;w->event.Paint.pChar=pchar;break;
    case FOCUSOUT:w->event.FocusOut.pInt=pint;w->event.FocusOut.pChar=pchar;break;
    case FOCUSIN:w->event.FocusIn.pInt=pint;w->event.FocusIn.pChar=pchar;break;
    case PRESS:w->event.Press.pInt=pint;w->event.Press.pChar=pchar;break;
    case RELEASE:w->event.Release.pInt=pint;w->event.Release.pChar=pchar;break;
    default:return 0;
    }
 	switch(w->type)
	{
	case WINDOW:XWindowEvent(w,event);break;
	case BUTTON:XButtonEvent(w,event);break;
	case LABEL:break;//label não tem eventos associados
	case TEXTBOX:XTextBoxEvent(w,event);break;
	case CHECKBOX:XCheckBoxEvent(w,event);break;
	case IMAGE:break;
	default:return 0;
	}
	return 1;
}

int evalWidget(XWidget *w, int type)
{
    char ch;
    int level=guiLevel;
    int indexL;
    char temp;

    if(PROG.stackCharVal[PROG.stackCharPos++]!='+')
		return 0;
    guiLevel++;
    XWidget *wid=NULL;
    switch(type)
    {
    case LABEL:wid=XLabelNew(w);break;
    case IMAGE:wid=XImageNew(w);break;
    case TEXTBOX:wid=XTextBoxNew(w);break;
    case CHECKBOX:wid=XCheckBoxNew(w);break;
    case BUTTON:wid=XButtonNew(w);break;
    default:return 0;
    }
    while(level<guiLevel)
    {
        ch=PROG.stackCharVal[PROG.stackCharPos++];
        switch(ch)
        {
        case 1:
        	indexL=PROG.stackIntVal[PROG.stackIntPos++];
        	if(type!=IMAGE)
				XWidgetSetCaption(wid,VAR[indexL].data.String);
			else
				XImageLoad(wid,VAR[indexL].data.String);
			break;
        case 2:break;//id
        case 3:XWidgetSetWidth(wid,PROG.stackIntVal[PROG.stackIntPos++]);break;
        case 4:XWidgetSetHeight(wid,PROG.stackIntVal[PROG.stackIntPos++]);break;
        case 5:XWidgetSetTop(wid,PROG.stackIntVal[PROG.stackIntPos++]);break;
        case 6:XWidgetSetLeft(wid,PROG.stackIntVal[PROG.stackIntPos++]);break;
        case 7:temp=PROG.stackCharVal[PROG.stackCharPos++];
            if(temp==1)XWidgetShow(wid);else XWidgetHide(wid);break;
        case 8:break;//state
        case 9:evalEvent(wid);break;
        case 45:guiLevel--;break;
        case 43:guiLevel++;break;
        default:return 0;
        }
    }
    return 1;
}

int evalLog()
{
    int indexL;

    if(LogOpened==0) //ainda não foi criado
    {
        char log[50];
        strcpy(log,"test.cod");
        strcat(log,".log");
        if((fp=fopen(log,"wb"))==NULL)
            return 0;
        LogOpened=1;
    }
    if(PROG.stackCharVal[PROG.stackCharPos++]!='V')
        return 0;
    indexL=PROG.stackIntVal[PROG.stackIntPos++];
    if(VAR[indexL].data.String==NULL)
        fputs("LOG: NULL",fp);
    else
        fprintf(fp,"LOG: %s\n", VAR[indexL].data.String);
    return 1;
}

int evalDebug()
{
    int indexD;

    if(PROG.stackCharVal[PROG.stackCharPos++]!='V')
		return 0;
    indexD=PROG.stackIntVal[PROG.stackIntPos++];
    if(VAR[indexD].type==2)
    {
        printf("DEBUG: %d\n",VAR[indexD].data.Number);
        return 1;
    }
    if(VAR[indexD].data.String==NULL)
        puts("DEBUG: NULL");
    else
        printf("DEBUG: %s\n",VAR[indexD].data.String);
    return 1;
}

int evalFunction()
{
    int indexF;
    short tok=1;
    int *tSInt=PROG.stackIntVal;
    char *tChar=PROG.stackCharVal;
    short tPInt,tPChar;

    indexF=PROG.stackIntVal[PROG.stackIntPos++];
    tPInt=PROG.stackIntPos;
    tPChar=PROG.stackCharPos;
    PROG.stackIntVal=FUNC[indexF].sInt;
    PROG.stackCharVal=FUNC[indexF].sChar;
    PROG.stackIntPos=0;
    PROG.stackCharPos=0;

    while(tok!=-1)
        tok=evalBlock();
    PROG.stackIntPos=tPInt;
    PROG.stackCharPos=tPChar;
    PROG.stackCharVal=tChar;
    PROG.stackIntVal=tSInt;
    return 1;
}

int evalCompNumber(int n1,int n2, int cond)
{
    switch(cond)
    {
    case 40:if(n1==n2)return 2;
    case 41:if(n1!=n2)return 2;
    case 42:if(n1<n2)return 2;
    case 43:if(n1<=n2)return 2;
    case 44:if(n1>n2)return 2;
    case 45:if(n1>=n2)return 2;
    default:return 0;
    }
    return 1;
}

int evalIf()   //esta função vai desenvolver sobre a keyword IF, ELSE e ELSEIF
{
    int indexIf,endPos,endInt,cond,number;
    int result=0;
    char code;

    code=PROG.stackCharVal[PROG.stackCharPos++];
    if(code=='C')//é um valor simples, preciso de buscar o valor index da variavel;
    {
        indexIf=PROG.stackIntVal[PROG.stackIntPos++];
        code=PROG.stackCharVal[PROG.stackCharPos++];
        if(code=='L')//o bloco só tem posição final
        {
            endPos=PROG.stackIntVal[PROG.stackIntPos++];
            endInt=PROG.stackIntVal[PROG.stackIntPos++];
            if(VAR[indexIf].type==1)//é uma string
            {
                if(strlen(VAR[indexIf].data.String)<1)
                {
                    PROG.stackCharPos=(short)endPos;
                    PROG.stackIntPos=(short)endInt;
                    return 1;
                }
                return 1;
            }
            else if(VAR[indexIf].type==2)//é um numero
            {
                if(VAR[indexIf].data.Number<1)
                {
                    PROG.stackCharPos=endPos;
                    PROG.stackIntPos=endInt;
                    return 1;
                }
                return 1;
            }
            else return 1;
        }
    }
    else if(code=='N')//compara o valor do index com um numero
    {
        indexIf=PROG.stackIntVal[PROG.stackIntPos++];
        code=PROG.stackCharVal[PROG.stackCharPos++];
        if(code=='L')//o bloco só tem uma linha de codigo
        {
            cond=PROG.stackIntVal[PROG.stackIntPos++];
            number=PROG.stackIntVal[PROG.stackIntPos++];
            endPos=PROG.stackIntVal[PROG.stackIntPos++];
            endInt=PROG.stackIntVal[PROG.stackIntPos++];
            result=evalCompNumber(VAR[indexIf].data.Number,number,cond);
            if(!result)
            {
                PROG.stackCharPos=(short)endPos;
                PROG.stackIntPos=(short)endInt;
                return 1;
            }
            return 1;
        }
        else if(code=='F')//é um bloco com endif
        {
            cond=PROG.stackIntVal[PROG.stackIntPos++];
            number=PROG.stackIntVal[PROG.stackIntPos++];
            PROG.stackIntPos++;//salta o espaço reservado pelo elsepos
            PROG.stackIntPos++;//salta o espaço reservado para elseint
            endPos=PROG.stackIntVal[PROG.stackIntPos++];
            endInt=PROG.stackIntVal[PROG.stackIntPos++];
            result=evalCompNumber(VAR[indexIf].data.Number,number,cond);
            if(!result)
            {
                PROG.stackCharPos=(short)endPos;
                PROG.stackIntPos=(short)endInt;
                return 1;
            }
            return 1;
        }
        else
            return 0;
    }
    return 0;
}

int evalAttrib()
{
    unsigned int indexOrigin,indexDest;
    char ch;

    indexDest=PROG.stackIntVal[PROG.stackIntPos++];//index da variavel que vai aceitar este valor

    ch=PROG.stackCharVal[PROG.stackCharPos++];
    if(ch=='V')//vai atribuir o valor vindo de uma variavel
    {
        indexOrigin=PROG.stackIntVal[PROG.stackIntPos++];
        if(VAR[indexOrigin].type==2)
        {
            VAR[indexDest].type=2;
            VAR[indexDest].data.Number=VAR[indexOrigin].data.Number;
            return 1;
        }
        strcpy(VAR[indexDest].data.String,VAR[indexOrigin].data.String);
        return 1;
    }
    else if(ch=='N')
    {
        VAR[indexDest].type=2;
        VAR[indexDest].data.Number=PROG.stackIntVal[PROG.stackIntPos++];
        return 1;
    }
    return 0;
}

int evalPuts()
{
    char code;
    int indexP;

    code=PROG.stackCharVal[PROG.stackCharPos++];
    while(code!='\n')
    {
        if(code=='V')//valor de uma variavel
        {
            indexP=PROG.stackIntVal[PROG.stackIntPos++];
            if(VAR[indexP].type==2)
                printf("%d",VAR[indexP].data.Number);
            else
                printf(VAR[indexP].data.String);
                fflush(stdout);
        }
        else if(code=='A')//atribuiçao
        {
            indexP=PROG.stackIntVal[PROG.stackIntPos++];
            if(PROG.stackCharVal[PROG.stackCharPos++]!='\n')
                return 0;
            scanf("%s",VAR[indexP].data.String);
            break;
        }
        else if(code=='N')//nova linha
        {
            if(PROG.stackCharVal[PROG.stackCharPos++]!='\n')
				return 0;
            putchar('\n');
            break;
        }
        code=PROG.stackCharVal[PROG.stackCharPos++];
    }
    return 1;
}

int evalSpin()
{
    char code=0;
    int indexSpin=0,startSpin=0,stopSpin=0,lengthSpin=0;
    int i=0, pTempInt=0, pTempChar=0, len,tok=1;

    code=PROG.stackCharVal[PROG.stackCharPos++];
    if(code=='N')//processo normal spin a from x to y
    {
        indexSpin=PROG.stackIntVal[PROG.stackIntPos++];
        startSpin=PROG.stackIntVal[PROG.stackIntPos++];
        stopSpin=PROG.stackIntVal[PROG.stackIntPos++];
        lengthSpin=PROG.stackIntVal[PROG.stackIntPos++];
        pTempChar=PROG.stackCharPos;
        pTempInt=PROG.stackIntPos;
        len=pTempChar+lengthSpin;
        VAR[indexSpin].type=2;
        if(lengthSpin==0)
        {
            VAR[indexSpin].data.Number=stopSpin;
            return 1;
        }
        for(VAR[indexSpin].data.Number=startSpin; VAR[indexSpin].data.Number<stopSpin; VAR[indexSpin].data.Number++)
        {
            while(len>PROG.stackCharPos)
            {
                tok=evalBlock();
                if(tok==2)
                    return 1;
            }
            PROG.stackIntPos=pTempInt;
            PROG.stackCharPos=pTempChar;
        }
        return 1;
    }
    else if(code=='U')//spin a to x
    {
        indexSpin=PROG.stackIntVal[PROG.stackIntPos++];
        stopSpin=PROG.stackIntVal[PROG.stackIntPos++];
        lengthSpin=PROG.stackIntVal[PROG.stackIntPos++];
        pTempChar=PROG.stackCharPos;
        pTempInt=PROG.stackIntPos;
        if(strlen(VAR[indexSpin].data.String)<1)
            VAR[indexSpin].data.Number=0;
        len=pTempChar+lengthSpin;
        VAR[indexSpin].type=2;
        if(lengthSpin==0)
        {
            VAR[indexSpin].data.Number=stopSpin;
            return 1;
        }
        for(; VAR[indexSpin].data.Number<stopSpin; VAR[indexSpin].data.Number++)
        {
            while(len>PROG.stackCharPos)
            {
                tok=evalBlock();
                if(tok==2)
                    return 1;
            }
            PROG.stackIntPos=pTempInt;
            PROG.stackCharPos=pTempChar;
        }
        return 1;
    }
    else if(code=='I')//spin a from x
    {
        indexSpin=PROG.stackIntVal[PROG.stackIntPos++];
        startSpin=PROG.stackIntVal[PROG.stackIntPos++];
        lengthSpin=PROG.stackIntVal[PROG.stackIntPos++];
        pTempChar=PROG.stackCharPos;
        pTempInt=PROG.stackIntPos;
        len=pTempChar+lengthSpin;
        VAR[indexSpin].type=2;
        for(VAR[indexSpin].data.Number=startSpin;; VAR[indexSpin].data.Number++)
        {
            while(len>PROG.stackCharPos)
            {
                tok=evalBlock();
                if(tok==2)
                    return 1;
            }
            PROG.stackIntPos=pTempInt;
            PROG.stackCharPos=pTempChar;
        }
        return 1;
    }
//    else if(code=='S')//spin from x to y
//    {
//        if(!getTheNextNumber())
//            showErr(ERRO.BROKEN_FILE);
//        init=atoi(returnTheToken());
//        if(getTheNextChar()!='$')
//            showErr(ERRO.BROKEN_FILE);
//        if(!getTheNextNumber())
//            showErr(ERRO.BROKEN_FILE);
//        fin=atoi(returnTheToken());
//        if(getTheNextChar()!='L')
//            showErr(ERRO.BROKEN_FILE);
//        if(!getTheNextNumber())
//            showErr(ERRO.BROKEN_FILE);
//        length=atol(returnTheToken());
//        if(getTheNextChar()!='\n')
//            showErr(ERRO.BROKEN_FILE);
//        tmpPtr=pointer;
//        for(i=init;i<=fin;i++)
//        {
//            pointer=tmpPtr;
//            while(tmpPtr+length>pointer)
//                evalBlock();
//        }
//    }
//    else if(code=='P')//spin ever
//    {
//        if(getTheNextChar()!='L')
//            showErr(ERRO.BROKEN_FILE);
//        length=atol(returnTheToken());
//        if(!getTheNextChar()!='\n')
//            showErr(ERRO.BROKEN_FILE);
//        tmpPtr=pointer;
//        for(;;)
//        {
//            pointer=tmpPtr;
//            while(tmpPtr+length>pointer)
//                evalBlock();
//        }
//    }
    else if(code=='V')//spin to a
    {
        indexSpin=PROG.stackIntVal[PROG.stackIntPos++];
        lengthSpin=PROG.stackIntVal[PROG.stackIntPos++];
        pTempChar=PROG.stackCharPos;
        pTempInt=PROG.stackIntPos;
        if(strlen(VAR[indexSpin].data.String)<1)
            VAR[indexSpin].data.Number=0;
        VAR[indexSpin].type=2;
        len=pTempChar+lengthSpin;
        if(lengthSpin==0)
            return 1;
        for(i=0; i<=VAR[indexSpin].data.Number; i++)
        {
            PROG.stackIntPos=pTempInt;
            PROG.stackCharPos=pTempChar;
            while(len>PROG.stackCharPos)
            {
                tok=evalBlock();
                if(tok==2)
                    return 1;
            }
        }
        return 1;
    }
    else if(code=='C')//spin to x
    {
        stopSpin=PROG.stackIntVal[PROG.stackIntPos++];
        lengthSpin=PROG.stackIntVal[PROG.stackIntPos++];
        pTempChar=PROG.stackCharPos;
        pTempInt=PROG.stackIntPos;
        len=pTempChar+lengthSpin;
        if(lengthSpin==0)
            return 1;
        for(i=0; i<=stopSpin; i++)
        {
            PROG.stackIntPos=pTempInt;
            PROG.stackCharPos=pTempChar;
            while(len>PROG.stackCharPos)
            {
                tok=evalBlock();
                if(tok==2)
                    return 1;
            }
        }
        return 1;
    }
    else
        return 0;
}

int evaluate()
{
	return evalBlock();
}

int evalBlock()
{
    int ret;
    char ch;

    ch=PROG.stackCharVal[PROG.stackCharPos++];
//	printf("code: %c\n",ch);
    switch(ch)
    {
    case 'A':evalAttrib();ret=1;break;//atribuição
    case 'L':evalLog();ret=1;break;//log
    case 'D':evalDebug();ret=1;break;//debug
    case 'P':evalPuts();ret=1;break;//puts
    case 'S':evalSpin();ret=1;break;//spin
    case 'I':evalIf();ret=1;break;//if
    case '\0':if(mainType=='G')gtk_exit(1);else exit(1);
    case 'F':evalFunction();ret=1;break;//function
    case 'B':ret=2;break;//break
	case '%':ret=3;break;//endf
    }
    if(insideEvent)
		return ret;
    if(mainType=='G')//é um Gui
    {
        switch(ch)
        {
        case 'W':evalWindow();ret=1;break;
        default:return 0;
        }
    }
    return ret;
}

