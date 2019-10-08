//
// Created by 23731 on 2019/10/8.
//


#include "regExpress.h"

typedef struct _sourceStack{
    char singleChar;
    int type;
    int location;
    struct _sourceStack * front_P;
} sourceStack;

//typedef struct _fitStack{
//    char fitChar;
//    int type;
//    int location;
//    struct  _fitStack * front_P;
//} fitStack;

bool isMatch(char * s, char * p)
{
    sourceStack * pStack_top = NULL;  //正则匹配串存储栈
    sourceStack * fitStack_top = NULL;   //已适配字符串存储栈
    sourceStack * s_stack_cache = NULL;   //辅助指针

    const int p_length = strlen(p);
    const int s_length = strlen(s);

    int while_var= p_length -1;

    while(while_var>=0) {   //正则字符处理
        if(p[while_var] == '*') {
            if(p[while_var - 1] == '*') {   //去掉 **
                while_var --;
                continue;
            }
            if(pStack_top && (pStack_top->singleChar == p[while_var-1]) && (pStack_top->type == 0) )  {  //去掉c*c*
                while_var -= 2;
                continue;
            } else {
                s_stack_cache = (sourceStack *)malloc(sizeof(sourceStack));
                s_stack_cache->front_P = pStack_top;
                s_stack_cache->singleChar = p[--while_var];
                s_stack_cache->type = 0;
                pStack_top = s_stack_cache;   //正常表达式，压入栈
                while_var--;
            }
        } else {
            if(pStack_top && (pStack_top->singleChar == p[while_var]) && (pStack_top->type) ) {
                pStack_top->type = pStack_top->type +1;
                while_var--;
            } else {
                s_stack_cache = (sourceStack *)malloc(sizeof(sourceStack));
                s_stack_cache->front_P = pStack_top;
                s_stack_cache->singleChar = p[while_var--];
                s_stack_cache->type = 1;
                pStack_top = s_stack_cache;   //正常表达式，压入栈
            }
        }
    }

//    while (pStack_top) {  //栈内容显示
//        printf("singalChar: %c ,type: %d \r",pStack_top->singleChar,pStack_top->type);
//        pStack_top=pStack_top->front_P;
//    }

    int loc_fit = -1;  //指示母字符串已匹配位置
    while(loc_fit < s_length-1) {  //若没匹配完即循环

        if(pStack_top && pStack_top->type == 0) {    //带*数据先都按0个匹配
            pStack_top->location = loc_fit;

            s_stack_cache = pStack_top->front_P;   //匹配数据转移进匹配栈
            pStack_top->front_P = fitStack_top;
            fitStack_top = pStack_top;
            pStack_top = s_stack_cache;

            continue;
        }

        if(pStack_top && pStack_top->singleChar == '.') {   //.号直接跳过相应个数
            loc_fit += pStack_top->type;
            pStack_top->location = loc_fit;

            s_stack_cache = pStack_top->front_P;   //匹配数据转移进匹配栈
            pStack_top->front_P = fitStack_top;
            fitStack_top = pStack_top;
            pStack_top = s_stack_cache;

            continue;
        }

        if(pStack_top && pStack_top->singleChar == s[loc_fit +1]) {
            while_var = pStack_top->type;
            while( (while_var+loc_fit)<s_length && s[loc_fit+while_var] == pStack_top->singleChar && --while_var);
            if(!while_var) {   //如果匹配该字符相应长度
                loc_fit += pStack_top->type;
                pStack_top->location = loc_fit;

                s_stack_cache = pStack_top->front_P;   //匹配数据转移进匹配栈
                pStack_top->front_P = fitStack_top;
                fitStack_top = pStack_top;
                pStack_top = s_stack_cache;

                continue;
            }
        }

        //进行至此，即为当前正则栈内的数据无法匹配，需要退回已匹配栈的内容
        if(!fitStack_top) return false;  //匹配栈内无东西退回，即一定不匹配

        if(fitStack_top->type == 0) { //如果是任意个数
            if(fitStack_top->singleChar == '.' || (fitStack_top->singleChar == s[loc_fit +1]) ) { //如果是可以匹配任意字符的.号
                loc_fit ++;
                fitStack_top->location = loc_fit;   //直接跳过一个字符后继续匹配
                continue;
            }

            //退回正则栈
            s_stack_cache = fitStack_top;
            fitStack_top  = fitStack_top->front_P;
            s_stack_cache->front_P = pStack_top;
            pStack_top = s_stack_cache;

            //匹配位置回退
            loc_fit = fitStack_top->location;
        }

        while (fitStack_top && !fitStack_top->type) { //一直退到最近的一个匹配任意个字符的匹配栈
            //退回正则栈
            s_stack_cache = fitStack_top;
            fitStack_top  = fitStack_top->front_P;
            s_stack_cache->front_P = pStack_top;
            pStack_top = s_stack_cache;
        }
//
//        if(fitStack_top->singleChar == '.' || (fitStack_top->singleChar == s[loc_fit +1]) ) { //如果是可以匹配任意字符的.号
//            loc_fit ++;
//            fitStack_top->location = loc_fit;   //直接跳过一个字符后继续匹配
//            continue;
//        }

        //退回正则栈
        s_stack_cache = fitStack_top;
        fitStack_top  = fitStack_top->front_P;
        s_stack_cache->front_P = pStack_top;
        pStack_top = s_stack_cache;
        //匹配位置回退
        if(fitStack_top) loc_fit = fitStack_top->location;
        else return false;   //退无可退，不匹配
        continue;


    }

    if(loc_fit >= s_length-1 && !pStack_top)   //全匹配条件
        return true;
    else
        return false;
}
