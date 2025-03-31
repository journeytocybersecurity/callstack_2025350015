
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

/*  SP (Stack Pointer), FP (Frame Pointer)

    SP는 현재 스택의 최상단 인덱스를 가리킵니다.
    스택이 비어있을 때 SP = -1, 하나가 쌓이면 `call_stack[0]` -> SP = 0, `call_stack[1]` -> SP = 1, ...

    FP는 현재 함수의 스택 프레임 포인터입니다.
    실행 중인 함수 스택 프레임의 sfp를 가리킵니다.
*/
int SP = -1;
int FP = -1;//전역변수!
/*return address에 대한 포인터 변수는 계속 쓰일 것이니 전역변수로 만들자!*/
char* ret = "Return Address";
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

    printf("====== Current Call Stack ======\n");//이런 함수는 어떻게 정의하는거야? 

    for (int i = SP; i >= 0; i--)//즉 현재 스택 최상단의 위치에서 한 칸씩 내려와 0에 이르겠다는 것!
    {
        if (call_stack[i] != -1)//Return address가 아니면 이렇게 하겠다!
            printf("%d : %s = %d", i, stack_info[i], call_stack[i]);/*개행문자가 없으니 뒤에 바로 [esp]가 나올수있다! 여기서 문자열 출력하는 %s는 stack_info를 받는다..*/
        /*stack_info는 문자열 배열이다. 즉 할당할 때 일반화가 쉽지 않다. */
        else
            printf("%d : %s", i, stack_info[i]);//return address가 나타났다!. 하지만 이 경우 call_stack을 출력하진 않으므로 call_stack[i]는 그냥 -1로 하자. 

        if (i == SP && i == FP)//매우 중요한 과정.
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
void push(int element, char* info)//*info 변수에 문자열을 할당하자!(배열에 문자열을 쉽게 넣으려면 포인터 말고는 방법 없다)
{
    call_stack[SP+1] = element;//한 칸을 확보 후 집어넣는다?!
    strcpy(stack_info[SP+1], info);//info가 가리키는 문자열을 복사하여 stack_info 로
    SP++;
}//최대 20칸 할당되며, info에 문자열을 넣으면(20자를 넘어가지 않는다는 전제하에) 바로 배열의 특정 행을 초기화 가능!
//stack_info는 포인터 배열이 아니니, *stack_info[SP][20]도 말이안된다. stcrpy 만이 답인가? 
/*char이라 그냥 대놓고 문자열을 꽂을 순 없으니 포인터 변수 사용*/
void pop(int array1[], char array2[][20])//행 전체를 제거할 것이니 array 뒤에 []가 하나만 있어도 됨. (하지만 2개 있었다면요?)
{
    array1[SP] = array1[SP + 1];//call_stack과 stack_info 에서 각각 한 행을 지워버리고 공백문자 투입. 
    array2[SP][20] = array2[SP + 1][20];//이 배열을 사용할 때도 20까지 써줘야함
    SP--;
}


//func 내부는 자유롭게 추가해도 괜찮으나, 아래의 구조를 바꾸지는 마세요
void func1(int arg1, int arg2, int arg3)
{
    int var_1 = 100, i;
    char* first = "arg1", * second = "arg2", * third = "arg3", * var = "var_1", * SFP = "func1 SFP";
    push(arg3, third);
    push(arg2, second);
    push(arg1, first);
    push(-1, ret);//-1은 저장되나 출력되지 않는다!
    strcpy(stack_info[SP+1], SFP);
    SP++;
    FP = SP;//단순히 둘이 같다는 것이 아닌 FP가 SP의 현재값(계속 증가하고 있었음)을 가져와 대입한 것. 즉 둘의 위치는 스택 최상단이다!
    ebp = FP;
    SP++;//넣을 변수는 하나뿐...
    push(var_1, var);
    /*func1의 스택 프레임 형성 (함수 프롤로그 + push)*/
    print_stack();
    func2(11, 13);
    // func2의 스택 프레임 제거 (함수 에필로그 + pop)
    SP--;
    ebp = FP;
    pop(call_stack, stack_info);
    pop(call_stack, stack_info);
    SP--;
    print_stack();
}


void func2(int arg1, int arg2)
{
    int var_2 = 200;
    char* first = "arg1", * second = "arg2", * var = "var_2", * SFP = "func2 SFP";
    push(arg2, second);
    push(arg1, first);
    push(-1, ret);
    push(ebp, SFP);
    FP = SP;
    ebp = FP;
    SP++;
    push(var_2, var);
    // func2의 스택 프레임 형성 (함수 프롤로그 + push)
    print_stack();
    func3(77);
    SP--;
    pop(call_stack, stack_info);
    pop(call_stack, stack_info);
    SP--;
    // func3의 스택 프레임 제거 (함수 에필로그 + pop)
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
    push(var_3, var1);
    push(var_4, var2);
    // func3의 스택 프레임 형성 (함수 프롤로그 + push)
    print_stack();
}


//main 함수에 관련된 stack frame은 구현하지 않아도 됩니다.
int main()
{
    func1(1, 2, 3);
    SP--;//정의되지 않게 할 변수 역시 하나뿐.실행후 위치는 SFP 옆. 
    /*ebp = FP;//FP의 현재 위치 기록해 ebp에 저장. 다음함수 호출에서 사용 예정.*/  
    pop(call_stack, stack_info); FP = -1;//FP를 치워둔다. 지금 ebp는 이미 main함수로 내려갔을 것이니..
    pop(call_stack, stack_info);//실제로는 return address 역시 main 함수의 것을 저장하기 때문에, 함부로 SP를 움직여 지워서는 안 된다. 
    SP--;
    // func1의 스택 프레임 제거 (함수 에필로그 + pop)
    print_stack();
    return 0;
}
