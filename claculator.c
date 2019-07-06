/*realise Par Anas KASMI et Oussama SABRI*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#define n 10
typedef struct monome{
    struct monome* PGauche;
    int deg;
    float coef;
    struct monome* PDroite;
}monome;

typedef struct polynome{
monome *first;
monome *last;
int taille;
}polynome;
polynome* tab[n];
static int nump=-1;
//////////////prototypes//////////////
polynome* creat_polynome(void);
monome* creat_monome(void);
void insertion(polynome *, monome *);
int suppression(polynome *,int );
void afficher(polynome *);
void afficher_dec(polynome *);
void destruction(polynome *);
polynome* multiplie_scalaire(polynome *p,float scalaire);
polynome* multiplieParMonome(polynome* ,monome* );
polynome* somme(polynome *,polynome *);
polynome* soustraction(polynome *,polynome *);
polynome* produit(polynome *,polynome *);
polynome* derive(polynome *);
polynome* primitive(polynome *);
float image(polynome *p,float );
float integral(polynome *,float ,float );
void menu_principale(void);

//////////////////////////////////////
polynome* creat_polynome()
{
polynome *p;
p=(polynome*)malloc(sizeof(polynome));
p->taille=0;
p->first=NULL;
p->last=NULL;
nump++;
//printf("polynome have been created..\n");
return p;
}
monome* creat_monome(){
monome *m;
m = (monome*)malloc(sizeof(monome));
printf("enter coef: ");
scanf("%f",&m->coef);
printf("enter deg: ");
scanf("%d",&m->deg);
m->PGauche=NULL;
m->PDroite=NULL;
return m;
}

void insertion(polynome *L, monome *m)
{
    monome *p,*q;
    p=L->first;
    while(p!=NULL && p->deg!=m->deg)
    {
     p=p->PDroite;
    }
    if(p!=NULL)
    {
        p->coef=p->coef+m->coef;
        if(p->coef==0)
        {
            suppression(L,m->deg);
        }
    }
    else
    {
        p=(monome*)malloc(sizeof(monome));
        p->coef=m->coef;
        p->deg=m->deg;
        if(L->first==NULL)
        {
            p->PDroite=NULL;
            p->PGauche=NULL;
            L->first=p;
            L->last=p;
        }
        else
        {
            if(L->first->deg<m->deg)
            {
                p->PGauche=NULL;
                p->PDroite=L->first;
                L->first->PGauche=p;
                L->first=p;
            }
            else if(L->last->deg>m->deg)
            {
                p->PDroite=NULL;
                p->PGauche=L->last;
                L->last->PDroite=p;
                L->last=p;
            }
            else
            {
                q=L->first->PDroite;
                while(q->PDroite!=NULL && m->deg<q->deg)
                {
                    q=q->PDroite;
                }
                p->PDroite=q;
                p->PGauche=q->PGauche;
                q->PGauche->PDroite=p;
                q->PGauche=p;
            }
        }
    }
}

int suppression(polynome *p,int deg)
{

    monome *k;
    k=p->first;
    while(k!=NULL && deg != k->deg)
    {
        k=k->PDroite;
    }
    if(k==NULL)
    {
        return 0;
    }
    else{
        if(k->deg == p->first->deg && k->deg == p->last->deg)
        {
            p->first=NULL;
            p->last=NULL ;
            k= p->first  ;
        }
        else if(k->deg == p->first->deg && k->deg != p->last->deg)
        {
            k->PDroite->PGauche=NULL;

        }
        else if(k->deg == p->last->deg)
        {
            k->PGauche->PDroite=NULL;
        }
        else{
            k->PDroite->PGauche=k->PGauche;
            k->PGauche->PDroite=k->PDroite;
        }
        free(k);
        p->taille--;
        return 1;
    }

}
void afficher(polynome *p)
{
    monome *k;
    k=p->first;
    if(k==NULL)
    {
        printf("affichage :la liste est vide!\n");
    }
    else{
        while(k!=NULL)
        {
            if(k->coef!=0)
            {
                    if(k->deg==0)
                    {
                            if(k->PDroite!=NULL && k->PDroite->coef > 0)
                            {
                                    printf(" + ");
                            }
                            else if(k->PDroite!=NULL && k->PDroite->coef < 0)
                            {
                                    printf(" ");
                            }
                        printf("%.f",k->coef);
                    }
                    else if(k->deg==1){
                            if(k->PDroite!=NULL && k->PDroite->coef > 0)
                            {
                                    printf(" + ");
                            }
                            else if(k->PDroite!=NULL && k->PDroite->coef < 0)
                            {
                                    printf(" ");
                            }
                        printf("%.f(X)",k->coef);
                    }
                    else{
                        printf("%.2f(X)^%d",k->coef,k->deg);
                    }
                     if(k->PDroite!=NULL && k->PDroite->coef > 0)
                    {
                            printf(" + ");
                    }
                    else if(k->PDroite!=NULL && k->PDroite->coef < 0)
                    {
                            printf(" ");
                    }


            }k=k->PDroite;
        }
    }


}
void afficher_dec(polynome *p)
{
    monome *k;
    k=p->last;
    while(k!=NULL)
    {
        printf("%.2f(X)^%d",k->coef,k->deg);
        if(k->PGauche!=NULL && k->PGauche->coef > 0)
        {
            printf(" + ");
        }

        k=k->PGauche;
    }
}
//////////////////////
void destruction(polynome *p)
{
    monome* k;
    while(p->first!=NULL)
    {
        k=p->first;
        p->first=p->first->PDroite;
        free(k);
    }
    p->last=NULL;
    p->first=NULL;
    p->taille=0;
    printf("le polynome a ete supprimer\n");
}

polynome* multiplie_scalaire(polynome *p,float scalaire)
{
    polynome *pp=creat_polynome();
    monome* k=p->first,*j=(monome*)malloc(sizeof(monome));

    while(k!=NULL)
    {

        j->coef=k->coef*scalaire;
        j->deg=k->deg;
        insertion(pp,j);
        k=k->PDroite;
    }
    return pp;
}
//////////////////////
polynome* multiplieParMonome(polynome* p,monome* m)
{
    polynome *pp=creat_polynome();
    monome* k=p->first,*j=(monome*)malloc(sizeof(monome));
    while(k!=NULL)
    {
        j->coef=k->coef*m->coef;
        j->deg=k->deg+m->deg;
        insertion(pp,j);
        k=k->PDroite;
    }
    return pp;
}
//////////////////////

polynome* somme(polynome *p1,polynome *p2)
{
    polynome* p3=creat_polynome();
    monome* k=p1->first;
        while(k!=NULL)
        {
            insertion(p3,k);
            k=k->PDroite;
        }
    k=p2->first;

        while(k!=NULL)
        {
            insertion(p3,k);
            k=k->PDroite;
        }

        return p3;
}
//////////////////////
polynome* soustraction(polynome *p1,polynome *p2)
{
    polynome* p3=creat_polynome();
    monome* k=p1->first;
    monome*j=(monome*)malloc(sizeof(monome));
    while(k!=NULL)
    {
        insertion(p3,k);
        k=k->PDroite;
    }
    k=p2->first;
    while(k!=NULL)
    {
        j->deg=k->deg;
        j->coef=k->coef*(-1);
        insertion(p3,j);
        k=k->PDroite;
    }
    return p3;
}
//////////////////////
polynome* produit(polynome *p1,polynome *p2)
{
    monome *k;
    polynome *p3=creat_polynome();
    k=p2->first;
    if(k==NULL && p1->first!=NULL){
        return NULL;
    }
    else{
        while(k!=NULL)
        {
            if(somme(p3,multiplieParMonome(p1,k)) !=NULL)
            {
                p3=somme(p3,multiplieParMonome(p1,k));
            }
            k=k->PDroite;
        }
        return p3;
        }
}
//////////////////////
polynome* derive(polynome *p)
{
    polynome* pp=creat_polynome();
    monome *k=p->first;
    monome*j=(monome*)malloc(sizeof(monome));

    while(k!=NULL)
    {
        j->coef=k->coef*k->deg;
        j->deg=k->deg-1;
        insertion(pp,j);
        k=k->PDroite;
    }
    return pp;
}
//////////////////////
polynome* primitive(polynome *p)
{
    polynome *p2=creat_polynome();
    monome *k=p->first;
    monome *j=(monome*)malloc(sizeof(monome));
    while(k!=NULL)
    {
        if(k->deg!=-1)
        {
        j->coef=(k->coef/(1+k->deg));
        j->deg=k->deg+1;
        insertion(p2,j);
        k=k->PDroite;
        }
        else{
            printf("erreur \n");
            k=NULL;
        }

    }
    return p2;

}
//////////////////////////////////////////
float image(polynome *p,float x)
{
    monome *k=p->first;
    float s=0;
    while(k!= NULL)
    {
        s=s+k->coef*pow(x,k->deg);
        k=k->PDroite;
    }
    return s;
}
/////////////////////////////
float integral(polynome *p,float x,float y)
{

	if(p->first!=NULL)
	{
	return image(primitive(p),x)-image(primitive(p),y);
	}
	else{
	printf("la liste est vide \n");
	return 0;
	}
}
//////////////////////////////////////
////////  menu principale   //////////
//////////////////////////////////////
void menu_principale()
{
    int x=40,y=9,i,k,j,deg,choix;
    float A,B;
    gotoxy(x-2,y-1);
    for(i=0;i<32;i++)
        printf("%c",205);
    system("COLOR 0B");
    gotoxy(x-2,y-1);
    printf("%c",201);
    gotoxy(x+29,y-1);
    printf("%c",187);
    gotoxy(x+7,y);
    printf("MENU PRINCIPALE\n");

    gotoxy(x-2,++y);
    for(i=0;i<32;i++)
        printf("%c",205);
    system("COLOR 0B");
    gotoxy(x-2,y);
    printf("%c",201);
    gotoxy(x+29,y);
    printf("%c",187);

    gotoxy(x,++y);
    printf("1-  cree un polynome\n");
    gotoxy(x,++y);
    printf("2-  L'insertion\n");
    gotoxy(x,++y);
    printf("3-  La Suppression\n");
    gotoxy(x,++y);
    printf("4-  Affichage (croissant/decroissant) \n" );
    gotoxy(x,++y);
    printf("5-  La somme\n");
    gotoxy(x,++y);
    printf("6- L'integrale\n");
    gotoxy(x,++y);
    printf("7-  Multiplication (scalaire/monome)\n");
    gotoxy(x,++y);
    printf("8- La soustraction\n");
    gotoxy(x,++y);
    printf("9- Le produit\n");
    gotoxy(x,++y);
    printf("10- La derivee\n");
    gotoxy(x,++y);
    printf("11- La primitive\n");
    gotoxy(x,++y);
    printf("12- L'image\n");
    gotoxy(x,++y);
    printf("13-  La destruction\n");


    gotoxy(x,y+2);
    printf("VOTRE CHOIX :");
    gotoxy(x+13,y+2);
    scanf("%d",&choix);
    switch(choix) {
                case 1 :
                    clrscr();
                    tab[nump]=creat_polynome();
                    printf("le polynome num :%d a ete cree \n",nump);
                    getch();
                    clrscr();
                    menu_principale();

                 break;
                 case 2 :
                    do{
                    clrscr();
                    printf("entrer le numero de polynome : ");
                    scanf("%d",&k);
                    }while(k>nump);
                    insertion(tab[k],creat_monome());
                    printf("l'opperation sur le polynome num : %d ete bien effectue",k);
                    getch();
                    clrscr();
                    menu_principale();

                 break;
                 case 3 :
                     do{
                    clrscr();
                    printf("entrer le numero de polynome : ");
                    scanf("%d",&k);
                    }while(k>nump);
                    printf("entrer le degre : ");
                    scanf("%d",&deg);
                    if( suppression(tab[k],deg) ==1)
                        printf("le monome avec deg: %d, du polynome num : %d a ete supprimer",deg,k);
                    else
                        printf("monome avec deg: %d n'existe pas ",deg);
                    getch();
                    clrscr();
                    menu_principale();
                 break;
                 case 4 :
                    do{
                    x=40;y=9;
                    clrscr();
                    printf("entrer le numero de polynome : ");
                    scanf("%d",&k);
                    }while(k>nump);
                    clrscr();
                    gotoxy(x-2,y-1);
                    for(i=0;i<32;i++)
                            printf("%c",205);
                    system("COLOR 0B");
                    gotoxy(x-2,y-1);
                    printf("%c",201);
                    gotoxy(x+29,y-1);
                    printf("%c",187);
                    gotoxy(x+7,y);
                    printf("affichage\n");
                    gotoxy(x-2,++y);
                    for(i=0;i<32;i++)
                        printf("%c",205);
                    system("COLOR 0B");
                    gotoxy(x-2,y);
                    printf("%c",201);
                    gotoxy(x+29,y);
                    printf("%c",187);
                    gotoxy(x,++y);
                    printf("1-  affichage croissant\n");
                    gotoxy(x,++y);
                    printf("2-  affichage decroissance\n");
                    gotoxy(x,y+2);
                    printf("VOTRE CHOIX :");
                    gotoxy(x+13,y+2);
                    scanf("%d",&choix);
                    clrscr();

                    if(choix==1)
                    {
                        afficher(tab[k]);
                        getch();
                        clrscr();

                    }
                    else if(choix==2)
                    {
                        afficher_dec(tab[k]);
                        getch();
                        clrscr();
                    }
                    clrscr();
                    menu_principale();
                 break;
                 case 5 :
                    do{
                    clrscr();
                    printf("entrer le numero du 1er polynome : ");
                    scanf("%d",&k);
                    printf("\nentrer le numero du 2eme polynome : ");
                    scanf("%d",&j);
                    }while(k>nump && j>nump);
                    nump++;
                    tab[nump]=somme(tab[k],tab[j]);
                    getch();
                    clrscr();
                    printf("la somme des deux polynome est le polynome num: %d \n",nump);
                    afficher(tab[nump]);
                    getch();
                    clrscr();
                    menu_principale();
                 break;
                 case 6 :
                    do{
                    clrscr();
                    printf("entrer le numero de polynome : ");
                    scanf("%d",&k);
                    }while(k>nump);
                    printf("entrer le point x : ");
                    scanf("%f",&A);
                    printf("entrer le point y : ");
                    scanf("%f",&B);
                    clrscr();
                    printf("%.2f est lintegrale de la fonction num: %d",integral(tab[k],A,B),k);
                    getch();
                    clrscr();
                    menu_principale();
                 break;
                 case 7 :
                     x=40;y=9;
                    clrscr();
                    gotoxy(x-2,y-1);
                    for(i=0;i<32;i++)
                            printf("%c",205);
                    system("COLOR 0B");
                    gotoxy(x-2,y-1);
                    printf("%c",201);
                    gotoxy(x+29,y-1);
                    printf("%c",187);
                    gotoxy(x+7,y);
                    printf("multiplication\n");
                    gotoxy(x-2,++y);
                    for(i=0;i<32;i++)
                        printf("%c",205);
                    system("COLOR 0B");
                    gotoxy(x-2,y);
                    printf("%c",201);
                    gotoxy(x+29,y);
                    printf("%c",187);
                    gotoxy(x,++y);
                    printf("1-  multiplication par scalaire\n");
                    gotoxy(x,++y);
                    printf("2-  multiplication par monome\n");
                    gotoxy(x,y+2);
                    printf("VOTRE CHOIX :");
                    gotoxy(x+13,y+2);
                    scanf("%d",&choix);
                    clrscr();
                    if(choix==1)
                    {
                        do{
                            clrscr();
                            printf("entrer le numero de polynome : ");
                            scanf("%d",&k);
                        }while(k>nump);
                        printf("entrer le scalaire \n");
                        scanf("%f",&A);
                        clrscr();
                        printf("la multiplication de %.2f avec le polynome num: %d est la polynome  :\n",A,nump);
                        afficher( multiplie_scalaire(tab[k],A ) );
                        getch();
                        clrscr();
                        menu_principale();

                    }
                    else if(choix==2)
                    {
                        do{
                            clrscr();
                            printf("entrer le numero de polynome : ");
                            scanf("%d",&k);
                        }while(k>nump);
                        printf("entrer le monome \n");
                        clrscr();
                        printf("la multiplication avec le polynome num: %d donnera la polynome  :\n",nump);
                        afficher(multiplieParMonome(tab[k],creat_monome()) );
                        getch();
                        clrscr();
                        menu_principale();
                    }
                    clrscr();
                    menu_principale();
                 break;
                 case 8 :
                     do{
                    clrscr();
                    printf("entrer le numero du 1er polynome : ");
                    scanf("%d",&k);
                    printf("entrer le numero du 2eme polynome : ");
                    scanf("%d",&j);
                    }while(k>nump && j>nump);
                        afficher(soustraction(tab[k],tab[j]) );
                        getch();
                        clrscr();
                        menu_principale();
                 break;
                 case 9 :
                     do{
                    clrscr();
                    printf("entrer le numero du 1er polynome : ");
                    scanf("%d",&k);
                    printf("entrer le numero du 2eme polynome : ");
                    scanf("%d",&j);
                    }while(k>nump && j>nump);
                    clrscr();
                    printf("le produit des deux polynomes est : ");
                    afficher(produit(tab[k],tab[j]) );
                    getch();
                    clrscr();
                    menu_principale();
                 break;
                 case 10 :
                    do{
                    clrscr();
                    printf("entrer le numero du polynome : ");
                    scanf("%d",&k);
                    }while(k>nump);
                    afficher(derive(tab[k]) );
                    getch();
                    clrscr();
                    menu_principale();
                 break;
                 case 11 :
                    do{
                    clrscr();
                    printf("entrer le numero du polynome : ");
                    scanf("%d",&k);
                    }while(k>nump);
                    afficher(primitive(tab[k]));
                    getch();
                    clrscr();
                    menu_principale();
                 break;
                 case 12 :
                     do{
                    clrscr();
                    printf("entrer le numero du polynome : ");
                    scanf("%d",&k);
                    }while(k>nump);
                    printf("Entrer un nombre");
                    scanf("%f",&A);
                    printf("l'image est : %.2f\n",image(tab[k],A));
                    getch();
                    clrscr();
                    menu_principale();
                 break;
                 case 13 :
                    do{
                    clrscr();
                    printf("entrer le numero du polynome : ");
                    scanf("%d",&k);
                    }while(k>nump);
                    destruction(tab[k]);
                    //tab[nump]=NULL;
                    for(A=k;A<nump;A++)
                    {
                        tab[A]=tab[A+1];
                    }
                    nump--;
                    printf("votre polynome a ete bien detruit");
                    getch();
                    clrscr();
                    menu_principale();
                 break;
                 case 14 :
                    clrscr();
                    menu_principale();
                 break;
                default :
                clrscr();
                menu_principale();
                system("COLOR 0C");
                gotoxy(x,y+1);
                for(i=0;i<34;i++)
                    printf("%c",205);
                gotoxy(x,y+1);
                printf("%c",200);
                gotoxy(x+34,y+1);
                printf("%c",188);
                gotoxy(x+1,y);
                printf(" votre choix n'est pas correct ! \n");
                getch();
                system("COLOR 0B");
                clrscr();
                menu_principale();
                }


}
//////////////////////
int main()
{
    menu_principale();
    return 0;
}
