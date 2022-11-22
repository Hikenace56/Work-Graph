/*-------------------------------------------------
 Description: Calcule le changement du travail qu’exerce la force avec la méthode numérique de la règle trapézoïdale.
-------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <gng1106plplot.h> // Donne des définitions pour utiliser la librarie PLplot
#define VRAI 1
#define FAUX 0
// Les constantes symboliques
#define NBR_POINT_COURBE  5000// Nombre de points à tracer
#define NBR_POINT_TRAP 1000// Nombre de points pour la méthode trapézoidale
#define N 5 // Ordre maximal des polynômes
#define FICHIER "donneesutilisateur.bin" // Nom du fichier contenant les entrées de l’utilisateur
// Définition de stucture

typedef struct
{
double x0 ;// la position initiale
double xf ;//  la position finale
double force[N+1]; // contient les coefficients de la fonction F(x)
double angle[N+1] ;// contient les coefficients de la fonction θ(x)

    } DONNEE; //contient les entrées de l’utilisateur

// Prototypes de fonction

void obtientEntree(DONNEE *Ptr);
double getValeurPositive();
void utiliserDonnees(DONNEE tbl[]);
void sauverDonnees(DONNEE tbl[]);
void calcultravailTotal(DONNEE *fPtr, double tbl[][NBR_POINT_COURBE], double pas);
double calcultravaillocal(double x, DONNEE *fPtr);
double calculePolynome(double x, double tbl[]);
void plot(double tbl1[], double tbl2[], int, char [], char[]);
double getMinDouble(double tbl[], int );
double getMaxDouble(double tbl[], int );
int estvide(DONNEE );
int estzero(double [], int);

void main()
{
 DONNEE donnee;
 DONNEE tbldonnee[6]={0};
 utiliserDonnees(tbldonnee);
 int ix,jx,nbr;
 char c;
 double pas,x;
 double tbltracer[2][NBR_POINT_COURBE], angle[NBR_POINT_COURBE], force[NBR_POINT_COURBE],position[NBR_POINT_COURBE];
 double (*tPtr)[2][NBR_POINT_COURBE];
 double (*tPtr2)[NBR_POINT_COURBE];

   printf("Do you want to use your saved data? \n");
   fflush(stdin);
   fflush(stdout);
   scanf("%d", &nbr);
   if(nbr == 1)
   {
     if(estvide(tbldonnee[5])!=1)
 {
     nbr=0;
     printf("Which data set do you wish to use? \n");
     for(ix=0;ix<5;ix=ix+1)
     {
       printf("\n%d) Initial position:%lf  Final position: %lf \n", ix+1, tbldonnee[ix].x0, tbldonnee[ix].xf);
       printf("Force coefficient                     Angle coefficient: \n");
       for(jx=0;jx<=N;jx=jx+1)
       {
           printf("%30.10lf, %30.10lf \n",tbldonnee[ix].force[jx],tbldonnee[ix].angle[jx]);
       }

       }

     scanf("%d", &nbr);
     while(nbr<1||nbr>5){printf("Enter a valid number");scanf("%d", &nbr);}
     donnee=tbldonnee[nbr-1];
     pas=(donnee.xf-donnee.x0)/NBR_POINT_COURBE;
     tPtr=&tbltracer;
     tPtr2=tbltracer;
     calcultravailTotal(&donnee,tbltracer,pas);
     plot(tPtr2+1, tPtr2, NBR_POINT_COURBE, "work", "position");
   }
   else
   {
       printf("You don't have any saved data \n");
       do
 {
     obtientEntree(&donnee);
     pas=(donnee.xf-donnee.x0)/NBR_POINT_COURBE;
     tPtr=&tbltracer;
     tPtr2=tbltracer;
     nbr=0;
     for(ix=0;ix<NBR_POINT_COURBE;ix=ix+1)
        {
            x=donnee.x0+ix*pas;
            position[ix]=x;
            force[ix]=calculePolynome(x,donnee.force);
            angle[ix]=calculePolynome(x,donnee.angle);
            if(calculePolynome(x,donnee.force)<0 || calculePolynome(x,donnee.angle)<0 || calculePolynome(x,donnee.angle)> M_PI/2);
        }
    if(nbr==1)
 {
     printf("Data is unvalid \n");
     plot(force,position, NBR_POINT_COURBE,"force","position");
     plot(angle,position, NBR_POINT_COURBE,"angle","position");
 }
 }while(nbr==1);
 calcultravailTotal(&donnee,tbltracer,pas);
 plot(tPtr2+1, tPtr2, NBR_POINT_COURBE, "work", "position");
 fflush(stdin);
 printf("Do you wish to save your data? Answer y/n");
 scanf("%c",&c);
 if(c!="n")
 {
     if(tbldonnee[5].x0>=5) printf("You can't add more data");
     else
        {
            tbldonnee[(int)tbldonnee[5].x0]=donnee;
            tbldonnee[5].x0+=1;
            sauverDonnees(tbldonnee);

        }
}}
   }

    else
 {
     do
 {
     obtientEntree(&donnee);
     pas=(donnee.xf-donnee.x0)/NBR_POINT_COURBE;
     tPtr=&tbltracer;
     tPtr2=tbltracer;
     nbr=0;
     for(ix=0;ix<NBR_POINT_COURBE;ix=ix+1)
        {
            x=donnee.x0+ix*pas;
            position[ix]=x;
            force[ix]=calculePolynome(x,donnee.force);
            angle[ix]=calculePolynome(x,donnee.angle);
            if(calculePolynome(x,donnee.force)<0 || calculePolynome(x,donnee.angle)<0 || (calculePolynome(x,donnee.angle)>= M_PI/2-0.001 && calculePolynome(x,donnee.angle)<= M_PI/2+0.001) || calculePolynome(x,donnee.angle)>= M_PI/2);
        }
    if(nbr==1)
 {
     printf("Data is unvalid \n");
     plot(force,position, NBR_POINT_COURBE,"force","position");
     plot(angle,position, NBR_POINT_COURBE,"angle","position");
 }
 }while(nbr==1);
 calcultravailTotal(&donnee,tbltracer,pas);
 plot(tPtr2+1, tPtr2, NBR_POINT_COURBE, "work", "position");
 fflush(stdin);
 printf("Do you want to save your data? Answer 1 for yes 0 for no \n");
 scanf("%d",&nbr);
 if(nbr!=0)
 {
     if(tbldonnee[5].x0>=5) printf("You can't add more entries \n");
     else
        {
            tbldonnee[(int)tbldonnee[5].x0]=donnee;
            tbldonnee[5].x0+=1;
            sauverDonnees(tbldonnee);
        }
}
}
 }






void obtientEntree(DONNEE *fPtr)
{
    double temp;
    // Obtenir la position initiale et finale
    do
    {
        printf("Enter initial position \n");
        fPtr->x0 = getValeurPositive();
        printf("Enter final position \n");
        fPtr->xf = getValeurPositive();
        if(fPtr->x0>=fPtr->xf) printf("Initial position should be inferior to final position \n");
    }while(fPtr->x0>=fPtr->xf);
    // Obtenir les coefficients de la fonctipn F(x)
    int ix, drapeau;
    double tempo;
    for(ix=0;ix<N+1;ix=ix+1)
{
      fflush(stdin);
      printf("Enter coefficient %d of force ", ix+1);
      scanf("%lf", &tempo);
      fPtr->force[ix]=tempo;

}
for(ix=0;ix<N+1;ix=ix+1)
{
    printf("Enter coefficient %d of angle ", ix+1);
    scanf("%lf",&temp);
    fPtr->angle[ix]=temp;
}
}

double getValeurPositive()
{
    double valeur; // valeur donnée par l'utilisateur.
    do
    {
        fflush(stdin);
        scanf("%lf",&valeur);
        if(valeur < 0.0)
            printf("Must be greater than 0.\n");
    }
    while(valeur < 0.0);
    return(valeur);
}

int estvide(DONNEE a)
{
    int ix,drap=0;
    for(ix=0;ix<NBR_POINT_COURBE;ix=ix+1)
    {
         if(a.force[ix]!=0 ||a.angle[ix]!=0 ) drap=1;
    }
    if(a.x0 ==0&& a.xf ==0&& drap==0)
        return(1);
    else
        return(0);
}
double getMaxDouble(double tbl[], int n)
{
   // Variable declarations
   double max;  // for storing maximum value
   int ix;      // indexing into an array
   // Instructions
   max = -DBL_MAX;  // most negative value for type double
   for(ix = 0; ix < n; ix = ix + 1)
   {
       if(max < tbl[ix]) max = tbl[ix];
   }
   return(max);
}


double calculePolynome(double x, double tbl[])
{
   double p=0.0;
   int ix;
   for(ix=0;ix<=N;ix=ix+1)
   {
    p=p+tbl[ix]*pow(x,N-ix);
    }
    return(p);
}

double calcultravaillocal(double x, DONNEE *fPtr)
{
  double t;
  double h;
  double yi;
  double somme=0.0;
  int ix;
  h=(x-fPtr->x0)/NBR_POINT_TRAP;
  yi=fPtr->x0+h;
  for(ix=1;ix<NBR_POINT_TRAP;ix=ix+1)
  {
  somme=somme+calculePolynome(yi,fPtr->force)*cos(calculePolynome(yi,fPtr->angle));
  yi=yi+h;
  }
  t=h/2*(calculePolynome(fPtr->x0,fPtr->force)+calculePolynome(x,fPtr->force)+2*somme);
  return(t);
}

void  calcultravailTotal(DONNEE *fPtr, double tbl[2][NBR_POINT_COURBE], double pas)
{
  double x=fPtr->x0;
  int ix;
  for(ix=0;ix<NBR_POINT_COURBE;ix=ix+1)
  {
    tbl[0][ix]=x;
    tbl[1][ix]=calcultravaillocal(x, fPtr);
    x=x+pas;
   }
}


void plot(double tbl1[NBR_POINT_COURBE], double tbl2[NBR_POINT_COURBE], int nbr_points,char y[], char x[])

{
    // Variable declaration
    double maxf, minf;  // Minimum and maximum values of f(x)
    double maxx, minx;
    // Setup plot configuration
    plsdev("wingcc");  // Sets device to wingcc - CodeBlocks compiler
    // Initialize the plot
    plinit();
    // Configure the axis and labels
    plwidth(3); // select the width of the pen
    minf = getMinDouble(tbl1,nbr_points);
    maxf = getMaxDouble(tbl1, nbr_points);
    minx = getMinDouble(tbl2,nbr_points);
    maxx = getMaxDouble(tbl2, nbr_points);
    if(minf==maxf)
    {
        maxf=minf+1;
        minf=minf-1;
    }
    plenv(minx ,maxx ,minf, maxf, 0, 0);
    plcol0(GREEN);           // Select color for labels
    char label[100];
    strcpy(label, "Graph of ");
    strcat(label, y);
    strcat(label, " versus ");
    strcat(label, x);
    pllab(("%s",x),("%s",y), label);
        // Plot the function.
    plcol0(BLUE);    // Color for plotting curve
    plline(nbr_points, tbl2, tbl1);
    plend();
}

double getMinDouble(double tbl[], int n)
{
   // Variable declarations
   double min;  // for storing minimum value
   int ix;      // indexing into an array
   // Instructions
   min = DBL_MAX;  // most positive value for type double
   for(ix = 0; ix < n; ix = ix + 1)
   {
       if(min > tbl[ix]) min = tbl[ix];
   }
   return(min);
}



void sauverDonnees(DONNEE tbl[])
{
 FILE *Ptr;

  Ptr=fopen(FICHIER, "wb");
    if(Ptr == NULL)
        printf("Can't open file %s\n", FICHIER);
    else
  {
   fwrite(tbl, sizeof(DONNEE), 6, Ptr);
   fclose(Ptr);
  }
}

void utiliserDonnees(DONNEE tbl[])
{
 FILE *Ptr;
 Ptr=fopen(FICHIER, "rb");
    if(Ptr == NULL)
        printf("Can't open file %s\n", FICHIER);
    else
  {
   fread(tbl, sizeof(DONNEE), 6, Ptr);
   fclose(Ptr);
  }
}
int estzero(double tbl[], int n)
{
    int i;
    for(i=0;i<n;i=i+1)
    {
        if(tbl[i]!=0) return(0);
    }
    return(1);
}

