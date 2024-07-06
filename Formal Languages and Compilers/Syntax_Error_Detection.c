#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define N 1000000

typedef struct table table;

struct table
{
    int id;
    char name[100], iT[100], dT[100], sc[100], val[100];
} tabVal[100];

char check[100], cur[100], dType[100][50], scope[100][50];
int cnt, iType, topD, topS, elseCnt, lineNum;

void step1();
void step2();
void step3();
void step4();

char* subString(char* str, int left, int right);
bool isKeyword(char* str);
bool isIdentifier(char* str, int len);
bool isSeparator(char ch);
bool isSingOprtr(char ch);
bool isMultOprtr(char* str);
bool isBinary(char* str, int len);
bool isDecimal(char* str, int len);
bool isOctal(char* str, int len);
bool isHexadecimal(char* str, int len);


void stepOne();
void stepTwo();

void tabInsert();
void tabSearch();
void scopeVal();

int main(void)
{
    step1();
    step2();
    step3();
    step4();

    return 0;
}

void step1()
{
    FILE *codeFile, *extractFile, *filterFile;
    char str[200];
    int i, j, l, cnt = 0, stat = 0;

    codeFile = fopen("input.c", "r");
    extractFile = fopen("step1.txt", "w");

    if(!codeFile)
        printf("No File Available.\n");

    else
    {
        while(fgets(str, N, codeFile))
        {
            //fprintf(extractFile, "%d ", ++cnt);
            l = strlen(str);
            i = 0;

            while(i < l)
            {
                if(str[i] != ' ')
                    break;
                ++i;
            }

            for(; i < l; ++i)
            {
                if(stat == 0 && str[i] == '/')
                {
                    if(i < l-1 && str[i + 1] == '/')
                    {
                        for(j = i + 1; j < l; ++j);
                        break;
                    }
                    else if(i < l-1 && str[i + 1] == '*')
                    {
                        stat = 1;
                        ++i;
                        continue;
                    }
                }

                if(stat == 1)
                {
                    if(str[i] == '*' && str[i + 1] == '/')
                    {
                        stat = 0;
                        ++i;
                        continue;
                    }
                }
                else
                {
                    if(str[i] == ' ')
                    {
                        for(j = i + 1; j < l; ++j)
                        {
                            if(str[j] != ' ')
                                break;
                        }
                        if(str[j] == '/')
                        {
                            if(str[j + 1] == '/')
                            {
                                ++j;
                                while(j < l)
                                    ++j;
                                break;
                            }
                            else if(str[j + 1] == '*')
                            {
                                stat = 1;
                                ++i;
                                continue;
                            }
                        }

                        i = j;
                        fputc(' ', extractFile);
                        fputc(str[i], extractFile);
                        continue;
                    }
                    if(str[i] != '\n')
                        fputc(str[i], extractFile);
                }
            }
            fputc('\n', extractFile);
        }
    }

    fclose(codeFile);
    fclose(extractFile);
}

void step2()
{
    FILE *inFile, *outFile;
    int len, left, right;
    char str[N];
    int i, j, l, stat = 0;

    elseCnt = -1;

    inFile = fopen("step1.txt", "r");
    outFile = fopen("step2.txt", "w");

    if(!inFile)
        printf("No File Available.\n");

    else
    {
        lineNum = 0;
        while(fgets(str, N-1, inFile))
        {
            ++lineNum;
            len = strlen(str);

            if(strcmp(str, "\n") == 0)
            {
                fprintf(outFile, "\n");
                continue;
            }

            left = 0;
            right = 0;
            while (right <= len && left <= right)
            {
                if(!isSeparator(str[right]) && !isSingOprtr(str[right]))
                    right++;
                if((isSeparator(str[right]) || isSingOprtr(str[right])) && left == right)
                {
                    if (isSeparator(str[right]) == true)
                        fprintf(outFile, "[sep %c] ", str[right]);
                    else
                    {
                        if(isSingOprtr(str[right + 1]))
                        {
                            ++right;
                            char* subStr = subString(str, left, right);
                            if(isMultOprtr(subStr))
                                fprintf(outFile, "[op %s] ", subStr);
                            else
                                fprintf(outFile, "[unkn %s] ", subStr);
                        }
                        else
                            fprintf(outFile, "[op %c] ", str[right]);
                    }
                    ++right;
                    left = right;
                }
                else if(isSeparator(str[right]) || isSingOprtr(str[right]) || (right == len && left != right))
                {
                    char* subStr = subString(str, left, right - 1);
                    if (isKeyword(subStr))
                        fprintf(outFile, "[kw %s] ", subStr);

                    else if (isBinary(subStr, right - left) || isOctal(subStr, right - left) ||
                             isHexadecimal(subStr, right - left) || isDecimal(subStr, right - left))
                        fprintf(outFile, "[number %s] ", subStr);

                    else if (isIdentifier(subStr, right - left) == true && isSeparator(str[right - 1]) == false)
                        fprintf(outFile, "[id %s] ", subStr);

                    else
                        fprintf(outFile, "[unkn %s] ", subStr);
                    left = right;
                }
                if(str[right] == '\n')
                {
                    fprintf(outFile, "\n");
                    break;
                }
            }
        }
    }

    fclose(inFile);
    fclose(outFile);
}

void step3()
{
    topD = topS = -1;
    stepOne();
    stepTwo();

}

void step4()
{
    int brcktCnt[3], i, j, len;

    brcktCnt[0] = brcktCnt[1] = brcktCnt[2] = - 1;
    lineNum = 0;

    FILE *inFile, *dupFile;
    char ch[100], cur[100], prev[100], comm[100];

    inFile = fopen("input.c", "r");
    dupFile = fopen("step2.txt", "r");

    if(!inFile)
        printf("File Not Found.\n");
    else
    {
        // Bracket Balance
        while(fgets(ch, N, inFile))
        {
            ++lineNum;
            len = strlen(ch);

            for(i = 0; i < len; ++i)
            {
                if (ch[i] == '[')
                    ++brcktCnt[0];
                else if(ch[i] == '{')
                    ++brcktCnt[1];
                else if(ch[i] == '(')
                    ++brcktCnt[2];
                else if(ch[i] == ')')
                    --brcktCnt[2];
                else if(ch[i] == '}')
                    --brcktCnt[1];
                else if(ch[i] == ']')
                    --brcktCnt[0];

                if(brcktCnt[0] < -1)
                {
                    brcktCnt[0] = -1;
                    printf("Unbalanced ']' at line %d\n", lineNum);
                }
                if(brcktCnt[1] < -1)
                {
                    brcktCnt[1] = -1;
                    printf("Unbalanced '}' at line %d\n", lineNum);
                }
                if(brcktCnt[2] < -1)
                {
                    brcktCnt[3] = -1;
                    printf("Unbalanced ')' at line %d\n", lineNum);
                }
            }
        }

        if(brcktCnt[0] != -1)
            printf("Unbalanced ']' at line %d\n", lineNum);
        if(brcktCnt[1] != -1)
            printf("Unbalanced '}' at line %d\n", lineNum);
        if(brcktCnt[2] != -1)
            printf("Unbalanced ')' at line %d\n", lineNum);

        // Duplicate detect
        lineNum = 0;
        strcpy(prev, "");
        //strcpy(ch, "");
        /*
        char c;
        while(c = fgetc(dupFile))
            printf("%c", c);
        */

        while(fgets(ch, N, dupFile))
        {
            ++lineNum;
            if(!strcmp(ch, "\n"))
                continue;
            len = strlen(ch);

            for(i = 0; i < len;)
            {
                while(ch[i] != '[' && i < len)
                    ++i;
                if(i == len)
                    break;
                ++i;
                j = 0;
                while(ch[i] != ' ' && i < len)
                {
                    cur[j] = ch[i];
                    ++i;
                    ++j;
                }
                cur[j] = '\0';
                if(i == len)
                    break;
                ++i;
                j = 0;
                while(ch[i] != ']' && i < len)
                {
                    comm[j] = ch[i];
                    ++i;
                    ++j;
                }
                comm[j] = '\0';
                if(i == len)
                    break;

                if(!strcmp(comm, "for"))
                {
                    ++i;
                    char tmp[100];
                    while(ch[i] != ']')
                        ++i;
                    if(ch[i-1] != '(')
                        printf("Expected '(' at line %d\n", lineNum);
                    else
                    {
                        while(1)
                        {
                            while(ch[i] != '[' && i < len)
                                ++i;
                            if(i == len)
                                break;
                            ++i;
                            j = 0;
                            while(ch[i] != ' ' && i < len)
                            {
                                tmp[j] = ch[i];
                                ++i;
                                ++j;
                            }
                            tmp[j] = '\0';
                            if(strcmp(tmp, "sep"))
                            {
                                while(ch[i] != ']' && i < len)
                                    ++i;
                                continue;
                            }
                            ++i;
                            j = 0;
                            while(ch[i] != ']' && i < len)
                            {
                                tmp[j] = ch[i];
                                ++i;
                                ++j;
                            }
                            tmp[j] = '\0';
                            if(!strcmp(tmp, ";"))
                                break;
                            else
                                continue;
                        }
                        if(strcmp(tmp, ";"))
                            printf("Expected ';' at line %d.\n", lineNum);
                        else
                        {
                            while(1)
                            {
                                while(ch[i] != '[' && i < len)
                                    ++i;
                                if(i == len)
                                    break;
                                ++i;
                                j = 0;
                                while(ch[i] != ' ' && i < len)
                                {
                                    tmp[j] = ch[i];
                                    ++i;
                                    ++j;
                                }
                                tmp[j] = '\0';
                                if(strcmp(tmp, "sep"))
                                {
                                    while(ch[i] != ']' && i < len)
                                        ++i;
                                    continue;
                                }
                                ++i;
                                j = 0;
                                while(ch[i] != ']' && i < len)
                                {
                                    tmp[j] = ch[i];
                                    ++i;
                                    ++j;
                                }
                                tmp[j] = '\0';
                                if(!strcmp(tmp, ";"))
                                    break;
                                else
                                    continue;
                            }
                            if(strcmp(tmp, ";"))
                                printf("Expected ';' at line %d.\n", lineNum);
                            else
                            {
                                ++i;
                                while(ch[i] != ']')
                                    ++i;
                                if(ch[i-1] != ')')
                                    printf("Expected ')' at line %d\n", lineNum);
                            }
                        }
                    }
                }
                else if(!strcmp(cur, prev) && strcmp(comm, "(") && strcmp(comm, "{") && strcmp(comm, ")") && strcmp(comm, "}"))
                    printf("Duplicate token at line %d\n", lineNum);
                strcpy(prev, cur);
            }
        }

    }
    fclose(inFile);
    fclose(dupFile);
}

char* subString(char* str, int left, int right)
{
    int i;
    char* subStr = (char*)malloc(sizeof(char) * (right - left + 2));

    for (i = left; i <= right; i++)
        subStr[i - left] = str[i];
    subStr[right - left + 1] = '\0';
    return (subStr);
}

bool isKeyword(char* str)
{
    if (!strcmp(str, "if"))
        ++elseCnt;
    if (!strcmp(str, "else"))
        --elseCnt;
    if(elseCnt < -1)
    {
        printf("'else' without a previous 'if' at line %d\n", lineNum);
        elseCnt = -1;
    }
    if (!strcmp(str, "void") || !strcmp(str, "int") || !strcmp(str, "float") || !strcmp(str, "char") ||
            !strcmp(str, "for") || !strcmp(str, "if") || !strcmp(str, "else") || !strcmp(str, "while") ||
            !strcmp(str, "return") || !strcmp(str, "main") || !strcmp(str, "double"))
        return true;
    return false;
}

bool isIdentifier(char* str, int len)
{
    int i;

    if(!isalpha(str[0]) &&  str[0] != '_')
        return false;
    for(i = 1; i < len; ++i)
        if(!isalnum(str[i]) && !(str[i] == '_'))
            return false;
    return true;
}

bool isSeparator(char ch)
{
    if (ch == ' ' || ch == ',' || ch == ';' || ch == '(' ||
            ch == ')' || ch == '[' || ch == ']' || ch == '{' ||  ch == '}')
        return true;
    return false;
}

bool isSingOprtr(char ch)
{
    if (ch == '+' || ch == '-' || ch == '/' || ch == '*' || ch == '%' ||
            ch == '=' || ch == '>' || ch == '!' || ch == '<')
        return true;
    return false;
}

bool isMultOprtr(char* str)
{
    if (!strcmp(str, "==") || !strcmp(str, "++") || !strcmp(str, "--") || !strcmp(str, "+=") ||
            !strcmp(str, "-=") || !strcmp(str, "*=") || !strcmp(str, "/=") || !strcmp(str, "%=") ||
            !strcmp(str, "<=") || !strcmp(str, ">=") || !strcmp(str, "!=") || !strcmp(str, "&&") ||
            !strcmp(str, "||"))
        return true;
    return false;
}

bool isBinary(char* str, int len)
{
    int i;

    if(len < 3 || str[0] != '0' || str[1] != 'b')
        return false;

    for(i = 2; i < len; ++i)
        if(str[i] < '0' || str[i] > '1')
            break;

    if(i == len)
        return true;

    if(str[i] != '.')
        return false;

    ++i;
    while(i < len)
    {
        if(str[i] < '0' || str[i] > '1')
            return false;
        ++i;
    }

    return true;
}

bool isDecimal(char* str, int len)
{
    int i;

    for(i = 0; i < len; ++i)
        if(!isdigit(str[i]))
            break;
    if(i == len)
        return true;

    if(str[i] != '.')
        return false;

    ++i;
    while(i < len)
    {
        if(!isdigit(str[i]))
            return false;
        ++i;
    }

    return true;
}

bool isOctal(char* str, int len)
{
    int i;

    if(len < 2 || str[0] != '0')
        return false;

    for(i = 1; i < len; ++i)
        if(str[i] < '0' || str[i] > '7')
            break;

    if(i == len)
        return true;

    if(str[i] != '.')
        return false;

    ++i;
    while(i < len)
    {
        if(str[i] < '0' || str[i] > '7')
            return false;
        ++i;
    }

    return true;
}

bool isHexadecimal(char* str, int len)
{
    int i;

    if(len < 3 || str[0] != '0' || (str[1] != 'x' && str[1] != 'X'))
        return false;

    for(i = 2; i < len; ++i)
        if(!isdigit(str[i]) && (str[i] < 'A' || str[i] > 'F'))
            break;

    if(i == len)
        return true;

    if(str[i] != '.')
        return false;

    ++i;
    while(i < len)
    {
        if(!isdigit(str[i]) && !(str[i] >= 'A' && str[i] <= 'F'))
            return false;
        ++i;
    }

    return true;
}

void stepOne()
{
    FILE *inFile, *oneFile;
    int i, len, left = 0, right = 0;
    char str[N];

    inFile = fopen("step2.txt", "r");
    oneFile = fopen("stepOne.txt", "w");

    if(!inFile)
        printf("No File Available.\n");

    else
    {
        lineNum = 0;
        while(fgets(str, N-1, inFile))
        {
            ++lineNum;
            if(strcmp(str, "\n") == 0)
            {
                fprintf(oneFile, "\n");
                continue;
            }
            len = strlen(str);
            for(i = 0; i < len; ++i)
            {
                if(ferror(inFile) || feof(inFile))
                    break;
                if(str[i] == '[')
                {
                    fprintf(oneFile, "[");
                    ++i;
                    if(str[i] == 'i')
                    {
                        while(str[i] != ']')
                        {
                            fprintf(oneFile, "%c", str[i]);
                            ++i;
                        }
                        fprintf(oneFile, "]");
                    }
                    else
                    {
                        if(str[i] == 'u')
                        {
                            printf("Unknown Value Found at line %d.\n", lineNum);
                            while(str[i] != ']')
                                ++i;
                        }
                        else
                        {
                            while(str[i] != ' ')
                                ++i;
                            ++i;
                            while(str[i] != ']')
                            {
                                fprintf(oneFile, "%c", str[i]);
                                ++i;
                            }
                            fprintf(oneFile, "]");
                        }
                    }
                }
            }
            fprintf(oneFile, "\n");
        }
    }

    fclose(inFile);
    fclose(oneFile);
}

void stepTwo()
{
    FILE *oneFile, *outFile;
    int i, j, len;
    char str[N];

    oneFile = fopen("stepOne.txt", "r");
    outFile = fopen("output.txt", "w");

    if(!oneFile)
        printf("No File Available.\n");

    else
    {
        lineNum = 0;
        while(fgets(str, N-1, oneFile))
        {
            ++lineNum;
            if(strcmp(str, "\n") == 0)
            {
                //fprintf(oneFile, "\n");
                continue;
            }
            len = strlen(str);
            for(i = 0; i < len; ++i)
            {

                if(str[i] == '[')
                {
                    fprintf(outFile, "[");
                    ++i;
                    if(str[i] == 'i' && str[i+1] == 'd')
                    {
                        fprintf(outFile, "id ");
                        i += 2;
                        j = 0;
                        while(str[i] != ']')
                        {
                            check[j] = str[i];
                            ++i;
                            ++j;
                        }
                        check[j] = '\0';

                        if(str[i+2] == '(')
                            iType = 1;
                        else
                            iType = 0;
                        tabSearch();
                        int x;
                        for(x = cnt; x >= 0; --x)
                            if(strcmp(tabVal[x].name, check) == 0)
                                break;
                        fprintf(outFile, "%d]", x+1);
                        if(str[i+2] == ';')
                            --topD;
                        else if (str[i+2] == ')' || str[i+2] == '}')
                            topD = -1;
                        if(str[i+2] == '=')
                        {
                            fprintf(outFile, "[=][");
                            i += 5;
                            j = 0;
                            int k, l, m;
                            for(m = cnt; m >= 0; --m)
                                if(strcmp(tabVal[m].name, check) == 0)
                                    break;
                            while(str[i] != ']')
                            {
                                check[j] = str[i];
                                ++i;
                                ++j;
                            }
                            check[j] = '\0';
                            l = strlen(check);

                            for(k = 0; k < l; ++k)
                                if(!isdigit(check[k]))
                                    break;
                            if(k == l)
                            {
                                fprintf(outFile, "%s]", check);
                                strcpy(tabVal[m].val, check);
                            }
                            else if(check[k] == '.')
                            {
                                ++k;
                                for(; k < l; ++k)
                                    if(!isdigit(check[k]))
                                        break;
                                if(k == l)
                                {
                                    fprintf(outFile, "%s]", check);
                                    strcpy(tabVal[m].val, check);
                                }
                            }
                            else if(k == 0)
                            {
                                cur[0] = ' ';
                                for(m = 3; m < l; ++m)
                                    cur[m-2] = check[m];
                                cur[m] = '\0';
                                for(m = cnt-1; m >= 0; --m)
                                {
                                    if(strcmp(tabVal[m].name, cur) == 0 && strcmp(tabVal[m].iT, "func") == 0 && str[i+2] == '(')
                                    {
                                        fprintf(outFile, "id %d]", tabVal[m].id);
                                        break;
                                    }
                                    else if(strcmp(tabVal[m].name, cur) == 0 && strcmp(tabVal[m].iT, "var") == 0 && str[i+2] != '(')
                                    {
                                        fprintf(outFile, "id %d]", tabVal[m].id);
                                        break;
                                    }
                                }
                            }

                        }
                    }
                    else
                    {
                        j = 0;
                        while(str[i] != ']')
                        {
                            check[j] = str[i];
                            ++i;
                            ++j;
                        }
                        check[j] = '\0';
                        fprintf(outFile, "%s]", check);

                        if(strcmp(check, "int") == 0 || strcmp(check, "float") == 0 || strcmp(check, "double") == 0 || strcmp(check, "char") == 0)
                        {
                            ++topD;
                            strcpy(dType[topD], check);
                        }
                        if(strcmp(check, "}") == 0)
                            --topS;
                    }
                }
            }
        }
    }
    fclose(oneFile);
    fclose(outFile);
}

void tabInsert()
{
    tabVal[cnt].id = cnt + 1;
    strcpy(tabVal[cnt].name, cur);
    strcpy(tabVal[cnt].dT, dType[topD]);
    if(topS == -1)
        strcpy(tabVal[cnt].sc, "global");
    else
        strcpy(tabVal[cnt].sc, scope[topS]);

    if(iType == 1)
    {
        strcpy(tabVal[cnt].iT, "func");
        scopeVal();
    }
    else
        strcpy(tabVal[cnt].iT, "var");
    strcpy(tabVal[cnt].val, "NULL");

    ++cnt;
}

void tabSearch()
{
    int i;

    strcpy(cur, check);

    for(i = 0; i < cnt; ++i)
    {
        if(strcmp(check, tabVal[i].name) == 0 && strcmp(tabVal[i].sc, scope[topS]) == 0)
        {
            if(topD != -1)
                printf("Duplicate Identifier Declaration at line %d.\n", lineNum);
            break;
        }
        else if(strcmp(check, tabVal[i].name) == 0 && strcmp(tabVal[i].sc, "global") == 0)
            break;
        else if(strcmp(check, tabVal[i].name) == 0 && iType == 1 && strcmp(tabVal[i].iT, "func") == 0)
            break;
    }
    if(i == cnt)
    {
        if(topD == -1)
            printf("Undefined Identifier at line %d.\n", lineNum);
        else
            tabInsert();
    }
}

void scopeVal()
{
    int i;

    for(i = cnt; i >= 0; --i)
        if(strcmp(tabVal[i].iT, "func") == 0)
            break;

    if(i >= 0)
    {
        ++topS;
        strcpy(scope[topS], tabVal[i].name);
    }
}
