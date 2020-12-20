/* 
A simple output prediction model using c prograing language.
This program needs a dataset file as an input where x and y values are present. 
During training process the program will fetch the x and y value from the dataset file and after doing analysis of x and y it will create a prediction model.
After the training process, when we give an unknown input (x) to the program, according to the model it will predict the output (y).
Reference: "Projection of Points Within a Given Range- A General Mathematical Equation" (www.amazon.com/author/animeshsahu)
*/
#include<stdio.h>
float findk(float,float,float,float);//calculate the slope k
int finde(float,float);//calculate total number of elements E (from referance)
int finda(float,float,int);//calculate multiplication factor A (from referance)
float findxp(float,int,int);//calculate the projected number (from referance)
void print_pre_nxt(float,float,float,float,float);
void error_dataset(void);
void know_details(void);

typedef struct stxy{//buffer structure
	float x;
	float y;
}xy;
typedef struct stxky{//model structure
	float x0;
	float y0;
	float k;
	float x1;
	float y1;
}xky;
//main function
void main(){
	int a;//a=multiplication factor
	unsigned int i=0,c,n,e;//i=model index,c=count,n=number of dataset,e=total number of elements
	float k,x,xp,y,dy,dk;//k=slope,x=input,xp=projected x,y=output,dy=change in y,dk=change in slope
	xy pre, nxt;//buffers
	printf("Do you want to know about the project in detail? type: 1 for yes, 0 for no --> ");//showing details
	scanf("%d",&c);
	if(c==1){
		know_details();
		printf("\n");
	}
	//printf("\n");
	FILE *fp=fopen ("dataset-xy.txt","r");//opening the dataset file
	if(fp!=0){
		//scaning the required inputs
		fscanf(fp,"%d",&n);//scanning total number of dataset present in the file
		printf("Enter the tolerance, small tolerance better predictions (recomended -1<dk<1)... dk= ");
		scanf("%f",&dk);
		if(dk<0){
			dk=-dk;
		}
		printf("The model is going to be trained by %d datasets...\n",n);
//preparing to train the model------------------------------------
		printf("Please wait... Preparing to train the model... !\n");
		n--;
		xky m[n];//model
		n--;
		//
		fscanf(fp,"%g",&pre.x);
		fscanf(fp,"%g",&pre.y);
		fscanf(fp,"%g",&nxt.x);
		fscanf(fp,"%g",&nxt.y);
		//
		k=findk(pre.x,pre.y,nxt.x,nxt.y);
		//copying data to model 0
		m[i].x0=pre.x;
		m[i].y0=pre.y;
		m[i].k=k;
		m[i].x1=nxt.x;
		m[i].y1=nxt.y;
		printf("\t\t");
		print_pre_nxt(pre.x,pre.y,nxt.x,nxt.y,k);
		c=0;
//training starting-----------------------------------
		printf("\nPlease wait... Training the model... !\n");
		iteration:
		//sleep(1);
		printf("Iteration- %d:\t",c);
		//shifting and fatching
		pre=nxt;
		fscanf(fp,"%g",&nxt.x);
		fscanf(fp,"%g",&nxt.y);

		k=findk(pre.x,pre.y,nxt.x,nxt.y);
		print_pre_nxt(pre.x,pre.y,nxt.x,nxt.y,k);
		
		//creating the model

		//updating the model
		//if(k==m[i].k){ // for exac match
		if((m[i].k) - dk <= k && k <= (m[i].k) + dk){ //match with tollerance(dk), small dk better prediction
			m[i].x1=nxt.x;
			m[i].y1=nxt.y;
		}
		else{
		//creating next model index
			i++;
			m[i].x0=pre.x;
			m[i].y0=pre.y;
			m[i].k=k;
			m[i].x1=nxt.x;
			m[i].y1=nxt.y;
		}

		c++;
		
		printf("%d %% completed.\n",c*100/n);//showing percentage completed
		if(c<n){ //(c < total data set - 2)
			goto iteration;//going for next iteration
		}

//training completed------------------------------------
		printf("Training of the model is successful... !\nSee the model bellow...\n");
		//printing the model
		for(c=0;c<=i;c++){
			printf("model[%d]= x0= %f\ty0= %f\tk= %f\tx1= %f\ty1= %f\n",c,m[c].x0,m[c].y0,m[c].k,m[c].x1,m[c].y1);
		}		
		printf("The model is ready to predict the result... !\n");
		predict:
		//taking input x
		printf("To predict the number y, enter the number x... x= ");
		scanf("%g",&x);
		
		e=finde(m[0].x0,m[i].x1);//total number of elements present within the model
		a=finda(x,m[i].x1,e);//multiplication factor
		//projecting x to xp		
		xp=findxp(x,a,e);
		//resolving the projection of x, because applied equation (in findxp function) is for int not for float
		if(xp<m[0].x0){	
			xp=m[0].x0;
		}
		if(m[i].x1<xp){
			xp=m[i].x1;
		}
		
		dy=(m[i].y1)-(m[0].y0);//change in y
		printf("Developer note: dy=%g ",dy);
		dy=dy*a;//total change in y according to x
		printf("e=%d a=%d x=%g xp=%g dy*a=%g\n",e,a,x,xp,dy);//just for developer info
		//predicting the y
		for(c=0;c<=i;c++){
			if(m[c].x0<=xp && xp<=m[c].x1){
				y=(m[c].k)*(xp - m[c].x0) + (m[c].y0);
				//modifying the y according to x
				y=y+dy;
				break;
			}
		}
		//printing the predicted result y for given x
		printf("Prediction:    -----> x= %g\t-----> y= %g\n",x,y);
		//mechanism to predict for anothen input x
		printf("Want to predict more? type: 1 for yes, 0 for no -> ");
		scanf("%d",&c);
		if(c==1){
			goto predict;
		}
		else{
		//message to user
			printf("Thank you... !\n");
		}	
	}
	else{
	//error if dataset file is not present
		error_dataset();
	}
}
/* function definations */
//calculate slope k
float findk(float x0,float y0,float x1,float y1){
	float k1,k2,k3;
	k1=y1-y0;
	k2=x1-x0;
	k3=k1/k2;
	return k3;
}
//calculate total number of elements e, according to reference (www.amazon.com/author/animeshsahu)
int finde(float l,float u){
	int rl,ru;
	rl=(int)l;
	ru=(int)u;
	//return ru-rl+1;
	return ru-rl;
}
//calculate multiplication factor a, according to referance
int finda(float x,float u,int e){
	float f1,f2;
	int a;
	f1=x-u;
	f2=f1/e;
	//ceil function
	a=(int)f2;
	if(f2>0){
		a++;
	}
	return a;
}
//projection of x, according to referance
float findxp(float x,int a,int e){
	int ae;
	float x1;
	ae=a*e;
	x1=x-ae;
	return x1;
}
//print buffer data x,y and slope k
void print_pre_nxt(float prex,float prey,float nxtx,float nxty,float k){
	printf("pre: x= %f\ty= %f\tnxt: x= %f\ty= %f\tk= %f\t",prex,prey,nxtx,nxty,k);
}
//printing details
void know_details(){
printf("--> This is a simple output prediction model using c programing language.\n");
printf("--> This program needs a dataset file (recomended: total number of datast present in the file, n>5) as an input where x and y values are present.\n");
printf("--> During the training process the program will fetch the x and y value from the dataset file and after doing analysis of x and y it will create a model.\n");
printf("--> After the training process, when we give an unknown input (x) to the program, according to the model it will predict the output (y).\n");
printf("--> This program asks amount of tolerance (dk) as an input from the user, best predictions can be made for tolerance dk=0, because for tolerance dk=0, maximum number of model can be achived.\n");
printf("--> Some concepts and equations are taken from the bellow reference...\nReference: \"Projection of Points Within a Given Range- A General Mathematical Equation\" (www.amazon.com/author/animeshsahu)\n");
}
//error message if dataset file is not present
void error_dataset(){
	printf("Error !!!  Please provide dataset file for training the model...\n");
	printf("Note 1: Dataset file name must be same as --> dataset-xy.txt \n");
	printf("Note 2: Dataset file must be present in the same directry where this program is present...\n");
	printf("Note 3: Dataset inside the file should be of following format...\n");
	printf("n\n");
	printf("x_1\ty_1\n");
	printf("x_2\ty_2\n");
	printf("x_3\ty_3\n");
	printf(" .\t .\n");
	printf(" .\t .\n");
	printf(" .\t .\n");
	printf("x_n\ty_n\n");
 	printf("\t\tWhere n is total_number_of_dataset_present_in_the_file\n");
 	printf("\t\tRecomended: n>5, more data better predictions...\n");
	printf("See the example bellow.......\n");
	printf("7\n");
	printf("-10.6\t2.27\n");
	printf("-1\t30.7\n");
	printf("3.8\t42.1\n");
	printf("5.55\t50\n");
	printf("9.2\t66.9\n");
	printf("15\t80\n");
	printf("21.2\t99.97\n");
	printf("\t\tIn this example n=7\n\t\tn is total_number_of_dataset_present_in_the_file\n");
	printf("Create the dataset file and try aganin...\nThank you...\n");
}
//end of the program
//
