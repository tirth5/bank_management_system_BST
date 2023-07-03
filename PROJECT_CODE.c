#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct History
{ // For History Of Transaction
    int amount;
    int id;
} view[2000001][100];

struct Person // For Detail Of User
{
    char Username[500], Password[500];
    int account_No;
    int balance;
    int curr;
    struct Person *left, *right;

} * start, *begin;

int have[200001]; // For  Store Account No. is Three or Not
void menu(struct Person *);
struct Person *newNode(char name[], char password[], int acc) // For Create New Node
{
    struct Person *temp = (struct Person *)malloc(sizeof(struct Person));
    strcpy(temp->Username, name);
    strcpy(temp->Password, password);
    temp->account_No = acc;
    temp->balance = 0;
    temp->curr = 0;
    for (int i = 0; i < 100; i++)
    {
        view[acc][i].id = -1;
        view[acc][i].amount = 0;
    }
    temp->left = temp->right = NULL;
    return temp;
}
struct Person *insert(struct Person *node, char name[], char password[], int acc) // Insert Node(Username Based)
{
    if (node == NULL)
        return newNode(name, password, acc);

    if (strcmpi(name, node->Username) < 0)
        node->left = insert(node->left, name, password, acc);
    else
        node->right = insert(node->right, name, password, acc);

    return node;
}
struct Person *insert_acc(struct Person *node, char name[], char password[], int acc) // Insert Node(Account No. Based)
{
    if (node == NULL)
        return newNode(name, password, acc);

    if (node->account_No > acc)
        node->left = insert_acc(node->left, name, password, acc);
    else
        node->right = insert_acc(node->right, name, password, acc);

    return node;
}

void Search(struct Person *root, char name[], char password[]) // Search For User Exist Or Not
{

    while (root != NULL)
    {
        if (strcmp(name, root->Username) > 0)
        {
            root = root->right;
        }
        else if (strcmp(name, root->Username) < 0)
        {
            root = root->left;
        }
        else
        {
            if (strcmp(password, root->Password) == 0)
            {
                printf("\nLOGIN SUCCESSFUL :)\n");
                menu(root);
                printf("\n");
                return;
            }
            else
            {
                printf("\nINVALID PASSWORD :(\n");
                printf("\n");
                return;
            }
        }
    }
    printf("\nUSERNAME NOT FOUND :(\n");
    printf("\n");
}
struct Person *findAcc(int acc, struct Person *root)
{
    while (root != NULL)
    {
        if (acc < root->account_No)
        {
            root = root->left;
        }
        else if (acc > root->account_No)
        {
            root = root->right;
        }
        else
        {
            return root;
        }
    }
    return NULL;
}
struct Person *findname(char name[], struct Person *root)
{
    while (root != NULL)
    {
        if (strcmpi(name, root->Username) < 0)
        {
            root = root->left;
        }
        else if (strcmpi(name, root->Username) > 0)
        {
            root = root->right;
        }
        else
        {
            return root;
        }
    }
    return NULL;
}
void findUser(char name[], struct Person *root, int amt, int acc_no)
{
    while (root != NULL)
    {
        if (strcmpi(name, root->Username) < 0)
        {
            root = root->left;
        }
        else if (strcmpi(name, root->Username) > 0)
        {
            root = root->right;
        }
        else
        {
            root->balance += amt;
            view[root->account_No][root->curr].amount = amt;
            view[root->account_No][root->curr].id = acc_no;
            root->curr++;
            return;
        }
    }
}
void madeTrans(struct Person *root) // For Transfer Money
{

    printf("ENTER A ACCOUNT NUMBER OF MONEY RECIEVER : ");
    int acc_no;
    scanf("%d", &acc_no);
    if (acc_no == root->account_No)
    {
        printf("THIS IS YOUR ACCOUNT NO.\n");
        return;
    }
    struct Person *tem = findAcc(acc_no, begin);
    struct Person *temp = findname(tem->Username, start);
    if (temp != NULL)
    {
        printf("ENTER AMOUNT : ");
        int amt;
        scanf("%d", &amt);
        if (amt > root->balance)
        {
            printf("TRANSICTION FAILED\n");
            printf("NOTE : ");
            printf("YOUR CURRENT BALANCE IS LOW :(\n");
            return;
        }
        printf("%d $ TRANSFERED SUCCESSFULY TO ACCOUNT NO. %d FROM ACCOUNT NO. %d\n",amt,acc_no,root->account_No);
        findUser(temp->Username, start, amt, root->account_No);
        root->balance = root->balance - amt;
        view[root->account_No][root->curr].amount = amt * (-1);
        view[root->account_No][root->curr].id = temp->account_No;
        root->curr++;
    }
    return;
}

void inorder(struct Person *root) // For Printing User List
{
    if (root != NULL)
    {

        inorder(root->left);
        printf("         %d             |          %s         \n", root->account_No, root->Username);
        printf("\n");
        inorder(root->right);
    }
}
void Registration()
{
    printf("ENTER USERNAME : "); // Input Username
    char name[500];
    fflush(stdin);
    gets(name);
    printf("ENTER PASSWORD : "); // Input Password
    char password[500];
    fflush(stdin);
    gets(password);
    srand(time(0));
    int acc = rand() % 200000 + 1;
    while (have[acc] > 0) // For Assign Random Account No.
    {
        acc = rand() % 200000 + 1;
    }
    have[acc]++;
    printf("\nYOUR ACCOUNT NO. : %d\n", acc);
    start = insert(start, name, password, acc);     //  For Insert In Binary Tree(Username Based)
    begin = insert_acc(begin, name, password, acc); // For Insert In Binary Tree(Account No. Based)
    printf("REGISTARTION SUCCESSFUL :)");
    printf("\n\n");
}

void Login()
{
    printf("ENTER USERNAME : "); // Input Username
    char name[500];
    fflush(stdin);
    gets(name);
    printf("ENTER PASSWORD : "); // Input Password
    char password[500];
    fflush(stdin);
    gets(password);
    Search(start, name, password);
    printf("\n");
}

void checkBal(struct Person *root) // For Checking Cuurent Balance
{
    printf("CURRENT BALANCE : ");
    printf("%d\n", root->balance);
    return;
}

void addMoney(struct Person *root) // For Add Money into Account
{
    printf("ENTER AMOUNT : ");
    int amt;
    scanf("%d", &amt);
    root->balance += amt;
    view[root->account_No][root->curr].amount = amt;
    view[root->account_No][root->curr].id = -1;
    root->curr++;
    printf("%d $ CREDITED SUCCESSFULY :)\n", amt);
    return;
}
void withdraw(struct Person *root)
{ // For Withdraw Money From Account
    printf("ENTER AMOUNT : ");
    int amt;
    scanf("%d", &amt);
    if (root->balance < amt)
    {
        printf("NOTE:");
        printf("YOUR CURRENT BALANCE IS LOW :(");
        return;
    }
    view[root->account_No][root->curr].amount = amt * (-1);
    view[root->account_No][root->curr].id = -1;
    root->curr++;
    printf("%d $ DEBITED SUCESSFULY :)\n", amt);
    root->balance -= amt;
}
void profile(struct Person *root) // For User Profile
{
    printf("O-----------------------------------O\n");
    printf("|           ACCOUNT DETAILS         |\n");
    printf("O-----------------------------------O\n");
    printf("USERNAME : ");
    printf("  %s\n", root->Username);
    printf("ACCOUNT NO. : ");
    printf("  %d\n", root->account_No);
    printf("CURRENT BALANCE : ");
    printf("  %d\n", root->balance);
    return;
}

void watch(struct Person *root) // For User Transaction History
{
    for (int i = 0; i < root->curr; i++)
    {
        if (view[root->account_No][i].amount > 0 && view[root->account_No][i].id > 0)
        {
            printf("%d $ RECIEVED FROM ACCOUNT NO. %d\n", view[root->account_No][i].amount, view[root->account_No][i].id);
        }
        else if (view[root->account_No][i].amount < 0 && view[root->account_No][i].id > 0)
        {
            printf("%d $ TRANSFER IN ACCOUNT NO. %d\n", (-1) * view[root->account_No][i].amount, view[root->account_No][i].id);
        }
        else if (view[root->account_No][i].amount > 0)
        {
            printf("%d $ CREDITED\n", view[root->account_No][i].amount);
        }
        else if (view[root->account_No][i].amount < 0)
        {
            printf("%d $ DEBITED\n", (-1) * view[root->account_No][i].amount);
        }
    }
}
void changePass(struct Person *root) // For Changing Current Password
{
    printf("ENTER YOUR PASSWORD : ");
    char pass[500];
    fflush(stdin);
    gets(pass);
    if (strcmp(pass, root->Password) == 0)
    {
        printf("PASSWORD MATCHED : \n");
        printf("ENTER NEW PASSWORD : ");
        fflush(stdin);
        gets(pass);
        strcpy(root->Password, pass);
        printf("YOUR PASSWORD CHANGED SUCCESSFULY\n");
        return;
    }
    printf("PASSWORD DO NOT MATCH :(");
    printf("\n\n");
}
void menu(struct Person *root) // For Menu
{

    while (1)
    {
        printf("\n");
        printf("O-----------# SERVICE PORTAL #----------O\n");
        printf("|                                       |\n");
        printf("|    PRESS 1 : CHECK BALANCE            |\n");
        printf("|    PRESS 2 : ADD MONEY                |\n");
        printf("|    PRESS 3 : WITHDRAW MONEY           |\n");
        printf("|    PRESS 4 : TRANSFER MONEY           |\n");
        printf("|    PRESS 5 : VIEW ACCOUNT DETAILS     |\n");
        printf("|    PRESS 6 : VIEW TRANACTION          |\n");
        printf("|    PRESS 7 : CHANGE PASSWORD          |\n");
        printf("|    PRESS 8 : LOGOUT                   |\n");
        printf("|                                       |\n");
        printf("O---------------------------------------O\n");
        printf("\nCHOOSE YOUR OPTION : ");
        int op;
        scanf("%d", &op);

        switch (op)
        {
        case 1:
            checkBal(root);
            break;

        case 2:
            addMoney(root);
            break;

        case 3:
            withdraw(root);
            break;

        case 4:
            madeTrans(root);
            break;

        case 5:
            profile(root);
            break;

        case 6:
            watch(root);
            break;

        case 7:
            changePass(root);
            break;

        case 8:
            return;

        default:
            printf("INVALID OPTION\n\n");
        }
    }
}
int main()
{
    // Menu

    while (1)
    {
        printf("\n");
        printf("O-----------# LOGIN MENU #----------O\n");
        printf("|                                   |\n");
        printf("|    PRESS 1 : FOR REGISTARTION     |\n");
        printf("|    PRESS 2 : FOR LOGIN            |\n");
        printf("|    PRESS 3 : FOR DISPLAY          |\n");
        printf("|    PRESS 4 : EXIT MENU            |\n");
        printf("|                                   |\n");
        printf("O-----------------------------------O\n");
        int op;
        printf("\nCHOOSE YOUR OPTION : ");
        scanf("%d", &op);
        switch (op)
        {
        case 1:
            Registration(); // For New Registration
            break;

        case 2:
            Login(); // For Login
            break;

        case 3:
            printf("\n\nDISPALYING ACCOUNT DEATIALS OF ALL USER\n\n"); // For Display Detials of All User
            printf("O--------------------------------------------------O\n");
            printf("|       ACCOUNT NO.        |        USERNAME       |\n");
            printf("O--------------------------------------------------O \n");
            inorder(start);
            break;

        case 4:
            printf("THANK YOU, SEE YOU SOON :)"); // For exit
            return 0;

        default:
            printf("INVALID OPTION\n\n");
        }
    }
}