
/*  call_stack

    실제 시스템에서는 스택이 메모리에 저장되지만, 본 과제에서는 `int` 배열을 이용하여 메모리를 구현합니다.
    원래는 SFP와 Return Address에 실제 가상 메모리 주소가 들어가겠지만, 이번 과제에서는 -1로 대체합니다.

    int call_stack[]      : 실제 데이터(`int 값`) 또는 `-1` (메타데이터 구분용)을 저장하는 int 배열
    char stack_info[][]   : call_stack[]과 같은 위치(index)에 대한 설명을 저장하는 문자열 배열

    ==========================call_stack 저장 규칙==========================
    매개 변수 / 지역 변수를 push할 경우   : int 값 그대로
    Saved Frame Pointer 를 push할 경우  : call_stack에서의 index
    반환 주소값을 push할 경우       : -1
    =======================================================================


    ==========================stack_info 저장 규칙==========================
    매개 변수 / 지역 변수를 push할 경우        : 변수에 대한 설명
    Saved Frame Pointer 를 push할 경우  : 어떤 함수의 SFP인지
    반환 주소값을 push할 경우                 : "Return Address"
    ========================================================================
*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#define STACK_SIZE 50 // 최대 스택 크기

int     call_stack[STACK_SIZE];         // Call Stack을 저장하는 int 배열(stack_size 개까지의 행만 들어갈수있다)
char    stack_info[STACK_SIZE][20];     // Call Stack 요소에 대한 설명을 저장하는 문자열 배열(끝에 null문자 들어감)(다차원 포인터 배열: stack_size는 행, 20은 열)
/*stack_info는 문자열 배열이다. 즉 할당할 때 일반화가 쉽지 않다. */
/*  SP (Stack Pointer), FP (Frame Pointer)

    SP는 현재 스택의 최상단 인덱스를 가리킵니다.
    스택이 비어있을 때 SP = -1, 하나가 쌓이면 `call_stack[0]` -> SP = 0, `call_stack[1]` -> SP = 1, ...

    FP는 현재 함수의 스택 프레임 포인터입니다.
    실행 중인 함수 스택 프레임의 sfp를 가리킵니다.
*/
int SP = -1;
int FP = -1;
/*return address에 대한 포인터 변수는 계속 쓰일 것이니 전역변수로 만들자!*/
char *ret = "Return Address";
int ebp;//ebp의 위치에 관한것도 마찬가지. 
void func1(int arg1, int arg2, int arg3);
void func2(int arg1, int arg2);
void func3(int arg1);

/*
    현재 call_stack 전체를 출력합니다.
    해당 함수의 출력 결과들을 바탕으로 구현 완성도를 평가할 예정입니다.
*/
void print_stack()
{
    if (SP == -1)
    {
        printf("Stack is empty.\n");
        return;
    }

    printf("====== Current Call Stack ======\n");

    for (int i = SP; i >= 0; i--)//즉 현재 스택 최상단의 위치에서 한 칸씩 내려와 0에 이르겠다는 것!
    {
        if (call_stack[i] != -1)
            printf("%d : %s = %d", i, stack_info[i], call_stack[i]);/*여기서 문자열 출력하는% s는 stack_info를 받는다..*/
        
        else
            printf("%d : %s", i, stack_info[i]);//return address에서 call_stack은 -1로. 

        if (i == SP && i == FP)
            printf("    <=== [esp],[ebp]\n");//개행
        else if (i == SP)
            printf("    <=== [esp]\n");
        else if (i == FP)
            printf("    <=== [ebp]\n");
        else
            printf("\n");
    }
    printf("================================\n\n");
}
/*push와 pop은 LIFO 구조에 따라 맨 위의 것에 대해서만 하면 되니 그냥 SP를 기준으로 하면 된다!*/
void push(int element, char *info)//*info 변수에 문자열을 할당하자!(배열에 문자열을 쉽게 넣으려면 포인터 말고는 방법 없다)
{
    call_stack[SP+1] = element;//한 칸 위로 올린 후 집어넣는다.
    strcpy(stack_info[SP+1], info);//info가 가리키는 문자열을 복사하여 stack_info 로
    SP++;
}//최대 20칸 할당되며, info에 문자열을 넣으면(20자를 넘어가지 않는다는 전제하에) 바로 배열의 특정 행을 초기화 가능!
void pop(int array1[], char array2[][20])
{
    array1[SP] = array1[-1];
    array2[SP][20] = array2[-1][20];
    SP--;
}
void push_sfp(int element, char *info1,int i)//지역변수를 push할 때 사용할 함수. //int i를 추가로 정의한 이유: SP에서 몇 칸 아래로 가야 하는가?

    {
        call_stack[SP-i] = element;
        strcpy(stack_info[SP-i], info1);
    }




//func 내부는 자유롭게 추가해도 괜찮으나, 아래의 구조를 바꾸지는 마세요
void func1(int arg1, int arg2, int arg3)
{
    int var_1 = 100;
    char* first = "arg1", * second = "arg2", * third = "arg3", * var = "var_1", * SFP = "func1 SFP";
    push(arg3, third);
    push(arg2, second);
    push(arg1, first);
    push(-1, ret);//-1은 저장되나 출력되지 않는다!
    push(-1, SFP);//1칸 올리고, 빈 칸에 SFP Push. //-1은 기존함수 FP가 main에 있어 보이지 않음을 뜻한다. 
    FP = SP;//FP가 SP의 현재값(계속 증가하고 있었음)을 가져와 대입한 것. 즉 둘의 위치는 모두 스택 최상단이다!
    ebp = FP;//현재 FP 위치는 ebp에 저장하고, 이것이 SFP다. 
    SP++;//넣을 변수는 하나뿐...
    push_sfp(var_1, var, 0);
    /*func1의 스택 프레임 형성 (함수 프롤로그 + push)*/
    print_stack();
    func2(11, 13);
    // func2의 스택 프레임 제거 (함수 에필로그 + pop)
    SP--;
    ebp = call_stack[FP];//SFP 제거되기 전 이전함수 FP를 저장해야 함. 
    pop(call_stack, stack_info);
    FP = ebp;
    pop(call_stack, stack_info);
    SP-=2;
    print_stack();
}


void func2(int arg1, int arg2)
{ // func2의 스택 프레임 형성 (함수 프롤로그 + push)
    int var_2 = 200;
    char* first = "arg1", * second = "arg2", * var = "var_2", * SFP = "func2 SFP";
    push(arg2, second);
    push(arg1, first);
    push(-1, ret);
    push(ebp, SFP);
    FP = SP;
    ebp = FP;
    SP++;
    push_sfp(var_2, var, 0);
   
    print_stack();
    func3(77);
    // func3의 스택 프레임 제거 (함수 에필로그 + pop)
     
    SP-=2;
    print_stack();
    ebp = call_stack[FP];//SFP 제거되기 전 이전함수 FP 저장.
    pop(call_stack, stack_info);
    FP = ebp;
    print_stack();
    pop(call_stack, stack_info);
    SP--;
    
    
    print_stack();
}


void func3(int arg1)
{
    int var_3 = 300;
    int var_4 = 400;
    char* first = "arg1", * SFP = "func3 SFP", * var1 = "var_3", * var2 = "var_4";
    push(arg1, first);
    push(-1, ret);
    push(ebp, SFP);
    FP = SP;
    SP += 2;//칸 확보 후 위로 올라감. 
    push_sfp(var_3, var1,0);
    push_sfp(var_4, var2,1);
    // func3의 스택 프레임 형성 (함수 프롤로그 + push)
    print_stack();
}


//main 함수에 관련된 stack frame은 구현하지 않아도 됩니다.
int main()
{
    func1(1, 2, 3);
    // func1의 스택 프레임 제거 (함수 에필로그 + pop)
    
    SP--;
    ebp = call_stack[FP];
    pop(call_stack, stack_info);
    FP = ebp;
    pop(call_stack, stack_info);
    SP-=3;//arg들 제거.원소가 아무것도 없을 때 SP가 -1이 되니까...!
    
    print_stack();
    return 0;
}
