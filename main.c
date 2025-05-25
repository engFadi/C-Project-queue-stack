//Fadi Bassous
//1221005
//sec 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct queue {
    char string[100];
    struct queue *next;

} typedef queue;

struct undo {
    int index;
    char inre[10];
    char word[50];
    struct undo *next;

} typedef undo;


struct redo {
    int index;
    char inre[10];
    char word[50];
    struct redo *next;

} typedef redo;

queue *front = NULL, *rear = NULL;
undo *topU = NULL;
redo *topR = NULL;

char text[200];//Global text from file
char *tempT[200];//Global temp text from file


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void readFromFile() {
    FILE *file;
    file = fopen("readString.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
    }

    if (fgets(text, sizeof(text), file) == NULL) {
        perror("Error reading file");
        fclose(file);
    }

    fclose(file);
    strcpy(tempT,text) ;//save the file in temp to print

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void enqueue(char string[]) {

    char *piece = strtok(string, " ");//to split the sentence then add each word in a single node
    queue *newQ = (queue *) malloc(sizeof(queue));
    if (newQ == NULL) {
        printf("out of space");
        exit(1);
    }
    strcpy(newQ->string, piece);

    newQ->next = NULL;
    if (front == NULL && rear == NULL) {
        front = rear = newQ;
    }

    while (piece != NULL)
    {
        piece = strtok(NULL, " ");
        if (piece != NULL) {
            queue *newQ2 = (queue *) malloc(sizeof(queue));
            if (newQ2 == NULL) {
                printf("out of space");
                exit(1);
            }
            strcpy(newQ2->string, piece);
            newQ2->next = NULL;
            rear->next = newQ2;
            rear = newQ2;
        }
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void dequeue() {
    queue *temp = front;
    if (front == NULL) {
        printf("Queue is empty");
        return;
    }
    if (front == rear)
        front = rear = NULL;
    else {

        front = front->next;
    }
    free(temp);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void creatQueue() {


    char str[100];
    scanf(" %[^\n]", str);
    enqueue(str);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void popU() {
    undo *temp;
    if (topU == NULL) {
        printf("Empty stack");
        return;
    }
    temp = topU;
    topU = topU->next;
    free(temp);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void pushU(char word[50], char inre[10], int index) {
    undo *newS = (undo *) malloc(sizeof(undo));
    if (newS == NULL) {
        printf("out of space");
        exit(1);
    }
    strcpy(newS->word, word);
    strcpy(newS->inre, inre);
    newS->index = index;
    newS->next = topU;
    topU = newS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void undoPrint() {

    undo *temp = topU;
    printf("undo\n");
    while (temp) {
        printf("%s %s %d\n", temp->word, temp->inre, temp->index);
        temp = temp->next;

    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void printText() {
    printf("%s\n", text);
    printf("\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void popR() {
    redo *temp;
    if (topR == NULL) {
        printf("Empty stack");
        return;
    }
    temp = topR;
    topR = topR->next;
    free(temp);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void pushR(char word[50], char inre[10], int index) {
    redo *newS = (redo *) malloc(sizeof(redo));
    if (newS == NULL) {
        printf("out of space");
        exit(1);
    }
    strcpy(newS->word, word);
    strcpy(newS->inre, inre);
    newS->index = index;
    newS->next = topR;
    topR = newS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void insert(int index) {
    queue *tempQ = front;
    int length = 0;
    index-1;
    char *temp=text;
    while (temp[length] != '\0' && length < 100)//evaluate a length of the text
    {
        length++;
    }
    while (tempQ) {

        if (index < length)
        {

                int l = strlen(tempQ->string);
                int shiftAmount = l + 1;

                for (int i = strlen(text); i >= index; i--)//shift
                {
                    text[i + shiftAmount] = text[i];

                }
                // Now insert the new word
                for (int i = 0; i < l; i++) {
                    text[index + i] = tempQ->string[i];

                }
                // Insert the space after the word
                text[index + l] = ' ';
                pushU(tempQ->string, "insert", index);
                tempQ = tempQ->next;
                dequeue();

        }

        else//insert at the end if the text
        {
            strcat(text, " ");
            strcat(text, tempQ->string);
            pushU(tempQ->string, "insert", -1);
            tempQ = tempQ->next;
            dequeue();
        }

    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Remove1(char string[100])//this function to remove the removed string when we  perform the Undo operation
{
    char temp[100];
    char *str1 = strcpy(temp, text);
    char *str = strtok(str1, " ");
    int index = 0;
    int count = 1;
    while (strcmp(str, string) != 0) {
        index += strlen(str);
        str = strtok(NULL, " ");
        count++;
        if (str == NULL) {
            printf("can't remove\n");
            return;
        }
    }

    int l = index + count;
    printf("%d\n", l);
    pushU(str, "remove", l);
    int len = strlen(str);
    for (int i = l + len; l < strlen(text); i++, l++) //shift lift
    {
        text[l - 1] = text[i];
    }

}
void Remove(char string[100]) //remove any inserted string
{
    char temp[100];
    char *str1 = strcpy(temp, text);
    char *str = strtok(str1, " ");
    int index = 0;
    int count = 1;
    while (strcmp(str, string) != 0) {
        index += strlen(str);
        str = strtok(NULL, " ");
        count++;
        if (str == NULL) {
            printf("can't remove\n");
            return;
        }
    }

    int l = index + count;
    printf("%d\n", l);
    int len = strlen(str);
    for (int i = l + len; l < strlen(text); i++, l++) //shift lift
    {
        text[l - 1] = text[i];
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void undoToRedo() {
    undo *temp = topU;
    if (temp == NULL) {
        printf("Undo stack is empty\n");
        return;
    }

    if (temp->index != -1 && strcmp(temp->inre,"insert")==0)
    {
        Remove(temp->word);
        pushR(temp->word, temp->inre, temp->index);
        popU();
        return;
    }
     if(strcmp(temp->inre,"remove")==0)
    {
        int index = temp->index-1;
        int l = strlen(temp->word);
        int shiftAmount = l + 1;

        for (int i = strlen(text); i >= index; i--)//shift right
        {
            text[i + shiftAmount] = text[i];
        }
        // Now insert the new word
        for (int i = 0; i < l; i++) {
            text[index + i] = temp->word[i];
        }
        // Insert the space after the word
        text[index + l] = ' ';
        pushR(temp->word, temp->inre, temp->index);
        popU();
        return;
    }



    Remove(temp->word);
    pushR(temp->word, temp->inre, temp->index);
    popU();

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void redoToUndo() {
    redo *temp = topR;
    if (temp == NULL) {
        printf("Redo stack is empty\n");
        return;
    }
    if (temp->index != -1 && strcmp(temp->inre,"insert")==0) {

        int index = temp->index;
        int l = strlen(temp->word);
        int shiftAmount = l + 1;

        for (int i = strlen(text); i >= index; i--)//shift right
        {
            text[i + shiftAmount] = text[i];
        }
        // Now insert the new word
        for (int i = 0; i < l; i++) {
            text[index + i] = temp->word[i];
        }
        // Insert the space after the word
        text[index + l] = ' ';
        pushU(temp->word, temp->inre, temp->index);
        popR();
        return;
    }
    if(strcmp(temp->inre,"remove")==0)
    {

        Remove(temp->word);
        pushU(temp->word, temp->inre, temp->index);
        popR();
        return;

    }

    pushU(temp->word, temp->inre, temp->index);
    strcat(text, " ");
    strcat(text, temp->word);
    popR();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void redoPrint() {

    redo *temp = topR;
    printf("redo\n");

    while (temp) {
        printf("%s %s %d\n", temp->word, temp->inre, temp->index);
        temp = temp->next;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void saveToFile() {

    FILE *file = fopen("output.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(file, "%s", text);
    fclose(file);

    // Clear the stack topU
    while (topU) {
        popU();
    }

    // Clear the stack topR
    while (topR) {
        popR();
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void freeMemory() {
    queue *q = front;
    queue *tempQ;
    undo *u = topU;
    undo *tempU;
    redo *r = topR;
    redo *tempR;
    while (q) {
        tempQ = q;
        q = q->next;
        free(tempQ);

    }
    while (u) {
        tempU = u;
        u = u->next;
        free(tempU);

    }
    while (r) {
        tempR = r;
        r = r->next;
        free(tempR);

    }

}

//main------------------------------------------------------------------------------------------------------------------
void menu();

int main() {
    menu();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void menu() {
    int choice = 0;

    puts("1. Load the input file which contains the initial text."
         "\n2. Print the loaded text."
         "\n3. Insert strings to the text."
         "\n4. Remove strings from the text."
         "\n5. Perform Undo operation."
         "\n6. Perform Redo operation"
         "\n7. Print the Undo Stack and the Redo stack"
         "\n8. Save the updated text to the output file."
         "\n9. Exit");
    printf("\n");
    scanf(" %d", &choice);

    while (choice != 1) {
        printf("press 1 to load the file:\n");
        printf("\n");
        scanf(" %d", &choice);
    }
    char s[100];
    int i;
    while (1) {
        switch (choice) {
            case 1:
                readFromFile();
                printf("The input file has been loaded , press 2 to display its contents.\n");
                printf("\n");
                break;
            case 2:
                printText();
                break;
            case 3:
                printf("insert a new word:\n");
                creatQueue();
                printf("location(200 at the end of the text): %s\n",text);
                scanf("%d",&i);
                insert(i);
                break;
            case 4:
                scanf("%s", s);
                Remove1(s);
                break;
            case 5:
                undoToRedo();
                break;
            case 6:
                redoToUndo();
                break;
            case 7:
                undoPrint();
                printf("\n");
                redoPrint();
                break;
            case 8:
                saveToFile();
                break;
            case 9:
                freeMemory();
                exit(1);
            default:
                puts("wrong input\n");
        }
        puts("1. Load the input file which contains the initial text."
             "\n2. Print the loaded text."
             "\n3. Insert strings to the text."
             "\n4. Remove strings from the text."
             "\n5. Perform Undo operation."
             "\n6. Perform Redo operation"
             "\n7. Print the Undo Stack and the Redo stack"
             "\n8. Save the updated text to the output file."
             "\n9. Exit");
        printf("\n");
        scanf(" %d", &choice);
    }
}