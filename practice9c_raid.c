/*
[[任務]]
把Python不能做的事在C寫好
*/

#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 

#define N  64     // 一次幾個 unsigned long long  
#define M  (8*N)
#define MAX_NUM_HDD 15

char* questionMark2space(char* p){
 
  char* q = (char*) malloc((strlen(p)+1) * 1);
  strcpy(q,p); 

  int i = 0 ; 
  while (q[i]!='\0'){
    if (q[i]=='?'){ q[i] = ' '; }
    i++ ; 
  }

  return q; 
}

int main(int argc, char* argv[]){

  // a nHDD each_size last_size input_path output_paths 

  if (argc < 6) {
    printf("參數太少.\n");
    return 0; 
  }

  int nHDD = atoi(argv[1]); 
  int each_size = atoi(argv[2]); 
  int last_size = atoi(argv[3]) ; 
  int i = 0, j = 0, k = 0 ,l = 0  ; 

  if (5+nHDD != argc){
    printf("參數數量不對. \n"); 
    return 0; 
  }
 
   // printf("%c%c%c\n", argv[4][0],argv[4][1],argv[4][2]); 
  //char* input_path = argv[4]; 
  char* input_path = questionMark2space(argv[4]) ; 

 
  char* output_paths[15] ; 
  for (i = 5; i<argc; i++){
    output_paths[i-5] =questionMark2space(argv[i]); 
  }

  // 先假設現在每一節剛好都是滿的。 
  // 先假設現在每一節剛好都是滿的。 
  // 先假設現在每一節剛好都是滿的。 
  // 先假設現在每一節剛好都是滿的。 
  // 先假設現在每一節剛好都是滿的。 


  unsigned long long u[N], v = 0 , r = 0  ;
  char a[15][M*2]; 
  char final[M*2]; 
  char *p ; 
  int round = 0, R = 0 ; 
  int residue = 0 ; 

  round = last_size / M; 
  residue = each_size / M ; 
  printf("round= %d\n\n", round);

  FILE* f[15]; 
  FILE* g[15];   // = fopen(output_path ,"wb");


  for (i=0;i< nHDD-1; i++){
    f[i] = fopen(input_path,"rb");
    for (j=0;j<i; j++){
      fseek(f[i],each_size, SEEK_SET);  

    }
  }

  /* 
  截自目前為止，已經把 f[i] 的(同一個)檔案都開好了，
  指標也調到對應的位置，如下圖： 
  
                                N段，故 M = 8*N
                    /                                     \
        a[0]:       ==== ==== ==== ==== ==== ==== ==== ====
    /   a[1]:       ==== ==== ==== ==== ==== ==== ==== ====
        a[2]:       ==== ==== ==== ==== ==== ==== ==== ====
  nHDD                               |
                                     |
    \   a[nHDD-2]:  ==== ==== ==== ==== ==== ==== ==== ====
        a[nHDD-1]:  ==== ==== ==== ==== ==== ==== ==== ====

  其中等號 = 裡的一個槓算一個 byte 的數據。 
  接下來就是讀檔→ xor→ 輸出。 
  
  */

  for(i=0; i< nHDD;  i++) {
  g[i] = fopen(output_paths[i],"wb");

  } 

  for (R=0;R<round; R++){

    for (i=0; i<nHDD-1; i++){
      fread(a[i],1,M, f[i]); 

    }

  
    for (i=0; i<    N           ; i++){
      v = 0 ; 
      for (j=0; j <nHDD -1; j++){
        r =  *(unsigned long long*)(a[j]+8*i); 
        v = v ^ r ;
      }        
      u[i] = v ; 
    }

    // 輸出前三(nHDD-1)個
    for (i = 0;i<nHDD-1; i++){
        fwrite(a[i], 1,M, g[i]) ; 
    }

    // 輸出最後一個
    p = (char*)u; 
    fwrite(p, 1,M, g[nHDD-1]) ;     // https://www.runoob.com/cprogramming/c-function-fwrite.html

  }

  // 這邊開始處理剩下的
  // 即 503702143 % (8*N) = 127
  //    503702142 % (8*N) = 126
  
  each_size -= round * M  ;
  last_size %= M ; 

  for (i=0; i<nHDD-2; i++){
    fread(a[i],1,each_size, f[i]); 
  }
    fread(a[nHDD-2],1,last_size, f[nHDD-2]); 

  for (i=last_size; i< each_size -1 ; i++ ){
    a[nHDD-2][i]='\0'; 
  }
  a[nHDD-2][each_size-1] = (char)(each_size-last_size); 
/*截至目前已經把 nHDD-1 個末尾都整頓成 127 個字節了 */

  // 做 xor 階段
  for( i = 0; i < each_size ; i++){
    v=0; 
    for (j=0; j<nHDD-1; j++){
      v ^= (int)(a[j][i]);
    }

    final[i] =(char) v ;
  }

  // fwrite
  for (i = 0;i<nHDD-1; i++){
      fwrite(a[i], 1,each_size, g[i]) ; 
  }
  p = (char*)u; 
  fwrite(final, 1,each_size, g[nHDD-1]) ;   


  // fclose 
  for (i = 0; i < nHDD - 1; i ++){
    fclose(f[i]); 
    fclose(g[i]); 

  }
    
  printf("本程式只確定能raid < 2G 的檔案。\n本程式尚未防範檔案路徑含有空白鍵的情況.\n\n");
  return 0; 
}

/*
cd "C:\Users\User\Documents\myvscode\240301_raid5" && gcc practice9c_raid.c -o practice9c_raid.exe

a nHDD each_size last_size input_path output_paths 
a 4 503702143 503702142 1118549489.mp4 1118549489g0.mp4 1118549489g1.mp4 1118549489g2.mp4 1118549489g3.mp4
practice9c_raid 4 503702143 503702142 1118549489.mp4 1118549489g0.mp4 1118549489g1.mp4 1118549489g2.mp4 1118549489g3.mp4

cd "C:\Users\User\Documents\myvscode\240301_raid5" && gcc practice9c_raid.c -o practice9c_raid.exe && python "C:\Users\User\Documents\myvscode\240301_raid5\practice14.py"

*/
