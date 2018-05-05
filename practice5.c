#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#define dictionary "dictionaries/small"

#define LENGTH 45

typedef struct node
{
    char word[LENGTH+1];
    struct node *next;
}
node;

#define hash_size 3

node *hashtable[hash_size];



int main(void)
{
    // TODO
    // 把dictionary 這個檔案開起來
    FILE *fp = fopen(dictionary,"r");
    
    if (fp == NULL)
    {
        printf("Could not open dictionary\n");
        return false;
    }
    
    // 給hashtable 一個記憶體空間
    // 
    for (int i=0; i<hash_size; i++)
    {
        hashtable[i]  = NULL;
    }
    
    // 創造一個空間給單字暫存, length 已經被定義為45
    char new_word[LENGTH+1];
    
    // 讀file，把每個 單字貼到 new_word[] 中 直到讀完整個字典
    while (fscanf(fp, "%s" , new_word) != EOF)
    {
        // 創造一個暫時性的記憶體空間來存 
        // Q: 之後如何 free?
        node *space = malloc(sizeof(node));
        if (space == NULL)
        {
            printf("Could not malloc new node.\n");
            return false;
        }
        
         // 將讀入的 new_word 單字 放入 temp  裡面對應長度的 Linked list (temp[i] -> word )
        strcpy( space -> word, new_word);
        
        hashtable[strlen(new_word)%hash_size] ->next = space;
        node *cursor = space;
        space = space -> next;
        cursor -> next = space;
    }
    
    for(int i =0; i<hash_size; i++)
    {
        while(hashtable[i] != NULL)
        {
        printf("block: %i / word: %s\n" , i , hashtable[i] ->word);
        hashtable[i] = hashtable[i] ->next;
        }
    }
    
    fclose(fp);
    return true;
}
