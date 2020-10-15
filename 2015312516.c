#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

int countWord(char word[]);
int isWord(char word[]);
int haveStar(char word[]);
int isPhrase(char word[]);
int searchWord(char* dir, char input[]);
int searchPhrase(char* dir, char input[]);
int searchMulti(char* dir, char input[]);
int searchRegular(char* dir, char input[]);

//
int main(int argc, char* argv[]){
    
    //char buffer[100];
    //char word[100];
    char* dir=argv[1]; 
    //int fd,fd2;
    int n;
    int line=1;
    int cmp;
    //fd=open(dir,O_RDWR|O_CREAT,0755);
    //fd2=open("test2.txt",O_RDWR|O_CREAT,0755);
    char input[200];
    char output[200];
    int lineindex=-1;
    int wordindex=0;
    int inputcount=5;
    int count=0;
    while(1){
	fgets(input,sizeof(input),stdin);
	int a=countWord(input);
	input[a-1]='\0';
	if(isWord(input)){ // no space (' ') 
	    if(haveStar(input)){ // word*word
		printf("searchRegular\n");    
		//searchRegular(dir,input);
	    }else{ // no star, no space
		searchWord(dir,input);
	    }
		
	}else{ // have space
		if(isPhrase(input)){ // have ""  
		    searchPhrase(dir,input);
		}else{ // have space but no ""
		    printf("searchMulti\n");
		    //searchMulti(dir,input);
		}
	}
    }

}


int countWord(char word[]){
    int i=0;
    int count=0;
    while(word[i]!='\0'){
	count++;
	i++;
    }
    
    return count;
}

int isWord(char word[]){
    int i=0;
    while(word[i]!='\0'){
	if(word[i]==' '){
	    return 0;
	}
	i++;
    }
    return 1;
}

int haveStar(char word[]){
    int i=0;
    while(word[i]!='\0'){
	if(word[i]=='*'){
	    return 1;
	}
	i++;
    }
    return 0;
}

int isPhrase(char word[]){
    int i=0;
    while(word[i]!='\0'){
	if(word[i]=='"'){
	    return 1;
	}
	i++;
    }
    return 0;
}

int searchWord(char* dir,char input[]){
    int fd;
    int fd2;
    char buffer[100];
    char word[100];
    int n;
    int line=1;
    int cmp;
    fd=open(dir,O_RDWR|O_CREAT,0755);
    fd2=open("test2.txt",O_RDWR|O_CREAT,0755);
    //char input[50];
    char output[200];
    int lineindex=-1;
    int wordindex=0;
    int inputcount=countWord(input);
    int count=0;
    while((n=read(fd,buffer,1))>0){
	if(buffer[0]==32||buffer[0]=='\n'||buffer[0]=='\t'){
	    lineindex++;
	    word[wordindex]='\0';
	    
	    count=0;
	    
	    for(int m=0;m<wordindex;m++){
		word[m]=tolower(word[m]);
		if(word[m]==input[m])
		    count++;
	    }
	    
	    if(count!=0&&count==wordindex&&wordindex==inputcount){
		char numbuf[10];
		int outline=line;
		int j=0;
		int aCount=0;
		while(outline>=0){
		    if(outline>=0&&outline<10){
			numbuf[j]=outline%10+48;
			aCount++;
			j++;
			break;
		    }
		    numbuf[j]=outline%10+48;
		    aCount++;
		    j++;
		    outline=(outline-outline%10)/10;
		}
		
		for(int i=0;i<j;i++){
		    output[i]=numbuf[j-i-1];
		}
		output[j]=':';
		numbuf[j]=':';
		j++;
		int ibuf=lineindex-wordindex;
		int bCount=0;
		while(ibuf>=0){
		    if(ibuf>=0&&ibuf<10){
			numbuf[j]=ibuf%10+48;
			bCount++;
			j++;
			break;
		    }
		    numbuf[j]=ibuf%10+48;
		    bCount++;
		    j++;
		    ibuf=(ibuf-ibuf%10)/10;
		}
		

		for(int i=0;i<bCount;i++){
		    output[aCount+i+1]=numbuf[j-i-1];
		}
		output[j]=' ';		
		output[j+1]='\0';
		printf("%s",output);
		write(fd2,output,j+1);
	    }
		word[0]='\0';
		wordindex=0;
		
		if(buffer[0]=='\n'){
	    		line++;
	    		lineindex=-1;
		}
		
	}

	else{
	    word[wordindex]=buffer[0]; 
	    wordindex++;
	    lineindex++;
	}
	
    }
    close(fd);
    close(fd2);

}

int searchPhrase(char* dir, char input[]){
	int len=countWord(input);
	int inputSpace=0;

	input[0]=' ';
	input[len-1]=' ';
	input[len]='\0';
	
	char firstinput[100];
	for(int i=0;i<len-1;i++){
	    firstinput[i]=input[i+1];
	}
	firstinput[len-1]='\0';
	char lastinput[100];
	for(int i=0;i<len-1;i++){
	    lastinput[i]=firstinput[i];
	}
	lastinput[len-2]='\0';
	printf("%s\n",lastinput);	
    int fd,fd2;
    char buffer[100];
    char word[100];
    int n;
    int line=1;
    fd=open(dir,O_RDWR|O_CREAT,0755);
    fd2=open("test2.txt",O_RDWR|O_CREAT,0755);
    char output[200];
    int lineindex=-1;
    int wordindex=0;
    int inputcount=countWord(input);
    int count=0;
	int numSpace=0;


    while((n=read(fd,buffer,1))>0){
	if(buffer[0]=='\n'){
	    
	    lineindex++;
	    word[wordindex]='\0';
	    
	    
	    count=0;
	    int k=-1;
	    do{
		count=0;
		k++;
	    for(int m=0;m<inputcount;m++){
		word[m+k]=tolower(word[m+k]);
		if(k>=1&&k!=lineindex-inputcount){
		if(word[m+k]==input[m])
		    count++;
		}else if(k==lineindex-inputcount){
		    if(word[m+k]==lastinput[m])
			count++;
		}else{
		    if(word[m+k]==firstinput[m])
			count++;
		}
	    }
	    if(k==0)
		inputcount=countWord(firstinput);
	    if(k==lineindex-inputcount)
		inputcount=countWord(lastinput);
	
	    if(count!=0&&count==inputcount){			
		
		char numbuf[10];
		
		int outline=line;
		int j=0;
		int aCount=0;
		while(outline>=0){
		    if(outline>=0&&outline<10){
			numbuf[j]=outline%10+48;
			aCount++;
			j++;
			break;
		    }
		    numbuf[j]=outline%10+48;
		    aCount++;
		    j++;
		    outline=(outline-outline%10)/10;
		}

		for(int i=0;i<j;i++){
		    output[i]=numbuf[j-i-1];
		}
		
		output[j]=':';
		numbuf[j]=':';
		j++;

		int ibuf;
		if(k==0||k==lineindex-inputcount)
		    ibuf=k;
		else
		    ibuf=k+1;
		int bCount=0;
		while(ibuf>=0){
		    if(ibuf>=0&&ibuf<10){
			numbuf[j]=ibuf%10+48;
			bCount++;
			j++;
			break;
		    }
		    numbuf[j]=ibuf%10+48;
		    bCount++;
		    j++;
		    ibuf=(ibuf-ibuf%10)/10;
		}
		

		for(int i=0;i<bCount;i++){
		    output[aCount+i+1]=numbuf[j-i-1];
		}
		output[j]=' ';		
		output[j+1]='\0';
		printf("%s",output);
		write(fd2,output,j+1);
		}
	    }while(k<lineindex-inputcount+1);
		line++;
		lineindex=-1;
		wordindex=0;
	
	}else{
	    word[wordindex]=buffer[0]; 
	    wordindex++;
	    lineindex++;
	}
    }
    close(fd);
    close(fd2);
}
   
