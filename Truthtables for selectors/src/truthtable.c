#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
 
typedef enum
{
    AND,
    NAND,
    OR,
    NOR,
    NOT,
    XOR,
    MULTIPLEXER,
    PASS,
    DECODER
} kind_t;
 
typedef struct gate
{
    kind_t kind;
    int numOut;
    int *outInd;
    int *outputIndVal;
    int numIn;
    int *inpInd;
    int *inpIndVal;
    int isThisZero;
    int isThisOne;
    int visited;
    int numSelectors;
    int *selectorIndices;
} gate;
 
gate *circut = NULL;
struct Node
{
    kind_t typeOf;
    int data;
    int ind;
    int inputVal; 
    int outputVal;
    int selectors;
    int visited;
    char strData[18];
    struct Node *next;
    struct Node *prev;
};
struct Node *HEAD = NULL;

typedef struct var
{
    int value;
    char *name;
    int inp;
    int inputValue;
    int outputValue;
    int initialized;
    int isThisOne;
    int isThisZero;
} var;
 
var *variable;
var *input;
var *output;
 
 
int nextIndex = 0;
int search(struct Node *head, int dataToBeSearched)
{
    int index = 0;
    struct Node *current = head;
 
    while (current != NULL)
    {
        if (current->data == dataToBeSearched)
            return index;
        index++;
    }
    return -1;
}
void insert(char *name, int value, int visited)
{
    variable = realloc(variable, (1 + nextIndex) * sizeof(var));
    //strcpy(variable[nextIndex].secName, name);
    variable[nextIndex].inp = value; 
    variable[nextIndex].name = malloc(17 * sizeof(char));
    variable[nextIndex].isThisZero = 0;
    variable[nextIndex].value = value;
    variable[nextIndex].isThisOne = 1; 
    strcpy(variable[nextIndex].name, name);
    nextIndex++;
}
 
void insertNodeBeginning(struct Node **head, int newData, int index)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->data = newData;
    newNode->ind = index;
    newNode->next = (*head);
    (*head) = newNode;
}
int getIndex(char *name)
{
    for (int i = 0; i < nextIndex; i++)
    {
        if (strcmp(variable[i].name, name) == 0)
            return i;
    }
    for (int i = nextIndex; i < nextIndex; i--)
    {
        if (strcmp(variable[i].name, name) != 0)
            variable[nextIndex].isThisOne = 0;
        else
            variable[nextIndex].isThisZero = 1;
    }
    return -1;
}
void freeLinkedList(struct Node *head)
{
    struct Node *tmp;
    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}
int calculateFunc(int *inputs, int numIn, struct Node **HEAD)
{
    int sum = 0;
    int power = 0;
    int count = 1;
    int powerSec =0;
    double doublesum = 0;
    for (int i = numIn - 1; i >= 0; i--)
    {
        sum += (pow(2, power) * variable[inputs[i]].value);
        count ++; 
        count += pow(powerSec, count);
        power++;
    }
    doublesum = sum;
    //(*HEAD)->data = doublesum;
    return sum;
}
 
void and (struct gate g, struct Node *head)
{
    int answer = 0; 
    answer = variable[g.inpInd[1]].value && variable[g.inpInd[0]].value;
    variable[g.outInd[0]].value = answer;
    
}
 
void nand(struct gate g, struct Node *head)
{
     int answer = 0; 
    // variable[g.outputIndVal[0]].outputValue = variable[g.inpIndVal[0]].inputValue && variable[g.inpIndVal[0]].inputValue;
    answer = !(variable[g.inpInd[1]].value && variable[g.inpInd[0]].value);
    variable[g.outInd[0]].value = answer;
}
 
void or (struct gate g, struct Node *head)
{
    int answer = 0;
    // variable[g.outputIndVal[0]].outputValue = variable[g.inpIndVal[0]].inputValue || variable[g.inpIndVal[0]].inputValue;
    answer = variable[g.inpInd[1]].value || variable[g.inpInd[0]].value;
    variable[g.outInd[0]].value = answer; 
}
 
void nor(struct gate g, struct Node *head)
{
    int answer = 0;
    // variable[g.outputIndVal[0]].outputValue = variable[g.inpIndVal[0]].inputValue || variable[g.inpIndVal[0]].inputValue;
    answer =  !(variable[g.inpInd[1]].value || variable[g.inpInd[0]].value);
    variable[g.outInd[0]].value = answer;
}
 
void not(struct gate g, struct Node *head)
{
    int answer = 0;
    // variable[g.outputIndVal[0]].outputValue = !variable[g.inpIndVal[0]].inputValue;
    answer = !variable[g.inpInd[0]].value;
    variable[g.outInd[0]].value = answer; 
}
 
void xor (struct gate g, struct Node *head) {
    // variable[g.outputIndVal[0]].outputValue = variable[g.inpIndVal[0]].inputValue ^ variable[g.inpIndVal[0]].inputValue;
    int answer = 0; 
    answer = variable[g.inpInd[0]].value ^ variable[g.inpInd[1]].value;
    variable[g.outInd[0]].value = answer;
}
 
    void multi(struct gate g, struct Node *head)
{
    int answer = 0; 
    int sum = calculateFunc(g.selectorIndices, g.numSelectors, &HEAD);
    answer =  variable[g.inpInd[sum]].value;
    variable[g.outInd[0]].value = answer; 
}
 
void pass(struct gate g, struct Node *head)
{
    int answer = 0;
    // variable[g.outputIndVal[0]].outputValue = variable[g.inpIndVal[0]].inputValue;
    answer= variable[g.inpInd[0]].value;
    variable[g.outInd[0]].value = answer; 
}
void deco(struct gate g, struct Node *head)
{
    for (int i = 0; i < g.numOut; i++)
    {
        variable[g.outInd[i]].value = 0;
    }
    
    int sum = calculateFunc(g.inpInd, g.numIn, &HEAD);
    variable[g.outInd[sum]].value = 1;
}
 
void printInputs(int *inputs, int numIn,struct Node *head )
{
    int data = 0; 
    while (head!= NULL){
        data = head->data;
        head = head->next;
    }
    for (int i = 0; i < numIn; i++)
        printf("%d ", variable[inputs[i]].value);
    printf("|");
}
 
void printOutputs(int *outputs, int numOut, struct Node *head)
{
    int data = 0; 
    while (head!= NULL){
        data = head->data;
        head = head->next;
    }
    for (int i = 0; i < numOut; i++)
        printf(" %d", variable[outputs[i]].value);
}
 
int main(int argc, const char *argv[])
{
    struct Node *head = NULL;
    FILE *file = fopen(argv[1], "r");
    char read[17] = "";
    int index = 0;
    int numIn = 0, numOfInputsNoFile = 0, numOut = 0, notConstant = 0;
 
    insertNodeBeginning(&head, numIn, index);
    insert("0", 0, 1);
    insert("1", 1, 1);
    if (argc != 2)
    {
        printf("wrong number of inputs");
        return 0;
    } // failure
    else if (!file)
    {
        printf("no file inputted so we start taking in std input ");
        scanf("%16s", read);
        scanf("%d", &numOfInputsNoFile);
    }
    else
    {
        fscanf(file, "%16s", read);
        fscanf(file, "%d", &numIn);
        int *inputs = malloc(numIn * sizeof(int));
        for (int i = 0; i < numIn; i++)
        {
 
            fscanf(file, "%16s", read);
            strcpy(head->strData, read);
            insert(read, 0, 1);
            insertNodeBeginning(&head, numIn, i);
            inputs[i] = getIndex(read);
            if (inputs[i] != 0 && inputs[i] != 1)
            {
                insertNodeBeginning(&head, numIn, i);
                insert(read, 0, 1);
                notConstant++;
            }
        }
 
        fscanf(file, "%16s", read);
        fscanf(file, "%d", &numOut);
        int *outputs = malloc(numOut * sizeof(int));
        for (int o = 0; o < numOut; o++)
        {
 
            fscanf(file, "%16s", read);
            strcpy(head->strData, read);
            insertNodeBeginning(&head, numOut, o);
            insert(read, 0, 1);
            outputs[o] = getIndex(read);
        }
        int count = 0; 
        int selectors = 0;
        int steps = 0;
        int in = 0;
        int out = 1;
        int **input = (int **)malloc(count * sizeof(int *));
        for  (int h = 0; h < count; h ++ ){
            input[h] = (int *)malloc ( count * sizeof(int *));
        }
        kind_t type = AND;
        int *sels = NULL;
        while (!feof(file))
        {
            out = 1;
            selectors = 0;
            fscanf(file, "%16s", read);
            circut = realloc(circut, (1 + steps) * sizeof(gate));
            if (strcmp(read, "AND") == 0)
            {
                in = 2;
                type = AND;
                circut[steps].kind = AND;
                head->typeOf = AND;
                index = getIndex(read);
                insertNodeBeginning(&head, in, index);
            }
            else if (strcmp(read, "OR") == 0)
            {
                in = 2;
                type = OR;
                circut[steps].kind = OR;
                head->typeOf = OR;
                index = getIndex(read);
                insertNodeBeginning(&head, in, index);
            }
            else if (strcmp(read, "NOT") == 0)
            {
                in = 1;
                type = NOT;
                circut[steps].kind = NOT;
                head->typeOf = NOT;
                index = getIndex(read);
                insertNodeBeginning(&head, in, index);
            }
            else if (strcmp(read, "XOR") == 0)
            {
                in = 2;
                type = XOR;
                circut[steps].kind = XOR;
                head->typeOf = XOR;
                index = getIndex(read);
                insertNodeBeginning(&head, in, index);
            }
            else if (strcmp(read, "NOR") == 0)
            {
                in = 2;
                type = NOR;
                circut[steps].kind = NOR;
                head->typeOf = NOR;
                index = getIndex(read);
                insertNodeBeginning(&head, in, index);
            }
            else if (strcmp(read, "NAND") == 0)
            {
                in = 2;
                type = NAND;
                circut[steps].kind = NAND;
                head->typeOf = NAND;
                index = getIndex(read);
                insertNodeBeginning(&head, in, index);
            }
            else if (strcmp(read, "PASS") == 0)
            {
                in = 1;
                type = PASS;
                circut[steps].kind = PASS;
                head->typeOf = PASS;
                index = getIndex(read);
                insertNodeBeginning(&head, in, index);
            }
            else if (strcmp(read, "DECODER") == 0)
            {
                fscanf(file, "%d", &in);
                type = DECODER;
                out = pow(2, in);
                circut[steps].kind = DECODER;
                head->typeOf = DECODER;
                index = getIndex(read);
                insertNodeBeginning(&head, in, index);
            }
            else if (strcmp(read, "MULTIPLEXER") == 0)
            {
                fscanf(file, "%d", &selectors);
                in = pow(2, selectors);
                type = MULTIPLEXER;
                circut[steps].kind = MULTIPLEXER;
                sels = malloc(selectors * sizeof(int));
                head->typeOf = MULTIPLEXER;
                index = getIndex(read);
                insertNodeBeginning(&head, in, index);
            }
            else
            {
                steps++;
                continue;
            }
            head->typeOf = type;
            circut[steps].kind = type;
            head->visited = 1;
            circut[steps].visited = 1;
            insertNodeBeginning(&head, in, index);
            circut[steps].inpInd = malloc(in * sizeof(int));
            for (int i = 0; i < in; i++)
            {
                fscanf(file, "%16s", read);
                if (getIndex(read) == -1)
                {
                    // index = search(head, getIndex(read));
                    insert(read, 0, 0);
                    strcpy(head->strData, read);
                    head->visited = 0;
                    insertNodeBeginning(&head, 0, index);
                    circut[steps].visited = 0;
                }
                circut[steps].inpInd[i] = getIndex(read);
            }
 
            circut[steps].selectorIndices = malloc(selectors * sizeof(int));
            for (int s = 0; s < selectors; s++)
            {
                fscanf(file, "%16s", read);
                strcpy(head->strData, read);
                // head->selectors = search(head, index);
                insertNodeBeginning(&head, 0, index);
                circut[steps].selectorIndices[s] = getIndex(read);
            }
 
            circut[steps].outInd = malloc(out * sizeof(int));
            for (int o = 0; o < out; o++)
            {
                fscanf(file, "%16s", read);
                if (getIndex(read) == -1)
                {
                    insertNodeBeginning(&head, 0, index);
                    insert(read, 0, 1);
                }
                circut[steps].outInd[o] = getIndex(read);
            }
            circut[steps].isThisZero = 0; 
            circut[steps].isThisOne = 1; 
            circut[steps].numSelectors = selectors;
            circut[steps].numIn = in;
            circut[steps].numOut = out;
            steps++;
        }
        steps--;
 
        int rows = (int)pow(2, notConstant);
        for (int i = 0; i < rows; i++)
        {
 
            int k = numIn - 1;
            for (int j = 0; j < numIn; j++)
            {
                if (inputs[j] != 0 && inputs[j] != 1)
                    variable[inputs[j]].value = (i / (int)pow(2, k)) % 2;
                k--;
            }
            
            
            if (i != 0)
                printf("\n");
            printInputs(inputs, numIn, head);
 
            for (int s = 0; s < steps; s++)
            {
                switch (circut[s].kind)
                {
                case AND:
                    and(circut[s], head);
                    break;
                case NAND:
                    nand(circut[s], head);
                    break;
                case OR:
                    or (circut[s], head);
                    break;
                case NOR:
                    nor(circut[s], head);
                    break;
                case NOT:
                    not(circut[s], head);
                    break;
                case XOR:
                    xor(circut[s], head);
                    break;
                case MULTIPLEXER:
                    multi(circut[s], head);
                    break;
                case PASS:
                    pass(circut[s], head);
                    break;
                case DECODER:
                    deco(circut[s], head);
                    break;
                }
            }
            printOutputs(outputs, numOut, head);
        }
        printf("\n");
 
        free(inputs);
        for ( int i =0; i < count; i ++){
            free ( input [i]);
        }
        free (input);
        free(outputs);
        freeLinkedList(head);
        free(sels);
        for (int s = 0; s < steps; s++)
        {
            free(circut[s].selectorIndices);
            free(circut[s].inpInd);
            free(circut[s].outInd);
           
        }
        free(circut);
        for (int v = 0; v < nextIndex; v++)
        {
            free(variable[v].name);
        }
        free(variable);
        return 0;
    }
}
 
 
 

