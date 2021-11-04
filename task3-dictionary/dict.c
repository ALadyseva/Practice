#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

struct TreeLink {
	char *word;
	int cnt; /*number of occurrences*/
	struct TreeLink *pleft, *pright;
};

char *copy_word(char *wrd){
	char *tmp;
	tmp = (char *)malloc(strlen(wrd)+1);
	if (tmp != NULL)
		strcpy(tmp, wrd);
	return tmp;
}

struct TreeLink *addtree(struct TreeLink *p, char *w, int *max){
	
	if (p == NULL){
		p = (struct TreeLink*)malloc(sizeof(struct TreeLink));
		p->word = copy_word(w);
		p->cnt = 1;
		p->pleft = NULL;
		p->pright = NULL;
	}
	else {		
		int status;
		status = strcmp(w, p->word);

		if (status == 0){
			p->cnt++;
			if ((p->cnt) > *max)
				*max = (p->cnt);
		}
		else if (status < 0){
			p->pleft = addtree(p->pleft, w, max);
		}
		else {
			p->pright = addtree(p->pright, w, max);
		}
	}
	return p;
}

void printree(struct TreeLink *p, int max, int w_quantity, FILE *f_out){
	if (p == NULL)
		return;
	double freq;
	if (p->cnt == max){
		freq = 1.0*(p->cnt)/w_quantity;
		fprintf(f_out,"%s %d %f\n", p->word, p->cnt, freq);
	}
	printree(p->pleft, max, w_quantity, f_out);
	printree(p->pright, max, w_quantity, f_out);
}

void del_tree(struct TreeLink *p){
	if (p == NULL)	
		return;
	del_tree(p->pleft);
	del_tree(p->pright);
	
	p->pleft = NULL;
	p->pright = NULL;
	free(p->word);
	free(p);
}


int main(int argc,char **argv){

	struct TreeLink *tree = NULL;
	long w_quantity = 0; 
	int max = 0;
	
	int flag_i = 0;
	int flag_o = 0;
	int f_out_pos;
	int f_in_pos;

	FILE *f_in;
	FILE *f_out;
	
	/* checking for options*/
	for (int i=1; i<argc; i++){
		if (strcmp(argv[i], "-i") == 0){
			flag_i = 1;
			f_in_pos = i+1;
		}
		if (strcmp(argv[i], "-o") == 0){
			flag_o = 1;
			f_out_pos = i+1;
		}
	}

	/*working with input files*/
	if (flag_i){
		if ((f_in = fopen(argv[f_in_pos], "r")) == NULL) {
			fprintf(stderr, "ERROR: Input file '%s' is not found\n", argv[f_in_pos]);
			return 1;
		}
		printf("Reading from file '%s'\n", argv[f_in_pos]);
	}
	else{
		f_in = stdin;
		printf("Reading from stdin\n");
	}

	/*working with input files*/
	if (flag_o){
		if ((f_out = fopen(argv[f_out_pos], "w")) == NULL){
			fprintf(stderr, "ERROR: Output file '%s' is not foung\n", argv[f_out_pos]);
			return 2;	
		}
		printf("Writing in file '%s'\n", argv[f_out_pos]);
	}
	else{
		f_out = stdout;
		printf("Writing in stdout\n");
	}

	char *tmp = NULL;
	int t_size = 0;
	int t_numb = 0;
	char c;

	do {
		c = fgetc(f_in);
	
		if ((c!='\n')&&(c!='\r')&&(c!='\t')&&(c!=' ')&&(c!=EOF)&& isalnum(c)){
			if (t_numb == t_size){
				t_size = 2*t_size +1;
				tmp = (char*)realloc(tmp, t_size);
			}
			tmp[t_numb] = c;
			t_numb++;
		}
		else if (t_numb){
			t_size =t_size +1;
			tmp = (char*)realloc(tmp, t_size);
			tmp[t_numb]=0;
			tree = addtree(tree, tmp, &max);
			free(tmp);
			tmp = NULL;
			t_size = t_numb = 0;
			w_quantity++;
		}
		
		if (!isalnum(c)&&(c!='\r')&&(c!='\t')&&(c!='\n')&&(c!=' ')&&(c!=EOF)){
			tmp = (char*)malloc(2);
			tmp[0]=c;
			tmp[1]=0;
			tree = addtree(tree, tmp, &max);
			free(tmp);
			tmp = NULL;
			w_quantity++;
		}

	}while (c!=EOF);
	
	while (max>0){
		printree(tree, max, w_quantity, f_out);
		max--;
	}
	fclose(f_in);
	fclose(f_out);
	del_tree(tree);

	return 0;
}

