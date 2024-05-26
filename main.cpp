#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<math.h>

struct Info
{
    uint32_t total_count:10;
    int32_t alto:11;
    int32_t ancho:11;
};

struct Header
{
    struct Info info;
    int32_t start_index_1;
    int32_t start_index_2;
};
struct Data
{
    float voltage;
    float current;
    float radiation;
    int32_t next_index;
};
int main()
{
    FILE *f=fopen("pv_data.dat","rb");
    FILE *txt1 =fopen("panel_1.txt","wb");
    FILE *txt2 =fopen("panel_2.txt","wb");
    struct Header header;
    struct Info info;
    /* int cant;
    fseek(f,0,SEEK_END);
    cant=(ftell(f)/sizeof(struct Header)); //Cantidad de datos
    printf("\n\nCantidad de paquetes:%d\n\n",cant);
    fseek(f,0,SEEK_SET);
    */
    
    fread(&header,sizeof(struct Header),1,f);
    printf("Cantidad de paquetes:%d\n\n",header.info.total_count);
    struct Data *data=new Data[header.info.total_count];
    fread(data,sizeof(struct Data),header.info.total_count,f);

    struct Data *panel_1=new Data[header.info.total_count/2];
    struct Data *panel_2=new Data[header.info.total_count/2];
    int32_t aux1,aux2,aux3;

    for(int i=0;i<header.info.total_count;i++)
    {
        if(i==0)
        {
            panel_1[i]=data[header.start_index_1];
            aux1=data[header.start_index_1].next_index;
            aux2=aux1;

            panel_2[i]=data[header.start_index_2];
            aux3=data[header.start_index_2].next_index;
        }
        else if(aux2 !=-1)//Distinto (!=)
        {
            aux1=aux2;
            panel_1[i]=data[aux1];
            aux2=data[aux1].next_index;

            panel_2[i]=data[aux3];
            aux3=data[aux3].next_index;
        }
    }
    fprintf(txt1,"Panel 1\n\n");
    for (int i = 0; i < (header.info.total_count/2); i++)
    {
        fprintf(txt1,"[%d]\n",i);
        fprintf(txt1,"Voltage:%.2f v\n",panel_1[i].voltage);
        fprintf(txt1,"Current:%.2f Amper\n",panel_1[i].current);
        fprintf(txt1,"Radiation:%.2f w/m^2\n",panel_1[i].radiation);
        fprintf(txt1,"\n---------------------------\n");
    }

      fprintf(txt1,"Panel 2\n\n");
    for (int i = 0; i < (header.info.total_count/2); i++)
    {
        fprintf(txt2,"[%d]\n",i);
        fprintf(txt2,"Voltage:%.2f v\n",panel_2[i].voltage);
        fprintf(txt2,"Current:%.2f Amper\n",panel_2[i].current);
        fprintf(txt2,"Radiation:%.2f w/m^2\n",panel_2[i].radiation);
        fprintf(txt2,"\n---------------------------\n");
    }
    
    double *rendi_1=new double[header.info.total_count/2];
    double *rendi_2=new double[header.info.total_count/2];
    double area,max_r1,max_r2,min_r1,min_r2,suma_r1,suma_r2,media_r1,media_r2;

    area=(double)(header.info.alto*header.info.ancho)/1000000;
    printf("Area del Panel:%lfm^2 \n",area);

    for (int i=0; i<header.info.total_count/2; i++)
    {
        rendi_1[i]=((panel_1[i].voltage*panel_1[i].current)/(area*panel_1[i].radiation))*100;
        suma_r1=suma_r1+rendi_1[i];

        rendi_2[i]=((panel_2[i].voltage*panel_2[i].current)/(area*panel_2[i].radiation))*100;
        suma_r2=suma_r2+rendi_2[i];

        if(i==0)
        {
            max_r1=rendi_1[i];
            max_r2=rendi_2[i];
            min_r1=rendi_1[i];
            min_r2=rendi_2[i];
        }
        if(rendi_1[i]>max_r1)
        {
            max_r1=rendi_1[i];
        }
        else if(rendi_1[i]<min_r1)
        {
            min_r1=rendi_1[i];
        }
        if(rendi_2[i]>max_r2)
        {
            max_r2=rendi_2[i];
        }
        else if(rendi_2[i]<min_r2)
        {
            min_r2=rendi_2[i];
        }
    }
    media_r1=suma_r1/(header.info.total_count/2);
    media_r2=suma_r2/(header.info.total_count/2);
    
    printf("Rendimiento del Panel 1: \n Rendimiento Maximo:%lf \t Rendimiento Minimo:%lf \t Media:%lf \n",max_r1,min_r1,media_r1);
    printf("Rendimiento del Panel 2: \n Rendimiento Maximo:%lf \t Rendimiento Minimo:%lf \t Media:%lf \n",max_r2,min_r2,media_r2);


    delete[] panel_1;
    delete[] panel_2;
    delete[] data;
    fclose(f);
    fclose(txt1);
    fclose(txt2);

}

