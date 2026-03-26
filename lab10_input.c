/* LAB 10: CODE GENERATION - User Input Version */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

int  temp_c=0;
char asm_out[200][80]; int asm_n=0;

void emit(const char*fmt,...){
  va_list ap;va_start(ap,fmt);
  vsnprintf(asm_out[asm_n++],80,fmt,ap);va_end(ap);
}

char src10[200]; int p10;
void s10(){while(isspace(src10[p10]))p10++;}
char* new_tmp(){char*t=malloc(8);sprintf(t,"T%d",temp_c++);return t;}

char* cg_expr(); char* cg_term(); char* cg_factor();

char* cg_factor(){
  s10();
  if(src10[p10]=='('){p10++;char*v=cg_expr();s10();if(src10[p10]==')')p10++;return v;}
  char buf[32];int i=0;
  while(isalnum(src10[p10])||src10[p10]=='_')buf[i++]=src10[p10++];
  buf[i]=0; char*t=malloc(32);strcpy(t,buf);return t;
}
char* cg_term(){
  char*v=cg_factor();s10();
  while(src10[p10]=='*'||src10[p10]=='/'){
    char op=src10[p10++];char*r=cg_factor();
    char*t=new_tmp();
    emit("MOV  AX, %s",v);
    if(op=='*')emit("IMUL AX, %s",r);else{emit("CDQ");emit("IDIV %s",r);}
    emit("MOV  %s, AX",t);
    v=t;s10();
  }
  return v;
}
char* cg_expr(){
  char*v=cg_term();s10();
  while(src10[p10]=='+'||src10[p10]=='-'){
    char op=src10[p10++];char*r=cg_term();
    char*t=new_tmp();
    emit("MOV  AX, %s",v);
    if(op=='+')emit("ADD  AX, %s",r);else emit("SUB  AX, %s",r);
    emit("MOV  %s, AX",t);
    v=t;s10();
  }
  return v;
}

void gen(char*lhs,char*rhs){
  strcpy(src10,rhs);p10=0;asm_n=0;temp_c=0;
  char*res=cg_expr();
  if(lhs)emit("MOV  %s, %s",lhs,res);
  printf("\n  Statement  : %s = %s\n",lhs,rhs);
  printf("  Assembly   :\n");
  for(int i=0;i<asm_n;i++)printf("    %-22s\n",asm_out[i]);
}

int main(){
  char line[200],lhs[50],rhs[150];
  printf("========================================\n");
  printf("   LAB 10: CODE GENERATION\n");
  printf("   Expression -> x86 Assembly\n");
  printf("========================================\n");
  printf("Enter assignment: variable = expression\n");
  printf("   e.g: z = a + b * c\n");
  printf("   e.g: x = (a + b) * c\n");
  printf("Type 'exit' to quit.\n");
  printf("========================================\n\n");
  while(1){
    printf("Enter: ");
    fgets(line,sizeof(line),stdin);
    int len=strlen(line);
    if(len>0&&line[len-1]=='\n')line[len-1]='\0';
    if(!strcmp(line,"exit")) break;
    /* parse lhs = rhs */
    char*eq=strchr(line,'=');
    if(!eq){printf("  Format: var = expression\n\n");continue;}
    *eq='\0';
    strcpy(lhs,line); strcpy(rhs,eq+1);
    /* trim lhs */
    int l=strlen(lhs);while(l>0&&isspace(lhs[l-1]))lhs[--l]='\0';
    char*r=rhs;while(isspace(*r))r++;
    gen(lhs,r);
    printf("\n  Instruction Key:\n");
    printf("    MOV = move, ADD = add, SUB = subtract\n");
    printf("    IMUL= multiply, IDIV= divide, CDQ= sign extend\n\n");
  }
  printf("========================================\n");
  return 0;
}
