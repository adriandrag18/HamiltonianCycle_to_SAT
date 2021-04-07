#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void drumuri (int a, int max, FILE *out){
	fprintf(out, "(");
	for(int j = 1; j <= max;  j++){
		if(j > 1)
			fprintf(out, "|");
		fprintf(out, "a%d-%d",j,a);
	}
	fprintf(out, ")");
}

void edges (int a, int b, FILE *out){
	fprintf(out, "((x%d-%d|~x%d-%d)&(~x%d-%d|x%d-%d))",a,b,b,a,a,b,b,a);
}

void clauza (bool a[], int b, int n, FILE *out){
	int i, j, d=0,c=0;// d si c pentru un format corect daca sunt 0 este prima clauza si nu trebuie puis & sau | inainte
	for( i = 1; i < n;  i++){
		if(a[i] == 1) 
			for(j = i+1; j <= n; j++){
				if(a[j] == 1) {
					if (c == 0){
						c++;
					}else
						fprintf(out, "|");
					fprintf(out, "(");
					d = 0;
					for (int k = 1; k <= n; k++){
					if(a[k] == 1){
						if (d==0)
							d++;
						else 
							fprintf(out, "&");
						
						if (k == i || k == j)
							fprintf(out, "x%d-%d",b,k);
						else
							fprintf(out, "~x%d-%d",b,k);
						}
					}
					fprintf(out, ")");
				}		
			}
	}
}

void laturi(bool a[], int dist, int nod, int n, FILE *out){
	int k = 0;			//k pentru un format corect daca sunt 0 este prima clauza si nu trebuie puis & sau | inainte
	for(int i = 2; i<= n; i++){
		if(a[i] == 1){
			if(k == 0)
				k++;
			else 
				fprintf(out, "|");
			fprintf(out, "( a%d-%d & x%d-%d )",dist,i,i,nod);
		}
	}
}

void main(){

	bool graf[101][101];
	FILE *in = fopen("graph.in","r"), *out = fopen("bexpr.out","w");
	int i,j,n,a,b,nrMuchii,l=0; //l pentru formatul corect daca l = 0 afisam prima clauza din fisier si nu trebuie pus & 
	
	fscanf(in, "%d", &n);

	//initializam matricea
	for( i = 0; i <= n;  i++){
		for(j = 0; j <= n; j++){
			graf[i][j] = 0;
		}
	}
	
	//citit muchiile pana dam de -1
	fscanf(in, "%d", &a);
	while(a != -1){
		fscanf(in, "%d", &b);
		graf[a][b] = 1;		//cum graful este neorientat adca exista muchia (a,b) treebuie sa existe si muchia (b,a)
		graf[b][a] = 1;
		fscanf(in, "%d", &a);
	}

	//prin aceste formule se face verificarea ca graful este neorienta altfel clauzele nu ar putea fi adevarate
	for( i = 1; i <= n;  i++){
		for(j = i; j <= n; j++){
			if(graf[i][j] == 1){
				if(l==0)
					l++;
				else
					fprintf(out, "&");
				edges(i, j, out);		//de tipul (x1-2|~x2-1)&(~x1-2|x2-1)
			}
		}
	} 
 	 //prin acete formule se verfica daca fiecare nod are cel putin 2 muchii altfel un ciclu nu poate exita
	for(i = 1; i <= n;  i++){		//complexitate O(n^3)
		nrMuchii = 0;
		//if(i > 1)
		fprintf(out, "&");
		fprintf(out, "(");
		for(j = 1; j <= n; j++){
			if(graf[i][j] == 1){
				nrMuchii++;
			}
		}
		if(nrMuchii > 1)
			clauza(graf[i],i,n,out);
		else								//daca nu are doua muchii nu exita un ciclu hamiloniam
			for(j = 1; j <= n; j++){
				if(graf[i][j] == 1)
					fprintf(out, "x%d-%d",i,j);
		}
		fprintf(out, ")");
		if(i > 1){
			fprintf(out, "&");
			drumuri(i, n/2+1, out);
		}
	}


	for (int i = 2; i <= n; i++)
		if(graf[1][i] == 1){
			fprintf(out, "&((a1-%d|~x1-%d)&(~a1-%d|x1-%d))",i,i,i,i);
			
		}
	
	// daca nu exita muchia (1,x) fortam variabila a1-x sa fie 0 prin adaugarea clauzei ~a1-x
	for (int i = 1; i <= n; i++){
		if(graf[1][i] == 0){
			fprintf(out, "&~a1-%d",i);
		}
	}
//reprentarea distantelor posibile de la ficare nod
	 for (int i = 2; i <= n/2+1; i++){
	 	for(int j = 2; j <= n; j++){
	 		fprintf(out, "&((a%d-%d|~((",i,j);
	 		laturi(graf[j], i-1, j , n, out);
	 		fprintf(out, ")&~");
	 		drumuri(j, i-1, out);
	 		fprintf(out, "))&(~a%d-%d|((",i,j);
	 		laturi(graf[j], i-1, j , n, out);
	 		fprintf(out, ")&~");
	 		drumuri(j, i-1, out);
	 		fprintf(out, ")))");
	 	}
	 }
	fclose(in);
	fclose(out);
}
