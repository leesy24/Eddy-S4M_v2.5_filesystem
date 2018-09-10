/*
    icelove's Display Html

    1999/3/9


    Link with cgic.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "iDisplayHtml.h"
#include "cgic205/cgic.h"

/* Read Status Definition */
#define RSNORMAL        0
#define RSSTX           1
#define RSETX           2
#define RSCHECKTAG      3
#define RSLOOP          4
#define RSREPLACE       5

/* List Get Definition */
#define LGNOT           0
#define LGOK            1
#define LGENDLOOP       2
#define LGOKFILE        3
#define LGENDLOOPFILE   4


char BigBuff[20480];

void ClearSpace(char *data, char *buff);
void ErrorMsg(char * fmt, ... );

void printFile(char *fname);

int   _listPutRaw(LIST list, char *pname, char *val, int type);
pDATA CreateData(char *data, int type);
pNODE CreateNode(char *name, char *val, int type);
pNODE listSearch(LIST list, char *name);

int  ProcessLoop(LIST list, char *hbuff, long size);
void ProcessParse(LIST list, char *hbuff, long size);

// mcpanic adopted from old/cgic.h
// by icelove addition in 1998.5.20
void txt2html(char *data, int max);
void Puts(char * fmt, ... );
void hPuts(char *fmt);

/* ���� �޽����� ����Ѵ�. */
void ErrorMsg(char * fmt, ... )
{
	va_list args;
	char buff[256];

	va_start(args, fmt);
	vsprintf(buff, fmt, args);

    printf("<html><body><center>");
    printf("<font color=#ff0000 size=4>[ Error ]</font>&nbsp;&nbsp;&nbsp;&nbsp;");
    printf("<font color=#5555aa>%s</font>\n",buff);
    printf("</center></body></html>\n");

    exit(0);
}

/* ���޹��� ȭ���� ������ ȭ�鿡 �ѷ��ش�. */
void printFile(char *fname)
{
    FILE *fd;
    char readBuff[2510];
    int  readSize;

    fd = fopen(fname, "r");
    if ( fd == NULL ) { printf("Error : [%s]", fname); return; }
    while(1)
    {
        if ( fgets(readBuff, 2000, fd) == NULL ) break;
        txt2html(readBuff, 2500);
        printf("%s\n", readBuff);
    }
    fclose(fd);
}

/*
void printFile(char *fname)
{
    int fd;
    char readBuff[2048];
    int  readSize;

    fd = open(fname, O_RDONLY);
    if ( fd == -1 ) { printf("Error : [%s]", fname); return; }
    while(1)
    {
        readSize = read(fd, readBuff, 1024);
        if ( readSize <= 0 ) break;
        readBuff[readSize] = 0;
        txt2html(readBuff, 2000);
        printf("%s\n", readBuff);
    }
    close(fd);
}
*/

/* DATA��ü �ϳ��� �����Ѵ�. */
/* �����ϸ� �����͸�, �ƴϸ� NULL�� �����ش�. */
pDATA CreateData(char *data, int type)
{
    pDATA node;

    node = (pDATA)malloc(sizeof(DATA));
    if ( node == NULL ) return NULL;
    node->Next = NULL;
    node->Data = (char *)malloc(strlen(data)+1);
    if ( node->Data == NULL )
    {
        free(node);
        return NULL;
    }
    node->Type = type;
    strcpy(node->Data, data);

    return node;
}

/* data ���ڿ����� ��ĭ�� ����, buff �� ����ش�. */
void ClearSpace(char *data, char *buff)
{
    while( *data )
    {
        if ( *data != ' ' )
        {
            *buff = *data;
            buff++;
        }
        data++;
    }

    *buff = 0;
}

/* ����Ʈ�� �ϳ� �����Ѵ�. */
LIST listCreate(void)
{
    LIST list;

    list = (LIST)malloc(sizeof(struct _list));
    if ( list == NULL ) return NULL;
    list->NodeCount = 0;
    list->DataCount = 0;
    list->Node = NULL;

    return list;
}

/* ������ ����Ʈ�� ����Ÿ���� ��� �����Ѵ�. */
void listDelete(LIST list)
{
    pNODE tempNode;
    pNODE Node;
    pDATA Sll;
    pDATA tempSll;

	
    Node = list->Node;
    while(1)
    {
        if ( Node == NULL ) break;

        Sll = Node->Header;
        while(1)
        {
            if ( Sll == NULL ) break;

            tempSll = Sll->Next;
			free(Sll->Data);
            free(Sll);
            Sll = tempSll;
        }
        tempNode = Node->Next;
//        free(Node->Header);
        free(Node->Name);
        free(Node);

        Node = tempNode;
    }
    free(list);

}

/* LIST�� ����Ÿ�� �����ϴ��� ���캸��, �����ϸ� �ش� Node ��ġ�� �����ش�. */
/* �����ϸ� NULL�� �����ش�. */
pNODE listSearch(LIST list, char *name)
{
    pNODE node;

    if ( list == NULL ) return NULL;
    if ( list->Node == NULL ) return NULL;

    node = list->Node;
    while(1)
    {
        if ( node == NULL ) break;
        if ( strcmp(node->Name, name) == 0 ) return node;
        node = node->Next;
    }

    return NULL;
}

/* ��带 �ϳ� �����Ѵ�. */
pNODE CreateNode(char *name, char *val, int type)
{
    pNODE node;

    node = (pNODE)malloc(sizeof(NODE));
    if ( node == NULL ) return NULL;
    node->Name = (char *)malloc(strlen(name)+1);
    if ( node->Name == NULL )
    {
        free(node);
        return NULL;
    }
    strcpy(node->Name, name);
    node->MaxList = 0;
    node->Next = NULL;
    node->Header = CreateData(val, type);
    if ( node->Header == NULL )
    {
        free(node);
        return NULL;
    }
    node->Reader = node->Header;

    return node;
}

/* LIST�� ����Ÿ�� �ϳ� �߰� ��Ų��.  �̹� �ִٸ� �������� �����. */
/* �����ϸ� 1��, �ƴϸ� 0�� �����ش�. */
int _listPutRaw(LIST list, char *pname, char *val, int type)
{
    pNODE node, node2;
    pDATA data;
    char name[256];

    if ( list == NULL ) list = listCreate();
    if ( list == NULL ) return 0;

    ClearSpace(pname, name);
    node = listSearch(list, name);
    if ( node == NULL )
    {
        node = CreateNode(name, val, type);
        if ( node == NULL ) return 0;

        list->DataCount++;
        list->NodeCount++;
        node->MaxList++;
        if ( list->Node == NULL ) list->Node = node;
        else
        {
            node2 = list->Node;
            while(1)
            {
                if ( node2->Next == NULL ) break;
                node2 = node2->Next;
            }
            node2->Next = node;
        }
    }
    else
    {
        data = node->Header;
        while(1)
        {
            if ( data->Next == NULL ) break;
            data = data->Next;
        }

        data->Next = CreateData(val, type);
        if ( data->Next == NULL ) return 0;
        else
        {
            list->DataCount++;
            node->MaxList++;
        }
    }

    return 1;
}

/* format ������ ����Ÿ�� ���� �ִ´�.  ������ printf(); �� ����. */
/* �����ϸ� 1��, �ƴϸ� 0�� �����ش�. */
int listPutf(LIST list, char *name, char * fmt, ... )
{
	va_list args;

	va_start(args, fmt);
	vsprintf(BigBuff, fmt, args);

    return _listPutRaw(list, name, BigBuff, DATATYPESTRING);
}

int listPut(LIST list, char *pname, char *val)
{
    return _listPutRaw(list, pname, val, DATATYPESTRING);
}

int listPutFile(LIST list, char *pname, char *fname)
{
    return _listPutRaw(list, pname, fname, DATATYPEFILE);
}

int listPutfFile(LIST list, char *name, char * fmt, ... )
{
	va_list args;

	va_start(args, fmt);
	vsprintf(BigBuff, fmt, args);

    return _listPutRaw(list, name, BigBuff, DATATYPEFILE);
}

/* ����Ʈ���� �ڷḦ �ϳ� �����ش�.  ������ ��� ����ī���͸� �ϳ� �����Ѵ�. */
int listGet(LIST list, char *pname, char *buff)
{
    pNODE node;
    char name[256];
    int  type;

    if ( list == NULL ) return LGNOT;

    ClearSpace(pname, name);
    node = listSearch(list, name);

    if ( node == NULL ) return LGNOT;
    else
    {
        if ( node->Reader == NULL ) return LGNOT;
        if ( node->Reader->Data == NULL ) return LGNOT;

        strcpy(buff, node->Reader->Data);
        type = node->Reader->Type;
        if ( node->Reader->Next == NULL )
        {
            node->Reader = node->Header;
            return type==DATATYPEFILE ? LGENDLOOPFILE:LGENDLOOP;
        }
        else node->Reader = node->Reader->Next;
    }

    return type==DATATYPEFILE ? LGOKFILE:LGOK;
}

/* ����Ʈ�� ������ �Ųٷ� �ٲ۴�. */
void listReverse(LIST list, char *qName)
{
    pNODE node;
    pDATA temp;
    pDATA pretemp;
    pDATA head;
    char name[256];

    if ( list == NULL ) return;

    ClearSpace(qName, name);
    node = listSearch(list, name);

    if ( node == NULL ) return;
    head = NULL;

    while(1)
    {
        temp = node->Header;
        pretemp = NULL;
        while(1)
        {
            if ( temp->Next == NULL ) break;
            pretemp = temp;
            temp = temp->Next;
        }
        if ( pretemp != NULL ) pretemp->Next = NULL;

        if ( head == NULL )
        {
            head = temp;
            head->Next = NULL;
        }
        else
        {
            pretemp = head;
            while(1)
            {
                if ( pretemp->Next == NULL ) break;
                pretemp = pretemp->Next;
            }
            pretemp->Next = temp;
        }

        if ( temp == node->Header ) break;
    }

    node->Header = head;
    node->Reader = head;
}


/* ������ ����Ʈ�� ����Ÿ�� ������ ���Ϸ� �����Ѵ�. */
void listShowInfo(LIST list, char *fname)
{
    pDATA data;
    pNODE node;
    FILE *fd;
    long i;

    if ( list == NULL ) return;
    node = list->Node;

    fd = fopen(fname, "w+");
    if ( fd == NULL ) return;

    fprintf(fd, "Total Name: %lu\nTotal Data : %lu\n\n", list->NodeCount, list->DataCount);
    while(1)
    {
        if ( node == NULL ) break;
        fprintf(fd, "> Name : %s  [Count:%lu]\n", node->Name, node->MaxList);
        data = node->Header;

        for(i=0; i<node->MaxList; i++)
        {
            if ( data == NULL ) break;
            fprintf(fd, "\t- Value : %s\n", data->Data);
            data = data->Next;
        }

        node = node->Next;
    }
    fclose(fd);
}

/* ���۵� ����Ʈ�� ȭ������ ������. */
void listDisplay(LIST list, char *HtmlName)
{
    int fd;
    char *hbuff;
    char *readBuff;
    long FileSize;
    int  readSize;

    fd = open(HtmlName, O_RDONLY);
    if ( fd == -1 ) ErrorMsg("File Open Error : %s", HtmlName);
    FileSize = lseek(fd, 0, SEEK_END);
    hbuff = (char *)malloc(FileSize);
    if ( hbuff == NULL )
	ErrorMsg("listDisplay() : Html ���� ���۸� �������� ���߽��ϴ�.");
    lseek(fd, 0, SEEK_SET);
    readBuff = hbuff;
    while(1)
    {
        readSize = read(fd, readBuff, 1024);
        if ( readSize <= 0 ) break;
        readBuff += readSize;
    }
    close(fd);
    ProcessParse(list, hbuff, FileSize);
    free(hbuff);
}

/*
    �����м��� ó���Ѵ�.
    Html ȭ���� ��� ���ۿ� ũ�⸦ �Ѱ��ָ� �м��� �Ѵ�.
*/
void ProcessParse(LIST list, char *hbuff, long size)
{
    long i, j, LoopSize;
    char *pStartParse;
    char *pStartLoop;
    char *p;
    int  readStatus, namePos, Divider;
    char NameBuff[256];

    readStatus = RSNORMAL;

    p = hbuff;
    for(i=0; i<size; i++,p++)
    {
        switch( readStatus )
        {
        case RSNORMAL:          /* �±��� �������� Ȯ���Ѵ�. */
            if ( *p == '[' )
            {
                readStatus = RSCHECKTAG;
                pStartParse = p;
            }
            else putchar(*p);
            break;

        case RSCHECKTAG:        /* �ùٸ� �±����� üũ */
            if ( *p=='r' || *p=='R' || *p=='v' || *p=='V' )
            {
                readStatus = RSREPLACE;
                Divider = 0;
                namePos = 0;
            }
            else if ( (*p=='l' || *p=='L') && *(p+1)==']' )
            {
                readStatus = RSLOOP;
                LoopSize = 0;
                pStartLoop = p+2;
                p++;
                i++;
            }
            else
            {
                for(j=0; j<size; j++)   /* �±װ� �ùٸ��� ������ �״�� ȭ�鿡 ����Ѵ�. */
                {
                    putchar( *pStartParse );
                    if ( pStartParse == p ) break;
                    pStartParse++;
                }
                readStatus = RSNORMAL;
            }
            break;

        case RSLOOP:    /* LOOP �±��� ó�� */
            if ( *p == '[' )
            {
                if ( *(p+1)=='/' && (*(p+2)=='l' || *(p+2)=='L') && *(p+3)==']' )
                {
		    int cnt ;
                    cnt = ProcessLoop(list, pStartLoop, LoopSize);
//		    printf("Loop Count :%d ", cnt);
                    p += 3;
                    i += 3;
                    readStatus = RSNORMAL;
                }
                else LoopSize++;
            }
            else
            {
                LoopSize++;
            }
            break;

        case RSREPLACE:     /* ġȯ �±� ó�� */
            if ( Divider )
            {
                switch( *p )
                {
                case ']':
                    NameBuff[namePos] = 0;
                    switch( listGet(list, NameBuff, BigBuff) )
                    {
                    case LGNOT:
                        for(j=0; j<size; j++)
                        {
                            putchar( *pStartParse );
                            if ( pStartParse == p ) break;
                            pStartParse++;
                        }
                        break;

                    case LGOKFILE:
                    case LGENDLOOPFILE:
                        printFile(BigBuff);
                        break;

                    default:
                        printf("%s",BigBuff);
                        break;
                    }
                    readStatus = RSNORMAL;
                    break;

                case ' ':   case '\"':  case '\'':      /* ��ĭ, ', " ���� �ǳʶ� */
                    break;

                default:
                    NameBuff[namePos++] = *p;
                    if ( namePos >= 254 )   /* ���۳�ħ ���� */
                    {
                        for(j=0; j<size; j++)
                        {
                            putchar( *pStartParse );
                            if ( pStartParse == p ) break;
                            pStartParse++;
                        }
                        readStatus = RSNORMAL;
                    }
                    break;
                }
            }
            else
            {
                if ( *p==' ' || *p==',' || *p=='.' || *p==';' || *p==':' ) Divider = 1;
                else
                {
                    for(j=0; j<size; j++)
                    {
                        putchar( *pStartParse );
                        if ( pStartParse == p ) break;
                        pStartParse++;
                    }
                    readStatus = RSNORMAL;
                }
            }
            break;
        }
    }

    if ( readStatus != RSNORMAL )   /* ó������ ���� �±׸� ����Ѵ�. */
    {
        for(j=0; j<size; j++)
        {
            putchar( *pStartParse );
            if ( pStartParse == p ) break;
            pStartParse++;
        }
    }
}

/* �����±׸� ó���Ѵ�. �ִ� 10000 �������� ������ ����. */
/* ������ �� Ƚ���� �����ش�. */
int ProcessLoop(LIST list, char *hbuff, long size)
{
    long i, j;
    char *pStartParse;
    char *p;
    int  readStatus, namePos, LoopCount, rTagCounter, Divider, EndLoop;
    char NameBuff[256];

    EndLoop = 0;
    for(LoopCount=0; LoopCount<10000; LoopCount++)
    {
        readStatus = RSNORMAL;
        p = hbuff;
        rTagCounter = 0;
        for(i=0; i<size; i++,p++)
        {
            switch( readStatus )
            {
            case RSNORMAL:
                if ( *p == '[' )
                {
                    readStatus = RSCHECKTAG;
                    pStartParse = p;
                }
                else putchar( *p );
                break;

            case RSCHECKTAG:
                if ( *p=='r' || *p=='R' || *p=='v' || *p=='V' )
                {
                    readStatus = RSREPLACE;
                    Divider = 0;
                    namePos = 0;
                }
                else
                {
                    for(j=0; j<size; j++)   /* �±װ� �ùٸ��� ������ �״�� ȭ�鿡 ����Ѵ�. */
                    {
                        putchar( *pStartParse );
                        if ( pStartParse == p ) break;
                        pStartParse++;
                    }
                    readStatus = RSNORMAL;
                }
                break;

            case RSREPLACE:
                if ( Divider )
                {
                    switch( *p )
                    {
                    case ']':
                        NameBuff[namePos] = 0;
                        switch( listGet(list, NameBuff, BigBuff) )
                        {
                        case LGNOT:
                            for(j=0; j<size; j++)
                            {
                                putchar( *pStartParse );
                                if ( pStartParse == p ) break;
                                pStartParse++;
                            }
                            break;

                        case LGENDLOOP:
                            rTagCounter++;
                            printf("%s",BigBuff);
                            EndLoop = 1;
                            break;;

                        case LGOK:
                            rTagCounter++;
                            printf("%s",BigBuff);
                            break;

                        case LGENDLOOPFILE:
                            rTagCounter++;
                            printFile(BigBuff);
                            EndLoop = 1;
                            break;;

                        case LGOKFILE:
                            rTagCounter++;
                            printFile(BigBuff);
                            break;
                        }
                        readStatus = RSNORMAL;
                        break;

                    case ' ':   case '\"':  case '\'':      /* ��ĭ, ', " ���� �ǳʶ� */
                        break;

                    default:
                        NameBuff[namePos++] = *p;
                        if ( namePos >= 254 )   /* ���۳�ħ ���� */
                        {
                            for(j=0; j<size; j++)
                            {
                                putchar( *pStartParse );
                                if ( pStartParse == p ) break;
                                pStartParse++;
                            }
                            readStatus = RSNORMAL;
                        }
                        break;
                    }
                }
                else
                {
                    if ( *p==' ' || *p==',' || *p=='.' || *p==';' || *p==':' ) Divider = 1;
                    else
                    {
                        for(j=0; j<size; j++)
                        {
                            putchar( *pStartParse );
                            if ( pStartParse == p ) break;
                            pStartParse++;
                        }
                        readStatus = RSNORMAL;
                    }
                }
                break;
            }
        }
        if ( readStatus != RSNORMAL )
        {
            for(j=0; j<size; j++)
            {
                putchar( *pStartParse );
                if ( pStartParse == p ) break;
                pStartParse++;
            }
        }
        if ( rTagCounter == 0 ) break;      /* �������� V �±װ� ������ */
        if ( EndLoop ) break;
    }

    return LoopCount;
}


/***by icelove addition in 1998.5.20***********************************/

#include <stdarg.h>

/*
	���ڿ��� html ���� ���� �ֵ��� �±� �ڵ���� �Ϲ�ȭ ��Ų��.

	char buff[256] = "<font size=5>",
	txt2html(buff,256); 

	�̷��� �ϸ� buff �� ������ ���� ��ȯ�� �ȴ�.

	buff : "&#60font size=5&#62;"

	�̷��� ��ȯ�� �����ν� html ������ ������ ���ڿ���

	<font size=5>  �� ���̰� �ȴ�.

	int max;  �� ���ڿ� ������ ũ���̴�.
	��ȯ�ϴٰ� ũ�⸦ �Ѿ�� ��ȯ�� �����ϰ� �ǵ��� ����.
*/
/*
void txt2html(char *data, int max)
{
	int i, j, wpos=0, prepos=0, tpos=0;
	char wbuff[2048], prechar;

	wpos=0;
	for(i=0; i<strlen(data); i++)
	{
		prepos = wpos;
		wbuff[wpos] = 0;
		switch( data[i] )
		{
		case '&':
			strcat(wbuff, "&amp;");
			wpos += 5;
			tpos++;
			break;
		case ' ':
			if ( prechar == ' ' )
			{
				strcat(wbuff, "&nbsp;");
				wpos += 6;
			}
			else wbuff[wpos++] = ' ';
			tpos++;
			break;
		case '<':
			strcat(wbuff, "&#60;");
			wpos += 5;
			tpos++;
			break;
		case '>':
			strcat(wbuff, "&#62;");
			wpos += 5;
			tpos++;
			break;
		case '\t':
			for(j=0; j<8-(tpos%8); j++)
			{
				strcat(wbuff, "&nbsp;");
				wpos += 6;
			}
			tpos += j;
			prechar = ' ';
			break;
		case '\n':
			strcat(wbuff, "<br>");
			wpos += 4;
			tpos=0;
			break;
		default:
			prechar = data[i];
			wbuff[wpos++] = data[i];
			tpos++;
			break;
		}
		
		if ( wpos>=max-1 || wpos>=2040 )
		{
			wpos = prepos;
			break;
		}
	}
	wbuff[wpos] = 0;
	strcpy(data, wbuff);
}
*/
void txt2html(char *data, int max)
{
	int i, j, wpos=0;
	char wbuff[2048];

	wpos=0;
	for(i=0; i<strlen(data); i++)
	{
		wbuff[wpos] = 0;
		switch( data[i] )
		{
		case '&':
			strcat(wbuff, "&amp;");
			wpos += 5;
			break;
		case '<':
			strcat(wbuff, "&lt;");
			wpos += 4;
			break;
		case '>':
			strcat(wbuff, "&#62;");
			wpos += 5;
			break;
		case '\n':
			strcat(wbuff, "<br>");
			wpos += 4;
			break;
		default:
			wbuff[wpos++] = data[i];
			break;
		}
		
		if ( wpos>=max-1 || wpos>=2040 ) break;
	}
	wbuff[wpos] = 0;
	strcpy(data, wbuff);
}


/*
	fprintf(cgiOut, format) �� ��ġ�Ͽ� ������̴�.
*/
void Puts(char * fmt, ... )
{
	va_list args;
	char buff[1024];

	va_start(args, fmt);
	vsprintf(buff, fmt, args);

	fputs(buff, cgiOut);
}

/*
	html ������ �� ���ڿ��� �ִ� �״�� �����ش�.
	���ڿ� ���� �±� �ڵ尡 ������ html ���� �״�� ������ �� �ֵ��� ��ȯ�Ѵ�.
*/
void hPuts(char *fmt)
{
	char buff[2048];

	strcpy(buff, fmt);
	txt2html(buff, 2040);
	fputs(buff, cgiOut);
}

/**********end addition 1998.5.20******************************************/
