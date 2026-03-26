/* LAB 2: REMOVE COMMENTS - User Input Version */
#include <stdio.h>
#include <string.h>

void remove_comments(char *src, char *out){
  int i=0,j=0,in_block=0,in_line=0;
  while(src[i]){
    if(in_line){if(src[i]=='\n'){in_line=0;out[j++]='\n';}i++;continue;}
    if(in_block){if(src[i]=='*'&&src[i+1]=='/'){in_block=0;i+=2;}else{if(src[i]=='\n')out[j++]='\n';i++;}continue;}
    if(src[i]=='/'&&src[i+1]=='/'){in_line=1;i+=2;continue;}
    if(src[i]=='/'&&src[i+1]=='*'){in_block=1;i+=2;continue;}
    out[j++]=src[i++];
  }
  out[j]='\0';
}

int main(){
  char src[3000], out[3000];
  int idx=0; char line[200];
  printf("========================================\n");
  printf("   LAB 2: REMOVE COMMENTS FROM C CODE\n");
  printf("========================================\n");
  printf("Enter C code line by line.\n");
  printf("Type 'END' on a new line when done.\n\n");
  while(1){
    printf("> ");
    fgets(line,sizeof(line),stdin);
    if(strncmp(line,"END",3)==0) break;
    int len=strlen(line);
    if(idx+len<2900){strcpy(src+idx,line);idx+=len;}
  }
  src[idx]='\0';
  remove_comments(src,out);
  printf("\n--- ORIGINAL CODE ---\n%s",src);
  printf("\n--- AFTER REMOVING COMMENTS ---\n%s",out);
  printf("\n========================================\n");
  return 0;
}
