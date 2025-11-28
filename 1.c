#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#define SIZE 10000

struct numstacks{
    double a[SIZE];
    int top;
};

struct opstacks{
    char a[SIZE];
    int top;
};

typedef struct numstacks nstack;
typedef struct opstacks ostack;

int input(char str[], int n){
    int i=0, c=0;
    while((c=getchar())!='\n'&&c!='\0'&&c!=0){
        if(i<n)
        str[i++] = c;
    }
    str[i] = '\0';
    return i;
}

int search(char *a, char b){
    int n = strlen(a);
    for(int i = 0;i<n;i++){
        if(a[i] == b) return i;
    }
    return -1;
}

int prec(char a, char b){
    if (a =='*') a = 'x';
    if(b == '*') b = 'x';
    char pr[4] = {'/', 'x', '-', '+'};
    int k1 = search(pr, a), k2 = search(pr, b);
    if(k1<k2) return 1;
    else if(k1==k2) return 0;
    return -1;
}

void pushn(nstack *a, double val){
    if(a->top == SIZE-1){
        printf("STACK OVERFLOW\n");
        return;
    }
    a->a[++(a->top)] = val;
}

double popn(nstack *a){
    if(a->top == -1){
        return 0;
    }
    double c = a->a[(a->top)--];
    a->a[(a->top)+1] = 0;
    return c;
}

void pusho(ostack *a, char val){
    if(a->top == SIZE-1){
        printf("STACK OVERFLOW\n");
        return;
    }
    a->a[++(a->top)] = val;
}

int popo(ostack *a){
    if(a->top == -1){
        return 0;
    }
    int c = a->a[(a->top)--];
    a->a[(a->top)+1] = 0;
    return c;
}

double calc(double a, double b, char c){
    if(c == '+') return a+b;
    if(c == '-') return a-b;
    if(c == '/') return a/b;
    if(c == '*' || c == 'x') return a*b;
    return 0;
}

int isoper(char a){
    if(a == 'x'|| a == '*' || a=='-'||a=='/'||a=='+') return 1;
    return 0;
}

EXPORT double evaluate(char *a);

double evaluate(char *a){
    nstack ns;ns.top = -1;
    ostack os;os.top = -1;
    int i = 0, n = strlen(a);
    while(i<n){
        if(isdigit(a[i])){
            double num = 0;
            while(isdigit(a[i]) && i<n){
                num = num*10 + (a[i]-'0');
                i++;
            }
            double z = 0, v = 0, power=0;
            if(a[i] == '.'){
                i++;
                while(isdigit(a[i]) && i<n){
                    z = z*10 + (a[i] - '0');
                    v++;
                    i++;
                }
                num += z/pow(10,v);
            }
            int sign = 1;
            if(a[i] == 'e'){
                i++;
                if(a[i] == '+') {sign = 1; i++;}
                else if(a[i] == '-') {sign = -1; i++;}
                while(isdigit(a[i]) && i<n){
                    power = power*10 + (a[i] - '0');
                    i++;
                }
                num = num*pow(10, sign*power);
            }
            pushn(&ns, num);
        }
        else if(a[i] == '+' || a[i] == '-' || a[i] == '*' || a[i] == 'x' || a[i] == '/'){
            char curr = a[i];
            while(os.top != -1 && prec(os.a[os.top], curr) >= 0){
                double k = calc(ns.a[ns.top - 1], ns.a[ns.top], os.a[os.top]);
                popo(&os);
                popn(&ns); popn(&ns);
                pushn(&ns, k);
            }
            pusho(&os, curr);
            i++;
        }
        else if(a[i] == '(' || a[i] == '[' || a[i] == '{'){
            if(a[i-1] != '+' && a[i-1] != '-' && a[i-1] != '*' && a[i-1] != 'x' && a[i-1] != '/' && i!=0){
                pusho(&os, '*');
            }
            int level = 1, j=0, x=0;
            char b[n];
            for(int j = 0;j<n;j++) b[j] = 0;
            while(i<n){
                i++;
                if(a[i] == '(' || a[i] == '[' || a[i] == '{') {level++;x++;}
                else if(a[i] == ')' || a[i] == ']' || a[i] == '}') level--;
                if(level == 0) break;
                b[j++] = a[i];
            }
            if(level != 0){
                printf("INVALID INPUT 😂🤣.You should have checked before typing 🤦‍♂️ \n");
                printf("TERMINATING THE PROGRAM 😒🥱😴");
                exit(EXIT_FAILURE);
            }
            b[j] = 0;
            i++;
            if(a[i+1] != '+' && a[i+1] != '-' && a[i+1] != '*' && a[i+1] != 'x' && a[i+1] != '/' && a[i+1] != '\0' && i<n-1){
                pusho(&os, '*');
            }
            double calcu = evaluate(b);
            pushn(&ns, calcu);
        }
        else if(a[i] == ' '){
            i++;
        }
        else{
            printf("INVALID INPUT 😂🤣.You should have checked before typing 🤦‍♂️ \n");
            printf("TERMINATING THE PROGRAM 😒🥱😴");
            exit(EXIT_FAILURE);
        }
    }
    printf("DEBUG EVALUATE: ");
    for(int i=0;i<ns.top+1;i++) printf("%lf ", ns.a[i]);
    printf("\n");
    for(int i=0;i<os.top+1;i++) printf("%c ", os.a[i]);
    printf("\n");
    while(os.top != -1){
        double k = calc(ns.a[ns.top - 1], ns.a[ns.top], os.a[os.top]);
        popo(&os);
        popn(&ns); popn(&ns);
        pushn(&ns, k);
    }
    return ns.a[ns.top];
}

int main(){
    char a[1000] = {0};
    input(a, 1000);
    printf("result: %lf", evaluate(a));
}