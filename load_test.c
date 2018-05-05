#include <cs50.h>

#include <ctype.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <stdbool.h>
#include <string.h>

#include "dictionary.h"

#define DICTIONARY "dictionaries/small"

typedef struct node
{
    char word[46];
    struct node *next;
}
node;


/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
 
int main(void)
{
    // TODO
    // 把dictionary 這個檔案開起來
    FILE *fp = fopen(DICTIONARY,"r");
    
    if (fp == NULL)
    {
        printf("Could not open dictionary\n");
        return 1;
    }
    
    // 記錄 字典裡面包含的數目
    int word_count =0;
    
    // 創造一個 hashtable[26] 來放置讀進來的字數
    node *hashtable[26];
    
    for (int i=0; i<26; i++)
    {
        hashtable[i] = malloc(sizeof(node));
        hashtable[i] -> next = NULL;
    }
    
    // 創造一個暫存的節點來記錄每個 hashtable 存的位置
    node *temp[26];
    for (int i=0; i<26; i++)
    {
        temp[i] = hashtable[i];
    }
    
    char new_word[46];
    // 讀file，把每個 單字存到 new_word 中
    while (fscanf(fp, "%s" , new_word) != EOF)
    {
        // 將讀入的 new_word 單字 放入 temp  裡面對應長度的 Linked list (temp[i] -> word )
        strcpy( temp[strlen(new_word)%26] -> word, new_word);
        
        // 現創造一個暫時性的記憶體空間
        node *space = malloc(sizeof(node));
        
        // 先把next 的記憶體空間給出來
        temp[strlen(new_word)%26] -> next = space;
        
        // 確認有空間後讓 temp[i] 指向自己的 next
        temp[strlen(new_word)%26] = temp[strlen(new_word)%26]-> next;
        
        // 記錄存進去的單字
        word_count++;
        free(space);
    }
    
    // 讀完字典後，讓每個 temp[i] 的next 都指向 NULL
    for (int i =0; i<26; i++)
    {
        temp[i] -> next = NULL;
    }
        
    // CHECK TODO
    // 先創造一個 cursor 來記錄每次尋找的過程 
    
    printf("Word to search: ");
    char *word = get_string();
    
    for(int i=0; i< strlen(word); i++)
    {
        if(isupper(word[i]))
        {
            word[i] = word[i] +32;
        }
    }
    
    node *cursor[26];
    
    
    for (int i=0; i<26; i++)
    {
        cursor[i] = hashtable[i];
    }
    
    while (cursor[strlen(word)] != NULL)
    {
        // 如果輸入的word 和字典中的字相同就回傳 T
        if(strcmp(word , cursor[strlen(word)]->word) == 0)
        {
            printf("Is word in dictionary? : YES\n");
            return true;
        }
        else if(strcmp(word , cursor[strlen(word)]->word) != 0)
        {
            cursor[strlen(word)] = cursor[strlen(word)]-> next;
        }
    }
    
    
    printf("Is word in dictionary? : NO\n");
    return false;
    
    
    for (int i=0; i<26; i++)
    {
        temp[i] = hashtable[i];
    }
    
    for (int i=0; i<26; i++)
    {
        cursor[i] = NULL;
        // 判斷直到最後一個 list
        while(temp[i] != NULL)
        {
            // 
            if(temp[i]->next != NULL)
            {
                cursor[i] = temp[i] ->next;
                free(temp[i]);
                temp[i] = cursor[i];
            }
            else
            {
                free(cursor[i]);
                free(temp[i]);
            }
        }
    }
    

  /**
   * 印出字典的檢查過程
    for (int i=0; i<26; i++)
    {
        temp[i] = hashtable[i];
    }
    
   for (int i=0; i<26; i++)
    {
        while(temp[i] != NULL)
        {
            printf("block: %i, word: %s" , i , temp[i]->word);
            temp[i]= temp[i] -> next;
        }
    }
    
    printf("\n Total word in dictionary: %i\n" , word_count);
   **/
}