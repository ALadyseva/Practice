#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct item {
	char *data;
	struct item *next;
};

void cleaning(struct item **p){
	struct item *tmp;	
	while (*p){
		tmp = *p;
		*p = (*p)->next;
		free(tmp->data);
		free(tmp);
	}
	return;
}	

char **list_to_array(struct item *head){
	char **arr = NULL;
	int i = 0;
	while (head){
		arr = (char **)realloc(arr, (i+2)*sizeof(char *));
		arr[i] = (char *)malloc(sizeof(char) * (strlen(head->data)+1));
		strcpy(arr[i], head->data);
		i++;
		head = head->next;
	}
	arr[i] = NULL;
	return arr;
} 

void del_array(char **arr){
	if (arr == NULL)
		return;
	
	int i = 0;
	while (arr[i] != NULL){
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr[i]);
	arr[i] = NULL;
	free (arr);
	arr = NULL;
}

int is_cd(char **arr){
	int flag;
	if (strcmp(arr[0], "cd") == 0){
		if (arr[1] == NULL) {
			chdir(getenv("HOME"));
		}
		else if (arr[2] != NULL){
			printf("Error: the system cannot find specified path");
			return 1;
		}
		else{
			flag = chdir(arr[1]);
			if (flag == -1){
				perror(arr[1]);
			}
		}
		return 1;
	}
	return 0;
	
}

void proc_work(struct item **head){
	char **arr;
	int pid;
	arr = list_to_array(*head);
	
	if (is_cd(arr))
		return;
	
	if ((pid = fork())==0){
		execvp(arr[0], arr);
		perror(arr[0]);
		cleaning(head);
		del_array(arr);
		exit(1);
	}

	/*killing zombies*/
	wait(NULL);

	del_array(arr);
}


void outprint (struct item **p)
{
	struct item *tmp;
	while (*p) {
		printf("%s\n",(*p)->data);
		tmp=*p;
		*p =(*p)->next;
		free(tmp->data);
		free(tmp);
	}
}

int new_buf(char **p,int len)
{
	//char *buf;
	//buf=malloc(2*len*sizeof(buf));
	//strncpy(buf,*p,len);
	//free (*p);
	*p=realloc(*p, 2*len*sizeof(char));
	//buf=NULL;
	return 2*len;
}

struct item *buf_to_list(struct item *head, char *buf, int len)
{
	if (len!=0){
    		if (head==NULL) {
        		head=malloc(sizeof(*head));
        		(head)->data=malloc((len+1)*sizeof((head)->data));
        		strncpy((head)->data,buf,len+1);
        		(head)->next=NULL;
    		}
    		else
        		head->next=buf_to_list((head->next),buf,len);
	}
	return head;
}

int err(struct item **p, int *word_len)
{
	struct item *tmp;
	printf ("Error when entering data\n");
	while (*p){
		tmp = *p;
		*p = (*p)->next;
		free(tmp->data);
		free(tmp);
	}
	printf(">");
	*word_len=0;
	return 0;
}

int endbuf (struct item **head, char *buf, int word_len)
{
	buf[word_len]='\0';
	*head=buf_to_list(*head, buf, word_len);
	return 0;
}

int endlist (struct item **head, char *buf, int word_len)
{
	buf[word_len]='\0';
	*head=buf_to_list(*head, buf, word_len);
	proc_work(head);
	cleaning(head);
	printf(">");
	word_len=0;
	return 0;
}

void newline(struct item **head,char *buf, int *word_len, int *kol)
{
    if (*kol%2==0)
        *word_len=endlist(head, buf, *word_len);
    else
        *kol=err(head, word_len);
}

int main()
{
	struct item *head = NULL;
	int kol=0, buf_len=5, word_len=0, c;
	char *buf;
	buf=malloc(buf_len*sizeof(*buf));
	printf(">");
	while ((c=getchar())!=EOF){
		if (c=='"'){
            		kol++;
          		continue;
       		}
		if (c=='\n'){
               		newline(&head, buf, &word_len, &kol);
               		continue;
       		}
		if (buf_len==word_len+1)
	               		buf_len=new_buf(&buf,buf_len);
		if (kol%2==0){
			if ((c==' ')||(c=='\t')){
                		word_len=endbuf(&head, buf, word_len);
               			continue;
			}
			if ((c=='<')||(c==';')||(c==')')||(c=='(')){
				word_len=endbuf(&head, buf, word_len);
				buf[word_len++]=c;
				word_len=endbuf(&head, buf, word_len);
				continue;
			}
			
			if (c=='&'){
				word_len=endbuf(&head, buf, word_len);
				c = getchar();	
				if (c == '&'){
					buf[word_len++]=c;
					buf[word_len++]=c;
					word_len=endbuf(&head, buf, word_len);
				}
				else{
					ungetc(c, stdin);
					buf[word_len++]='&';
					word_len=endbuf(&head, buf, word_len);
				}
				continue;
			}
			if (c=='|'){
				word_len=endbuf(&head, buf, word_len);
				c = getchar();
				if (c == '|'){
					buf[word_len++]=c;
					buf[word_len++]=c;
					word_len=endbuf(&head, buf, word_len);
				}
				else{
					ungetc(c, stdin);
					buf[word_len++]='|';
					word_len=endbuf(&head, buf, word_len);
				}
				continue;
			}
			if (c=='>'){
				word_len=endbuf(&head, buf, word_len);
				c = getchar();
				if (c == '|'){
					buf[word_len++]=c;
					buf[word_len++]=c;
					word_len=endbuf(&head, buf, word_len);
				}
				else{
					ungetc(c, stdin);
					buf[word_len++]='>';
					word_len=endbuf(&head, buf, word_len);
				}
				continue;
			}
		}
		buf[word_len++]=c;	
	}
	cleaning (&head);
	if (buf)
		free(buf);
	return 0;
}
