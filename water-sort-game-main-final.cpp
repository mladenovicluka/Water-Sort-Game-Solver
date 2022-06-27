#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define N 10
#define X 300
#define Y 20
#define Z 2

struct stablo {
    struct stablo* sledeci;
    struct stablo* prethodni;
    int stanje[N][4];
    int brojkoraka;
    int pomeraj[2];
    int pokazivaci[20];
    int pobeda[2];
    int roditelj;

};
int is_solved(int a[N][4], int n, int k, int b[])
{
    int count = 0;

    for (int i = 0; i < n; ++i)
    {
        if (b[i] == -1)continue;
        if (b[i] > -1 && b[i] < 3)return 0;
        if (b[i] == 3)
        {
            int p = 0;
            for (int j = 0; j < 3; ++j)
            {
                if (a[i][j] == a[i][j + 1])p++;
            }
            if (p == 3)count++;
        }

    }
    return count == n - k;
}
int valid(int a[N][4], int n, int b[], int sa, int na)
{
    if (sa == na || sa >= n || na >= n)return 0;
    if (b[sa] == -1 || b[na] == 3)return 0;
    if (b[na] == -1)return 1;
    if (a[sa][b[sa]] != a[na][b[na]])return 0;

    return 1;
}
void presipaj(int a[N][4], int b[], int n, int sa, int na)
{
    int x = 3, y = 3;
    while (x == y && b[na] <= 2) {
        a[na][++b[na]] = a[sa][b[sa]];
        x = a[sa][b[sa]];
        a[sa][b[sa]--] = 0;
        y = a[sa][b[sa]];
    }
}
void napuni_boje(int a[], int n)
{
    for (int i = 0; i < n; ++i)a[i] = 4;
}
void napuni_paletu(int a[], int n)
{
    for (int i = 0; i < n; i++)a[i] = i + 1;
}
void set_random(int a[N][4], int n, int paleta[], int boje[], int b[],char *ime)
{
    FILE* fin;
    char imepom[100];
    memcpy(imepom, ime, 100);
    fin = fopen(strcat(imepom,".txt"), "r");
    fscanf(fin, "%d", &n);
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            fscanf(fin, "%d", &a[i][j]);
        }
        if (a[i][0])b[i] = 3;
        else b[i] = -1;
    }
}
void set(int svezna[X][Y][Z])
{
    for (int i = 0; i < X; ++i)
    {
        for (int j = 0; j < Y; ++j)
        {
            for (int k = 0; k < Z; ++k)
                svezna[i][j][k] = -1;
        }
    }
}
void printuj(int a[N][4], int n)
{
    printf("\n\n");

    for (int j = 3; j > -1; j--)
    {
        for (int i = 0; i < n; i++)
        {
            printf("|%d|\t", a[i][j]);

        }

        printf("\n");
    }


    printf("\n\n\n");
}
void set_vrhove(int a[], int n)
{
    for (int i = 0; i < n; ++i)a[i] = -1;

}
void kopiraj(int a[N][4], int n, int b[N][4])
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 4; j++) {
            b[i][j] = a[i][j];
        }
    }
}
void copy_vrhove(int a[], int n, int b[])
{
    for (int i = 0; i < n; i++) {
        b[i] = a[i];
    }
}
int dodaj_cvor(struct stablo* glavno, struct stablo root, int n, int k, int p, int counter[], int x, int i, int j, int svezna[X][Y][Z], int preth[])
{
    struct stablo pom;
    pom.pomeraj[0] = i;
    pom.pomeraj[1] = j;
    pom.pobeda[0] = -1;
    pom.pobeda[1] = -1;
    pom.brojkoraka = glavno[x].brojkoraka + 1;
    pom.roditelj = x;
    kopiraj(root.stanje, n, pom.stanje);
    copy_vrhove(root.pokazivaci, n, pom.pokazivaci);
    presipaj(pom.stanje, pom.pokazivaci, n, i, j);
    pom.prethodni = &glavno[x];
    root.sledeci = &pom;
    root = pom;
    int rebra = 0;
    if (is_solved(root.stanje, n, k, root.pokazivaci))
    {
        int brojac1 = 0;
        struct stablo curr = root;
        while (curr.prethodni)
        {
            int m = curr.pomeraj[0], nx = curr.pomeraj[1];
            svezna[preth[0]][brojac1][0] = m;
            svezna[preth[0]][brojac1][1] = nx;
            curr = *curr.prethodni;
            curr.pobeda[0] = m;
            curr.pobeda[1] = nx;
            brojac1++;
        }
        preth[0]++;
        rebra = 1;
    }

    glavno[counter[0]++] = pom;
    if (rebra)return 1;
    return 0;
}
void setujpotomke(struct stablo* glavno, struct stablo root, int n, int k, int p, int counter[], int x, int svezna[X][Y][Z], int preth[])
{
  
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (valid(root.stanje, n, root.pokazivaci, i, j))
            {
               
                int izaci=dodaj_cvor(glavno, root, n, k, p, counter, x, i, j, svezna, preth);
                if (izaci)return;
            }
        }
    }
}
void printtree(struct stablo* glavno, int dokle[], int n, int p)
{
    for (int i = 0; i < dokle[0]; i++)
    {
        printf("ID cvora :%d,nivo %d\n", i, glavno[i].brojkoraka);
        printuj(glavno[i].stanje, n);
    }
    for (int i = 0; i < dokle[0]; i++)
    {
        for (int j = 0; j < p - glavno[i].brojkoraka; j++)
        {
            printf("-");
        }
        if (i)
            printf("%d<-%d\n", i, glavno[i].roditelj);
        else
            printf("%d<-Koren\n", i);
    }
    printf("===========================================================================================\n");
    printf("\n\n\n\n\n\n\n\n\n");
}
void create_tree(struct stablo* glavno, int a[N][4], int n, int k, int p, int b[], int dokle[], int svezna[X][Y][Z], int preth[])
{
    int obradjenicvorovi = 0, svicvorovi[1] = { 1 };
    kopiraj(a, n, glavno[0].stanje);
    copy_vrhove(b, n, glavno[0].pokazivaci);
    glavno[0].brojkoraka = 0;
    glavno[0].sledeci = NULL;
    glavno[0].prethodni = NULL;
    glavno[0].pobeda[0] = -1;
    glavno[0].pobeda[1] = -1;
    struct stablo t;
    while (1)
    {
        if (obradjenicvorovi == svicvorovi[0])break;
        t = glavno[obradjenicvorovi];
        if (!(is_solved(glavno[obradjenicvorovi].stanje, n, k, glavno[obradjenicvorovi].pokazivaci) || glavno[obradjenicvorovi].brojkoraka == p))
            setujpotomke(glavno, t, n, k, p, svicvorovi, obradjenicvorovi, svezna, preth);
        if (is_solved(glavno[obradjenicvorovi].stanje, n, k, glavno[obradjenicvorovi].pokazivaci))
        {
            printf("-");


            break;
        }
        obradjenicvorovi++;
    }
    dokle[0] = obradjenicvorovi;
}
int poredi(int a[][4], int b[][4], int n)
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (a[i][j] != b[i][j])return 0;
        }
    }
    return 1;
}
int porediv2(int a[][2], int b[][2], int n)
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            if (a[i][j] != b[i][j])return 0;
        }
    }
    return 1;
}
void givehint(int n, int korak, int svezna[X][Y][Z], int preth[], int potezi[][2], int ako[2])
{
    printf("\n\n\n-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_\n\n\n");
    if (korak == 0)
    {
        ako[0] = svezna[0][korak][0];
        ako[1] = svezna[0][korak][1];
        printf("HINT %d - > %d\n", svezna[0][0][0], svezna[0][0][1]);
        printf("\n\n\n-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_\n\n\n");
    }
    else
    {
        for (int i = 0; i < preth[0]; ++i)
        {

            if (porediv2(potezi, svezna[i], korak))
            {
                ako[0] = svezna[i][korak][0];
                ako[1] = svezna[i][korak][1];
                printf("HINT %d - > %d\n", svezna[i][korak][0], svezna[i][korak][1]);
                printf("-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_\n\n");
                return;
            }
        }
    }
}
void reverse(int  svezna[X][Y][Z], int preth[])
{
    for (int i = 0; i < preth[0]; ++i)
    {
        int k = 0;
        while (svezna[i][k][0] != -1)
        {
            k++;
        }
        for (int j = 0; j < k / 2; j++)
        {
            int t1 = svezna[i][j][0], t2 = svezna[i][j][1];
            svezna[i][j][0] = svezna[i][k - j - 1][0];
            svezna[i][j][1] = svezna[i][k - j - 1][1];
            svezna[i][k - j - 1][0] = t1;
            svezna[i][k - j - 1][1] = t2;
        }
    }
}
void printresenja(int svezna[X][Y][Z], int preth[])
{
    for (int k = 0; k < preth[0]; k++)
    {
        printf("RESENJE BROJ %d\n\n", k + 1);
        int i = 0;
        while (svezna[k][i][0] != -1)
        {
            printf("Korak %d: Prespi sa %d na %d\n", i + 1, svezna[k][i][0], svezna[k][i][1]);
            i++;
        }
        printf("\n||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n\n");
    }
}

void igraj(int a[N][4], int n, int k, int b[], int p,char ime[100])
{
    int svezna[X][Y][Z];
    int preth[1] = { 0 };
    int dokle[1];
    struct stablo* glavno = (struct stablo*)malloc(99999999 * sizeof(struct stablo));
    int c[N][4];
    int koraci[20][2] = { {-1,-1} };
    kopiraj(a, n, c);
    int count = 0;
    int setovanjestabla = 0;
    if (is_solved(a, n, k, b))
    {
        printuj(a, n);
        printf("Igra je vec resena!\n");
        return;
    }
    while (1)
    {
        if (setovanjestabla)
        {
            printf("\n\n\nUspesno!\n\n\n");
            setovanjestabla = 0;
        }
        else
        {
            printf("\n\n");
            printuj(c, n);
        }
        printf("Jel hoces pomoc?\n");
        printf("1-Igram sam\n2-Moze hint\n3-Kreiranje stabla \n4-Ispis puta ka resenju\n5-Ispis svih resenja generisanih\n6-Ispis stabla level order!\n");
        int sta;
        scanf("%d", &sta);
        if (sta == 1)
        {
            if (count == p)
            {
                printf("NEMA VISE POTEZAAA!\n");
                break;
            }
            printf("Vas potez sa -> na: \n");
            int x, y;
            scanf("%d%d", &x, &y);
            if (valid(c, n, b, x, y))
            {
                presipaj(c, b, n, x, y);
                koraci[count][0] = x;
                koraci[count][1] = y;
            }
            else
            {
                printuj(c, n);
                printf("NIJE VALIDNO\n");
            }
            if (is_solved(c, n, k, b))
            {
                printuj(c, n);
                printf("*************************************************************************************\n");
                printf("*************************************************************************************\n");
                printf("*************************************************************************************\n");
                printf("*************************************************************************************\n");
                printf("*****************\t\t\tBRAVO!\t\t\t*********************\n");
                printf("*************************************************************************************\n");
                printf("*************************************************************************************\n");
                printf("*************************************************************************************\n");
                printf("*************************************************************************************\n");
                break;
            }
            count++;
        }
        if (sta == 2)
        {
            int ako[2] = { 0,0 };
            givehint(n, count, svezna, preth, koraci, ako);
            printf("Hoces da odigram umesto tebe ovaj potez 1-da 2-ne?\n");
            int dane;
            scanf("%d", &dane);

            if (dane == 1)
            {
                if (valid(c, n, b, ako[0], ako[1]))
                {
                    presipaj(c, b, n, ako[0], ako[1]);
                    koraci[count][0] = ako[0];
                    koraci[count][1] = ako[1];
                    count++;
                    printuj(c, n);
                }
                if (is_solved(c, n, k, b))
                {
                    printuj(c, n);
                    printf("*************************************************************************************\n");
                    printf("*************************************************************************************\n");
                    printf("*************************************************************************************\n");
                    printf("*************************************************************************************\n");
                    printf("*****************\t\t\tBRAVO!\t\t\t*********************\n");
                    printf("*************************************************************************************\n");
                    printf("*************************************************************************************\n");
                    printf("*************************************************************************************\n");
                    printf("*************************************************************************************\n");
                    break;
                }
            }
        }
        if (sta == 3)
        {
            setovanjestabla = 1;
            set(svezna);
            create_tree(glavno, a, n, k, p, b, dokle, svezna, preth);
            reverse(svezna, preth);
        }
        if (sta == 4)
        {

            FILE* fout;
            char imepom[100];
            memcpy(imepom, ime,100);
            fout = fopen(strcat(imepom,"-solved.txt"), "w");
            
            
            int res = 0;
            if (svezna[0][res][0] == -1)
            {
                printf("Nema resenja !\n");
            }
            else
            {
                int nasao = 0, izlaz = 1;

                for (int i = 0; i < preth[0]; ++i)
                {

                    if (porediv2(koraci, svezna[i], count))
                    {
                        res = count;
                        while (svezna[nasao][res][0] != -1)
                        {
                            printf("Korak %d: Prespi sa %d na %d\n", res + 1, svezna[i][res][0], svezna[i][res][1]);
                            fprintf(fout, "Step %d : From %d to %d\n", i + 1, svezna[i][res][0], svezna[i][res][1]);
                            res++;
                        }
                        izlaz = 0;
                        break;
                    }
                }
                if (izlaz)printf("Nema resenja iz te pozicije !!\n");
            }
            fclose(fout);
        }
        if (sta == 5)
        {
            if (svezna[0][0][0] == -1)
            {
                printf("Nema resenja !\n");
            }
            else
                printresenja(svezna, preth);
        }
        if (sta == 6)
        {
            printtree(glavno, dokle, n, p);
        }
        if (!sta || sta > 7)printf("NEVALIDAN UNOS \n");
    }
    exit(0);

}
int main()
{
    int n, k, p;
    printf("Unesi broj epruveta, broj praznih i broj poteza!\n");
    scanf("%d%d%d", &n, &k, &p);
    int dost_boje = n - k, kapacitet = 4;
    int matrix[N][4] = { 0 }, boje[N], paleta[N], pokazivaci[N];
    set_vrhove(pokazivaci, n);
    napuni_boje(boje, dost_boje);
    napuni_paletu(paleta, dost_boje);
    char ime1[100];
    scanf("%s", ime1);
   
    set_random(matrix, dost_boje, paleta, boje, pokazivaci,ime1);
    igraj(matrix, n, k, pokazivaci, p,ime1);
}
