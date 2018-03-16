//bitmap的作用是标记nvm上的对应字节是否被使用,'1'表示已经使用过‘0’表示没有

#include<stdio.h>
#include<time.h>
#include<stdlib.h>

#define MAXFILESIZE 15//单个文件最多可以占用的内存字节数
#define NVMSIZE    500//nvm的总字节

typedef struct nvm{
int file_belongs_to;//该数据所属文件用数字标号来区分不同的文件而不用文件名
int writecounter;//记录写的次数
int next;//所在文件的下一个字节的位置（小于NVMSIZE）
} nvm;//该结构体记录nvm上每一个字节的信息


typedef struct file{
int fileid;//文件的编号，从1开始
int fllesize;//文件的大小
int firstwordposition;//文件起始位置
} file;//文件元数据

/*
int  write_nvm(nvm*device,int head,int fileoffset){//对某个文件的某个字节进行修改，first是文件的头，offset是该字节的偏移量
 int i=0;
 int temp=head;
 while(temp<NVMSIZE){
    if(i<fileoffset) {temp=(device[temp]).next;
                    i++;}
    else   {device[temp].writecounter++;return 1;}
   }
 return 0;
}
*/

/*
void operate (file objectivefile,int operationtimes){//修改某个文件，修改位置修改次数都是随机的
int offset;
while(operationtimes>0){
   offset=rand()%(objectivefile.fllesize);//随机偏移量0~（filesize-1）
    write_nvm(objectivefile.firstwordposition,offset);
    operationtimes--;
   }
}
*/

void getfileinfo(nvm*device,int fileid,file*filelist){
int position=filelist[fileid-1].firstwordposition;
printf("info of file%d :",fileid);
while(position<NVMSIZE){printf("--%d",position);position=device[position].next;}
printf("\n");
}//根据文件id取得文件的所有位置

void getbyteinfo(nvm*device,int position){
printf("fileid=%3d,next=%3d,writetime=%d\n",device[position].file_belongs_to,device[position].next,device[position].writecounter);
}

void initialize_device(nvm*device,int sizeofdevice,char*bitmap){//初始化nvm
int i;
for(i=0;i<sizeofdevice;i++){bitmap[i]='0';
                            device[i].writecounter=0;
                            device[i].next=NVMSIZE;
                            device[i].file_belongs_to=0;}
}
//初始化nvm上的字节，和bitmap

int initialize_storage(int filenum ,nvm*mydevice,char*bitmap,int devicesize,file*filelist){//根据文件数量初识初始化nvm和bitmap

int i,j;
int filesize;
int nvm_being_writed;
int last;
int space=0;

for(i=0;i<filenum;i++){
    filesize=rand()%(MAXFILESIZE)+1;//随机生成文件大小在1~MAXFILESIZE之间。
    printf("fileid=%d,filesize=%d   ",i+1,filesize);//测试
    while(bitmap[nvm_being_writed=rand()%devicesize]=='1'){;}//选出一个还未被写入的字节
    printf("--%d",nvm_being_writed);
       bitmap[nvm_being_writed]='1';
       mydevice[nvm_being_writed].file_belongs_to=i+1;
       mydevice[nvm_being_writed].writecounter=1;
       mydevice[nvm_being_writed].next=NVMSIZE;//更改字节信息p
       last=nvm_being_writed;

       filelist[i].firstwordposition=nvm_being_writed;
       filelist[i].fileid=i+1;
       filelist[i].fllesize=filesize;//创建文件元数据


   for(j=1;j<filesize;j++){
     while(bitmap[nvm_being_writed=rand()%devicesize]=='1'){;}
     bitmap[nvm_being_writed]='1';
     printf("--%d",nvm_being_writed);
     mydevice[nvm_being_writed].file_belongs_to=i;
     mydevice[nvm_being_writed].writecounter=1;
     mydevice[last].next=nvm_being_writed;
     last=nvm_being_writed;
     mydevice[nvm_being_writed].next=NVMSIZE;
   }
   printf("\n");//测试
   space+=filesize;
}

printf("\ntotal Bytes has been used is :%d\n",space);
return space;
}


int main()
{
    nvm  mydevice[NVMSIZE];
    char bitmap[NVMSIZE];
    int  filenum;
    file*  fileList;
    initialize_device(mydevice,NVMSIZE,bitmap);
    printf("请输入文件数目(no more than 40)：");
    scanf("%d",&filenum);
    fileList=(file*)malloc(filenum*sizeof(file));
    initialize_storage(filenum,mydevice,bitmap,NVMSIZE,fileList);
    printf("\n-------------------------------------------------\n");
    printf("核实文件信息：\n");
    for(int i=0;i<filenum;i++){getfileinfo(mydevice,i+1,fileList);}
    return 0;
}
