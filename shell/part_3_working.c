#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
struct item {
	char *data;
	struct item *next;
};

struct flags {
    int kol;
    int word_len;
    int fon_flag;
    int err_flag;
    int in;
    int out;
    int n_dir;
    int op_num;
};

char **make_ar(char **ar)
{
    int i;
    ar=malloc(2*sizeof(*ar));
    for (i=0;i<2;i++)
        ar[i]=NULL;
    return ar;
}

char **free_ar(char **ar)
{
    int i;
    for (i=0;i<2;i++)
        free(ar[i]);
    free(ar);
    return NULL;
}

void free_list(struct item **head,int i)
{
    struct item *q;
    while (head[i]){
        q = head[i];
        head[i] = head[i]->next;
        free(q->data);
        free(q);
    }
}

char **make_mas(char **pv, int k, struct item *p)
{
    int j;
    pv=malloc(k*(sizeof(pv)));
    for(j=0;j<k-1;j++){
        pv[j]=(p)->data;
        p=(p)->next;
    }
    pv[k-1]=NULL;
    return pv;
}

void check(struct item *q, int *k)
{
    *k=1;
    while (q){
        (*k)++;
        q=q->next;
    }
}

int do_cd(char **pv,int k)
{
    int dflag;
    if (k==3){
        dflag=chdir(pv[1]);
        if (dflag==-1)
            perror(pv[1]);
    }
    else
        printf("Error when entering data\n");
    return 1;
}

void openning_in(struct flags **flag,char **ar, int*fd2,int *save2)
{
    if ((*flag)->in==1){
        *fd2=open(ar[1],O_RDONLY);
        if (*fd2==-1)
            printf("Error with openning file\n");
        else{
            *save2=dup(0);
            dup2(*fd2,0);
        }
    }
}
void openning_in_conv(struct flags **flag,char **ar, int*fd2,int *save2)
{
    if ((*flag)->in==1){
        *fd2=open(ar[1],O_RDONLY);
        if (*fd2==-1){
            printf("Error with openning file\n");
            _exit(2);
        }
        else{
            *save2=dup(0);
            dup2(*fd2,0);
        }
    }
}
void openning_out_conv(struct flags **flag,char **ar,int *fd1, int *save1)
{
    if ((*flag)->out==1){
        *fd1=open(ar[0],O_CREAT|O_TRUNC|O_WRONLY,0666);
        if (*fd1==-1){
            printf("Error with openning file\n");
            _exit(2);
        }
        else{
            *save1=dup(1);
            dup2(*fd1,1);
        }
    }
    if ((*flag)->out==2){
        *fd1=open(ar[0],O_CREAT|O_APPEND|O_WRONLY,0666);
        if (*fd1==-1){
            printf("Error with openning file\n");
            _exit(2);
        }
        else{
            *save1=dup(1);
            dup2(*fd1,1);
        }
    }
}
void openning_out(struct flags **flag,char **ar,int *fd1, int *save1)
{
    if ((*flag)->out==1){
        *fd1=open(ar[0],O_CREAT|O_TRUNC|O_WRONLY,0666);
        if (*fd1==-1)
            printf("Error with openning file\n");
        else{
            *save1=dup(1);
            dup2(*fd1,1);
        }
    }
    if ((*flag)->out==2){
        *fd1=open(ar[0],O_CREAT|O_APPEND|O_WRONLY,0666);
        if (*fd1==-1)
            printf("Error with openning file\n");
        else{
            *save1=dup(1);
            dup2(*fd1,1);
        }
    }
}
void closing_in(struct flags **flag,int*fd2,int *save2)
{
    if ((*flag)->in>0){
            dup2(*save2,0);
            close(*save2);
            close(*fd2);
    }
}

void closing_out(struct flags **flag,int *fd1,int *save1)
{
    if ((*flag)->out>0){
            dup2(*save1,1);
            close(*save1);
            close(*fd1);
    }
}

void execution(int k,struct item **p, int i)
{
    char **pv=NULL;
    pv=make_mas(pv,k,p[i]);
    execvp(pv[0],pv);
    perror(pv[0]);
    _exit(1);
}

void work(struct item **p,struct flags **flag,char **ar)
{
    char **pv=NULL;
    int cd_flag =0,r,k,pid=0,fd1,fd2, save1, save2;
    check(p[0],&k);
    openning_in(flag,ar,&fd2,&save2);
    openning_out(flag,ar,&fd1,&save1);
    if (k>1){
        pv=make_mas(pv,k,p[0]);
        if ((fd1!=-1)&&(fd2!=-1)){
            if (strcmp(pv[0],"cd\0")==0)
                cd_flag=do_cd(pv,k);
            else{
                pid=fork();
                if(pid==0){   /*child*/
                    execution(k,p,(*flag)->op_num);
                }
            }
            if (((*flag)->fon_flag==0)&&(cd_flag==0)){
                r=wait(NULL);
                while (r!=pid)
                    r=wait(NULL);
            }
        }
        free_list(p,0);
        free(pv);
    }
    closing_in(flag,&fd2,&save2);
    closing_out(flag,&fd1,&save1);
}
void waiting_conv(int *pids_num,struct flags **flag,int *ar_pid)
{
    int i,r;
    while (*pids_num>0){
        r=wait(NULL);
        for (i=0;i<(*flag)->op_num+1;i++){
            if ((ar_pid[i]==r)&&(r>0)){
               (*pids_num)--;
                ar_pid[i]=0;
            }
        }
    }
}

void conv(struct item **p,struct flags **flag,char **ar)
{
    int *ar_pid=NULL,fd[2],saved_fd,cd_flag=0,k,fd1,fd2,save1,save2,i,pids_num=0;
    ar_pid=malloc(((*flag)->op_num)*sizeof(*ar_pid));
    for (i = 0; i < (*flag)->op_num; i++) {
        pipe(fd);
        pids_num++;
        ar_pid[i] = fork();
        if (ar_pid[i]==0) {  /*child*/
            if ((i==0)&&((*flag)->in>0))
                openning_in_conv(flag,ar,&fd2,&save2);
            if (i!=0){
                dup2(saved_fd,0);
                close(saved_fd);
            }
            close(fd[0]);
            check(p[i],&k);
            dup2(fd[1],1);
            close(fd[1]);
            execution(k,p,i);
        }
        if (i!=0)
            close(saved_fd);
        close(fd[1]);
        saved_fd=fd[0];
        free_list(p,i);
    }
    ar_pid[(*flag)->op_num] = fork();
    pids_num++;
    if (ar_pid[(*flag)->op_num]== 0){ /*child*/
        check(p[(*flag)->op_num],&k);
        dup2(saved_fd,0);
        close(saved_fd);
        openning_out_conv(flag,ar,&fd1,&save1);
        execution(k,p,(*flag)->op_num);
    }
    free_list(p,(*flag)->op_num);
    close(saved_fd);
    if (((*flag)->fon_flag==0)&&(cd_flag==0))
        waiting_conv(&pids_num,flag,ar_pid);
    free(ar_pid);
}


int new_buf(char **p,int len)
{
    char *buf;
    buf=malloc(2*len*sizeof(buf));
    strncpy(buf,*p,len);
    free (*p);
    *p=buf;
    buf=NULL;
    return 2*len;
}

struct item *buf_to_list(struct item *head, char *buf, struct flags **flag)
{
    if ((*flag)->word_len!=0){
        if (head==NULL) {
            head=malloc(sizeof(*head));
            head->data=malloc(((*flag)->word_len+1)*sizeof(head->data));
            strncpy(head->data,buf,(*flag)->word_len+1);
            (head)->next=NULL;
        }
        else
            (head)->next=buf_to_list((head->next),buf,flag);
    }
    return head;
}

int err(struct item **p, struct flags **flag)
{
    int i;
    if ((*flag)->err_flag==1)
        printf ("Error when entering data\n");
    if ((*flag)->err_flag==2)
        printf ("Error when use &\n");
    if ((*flag)->err_flag==3)
        printf("Error when use > or <\n");
    if ((*flag)->err_flag==4)
        printf("Too many < or >\n");
    if ((*flag)->err_flag==5)
        printf("Nothing after > or <\n");
    if ((*flag)->err_flag==6)
        printf("Error when use > or < with |\n");
    if ((*flag)->err_flag==7)
        printf("Error when use |\n");
    if ((*flag)->err_flag==8)
        printf("Syntax error befor |\n");
    for (i=0;i<=(*flag)->op_num;i++)
        free_list(p,i);
    printf(">");
    (*flag)->word_len=0;
    (*flag)->in=0;
    (*flag)->out=0;
    (*flag)->op_num=0;
    return 0;
}

int endbuf (struct item **head, char *buf, struct flags **flag,char **ar)
{
    int i=0;
    if ((*flag)->word_len!=0){
        buf[(*flag)->word_len]='\0';
        if ((*flag)->n_dir==0)
            head[(*flag)->op_num]=buf_to_list(head[(*flag)->op_num], buf, flag);
        else{
            if (ar[(*flag)->n_dir-1]==NULL){
                ar[(*flag)->n_dir-1]=malloc(((*flag)->word_len+1)*sizeof(char));
                strncpy((ar[(*flag)->n_dir-1]),buf,(*flag)->word_len+1);
            }
            else
                i=3;
            (*flag)->n_dir=0;
        }
        (*flag)->word_len=0;
    }
    return i;
}

int endlist (struct item **head, char *buf, struct flags **flag,char **ar)
{
    int p,status;
    endbuf(head,buf,flag,ar);
    p=wait4(-1,&status,WNOHANG,NULL);
    while (p>0)
        p=wait4(-1,&status,WNOHANG,NULL);
    if ((*flag)->n_dir!=0){
        (*flag)->err_flag=5;
        (*flag)->kol=err(head,flag);
        (*flag)->err_flag=0;
    }
    else{
        if ((*flag)->op_num==0)
            work(head,flag,ar);
        else
            conv(head,flag,ar);
        printf(">");
        (*flag)->word_len=0;
    }
    p=wait4(-1,&status,WNOHANG,NULL);
    while (p>0)
        p=wait4(-1,&status,WNOHANG,NULL);
    return 0;
}

int newline(struct item **head,char *buf, struct flags **flag,char ***ar)
{
    if ((*flag)->err_flag==0)
        (*flag)->word_len=endlist(head, buf, flag,*ar);
    else
        ((*flag)->kol)=err(head,flag);
    *ar=free_ar(*ar);
    *ar=make_ar(*ar);
    (*flag)->in=0;
    (*flag)->out=0;
    (*flag)->err_flag=0;
    (*flag)->n_dir=0;
    (*flag)->op_num=0;
    return 0;
}

void check_red(int *c,struct flags **flag,int*count,int*i)
{
    if (*c=='>'){
        (*flag)->out++;
        (*count)++;
        (*i)++;
    }
    if (*c=='<'){
        (*flag)->in++;
        (*count)++;
        (*i)--;
    }
}

void redir(int *c,struct flags **flag)
{
    int count=0,i=0;
    if ((*c=='<')&&((*flag)->op_num>0))
        (*flag)->err_flag=6;
    if (((*c=='>')&&((*flag)->n_dir==2))||((*c=='<') &&((*flag)->n_dir==1)))
        (*flag)->err_flag=3;
    if ((*flag)->n_dir==0){
        while ((*c=='>')||(*c=='<')){
            check_red(c,flag,&count,&i);
            *c=getchar();
        }
        if (((*flag)->in>1)||((*flag)->out>2)||(abs(i)!=count))
            (*flag)->err_flag=4;
        else{
            if (i>0)
                (*flag)->n_dir=1;
            else
                (*flag)->n_dir=2;
        }
    }
    else
        (*flag)->err_flag=3;
}

struct flags *make_flag(struct flags *flag)
{
    flag=malloc(sizeof(*flag));
    flag->kol=0;
    flag->word_len=0;
    flag->fon_flag=0;
    flag->err_flag=0;
    flag->in=0;
    flag->out=0;
    flag->n_dir=0;
    flag->op_num=0;
    return flag;
}

int maximum(int a, int b)
{
    if (a>b)
        return a;
    else
        return b;
}

void ending_buf(struct item **head, char *buf, struct flags **flag,char **ar)
{
    int err_n=0;
    err_n=endbuf(head, buf,flag,ar);
    (*flag)->err_flag=maximum(err_n,(*flag)->err_flag);
}

void new_op(int *c,struct flags **flag, struct item **head)
{
    if (head[(*flag)->op_num]==NULL)
        (*flag)->err_flag=8;
    (*flag)->op_num++;
    if ((*flag)->out>0)
        (*flag)->err_flag=6;
    *c=getchar();
    while (*c=='|'){
        *c=getchar();
        (*flag)->err_flag=7;
    }
    if (*c=='\n')
        (*flag)->err_flag=7;
}
struct item **newhead(struct item ***head,int*head_size)
{
    struct item **head1 = NULL;
    int i;
    *head_size=*head_size*2;
    head1=malloc(7*sizeof(*head1));
    for (i=0;i<(*head_size/2);i++)
        head1[i]=(*head)[i];
    free(*head);
    return head1;
}

int main()
{
    struct item **head = NULL;
    struct flags *flag=NULL;
    int buf_len=5,c, head_size=1;
    char *buf;
    char **ar=NULL;
    flag=make_flag(flag);
    ar=make_ar(ar);
    buf=malloc(buf_len*sizeof(*buf));
    printf(">");
    head=malloc(7*sizeof(*head));
    while ((c=getchar())!=EOF){
        if ((flag->fon_flag!=0)&&(c!='\n'))
            flag->err_flag=2;
        if (buf_len==flag->word_len+1)
            buf_len=new_buf(&buf,buf_len);
	if (flag->kol%2==0){
            if (c=='|'){
                ending_buf(head, buf,&flag,ar);
                if (flag->op_num== head_size-1)
                    head=newhead(&head,&head_size);
                new_op(&c,&flag,head);
            }
            if ((c=='>')||(c=='<')){
                ending_buf(head, buf,&flag,ar);
                redir(&c,&flag);
	    }
            if (c==EOF)
                break;
            if (c=='&'){
                flag->fon_flag=1;
                continue;
            }
            if ((c==' ')||(c=='\t')){
                ending_buf(head, buf,&flag,ar);
                continue;
            }
        }
        if (c=='"'){
            flag->kol++;
            continue;
        }
        if (c=='\n'){
            if (((flag->kol)%2!=0)||((flag->err_flag)!=0))
                flag->err_flag=maximum(1,(flag->err_flag));
            flag->fon_flag=newline(head, buf,&flag,&ar);
            continue;
        }
        buf[flag->word_len++]=c;
    }
    ar=free_ar(ar);
    free(flag);
    return 0;
}
