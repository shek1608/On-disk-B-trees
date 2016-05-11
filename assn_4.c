#include <stdio.h>
 #include <string.h>
#include <stdlib.h>
int order = 4; /* B-tree order */

FILE *fp; /* Input file stream */
long root; /* Offset of B-tree root node */

char *filename;

// long stack[1024];
// int stacktop = 0;

typedef struct
{
	/* B-tree node */ 
	int n; /* Number of keys in node */ 
	int *key; /* Node's keys */ 
	long *child; /* Node's child subtree offsets */ 
} btree_node;

typedef struct
{
	int valid;
	int key;
	long offset;
} returnStructure;


struct queuenode
{
	long offset;
	struct queuenode *next;
}*front,*rear,*temp;

void addQ(long offset)
{
	if(front==NULL)
	{
		front=(struct queuenode *)malloc(sizeof(struct queuenode));
		front->offset=offset;
		front->next=NULL;
		rear=front;
	}
	else
	{
		temp=(struct queuenode *)malloc(sizeof(struct queuenode));
		temp->offset=offset;
		temp->next=NULL;
		rear->next=temp;
		rear=temp;
	}
}

long removeQ()
{
	long offset;
	if(front==NULL)	return -1;	//nothing, then return -1
	offset=front->offset;
	if(front->next==NULL)	//if only 1, then 
	{
		free(front);
		front=rear=NULL;
	}
	else
	{
		temp=front->next;
		free(front);
		front=temp;
	}
	return offset;
}

void printQ()
{
	if(root==-1)
		return;
	fseek(fp,root,SEEK_SET);
	int number[2];
	fread(number,sizeof(int),1,fp);
	//printf("Number is %d\n",number[0]);
	if(number[0]==0)	return;
	front=rear=NULL;
	//printf("Hello\n");
	addQ(root);
	int queuesize=1,level=1,levelnodes=1,nextlevelnodes=0;
	long offset;

	btree_node node;
	node.key=(int *)calloc(order-1,sizeof(int));
	node.child=(long *)calloc(order,sizeof(long));
	node.n = 0;

	while(queuesize>0)
	{
		printf("%2d: ",level);
		//printf("Hello2\n");

		level++;
		int a;
		for(a=0;a<levelnodes;a++)
		{
			offset=removeQ();
			queuesize--;
			fseek(fp,offset,SEEK_SET);
			fread(&node.n,sizeof(int),1,fp);
			fread(node.key,sizeof(int),order-1,fp);
			fread(node.child,sizeof(long),order,fp);
			int b;
			//printf("Queuesize is now %d\n",queuesize);
			//printf("Offset is now %d\n",offset);
			//printf("Number of nodes = %d\n",node.n);

			for(b=0;b<node.n-1;b++)
			{
			//	printf("Printing\n");
				printf("%d,",node.key[b]);
			}
			//printf("Here\n");
			printf("%d ",node.key[(node.n-1)]);
			for(b=0;b<node.n+1;b++)
			{
				if(node.child[b]!=0)
				{
					queuesize++;
					nextlevelnodes++;
					addQ(node.child[b]);
				}
			}

		}
		//printf("Out of for\n");
		printf("\n");
		levelnodes=nextlevelnodes;
		nextlevelnodes=0;
	}
	//printf("Out of while\n" );
	free(node.child);
	free(node.key);
	front=rear=NULL;
}	//end print

/*
void fileprint()
{
	long fileroot[2];
	long offvalue;
	int c[2];
	int a[order-1];
	long b[order];
	fseek(fp,0,SEEK_SET);
	fread(fileroot,sizeof(long),1,fp);
	printf("Root value %ld\n",fileroot[0]);
	int n1=1,n2=1,n3=1;
	while(1)
	{
		offvalue = ftell(fp);
		n3 = fread(c,sizeof(int),1,fp);
		n1 = fread(a,sizeof(int),order-1,fp);
		n2 = fread(b,sizeof(long),order,fp);
		int printtemp;
		if(n1!=0&&n2!=0&&n3!=0)
		{
			printf("At offset %ld: ",offvalue);
			printf("n=%d ",c[0]);
			printf("Key=");
			for(printtemp=0;printtemp<order-1;printtemp++)
				printf("%d->",a[printtemp]);
			printf(" Child=");
			for(printtemp=0;printtemp<order;printtemp++)
				printf("%ld->", b[printtemp]);
			printf("\n");
		}
		else break;
	}
}
*/
btree_node readfromfile(long offset)
{
	btree_node node;
	node.key=(int *)calloc(order-1,sizeof(int));
	node.child=(long *)calloc(order,sizeof(long));
	node.n = 0;

	fseek(fp,offset,SEEK_SET);
	fread(&node.n,sizeof(int),1,fp);
	fread(node.key,sizeof(int),order-1,fp);
	fread(node.child,sizeof(long),order,fp);
	//printf("Node read: n=%d key=%d child=%ld\n",node.n,node.key[0],node.child[0]);

	return node;

}


returnStructure add(int k, long offset)
{
	//printf("Add called\n");
	btree_node node;
	node = readfromfile(offset);

	int i,leaf=1;
	returnStructure obj;

	for(i=0;i<node.n;i++)
	{
		if(k<node.key[i])
			break;
	}
	//printf("I value is  %d\n",i );
	if(node.child[i]!=0)
	{
		leaf=0;
		obj = add(k,node.child[i]);
	}

	//if leaf
	if(leaf==1)
	{
		//printf("Am leaf\n");
		if(node.n<(order-1))	//no overflow!
		{
			//printf("No overflow\n");
			int a;
			btree_node newnode;
			newnode.key=(int *)calloc(order-1,sizeof(int));
			newnode.child=(long *)calloc(order,sizeof(long));
			newnode.n = node.n+1;
			for(a=0;a<i;a++)
			{
				newnode.key[a]=node.key[a];
				newnode.child[a]=node.child[a];
			}
			newnode.key[i]=k;
			for(a=i;a<node.n;a++)
			{
				newnode.key[a+1]=node.key[a];
				newnode.child[a+1]=node.child[a];
			}
			newnode.child[a+1]=node.child[a];
			fseek(fp,offset,SEEK_SET);
			//printf("Writing to file now for leaf no overflow...\n");
			fwrite(&newnode.n,sizeof(int),1,fp);
			fwrite(newnode.key,sizeof(int),order-1,fp);
			fwrite(newnode.child,sizeof(long),order,fp);
			returnStructure temp;
			temp.valid=-1;
			return temp;
		}
		else	//overflow
		{
			//printf("Overflow case\n");
			/*returnStructure temp;
			temp.valid=-1;
			return temp;*/

			int arrKey[order];
			int a;
			for(a=0;a<i;a++)
			{
				arrKey[a]=node.key[a];
			}
			arrKey[i]=k;
			for(a=i;a<node.n;a++)
			{
				arrKey[a+1]=node.key[a];
			}
			int m = order/2;
			//printf("a is %d M is %d\n",a, m );

			btree_node leftnode,rightnode;
			leftnode.n=0;
			leftnode.key=(int *)calloc(order-1,sizeof(int));
			leftnode.child=(long *)calloc(order,sizeof(long));	//number of keys+1 needed...
			rightnode.n=0;
			rightnode.key=(int *)calloc(order-1,sizeof(int));
			rightnode.child=(long *)calloc(order,sizeof(long));	//number of keys+1 needed...

			for(a=0;a<m;a++)
			{
				leftnode.key[a]=arrKey[a];
			}
			leftnode.n=m;
			int newn;
			for(a=m+1,newn=0;a<node.n+1;a++,newn++)
			{
				rightnode.key[newn]=arrKey[a];
			}
			rightnode.n=newn;

			fseek(fp,offset,SEEK_SET);
			fwrite(&leftnode.n,sizeof(int),1,fp);
			fwrite(leftnode.key,sizeof(int),order-1,fp);
			fwrite(leftnode.child,sizeof(long),order,fp);

			returnStructure objret;
			objret.valid=1;
			fseek(fp,0,SEEK_END);
			objret.offset=ftell(fp);
			objret.key=arrKey[m];

			fwrite(&rightnode.n,sizeof(int),1,fp);
			fwrite(rightnode.key,sizeof(int),order-1,fp);
			fwrite(rightnode.child,sizeof(long),order,fp);

			//printf("Being pushed up: %d\n",objret.key);


			return objret;
		}	//end of overflow
	}
	else	//not leaf
	{
		//printf("Not leaf\n");


		if(obj.valid==-1)	//just return NULL
		{
			//printf("Nothing to do here\n");
		
			return obj;
		}
		//2 cases, overflow there and not there

		if(node.n<(order-1))	//no overflow!
		{
			//printf("No overflow\n");
			int a;
			btree_node newnode;
			newnode.key=(int *)calloc(order-1,sizeof(int));
			newnode.child=(long *)calloc(order,sizeof(long));
			newnode.n = node.n+1;
			for(a=0;a<i;a++)
			{
				newnode.key[a]=node.key[a];
				newnode.child[a]=node.child[a];
			}
			newnode.key[i]=obj.key;
			newnode.child[i]=node.child[i];
			newnode.child[i+1]=obj.offset;
			for(a=i;a<node.n;a++)
			{
				newnode.key[a+1]=node.key[a];
				newnode.child[a+2]=node.child[a+1];
			}
			//newnode.child[a+2]=node.child[a];
			fseek(fp,offset,SEEK_SET);
			//printf("Writing to file now for nonleaf no overflow...\n");
			fwrite(&newnode.n,sizeof(int),1,fp);
			fwrite(newnode.key,sizeof(int),order-1,fp);
			fwrite(newnode.child,sizeof(long),order,fp);
			returnStructure temp;
			temp.valid=-1;
			return temp;

		}	//end of if no overflow
		else	//overflow
		{
			//printf("Overflow\n");
			int arrKey[order];
			long arrOffset[order+1];
			int a;
			for(a=0;a<i;a++)
			{
				arrKey[a]=node.key[a];
				arrOffset[a] = node.child[a];
			}
			arrKey[i]=obj.key;
			arrOffset[i]=node.child[i];
			arrOffset[i+1]=obj.offset;
			for(a=i;a<node.n;a++)
			{
				arrKey[a+1]=node.key[a];
				arrOffset[a+2]=node.child[a+1];
			}

			int m = order/2;
			//printf("M is %d\n",m );

			btree_node leftnode,rightnode;
			leftnode.n=0;
			leftnode.key=(int *)calloc(order-1,sizeof(int));
			leftnode.child=(long *)calloc(order,sizeof(long));	//number of keys+1 needed...
			rightnode.n=0;
			rightnode.key=(int *)calloc(order-1,sizeof(int));
			rightnode.child=(long *)calloc(order,sizeof(long));	//number of keys+1 needed...

			for(a=0;a<m;a++)
			{
				leftnode.key[a]=arrKey[a];
				leftnode.child[a]=arrOffset[a];
			}
			leftnode.child[a]=arrOffset[a];
			leftnode.n=m;
			int newn;
			for(a=m+1,newn=0;a<node.n+1;a++,newn++)
			{
				rightnode.key[newn]=arrKey[a];
				rightnode.child[newn]=arrOffset[a];
			}
			rightnode.child[newn]=arrOffset[a];
			rightnode.n=newn;

			fseek(fp,offset,SEEK_SET);
			fwrite(&leftnode.n,sizeof(int),1,fp);
			fwrite(leftnode.key,sizeof(int),order-1,fp);
			fwrite(leftnode.child,sizeof(long),order,fp);

			returnStructure objret;
			objret.valid=1;
			fseek(fp,0,SEEK_END);
			objret.offset=ftell(fp);
			objret.key=arrKey[m];

			fwrite(&rightnode.n,sizeof(int),1,fp);
			fwrite(rightnode.key,sizeof(int),order-1,fp);
			fwrite(rightnode.child,sizeof(long),order,fp);

			//printf("Being pushed up: %d\n",objret.key);


			return objret;



		}	//end of else overflow



	}


}



int find(int k, long offset)
{
	//printf("Find called\n");
	//return 1;

	btree_node findnode;
	findnode = readfromfile(offset);
	int i;
	//check node's data in a loop...if found return 1;
	for(i=0;i<findnode.n;i++)
	{
		//printf("In for loop\n");
		if(findnode.key[i]==k)	//found!
		{
			//printf("found\n");
			return 1;
		}
		if(k<findnode.key[i])
			break;
	}
	//printf("I is %d\n",i);
	//if not, then check if it has child; if no child return -1, if child, recursive call
	if(findnode.child[i]==0)
	{
		//printf("Child[%d] is 0",i);
		return 0;
	}
	else	find(k,findnode.child[i]);
}

void main(int parameterCount, char* argument[])
{
	

	if(parameterCount!=3)	//if command line input is wrong
	{
		//printf("Invalid input\n");
		return;
	}

	order = atoi(argument[2]);

	filename = argument[1];
	//printf("File is %s\n",filename);


	//Now to open/create file
	fp = fopen(filename,"r+b");

	if(fp!=NULL)
	{
		//printf("File exists!\n");
		fread(&root,sizeof(long),1,fp);
		//printf("Root index is %ld\n",root);
	}
	else
	{
		//printf("File does not exist...creating\n");
		root = -1;
		fp = fopen(filename,"w+b");
		fwrite(&root,sizeof(long),1,fp);
		btree_node rnode;
		rnode.n=0;
		rnode.key=(int *)calloc(order-1,sizeof(int));
		rnode.child=(long *)calloc(order,sizeof(long));	//number of keys+1 needed...
		root=ftell(fp);	//Updating root's location
		fwrite(&rnode.n,sizeof(int),1,fp);
		fwrite(rnode.key,sizeof(int),order - 1,fp);
		fwrite(rnode.child,sizeof(long),order,fp);
		//printf("Root node at start: n=%d key=%d child=%ld\n",rnode.n,*(rnode.key),*(rnode.child));
	}

	char userinput[1024], *temp;
	int k, choice;
	while(1)
	{
		choice=4;	//error
		//memset(userinput,0,30);
		fgets(userinput,1024,stdin);
		//printf("--Command is %s\n",userinput);
		if(!(strncmp(userinput,"print",5)))
			choice=2;
		else if(!(strncmp(userinput,"end",3)))
		{
			choice=3;
			break;
		}
		else
		{
			temp = strtok(userinput," ");
			//printf("--Part of command is %s\n",temp);
			if(!(strcmp(userinput,"add")))
			{
				choice=0;
				temp=strtok(NULL,"\n");
				//printf("--Number is %s\n",temp);
				k=atoi(temp);
				//printf("Number is %d\n",k);
			}
			else if(!(strcmp(userinput,"find")))
			{
				choice=1;
				temp=strtok(NULL,"\n");
				//printf("--Number is %s\n",temp);
				k=atoi(temp);
				//printf("Number is %d\n",k);
			}
		}
		//printf("Choice is %d\n",choice);

		switch(choice)
		{
			case 0:	//add
			//check if present!
				//int findRet = find(k);
			//if present, print
				if(find(k,root))
				{
					printf("Entry with key=%d already exists\n",k);
				}
				else //if not present, then add
				{
					returnStructure obj;
					obj = add(k,root);
					if(obj.valid!=-1)
					{
						btree_node newrootnode;
						newrootnode.n=1;
						newrootnode.key=(int *)calloc(order-1,sizeof(int));
						newrootnode.child=(long *)calloc(order,sizeof(long));	//number of keys+1 needed...
						newrootnode.key[0]=obj.key;
						newrootnode.child[0]=root;
						newrootnode.child[1]=obj.offset;
						fseek(fp,0,SEEK_END);
						root=ftell(fp);
						//printf("Root updated with %ld\n",root);
						fwrite(&newrootnode.n,sizeof(int),1,fp);
						fwrite(newrootnode.key,sizeof(int),order - 1,fp);
						fwrite(newrootnode.child,sizeof(long),order,fp);
					}
				}
				//fileprint();
				break;
			case 1:	//find
				if(find(k,root))
					{
						printf("Entry with key=%d exists\n",k);	 
					}
					else
					{
						printf("Entry with key=%d does not exist\n",k);
					}

				break;
			case 2:	//print
				printQ();
				break;
			default:
				break;
				//printf("Wrong input!\n");
		}


	}	//end of while!


	//Now write new root index to file and close!
	fseek(fp,0,SEEK_SET);
    fwrite(&root,sizeof(long),1,fp);
	close(fp);
}	//end main