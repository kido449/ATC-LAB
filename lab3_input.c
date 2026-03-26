/* LAB 3: WORD COUNT - User Input Version */
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(){
  char text[5000]; char line[300];
  int idx=0;
  printf("========================================\n");
  printf("   LAB 3: WORD COUNT (like Unix wc)\n");
  printf("========================================\n");
  printf("Enter text line by line.\n");
  printf("Type 'END' on a new line when done.\n\n");
  while(1){
    printf("> ");
    fgets(line,sizeof(line),stdin);
    if(strncmp(line,"END",3)==0) break;
    int len=strlen(line);
    if(idx+len<4900){strcpy(text+idx,line);idx+=len;}
  }
  text[idx]='\0';

  int lines=0,words=0,chars=0,in_word=0;
  for(int i=0;text[i];i++){
    chars++;
    if(text[i]=='\n') lines++;
    if(isspace(text[i])) in_word=0;
    else if(!in_word){words++;in_word=1;}
  }

  printf("\n--- WORD COUNT RESULT ---\n");
  printf("  Lines      : %d\n",lines);
  printf("  Words      : %d\n",words);
  printf("  Characters : %d\n",chars);
  printf("\n========================================\n");
  return 0;
}
