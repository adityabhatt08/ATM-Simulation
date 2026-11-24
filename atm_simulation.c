#include <stdio.h>
#include <string.h>

struct user
{
    char name[20];
    char username[30];
    char password[9];
    char account_no[12];
    float balance;
};

struct user currentUser = {0};
int userIndex = -1;

void welcome();
void login();
void signup();
int countUsers();
void redirect(void (*)(void));
void loadDetails(struct user [], int);
void fixString(char *);
void dashboard();
void absorbNewline();
void dashboardOptions();
void checkBalance();
void deposit();
void withdraw();
void logout();
void delete();

int main(){
    welcome();
    printf("Exiting...");
    return 0;
}

void welcome(){
    printf("Welcome to the digital ATM services.\n");
    printf("Please choose an option from below:\n");
    printf("1. Login\n2. Signup\n3. Exit\n");
    int option = 0;
    int input;
    enter:
    input = scanf("%d",&option);
    if(input!=1)
        return;
    switch(option){
        case 1: login(); break;
        case 2: signup(); break;
        case 3: return;
        default: printf("Invalid input! Please try again: "); goto enter;
    }
}

void login(){
    int count = countUsers();
    if(count==0){
        printf("No accounts found in the database. Please signup!\n");
        redirect(welcome);
        return;
    }
    struct user details[count];
    loadDetails(details,count);
    printf("Please enter your credentials:\n");
    printf("Username: ");
    char username[30];
    scanf("%29s",username);
    for(int i=0; i<count; i++){
        if(strcmp(username,details[i].username)==0){
            userIndex = i;
            break;
        }
        else if(i==count-1){
            printf("Username not found!\n");
            redirect(welcome);
            return;
        }
    }
    printf("Password: ");
    char password[9];
    scanf("%8s",password);
    if(strcmp(password,details[userIndex].password)==0){
        printf("\033[2J\033[H");
        fflush(stdout);
        printf("Login Successful!\n");
        currentUser = details[userIndex];
        dashboard();
    }
    else{
        printf("Wrong Password!\n");
        userIndex = -1;
        redirect(welcome);
    }
}

void signup(){
    absorbNewline();
    char name[20];
    char account_no[12];
    char username[30];
    char password[9];
    float amount;
    int count = countUsers();
    if(count==0){
        printf("Enter your name: ");
        fgets(name,20,stdin);
        fixString(name);
        printf("Enter your account number(11 digits): ");
        acn:
        scanf("%11s",account_no);
        if(strlen(account_no)<11){
            printf("Enter a valid account no.: ");
            goto acn;
        }
        printf("NOTE: Username should not contain spaces and more than 29 characters!\n");
        printf("Enter a username: ");
        scanf("%29s",username);
        printf("NOTE: Password should not contain spaces and more than 8 characters!\n");
        printf("Create a password: ");
        scanf("%8s",password);
        printf("Please deposit some amount into your account: ");
        am:
        scanf("%f",&amount);
        if(amount<0){
            printf("Please enter a valid amount: ");
            goto am;
        }
    }
    else{
        struct user details[count];
        loadDetails(details,count);
        printf("Enter your name: ");
        fgets(name,20,stdin);
        fixString(name);
        printf("Enter your account number(11 digits): ");
        ac:
        scanf("%11s",account_no);
        if(strlen(account_no)<11){
            printf("Enter a valid account number: ");
            goto ac;
        }
        for(int i=0; i<count; i++)
            if(strcmp(account_no,details[i].account_no)==0){
                printf("Account already exists! Please go to login page.\n");
                redirect(welcome);
                return;
            }
        printf("NOTE: Username should not contain spaces and more than 29 characters!\n");
        printf("Enter a username: ");
        us:
        scanf("%29s",username);
        for(int i=0; i<count; i++){
            if(strcmp(username,details[i].username)==0){
                printf("Username unavailable! Please choose another: ");
                goto us;
            }
        }
        printf("NOTE: Password should not contain spaces and more than 8 characters!\n");
        printf("Create a password: ");
        scanf("%8s",password);
        printf("Please deposit some amount into your account: ");
        amo:
        scanf("%f",&amount);
        if(amount<0){
            printf("Please enter a valid amount: ");
            goto amo;
        }
    }
    FILE *user, *pass, *nam, *acc, *bal, *cou;
    user = fopen("usernames.txt","a");
    pass = fopen("passwords.txt","a");
    nam = fopen("names.txt","a");
    acc = fopen("accounts.txt","a");
    bal = fopen("balances.txt","a");
    cou = fopen("count.txt","w");
    fprintf(user,"%s\n",username);
    fprintf(pass,"%s\n",password);
    fprintf(nam,"%s\n",name);
    fprintf(acc,"%s\n",account_no);
    fprintf(bal,"%f\n",amount);
    fclose(user);
    fclose(pass);
    fclose(nam);
    fclose(acc);
    fclose(bal);
    count++;
    fprintf(cou,"%d",count);
    fclose(cou);
    printf("\033[2J\033[H");
    fflush(stdout);
    printf("Account created successfully. You can now login with your credentials.\n");
    redirect(welcome);
}

int countUsers(){
    FILE *cou;
    cou = fopen("count.txt","r");
    int count = 0;
    if(cou==NULL){
        return 0;
    }
    fscanf(cou,"%d",&count);
    fclose(cou);
    return count;
}

void redirect(void (*menu)(void)){
    printf("\nRedirecting to previous menu...\n\n");
    menu();
}

void loadDetails(struct user details[],int count){
    FILE *user, *pass, *nam, *acc, *bal;
    user = fopen("usernames.txt","r");
    pass = fopen("passwords.txt","r");
    nam = fopen("names.txt","r");
    acc = fopen("accounts.txt","r");
    bal = fopen("balances.txt","r");
    for(int i=0; i<count; i++){
        fgets(details[i].name,20,nam);
        fixString(details[i].name);
        fscanf(user,"%29s",details[i].username);
        fscanf(pass,"%8s",details[i].password);
        fscanf(acc,"%11s",details[i].account_no);
        fscanf(bal,"%f",&details[i].balance);
    }
    fclose(user);
    fclose(pass);
    fclose(nam);
    fclose(acc);
    fclose(bal);
}

void fixString(char * str){
    str[strcspn(str,"\n")] = '\0';
}

void dashboard(){
    char ac_no[12];
    strcpy(ac_no,currentUser.account_no);
    ac_no[4] = '\0';
    printf("Welcome %s!\nYour account number is: %sXXXXXXX\n",currentUser.name,ac_no);
    dashboardOptions();
    
}

void absorbNewline(){
    char temp[5];
    fgets(temp,5,stdin);
}

void dashboardOptions(){
    printf("Choose an option from below:\n");
    printf("1. Check Balance\n2. Deposit Money\n3. Withdraw Money\n4. Logout\n5. Delete account\n");
    int option = 0;
    int input = scanf("%d",&option);
    if(input!=1)
        return;
    switch(option){
        case 1: checkBalance(); break;
        case 2: deposit(); break;
        case 3: withdraw(); break;
        case 4: logout(); break;
        case 5: delete(); break;
        default: printf("Invalid input. Try again!\n"); dashboardOptions();
    }
}

void checkBalance(){
    printf("Your Balance is: %.2f\n",currentUser.balance);
    redirect(dashboardOptions);
}

void deposit(){
    printf("Enter the amount to deposit: ");
    float amount = 0;
    scanf("%f",&amount);
    if(amount<=0){
        printf("Invalid amount!\n");
        redirect(dashboardOptions);
        return;
    }
    currentUser.balance += amount;
    FILE *bal;
    bal = fopen("balances.txt","r");
    int count = countUsers();
    float balances[count];
    for(int i=0; i<count; i++)
        fscanf(bal,"%f",&balances[i]);
    fclose(bal);
    balances[userIndex] = currentUser.balance;
    bal = fopen("balances.txt","w");
    for(int i=0; i<count; i++)
        fprintf(bal,"%f\n",balances[i]);
    fclose(bal);
    char ac_no[12];
    strcpy(ac_no,currentUser.account_no);
    ac_no[4] = '\0';
    printf("TRANSACTION ALERT : DEPOSIT SUCCESSFUL\n");
    printf("Account No : %sXXXXXXX\n",ac_no);
    printf("Amount : Rs. %.2f\n",amount);
    printf("Avail. Bal : Rs. %.2f\n",currentUser.balance);
    redirect(dashboardOptions);
}

void withdraw(){
    printf("Enter the amount to withdraw: ");
    float amount = 0;
    scanf("%f",&amount);
    if(amount<=0){
        printf("Invalid amount!\n");
        redirect(dashboardOptions);
        return;
    }
    if(amount>currentUser.balance){
        printf("Insufficient balance!\n");
        redirect(dashboardOptions);
        return;
    }
    currentUser.balance -= amount;
    FILE *bal;
    bal = fopen("balances.txt","r");
    int count = countUsers();
    float balances[count];
    for(int i=0; i<count; i++)
        fscanf(bal,"%f",&balances[i]);
    fclose(bal);
    balances[userIndex] = currentUser.balance;
    bal = fopen("balances.txt","w");
    for(int i=0; i<count; i++)
        fprintf(bal,"%f\n",balances[i]);
    fclose(bal);
    char ac_no[12];
    strcpy(ac_no,currentUser.account_no);
    ac_no[4] = '\0';
    printf("TRANSACTION ALERT : WITHDRAWAL SUCCESSFUL\n");
    printf("Account No : %sXXXXXXX\n",ac_no);
    printf("Amount : Rs. %.2f\n",amount);
    printf("Avail. Bal : Rs. %.2f\n",currentUser.balance);
    redirect(dashboardOptions);
}

void logout(){
    printf("Logging out...\n");
    strcpy(currentUser.name,"");
    strcpy(currentUser.username,"");
    strcpy(currentUser.password,"");
    strcpy(currentUser.account_no,"");
    currentUser.balance = 0;
    redirect(welcome);
}

void delete(){
    printf("Are you sure you want to delete your account?:\n");
    printf("1. Yes\n2. No\n");
    int option = 0;
    int input = scanf("%d",&option);
    if(input!=1)
        return;
    switch(option){
        case 1: printf("This account will be deleted.\n"); break;
        case 2: dashboardOptions(); return;
        default: printf("Invalid input!\n"); delete(); return;
    }
    int count = countUsers();
    struct user details[count];
    loadDetails(details,count);
    count--;
    FILE *cou;
    cou = fopen("count.txt","w");
    fprintf(cou,"%d",count);
    fclose(cou);
    FILE *acc, *bal, *nam, *pass, *user;
    acc = fopen("accounts.txt","w");
    bal = fopen("balances.txt","w");
    nam = fopen("names.txt","w");
    pass = fopen("passwords.txt","w");
    user = fopen("usernames.txt","w");
    if(count==0){
        fclose(acc);
        fclose(bal);
        fclose(nam);
        fclose(pass);
        fclose(user);
        logout();
        return;
    }
    for(int i=0; i<=count; i++){
        if(i==userIndex)
            continue;
        fprintf(acc,"%s\n",details[i].account_no);
        fprintf(bal,"%f\n",details[i].balance);
        fprintf(nam,"%s\n",details[i].name);
        fprintf(pass,"%s\n",details[i].password);
        fprintf(user,"%s\n",details[i].username);
    }
    fclose(acc);
    fclose(bal);
    fclose(nam);
    fclose(pass);
    fclose(user);
    logout();
}