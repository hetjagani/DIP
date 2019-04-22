#include <stdlib.h>
#define SIZE 256

typedef struct  pixel_data
{
    unsigned short left_id;
    unsigned short right_id;
    unsigned short val;
    double freq;
    char code[SIZE];
}node;

void sort(node hist[], int);
node *update_arr(node [], int);
void assign_codes(int, char *);
int find_id(int);
void gen_code();

node *node_list;
unsigned char *check_list;

void gen_code(){
    node *temp = (node *)calloc(SIZE, sizeof(node));
    for(int i=0; i<SIZE; ++i){
            temp[i] = node_list[i];
    }
    sort(node_list, SIZE);
    sort(temp, SIZE);

    
    for(int idx=1; idx<SIZE; ++idx){
        temp = update_arr(temp, SIZE-idx);
        node_list[SIZE+idx-1].left_id = temp[SIZE-idx-1].left_id;
        node_list[SIZE+idx-1].right_id = temp[SIZE-idx-1].right_id;
        node_list[SIZE+idx-1].freq = temp[SIZE-idx-1].freq;
        temp[SIZE-idx-1].val = node_list[SIZE+idx-1].val;
        sort(temp, SIZE-idx);
    }
    
    check_list = (unsigned char *)calloc(2*SIZE-1, sizeof(unsigned char));
    
    assign_codes(2*SIZE-1, "");
}

void assign_codes(int id, char code[]){
    int idx = find_id(id);
    check_list[idx] = 1;
    strcpy(node_list[idx].code, code);
    char temp[SIZE];
    
    if(node_list[idx].left_id != 0 && check_list[find_id(node_list[idx].left_id)] != 1){
        strcpy(temp, code);
        strcat(temp, "1");
        assign_codes(node_list[idx].left_id, temp);
    }
    if(node_list[idx].right_id != 0 && check_list[find_id(node_list[idx].right_id)] != 1){
        strcpy(temp, code);
        strcat(temp, "0");
        assign_codes(node_list[idx].right_id, temp);
    }
}

int find_id(int id){
    for(int i=0; i<2*SIZE-1; ++i){
        if(node_list[i].val == id){
            return i;
        }
    } 
    return 0;   
}


node *update_arr(node arr[], int cnt){
    node *temp = (node *)calloc(cnt, sizeof(node));
    temp[cnt-1].left_id = arr[cnt].val;
    temp[cnt-1].right_id = arr[cnt-1].val;
    temp[cnt-1].freq = arr[cnt].freq + arr[cnt-1].freq;
    //printf("%d \n", temp[cnt-1].val);
    for(int i=0; i<cnt-1; ++i){
        temp[i] = arr[i];
    }
    free(arr);
    return temp;
}

void sort(node arr[], int length){
    node key;
    int i, j; 

    for (i = 1; i < length; i++) { 
        key = arr[i]; 
        j = i - 1; 
        while (j >= 0 && arr[j].freq < key.freq) { 
            arr[j + 1] = arr[j]; 
            j = j - 1; 
        } 
        arr[j + 1] = key; 
    }
}