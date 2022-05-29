#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>

// global input array
int *input;
int size;
char file_name[256];

// load array input
typedef struct parameters{

int *array;
int hi;
int lo;


}parameters;


void loadInput(){


printf("please enter input file name\n");
fgets(file_name,256,stdin);
strtok(file_name,"\n"); 

FILE *fp=fopen(file_name,"r");

fscanf(fp,"%d\n",&size);


input=malloc(size*sizeof(int));



int i=0;
int temp;

while(!feof(fp)){

 
fscanf(fp,"%d",&input[i]);

i++;
     
}
 
fclose(fp);

}
// merge function 
void merge(int arr[], int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;

	/* create temp arrays */
	int L[n1], R[n2];

	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < n1; i++)
		L[i] = arr[l + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[m + 1 + j];

	/* Merge the temp arrays back into arr[l..r]*/
	i = 0; // Initial index of first subarray
	j = 0; // Initial index of second subarray
	k = l; // Initial index of merged subarray
	while (i < n1 && j < n2) {
		if (L[i] <= R[j]) {
			arr[k] = L[i];
			i++;
		}
		else {
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	/* Copy the remaining elements of L[], if there
	are any */
	while (i < n1) {
		arr[k] = L[i];
		i++;
		k++;
	}

	/* Copy the remaining elements of R[], if there
	are any */
	while (j < n2) {
		arr[k] = R[j];
		j++;
		k++;
	}
}

// merge sort function
//mergeSort(int arr[], int l, int r)
void *mergeSort(void *args)
{


         parameters *parameter=(parameters*) args;
         int l=parameter->lo;
         int r=parameter->hi;
         int *arr=parameter->array;



	if (l < r) {
		// Same as (l+r)/2, but avoids overflow for
		// large l and h
		int m = l + (r - l) / 2;
		
		// create thread for first part of array
		
		pthread_t th1;
		parameters p1;
		p1.array=parameter->array;
		p1.lo=m+1;
		p1.hi=r;
               pthread_create(&th1,NULL,&mergeSort,&p1);
           
                // create thraed for other half
                pthread_t th2;
                parameters p2;
                p2.array=parameter->array;
		p2.lo=l;
		p2.hi=m;
               pthread_create(&th2,NULL,&mergeSort,&p2);
                
                
                
                
                pthread_join(th1, NULL);
                pthread_join(th2, NULL);
                
                
		

		merge(arr, l, m, r);
	}
}

int main(){


loadInput();

// create thread to run function because i put a void function 

pthread_t temp;
parameters parameter;
parameter.array=input;
parameter.lo=0;
parameter.hi=size-1;

pthread_create(&temp,NULL,&mergeSort,&parameter);

 
 pthread_join(temp, NULL);
 




// print output in file

FILE *f;
f = fopen("output.txt", "w"); 

fprintf(f,"sorted array \n");

for(int i=0;i<size;i++)
fprintf(f,"%d ",input[i]);



fclose(f);



return 0;

}
