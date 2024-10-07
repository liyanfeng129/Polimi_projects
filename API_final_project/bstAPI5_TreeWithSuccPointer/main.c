#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 1025
int k = 0;
int n = 0;

typedef struct wordsBuffer{
    char *words;
    int offset;
    int bufferSize; // 400 kb
    int increaseSize; // 400 kb
    int wordSize; // k for word length + 1 for '\0'
}wordsBuffer;

wordsBuffer *new_wordsBuffer(int wordSize)
{
    wordsBuffer *buffer = malloc(sizeof(wordsBuffer));
    buffer->offset = 0;
    buffer->bufferSize = 400000;
    buffer->increaseSize = 400000;
    buffer->wordSize = wordSize + 1;
    buffer->words = calloc(buffer->bufferSize, sizeof (char));
    return buffer;
}

void resizeBuffer(wordsBuffer *buffer)
{
    buffer->bufferSize += buffer->increaseSize;
    buffer->words = realloc(buffer->words, buffer->bufferSize);
}

wordsBuffer *free_buffer(wordsBuffer *buffer)
{
    free(buffer->words);
    free(buffer);
    return NULL;
}

int new_word(wordsBuffer *buffer , char *word)
{
    if(buffer->bufferSize < buffer->offset + buffer->wordSize)
        resizeBuffer(buffer);
    int wordSize = buffer->wordSize;
    int offset = buffer->offset;
    char *temp = buffer->words + offset;
    memcpy(temp, word , wordSize - 1);
    temp[wordSize - 1] = '\0';
    buffer->offset += wordSize;
    return offset;
}

typedef struct bstTreeNode{
    int offset;
    struct bstTreeNode *parent;
    struct bstTreeNode *left;
    struct bstTreeNode *right;
    struct bstTreeNode *suc;
    struct bstTreeNode *next_comp;
}bstTreeNode;

typedef struct bstTree{
    wordsBuffer *buffer;
    bstTreeNode *root;
    int num_comp;

}bstTree;

typedef struct letterCheck{
    int n_least;
    int n_exact;
    int exact_ok;
    char letter;
    char *check;
    struct letterCheck *next;
}letterCheck;

letterCheck* new_letterCheck(char letter)
{
    letterCheck* pLetterCheck = NULL;
    pLetterCheck = malloc(sizeof(letterCheck));
    (*pLetterCheck).letter = letter;
    pLetterCheck->check = malloc((k+1)*sizeof(char));
    for(int i = 0; i < k ; i++)
        (*pLetterCheck).check[i] = '+';
    (*pLetterCheck).check[k] = '\0';
    pLetterCheck->n_least = -1;
    pLetterCheck->n_exact = 0;
    pLetterCheck->exact_ok = -1;
    pLetterCheck->next = NULL;
    return pLetterCheck;
}

char *new_string(int size)
{
    char *string = malloc((size+1)*sizeof(char));
    for(int i = 0; i < size; i++)
        string[i] = '1';
    string[size] = '\0';
    return string;
}
char *reset_letterArchive(char *archive)
{
    for(int i = 0; i < 64; i++)
        archive[i] = '1';
    return archive;
}

char *free_string( char *string)
{
    free(string);
    return NULL;
}

int indexOfChar(char c)
{
    if(c == '-') //0
        return 0;
    if(c >= '0' && c <= '9') //1-10
        return c-'0'+1;
    if( c>= 'A'  && c<='Z') // 11-36
        return c-'A'+11;
    if(c == '_') // 37
        return 37;
    if( c>= 'a'  && c<='z') // 38-63
        return c-'a'+38;
    return -1;
}


bool letterExists(char *letterArchive, char letter)
{
    int index = indexOfChar(letter);
    if(letterArchive[index] == '1')
        return true;
    else
        return false;
}

bool existenceCheck(char *letterArchive, char *word, int len)
{
    for(int i = 0; i < len; i++ )
        if(!letterExists(letterArchive, word[i]))
            return false;
    return true;
}


typedef struct refLetterGroup{
    char *ref;
    letterCheck **letterCheckList; //64 pointer for 64 different char
    letterCheck *firstCheck;
}refLetterGroup;

refLetterGroup *new_refLetterGroup(char *ref)
{
    int i,j = 0;
    refLetterGroup *ret = malloc(sizeof(refLetterGroup));
    ret->ref = malloc((k+1)*sizeof (char));
    memcpy(ret->ref, ref, k+1);
    // letterCheck **letterCheckList = malloc(k*sizeof(letterCheck*)); // array of pointer to letterCheck
    letterCheck *head = NULL;

    letterCheck **letterCheckList = calloc(64,sizeof(letterCheck*));
    for( i = 0; i < k; i++)
    {
        int index = indexOfChar(ref[i]);
        if (letterCheckList[index] == NULL)
        {
            letterCheckList[index] = new_letterCheck(ref[i]);
            letterCheckList[index]->next = head;
            head = letterCheckList[index];
            j++;
        }
        letterCheckList[index]->n_exact++;
    }

    ret->letterCheckList = letterCheckList;
    ret->firstCheck = head;
    return ret;
}

refLetterGroup *freeLetterGroup(refLetterGroup *letterGroup)
{
    free(letterGroup->ref);
    letterCheck *cur = letterGroup->firstCheck;
    while(cur!=NULL)
    {
        letterCheck *temp = cur;
        cur = cur->next;
        free(temp->check);
        free(temp);
    }
    free(letterGroup->letterCheckList);
    free(letterGroup);
    return NULL;
}


/*
 * returns 1 if s1 is greater than s2 alphabetically
 * returns -1 if s1 is smaller than s2 alphabetically
 * returns 0 if two strings are same
 * */
int stringCompare(char *s1, char *s2, int len)
{
    for(int i = 0; i < len; i++)
        if( s1[i] != s2[i] )
        {
            if(s1[i] > s2[i])
                return 1;
            else
                return -1;
        }
    return 0;
}

bstTreeNode *new_bstTreeNode(int offset)
{
    bstTreeNode *z = malloc(sizeof( bstTreeNode));
    z->left = NULL;
    z->right =NULL;
    z->parent = NULL;
    z->next_comp = NULL;
    z->suc = NULL;
    z->offset = offset;
    return z;
}

bstTree *new_bstTree(int wordSize)
{
    bstTree *tree = malloc(sizeof(bstTree));
    wordsBuffer *buffer = new_wordsBuffer(wordSize);
    char *word = malloc((wordSize+1)*sizeof(char));
    for(int i = 0; i < wordSize; i++)
        word[i] = '+';
    word[wordSize] = '\0';
    bstTreeNode *absoluteMin = new_bstTreeNode(new_word(buffer,word ));
    tree->root = absoluteMin;
    tree->num_comp = 0;
    tree->buffer = buffer;
    return tree;
}

bstTreeNode *bst_Min(bstTreeNode *x)
{
    while(x->left != NULL)
        x = x->left;
    return x;
}

bstTreeNode *bst_Max(bstTreeNode *x)
{
    while(x->right != NULL)
        x = x->right;
    return x;
}


bstTreeNode *bst_successor(bstTreeNode *x)
{
    bstTreeNode *y;
    if(x->right != NULL)
        return bst_Min(x->right);
    y = x->parent;
    while( y != NULL && x == y->right)
    {
        x = y;
        y = y->parent;
    }
    return y;
}

bstTreeNode *bst_predecessor(bstTreeNode *x)
{
    bstTreeNode *y;
    if(x->left != NULL)
        return bst_Max(x->left);
    y = x->parent;
    while( y != NULL && x == y->left)
    {
        x = y;
        y = y->parent;
    }
    return y;
}


void bst_insert(bstTree* t, bstTreeNode *z) // tree will never be empty, because of the absolute min
{
    bstTreeNode *y = NULL;
    bstTreeNode *x;
    x = t->root;
    char *buffer = t->buffer->words;
    while(x != NULL)
    {
        y = x;
        if(stringCompare(buffer+z->offset, buffer+x->offset, k) < 0)
            x = x->left;
        else
            x = x->right;
    }
    z->parent = y;
    if(stringCompare(buffer+z->offset, buffer+y->offset, k) < 0)
        y->left = z;
    else
        y->right = z;
    bst_predecessor(z)->suc = z;
    z->suc = bst_successor(z);
}

void comp_initialization(bstTree *t)
{
    bstTreeNode *cur = t->root;
    t->num_comp = -1;
    while(cur != NULL)
    {
        cur->next_comp = cur->suc;
        t->num_comp++;
        if(cur->suc == NULL) // if next_comp is pointing itself means it's the last word
            cur->next_comp = cur;
        cur = cur->suc;
    }
}

void comp_print_comp_set(bstTree *t)
{
    char *buffer = t->buffer->words;
    bstTreeNode *cur = t->root->next_comp;
    while(cur->next_comp != cur)
    {
        fputs(buffer+cur->offset, stdout);
        fputs("\n", stdout);
        cur = cur->next_comp;
    }
    fputs(buffer+cur->offset, stdout);
    fputs("\n", stdout);
}

void comp_fix ( bstTreeNode *node, bstTreeNode *root)
{

    bstTreeNode *pre = bst_predecessor(node);
    while( node != root && pre->next_comp == NULL && pre->next_comp != pre)
    {
        pre = bst_predecessor(pre);
        /*
         * pre == root means we reached the absolute min
         * pre->next_comp != NULL means this is a comp predecessor
         * return pre;
         *
         * pre->next_comp == pre means this is last comp in the old version
         * return pre
        */
    }
    if(pre->next_comp == pre)
    {
        pre->next_comp = node;
        node->next_comp = node;
    }
    else
    {
        bstTreeNode *next = pre->next_comp;
        pre->next_comp = node;
        node->next_comp = next;
    }
}




void bstTreeDeleteALl_recursive(bstTreeNode *node)
{
    if(node != NULL)
    {
        bstTreeDeleteALl_recursive(node->left);
        bstTreeDeleteALl_recursive(node->right);
        free(node);
    }
}


bstTree *free_bstTree(bstTree *t)
{
    bstTreeDeleteALl_recursive(t->root);
    free(t->buffer->words);
    free(t->buffer);
    free(t);
    return NULL;
}

bool treeContainsNode(bstTreeNode *root, char *buffer ,char *word, int len)
{
    while(root != NULL)
    {
        if(stringCompare(buffer+root->offset, word, len) < 0)
            root = root->right;
        else if(stringCompare(buffer+root->offset, word, len) > 0 )
            root = root->left;
        else
            return true;
    }
    return false;
}


bool isCompatible(char *word, refLetterGroup *letterGroup, char *lettersArchive)
{
    if(!existenceCheck(lettersArchive, word, k))
        return false;
    letterCheck *temp = letterGroup->firstCheck;
    while(temp!=NULL)
    {
        char* check = temp->check;
        char letter = temp->letter;
        int cont = 0; // how many times it appears
        int n_least = temp->n_least;
        int n_exact = temp->n_exact;
        int exact_ok = temp->exact_ok;
        for(int j = 0 ; j < k; j++) // for each letter of the word1
        {
            if(word[j] == letter)
                cont++;
            if(check[j] == '1' && word[j] != letter) // in position where this letter has to appear , and doesn't
                return false;
            if(check[j] == '0' && word[j] == letter) // in position where this letter can't appear, and does
                return false;
        }
        if( exact_ok > 0 && cont != n_exact) // if we know exactly how many times this letter appears, and cont doesn't match
            return false;
        else if(n_least > 0 && cont < n_least) //we know at least how many times it should appear, and it doesn't
            return false;
        temp = temp->next;
    }
    return true;
}

void comp_check(bstTree *t, refLetterGroup *letterGroup, char *letterArchive)
{
    char *buffer = t->buffer->words;
    bstTreeNode *cur = t->root->next_comp;
    bstTreeNode *pre = t->root;
    while(cur->next_comp != cur) // if true cur is last comp
    {
        if(!isCompatible(buffer+cur->offset, letterGroup, letterArchive))
        {
            t->num_comp--;
            bstTreeNode *temp = cur;
            cur = cur->next_comp;
            pre->next_comp = cur;
            temp->next_comp = NULL;
        }
        else
        {
            pre = cur;
            cur = cur->next_comp;
        }
    }

    if( !isCompatible(buffer + cur->offset, letterGroup, letterArchive))
    {
        t->num_comp--;
        pre->next_comp = pre;
    }
}



bool next_line(char *line) {
    char c;
    int len = 0;
    while ((c = (char) getchar()) != EOF) {
        line[len] = c;
        len++;
        if (line[len - 1] == '\n')
            return true;
    }

    return false;
}

void letterNotExists(char *letterArchive, char letter)
{
    int index = indexOfChar(letter);
    letterArchive[index] = '0';
}

int count_comp(bstTree *t)
{
    int i = 0;
    bstTreeNode *cur = t->root->next_comp;
    while(cur->next_comp != cur)
    {
        i++;
        cur = cur->next_comp;
    }
    i++;
    return i;
}

char *generateOutPut(refLetterGroup *letterGroup, char *letterArchive, char *word, char *res)
{
    int i,index;
    letterCheck *pLetterCheck;
    char* ref = letterGroup->ref;
    int *letterCount = calloc(64,sizeof(int));
    for(i = 0; i < k; i++)
        res[i]='?';
    for(i = 0; i < k; i++) //for each letter in word1
    {
        index = indexOfChar(word[i]);
        pLetterCheck =  letterGroup->letterCheckList[index];
        if(word[i] == ref[i])
        {
            res[i] = '+';
            letterCount[index]++;
            pLetterCheck->n_least = (letterCount[index] > pLetterCheck->n_least)? letterCount[index] : pLetterCheck->n_least;
            pLetterCheck->check[i] = '1'; // this letter has to be in this position
            letterCheck *temp = letterGroup->firstCheck;
            while(temp != NULL) // in this position cannot appear other letter
            {
                if(temp->letter!= word[i])
                    temp->check[i] = '0';
                temp = temp->next;
            }
        }
        else
        {
            if(pLetterCheck == NULL) // letter doesn't exist
            {
                res[i] = '/';
                letterNotExists(letterArchive, word[i]);
            }
        }
    }

    for( i = 0; i < k ;i++)
    {
        if(res[i] == '?') // this position has to be checked
        {
            index = indexOfChar(word[i]);
            pLetterCheck =  letterGroup->letterCheckList[index];
            if(letterCount[index] < pLetterCheck->n_exact)
            {
                // this position has no this letter, but it could be in somewhere else in ref word1
                res[i] = '|';
                pLetterCheck->check[i] = '0';
                letterCount[index]++;
                pLetterCheck->n_least = (letterCount[index] > pLetterCheck->n_least)? letterCount[index] : pLetterCheck->n_least;
            }
            else
            {
                res[i] = '/'; // there's more this letter, and therefor we know the exact number
                pLetterCheck->check[i] = '0';
                pLetterCheck->exact_ok = 1;
            }
        }
    }

    free(letterCount);
    return res;
}



int main() {
    char *res = NULL;
    bstTree *t;
    char *letterArchive = new_string(64);
    int initialRead = false; //flag
    int game_start = false; //flag
    refLetterGroup *letterGroup = NULL;
    char line[MAX];
    if(!fscanf(stdin, "%d\n", &k))//defining word1's length
        return -2;
    t = new_bstTree(k);
    res = malloc((k+1)*sizeof (char));
    for(int i = 0; i < k; i++)
    {
        res[i] = '+';
    }
    res[k] = '\0';
    while(next_line(line))
    {
        if(!initialRead)
        {
            if(line[0] != '+')
                bst_insert(t, new_bstTreeNode(new_word(t->buffer, line)));
            else
                initialRead = true;
        }
        if(initialRead  && !game_start) // "+nuovo_partita" or "inserisci_inizio"
        {
            if(line[0] == '+' && line[1] == 'n') // new game
            {
                /*
                 * read ref word1
                 * read n# attempts
                 * then merge tree with less node to other
                 * clean notExistingLetters
                 * */
                game_start = true;
                if(!next_line(line))
                    return -1;
                // line has ref word1
                if(letterGroup!=NULL)
                    letterGroup = freeLetterGroup(letterGroup); // free old letterGroup
                letterGroup = new_refLetterGroup(line);
                letterArchive = reset_letterArchive(letterArchive);
                //---------------------------------------- initialize


                comp_initialization(t);

                //-----------------------------------------
                if(!fscanf(stdin, "%d\n",&n))
                {
                    return -2;
                }
            }
            else if(line[0] == '+' && line[1] == 'i') // insert word1
            {
                int end_insert = false;
                while(!end_insert  && next_line(line))
                {
                    if(line[0]!='+')
                        bst_insert(t, new_bstTreeNode(new_word(t->buffer, line)));
                    else
                        end_insert = true;
                }
            }
        }
        else if (initialRead  && game_start )
        {
            if(line[0]!='+') // is a word1 that needs to be checked, here output is expected
            {
                if(treeContainsNode(t->root,t->buffer->words,line,k) ) // word1 is in dictionary
                {
                    n--; // one attempt less
                    if(stringCompare(line,letterGroup->ref,k) == 0) // we found the word1
                    {
                        fputs("ok\n",stdout);
                        game_start = false;
                    }
                    else
                    {
                        fputs(generateOutPut(letterGroup, letterArchive, line, res), stdout);
                        fputs("\n",stdout);
                        //------------------------------------------------------ // check compatibility after new input generated
                        comp_check(t, letterGroup, letterArchive);

                        fprintf(stdout, "%d\n", t->num_comp);// print total number of comp

                        //----------------------------------------------------------------
                        if(n == 0)
                        {
                            fputs("ko\n",stdout);
                            game_start = false;
                        }
                    }
                }
                else
                    fputs("not_exists\n",stdout);
            }
            else
            {
                if(line[1] == 's')// +stampa_filtrate, here output is excepted
                    comp_print_comp_set(t);
                else // +inserisci_inizio
                {
                    int end_insert = false;
                    while(!end_insert  && next_line(line))
                    {
                        if(line[0]!='+') // insert inside a game
                        {

                            //--------------------------------------------
                            bstTreeNode *node = new_bstTreeNode(new_word(t->buffer, line));
                            bst_insert(t, node);
                            if(isCompatible(t->buffer->words+node->offset,letterGroup,letterArchive))
                            {
                                t->num_comp++;
                                comp_fix( node,  t->root);
                            }
                            //------------------------------------------
                        }
                        else
                        {
                            end_insert = true;
                            t->num_comp = count_comp(t);
                        }
                    }
                }
            }
        }
    }

    if(letterArchive != NULL)
        letterArchive = free_string(letterArchive);
    if(letterGroup!=NULL)
        letterGroup = freeLetterGroup(letterGroup);
    if(res!=NULL)
        free(res);
    if(t!=NULL)
        t = free_bstTree(t);
    return 0;
}