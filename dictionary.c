/**
 * Implements a dictionary's functionality.
 */

#include <stdio.h>
#include <cs50.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

typedef struct node
{
    char word[LENGTH+1];
    struct node *next;
}
node;

// size of hashtable to creat
#define hash_size 26

// 先定義一個 hashtable 來讓字典存進去
// hash function: 字串長度 % 26
node *hashtable[hash_size];

// 記錄 字典裡面包含的數目
int dictionary_count =0;

// 記錄字典是否被讀入
bool dictionary_load = false;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // TODO
    
    int L = strlen(word);
    
    // 因為原始定義word 不能修改，所以創造一個word check 去修改大小寫
    char word_check[L+1];
    
    // 強制將word 裡面的每個 char 都轉成小寫來比較
    for(int i=0; i< L; i++)
    {
        word_check[i] = tolower(word[i]);
    }
    
    // 將最後一個字元設成 \0
    word_check[L] = '\0';
    
    // 先創造一個 指標temp 來記錄每次尋找的過程, 指需要創造一個 linked list 因為一次只找一個字 
    node *temp3 = hashtable[L%hash_size]->next;
   
    
    // 比對字串是否和 該 linked list 裡面的字串相同
    while (temp3 != NULL)
    {
        // 如果輸入的word 和字典中的字相同就回傳 T
        if(strcmp(word_check , temp3->word) == 0)
        {
            return true;
        }
        else
        {
            temp3 = temp3-> next;
        }
    }
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
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
        hashtable[i]  = malloc(sizeof(node));
        if(hashtable[i] == NULL)
        {
            printf("Could not malloc a hashtable.\n");
            return false;
        }
        hashtable[i] -> next = NULL;
    }
    
    // 創造一個空間給單字暫存, length 已經被定義為45
    char new_word[LENGTH+1];
    
    // 創造一個 cursor 來追蹤 hashtable linked list 現在的位置
    node *cursor[hash_size];
    for(int i=0; i<hash_size; i++)
    {
        cursor[i] = hashtable[i];
    }
    
    
    
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
        
        // 先把next 的記憶體空間給出來 , 表頭不存東西，直接存到 next
        cursor[strlen(new_word)%hash_size] -> next = space;
        
        // 確認有空間後讓 cursor[i] 指向自己的 next
        cursor[strlen(new_word)%hash_size] = cursor[strlen(new_word)%hash_size]-> next;
        
         // 將讀入的 new_word 單字 放入 temp  裡面對應長度的 Linked list (temp[i] -> word )
        strcpy( cursor[strlen(new_word)%hash_size] -> word, new_word);
        
        // 讓 最後一個linked list 節點會指向 NULL
        cursor[strlen(new_word)%hash_size] -> next = NULL;
        
        // 記錄存進去的單字數目
        dictionary_count++;
        
        // 確認字典的存取狀態
        dictionary_load = true;
    }
    fclose(fp);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    if(dictionary_load )
    {
        return dictionary_count;
    }
    return 0;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    for (int i=0; i<hash_size; i++)
    {
        while(hashtable[i] != NULL)
        {
            node *temp2 = hashtable[i];
            hashtable[i] = hashtable[i] ->next;
            free(temp2);
        }
    }
    return true;
}