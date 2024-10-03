#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX 100 //max reservations

typedef struct { //structer for storing reservations information
    char nom[50]; 
    char prenom[50]; 
    char tel[15];
    int age;
    int statut; 
    int id; 
    char date[20];
} Reserv;

Reserv reservations[MAX]; //an array within our structer to store each reservation uniquely
int count = 10; //how many reservations we have we start by 0
int id = 11; //starting id

void menu() {// main menu
    printf("\nMenu:\n");
    printf("1. Ajouter une reservation\n");
    printf("2. Modifier ou supprimer une reservation\n");
    printf("3. Afficher les details dune reservation\n");
    printf("4. Trier les reservations\n");
    printf("5. Rechercher des reservations\n");
    printf("6. Afficher les statistiques\n");
    printf("7. Quitter\n");
}

void editdelmenu() {//edit or delete menu
    printf("\nModifier ou Supprimer:\n");
    printf("1. Modifier une reservation\n");
    printf("2. Supprimer une reservation\n");
    printf("3. Retourner\n");
}

void searchmenu() {//searching menu
    printf("\nRecherche:\n");
    printf("1. Par ID\n");
    printf("2. Par Nom\n");
    printf("3. Retourner\n");
}

void sortmenu() {//sorting menu
    printf("\nTrier par:\n");
    printf("1. Nom\n");
    printf("2. Statut\n");
    printf("3. Retourner\n");
}

void statmenu() {//statistics menu
    printf("\nMenu des Statistiques:\n");
    printf("1. Moyenne dage des patients\n");
    printf("2. Nombre de patients par tranche dage\n");
    printf("3. Nombre total de reservations par statut\n");
    printf("4. Quitter\n");
}

void fgt(char input[], size_t n) { //function for easier fgets
    fgets(input, n, stdin);
    input[strcspn(input, "\n")] = 0; //remove \n
}

int getchoice(int min, int max) {//function to get the user choice because we have nested menu
    int choice;
    while (1) {
        printf("Entrez votre choix (%d-%d): ", min, max);
        if (scanf("%d", &choice) == 1 && choice >= min && choice <= max) { //checking the correct choice
            getchar(); // clearing new line
            return choice;
        } else {
            printf("Erreur, entrez un nombre valide.\n");
            while (getchar() != '\n'); //clearing the input
        }
    }
}

int checkage(int age) { //checking the age
    if (age > 0 && age < 90) {
        return 1;
    } else {
        printf("Erreur, Entrez un age valide\n");
        return 0;
    }
}

int checkname(char *nom) { //checking the name
    for (int i = 0; nom[i] != '\0'; i++) {
        if (!isalpha(nom[i]) && isspace(nom[i])) { //making sure the name doesnt contain numbers
            printf("Erreur, Entrer un nom valide.\n");
            return 0;
        }
    }
    return 1;
}

int checknum(char *num, int idx) { //checking the numbers
    for (int i = 0; i < count; i++) {
        if (i != idx && strcmp(reservations[i].tel, num) == 0) { //making sure that the number dont already exists
            printf("Erreur, ce numero existe deja\n");
            return 0;
        }  
    }
        
    if (strlen(num) != 10 || strspn(num, "0123456789") != 10) { //making sure the number is 10digits long
        printf("Erreur, Entrez un numero de telephone valide\n");
        return 0;
    }

    if (strncmp(num, "06", 2) != 0 && strncmp(num, "07", 2) != 0) {//making sure the number starts with 06 or 07
        printf("Erreur, le numero doit commencer par 06 ou 07\n");
        return 0;
    }

    return 1;
}

int checkstatut(int statut) {//checking status
    if (statut >= 1 && statut <= 4){
        return 1;}
    else
        printf("Erreur, Entrez une status valide");
        return 0;
}

int checkdate(char date[]) {//checking the date format
    int anne, moi, jour;
    if (sscanf(date, "%4d-%2d-%2d", &anne, &moi, &jour) != 3) {//making sure the date is in this format
        printf("Erreur, Entrez une format valide(YYYY-MM-DD)\n"); //YYYY-MM-DD
        return 0;
    }

    if (moi < 1 || moi > 12 || jour < 1 || jour > 31) {//making sure the number of days and months is correct
        printf("Erreur, Entrez jour ou mois valide\n");
        return 0;
    }

    if (moi == 2 && jour > 28) { //same thing for this one but this is for february 
        printf("Erreur, Entrez jour valide\n");//because it has 28 days
        return 0;
    } else if ((moi == 4 || moi == 6 || moi == 9 || moi == 11) && jour > 30) {//and this is for months that
        printf("Erreur, Entrez jour valide\n");                              // only have 30 days
        return 0;
    }

    return 1;
}

void add() {//for adding a reservation
    if (count >= MAX) {
        printf("les reservations sont completes.\n");//if max reservations limit is reached
        return;
    }

    Reserv new; //structer for a new reservation

    //adding each element of a reservation while checking the input is correct
    do {
        printf("Nom: ");
        fgt(new.nom, sizeof(new.nom));
    } while (!checkname(new.nom));

    do {
        printf("Prenom: ");
        fgt(new.prenom, sizeof(new.prenom));
    } while (!checkname(new.prenom));
    
    do {
        printf("Telephone: ");
        fgt(new.tel, sizeof(new.tel));
    } while (!checknum(new.tel, count));    

    char inputage[10];//here becasause we use fgets we must first assing a char
    do {
        printf("Age: ");
        fgt(inputage, sizeof(inputage));
        new.age = atoi(inputage);//then turn that char into an int because of how fgets work
    } while (!checkage(new.age));

    char inputstatut[10];//same thing for statut
    do {
        printf("Statut(1.Valide, 2.Raporte, 3.Annule, 4.Traite): ");
        fgt(inputstatut, sizeof(inputstatut));
        new.statut = atoi(inputstatut);
    } while (!checkstatut(new.statut));

    do {
        printf("Date(YYYY-MM-DD): ");
        fgt(new.date, sizeof(new.date));
    } while (!checkdate(new.date));

    new.id = id++; // new id (reference)
    
    reservations[count++] = new; //adding the new reservation to the reservations array we declared earlier
    printf("Cet operation est succes!");
}

void showres(Reserv res) {//here we declare a reservation variable inside the function to print out reservations
    printf("\nID: %d|Nom: %s Prenom: %s|Telephone: %s|Age: %d|Satut: %d|Date: %s \n",res.id, res.nom, res.prenom, res.tel, res.age, res.statut,res.date);
}

void showresdetails() {//here we loop through each reservation and show its details
    /*char search[10];
    printf("Entrez lID de la reservation: ");
    fgt(search, sizeof(search));
    int inputsearch = atoi(search);*/
    //the code above is if we want to display a reservation with its unique id

    if (count == 0){
        printf("Aucune reservation a afficher.\n");
    }

    for (int i = 0; i < count; i++) { //the loop for doing this
        //if (reservations[i].id == inputsearch) {
            showres(reservations[i]);
        }
    
    //}
    //printf("Aucune reservation trouvee avec cette ID.\n");
    return;
}

void edit() {//function to edit a reservation with id
    char search[10]; //because of fgets we use a char then convert it to an int
    printf("Entrez lID de la reservation: ");
    fgt(search, sizeof(search));
    int idSearch = atoi(search);

    for (int i = 0; i < count; i++) { //search loop to find said reservation with id
        if (reservations[i].id == idSearch) {
            printf("Modifier la reservation %d:\n", reservations[i].id);
            do {
                printf("Nom: ");
                fgt(reservations[i].nom, sizeof(reservations[i].nom));
            } while (!checkname(reservations[i].nom));

            do {
                printf("Prenom: ");
                fgt(reservations[i].prenom, sizeof(reservations[i].prenom));
            } while (!checkname(reservations[i].prenom));

            do {
                printf("Telephone: ");
                fgt(reservations[i].tel, sizeof(reservations[i].tel));
            } while (!checknum(reservations[i].tel, count));

            char inputage[10];
            do {
                printf("Age: ");
                fgt(inputage, sizeof(inputage));
                reservations[i].age = atoi(inputage);
            } while (!checkage(reservations[i].age));

            char inputstatut[10];
            do {
                printf("Statut: ");
                fgt(inputstatut, sizeof(inputstatut));
                reservations[i].statut = atoi(inputstatut);
            } while (!checkstatut(reservations[i].statut));

            do {
                printf("Date de reservation: ");
                fgt(reservations[i].date, sizeof(reservations[i].date));
            } while (!checkdate(reservations[i].date));

            printf("Reservation mise a jour avec succes!\n");
            return;
        }
    }
    printf("Aucune reservation trouvee avec cette ID.\n");//else we print there are no reservatiosn with this id
}

void delete() {//function to delete a reservation with id
    char search[10];//same thing we did with edit function
    printf("Entrez lID de la reservation: ");
    fgt(search, sizeof(search));
    int idSearch = atoi(search);

    for (int i = 0; i < count; i++) {//loop through each reservation to find the correct one
        if (reservations[i].id == idSearch) {//if found
            for (int j = i; j < count - 1; j++) {//then we start moving each reservation to the left
                reservations[j] = reservations[j + 1];//
            }
            count--;
            printf("Reservation supprimee avec succes!\n");//once the inside loop is done we print
            return;//then return
        }
    }
    printf("Aucune reservation trouvee avec cette ID.\n");//else we print there are no reservations with the id
}

void sortwithanme() { //sorting with name using bubble sort
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(reservations[j].nom, reservations[j + 1].nom) > 0) {//if the first reserve we ecnounter
                Reserv temp = reservations[j];//is bigger than the next then we swap until
                reservations[j] = reservations[j + 1];//we have an ascending order alphabetically
                reservations[j + 1] = temp;//storing in a temp variable but because we have a struct
                //we make the temp variable with our strutcer to preserve info
            }
        }
    }
    printf("Reservations triees par nom.\n");//print success
}

void sortwithstatus() {//sorting with status
    for (int i = 0; i < count - 1; i++) {//same thing as name we use bubble sort
        for (int j = 0; j < count - i - 1; j++) {
            if (reservations[j].statut > reservations[j + 1].statut) {//but now we only compare status
                Reserv temp = reservations[j];//in the ascending order
                reservations[j] = reservations[j + 1];
                reservations[j + 1] = temp;
            }
        }
    }
    printf("Reservations triees par statut.\n");//print success
}

void searchwithid() {//searching with id
    char search[10];
    printf("Entrez lID de la reservation: ");
    fgt(search, sizeof(search));
    int idSearch = atoi(search);

    for (int i = 0; i < count; i++) {//searching with linear search
        if (reservations[i].id == idSearch) {
            showres(reservations[i]);//here we call the displaying function we established earlier
            return;
        }
    }
    printf("Aucune reservation trouvee avec cette ID.\n");//print false
}

void searchwithname() {//searching with name
    char name[50];
    printf("Entrez le nom de la reservation: ");
    fgt(name, sizeof(name));

    int found = 0;//variable to check if we foudn said reservation with name
    for (int i = 0; i < count; i++) {//using linear search
        if (strcmp(reservations[i].nom, name) == 0) {//using strcmp to compare between name and search input
            showres(reservations[i]);//display function
            found = 1;//that means we found said reservation
        }
    }
    if (!found) {//if found is 0 then we print false
        printf("Aucune reservation trouvee avec ce nom.\n");
    }
}

void calculmoyenne() {//calculating average
    if (count == 0) { //checking if we have any reservations
        printf("Aucune reservation a afficher.\n");
        return;
    }

    double total = 0;//variable to store the total of ages
    for (int i = 0; i < count; i++) {//loop to store each age
        total += reservations[i].age;
    }

    printf("Moyenne dage: %.2f\n", total / count);
}

void countages() {//counting ages
    if (count == 0) {
        printf("Aucune reservation a afficher.\n");
        return;
    }

    int group[3] = {0}; //variable to store age groups
    //0-18 is group[0], 19-35 group[1], 36+ group[2]

    for (int i = 0; i < count; i++) {//a loop to store age group in its own group array
        if (reservations[i].age <= 18) group[0]++;
        else if (reservations[i].age <= 35) group[1]++;
        else group[2]++;
    }

    printf("Nombre de patients par tranche dage:\n"); //here we print everything
    printf("0-18: %d\n", group[0]);
    printf("19-35: %d\n", group[1]);
    printf("36+: %d\n", group[2]);
}

void countbystatus() {//counting by status
    if (count == 0) {
        printf("Aucune reservation a afficher.\n");
        return;
    }

    int status[4] = {0}; // 1 Valide, 2 Reporte, 3 Annule, 4 Traite

    for (int i = 0; i < count; i++) {//a loop to check each reservation
        if (reservations[i].statut >= 1 && reservations[i].statut <= 4) {
            status[reservations[i].statut - 1]++; //here we do -1 to accomodate for the array
            //because it starts with 0, and then we store by incrementing
        }
    }

    printf("Nombre total de reservations par statut:\n");//printing
    printf("Valide: %d\n", status[0]);
    printf("Reporte: %d\n", status[1]);
    printf("Annule: %d\n", status[2]);
    printf("Traite: %d\n", status[3]);
}

void def();//prototype function for defaults

int main() {
    int choice; //variable for choice

    def();//for inserting default reservations

    do {//a loop that only breaks when the user inputs 7(quitter)
        menu();//displaying main menu
        
        choice = getchoice(1, 7);//getting the choice from the user

        switch (choice) {//switch case so the user can choose
            case 1: add(); break; //adding
            case 2: editdelmenu();//edit delete menu
                int choice2 = getchoice(1,3);//getting the choice from the user that is limited to 3 choices
                if (choice2 == 1) { edit(); } //conditions for the choice
                else if (choice2 == 2) { delete(); } break;
            case 3: showresdetails(); break;//displaying reservation details
            case 4: sortmenu();//sorting menu
                int choice3 = getchoice(1,3);//getting the choice that is between 1 to 3
                if (choice3 == 1) { sortwithanme(); } //conditions for the choice
                else if (choice3 == 2) { sortwithstatus(); } break;
            case 5: searchmenu();//search menu
                int choice4 = getchoice(1,3);//getting the choice that is between 1 to 3
                if (choice4 == 1) { searchwithid(); } //conditions for the choice
                else if (choice4 == 2) { searchwithname(); } break;
            case 6: statmenu();//statistics menu
                int choice5 = getchoice(1,4);////getting the choice that is between 1 to 4
                if (choice5 == 1) { calculmoyenne(); } //conditions
                else if (choice5 == 2) { countages(); } 
                else if (choice5 == 3) { countbystatus(); } break;
            case 7: printf("Passez une bonne journee.\n"); break;//if the input is 7 then we print goodbye
            default: break;
        }
    } while (choice != 7);//while do loop unting the choice is not 7

    return 0;
}


void def(){
    strcpy(reservations[0].nom, "Jaiti");
    strcpy(reservations[0].prenom, "Taha");
    strcpy(reservations[0].tel, "0682226573");
    strcpy(reservations[0].date, "2004-08-16");
    reservations[0].age = 21;
    reservations[0].statut = 4;
    reservations[0].id = 1;
    strcpy(reservations[1].nom, "Katkot");
    strcpy(reservations[1].prenom, "Ghali");
    strcpy(reservations[1].tel, "0683276982");
    strcpy(reservations[1].date, "2023-12-03");
    reservations[1].age = 1;
    reservations[1].statut = 4;
    reservations[1].id = 2;
    strcpy(reservations[2].nom, "Sarfok");
    strcpy(reservations[2].prenom, "Youness");
    strcpy(reservations[2].tel, "0713654834");
    strcpy(reservations[2].date, "2016-06-12");
    reservations[2].age = 15;
    reservations[2].statut = 2;
    reservations[2].id = 3;
    strcpy(reservations[3].nom, "EL Ouariachi");
    strcpy(reservations[3].prenom, "Yasser");
    strcpy(reservations[3].tel, "0689342544");
    strcpy(reservations[3].date, "2005-06-12");
    reservations[3].age = 20;
    reservations[3].statut = 1;
    reservations[3].id = 4;
    strcpy(reservations[4].nom, "Ziza");
    strcpy(reservations[4].prenom, "Mohammed");
    strcpy(reservations[4].tel, "0623071034");
    strcpy(reservations[4].date, "2023-06-12");
    reservations[4].age = 40;
    reservations[4].statut = 3;
    reservations[4].id = 5;
    strcpy(reservations[5].nom, "Lkhbizi");
    strcpy(reservations[5].prenom, "Imane");
    strcpy(reservations[5].tel, "0733688450");
    strcpy(reservations[5].date, "2024-02-28");
    reservations[5].age = 37;
    reservations[5].statut = 2;
    reservations[5].id = 6;
    strcpy(reservations[6].nom, "Astir");
    strcpy(reservations[6].prenom, "Nabil");
    strcpy(reservations[6].tel, "0698497658");
    strcpy(reservations[6].date, "2024-08-30");
    reservations[6].age = 5;
    reservations[6].statut = 1;
    reservations[6].id = 7;
    strcpy(reservations[7].nom, "Potchi");
    strcpy(reservations[7].prenom, "Kawter");
    strcpy(reservations[7].tel, "0612563544");
    strcpy(reservations[7].date, "2024-09-23");
    reservations[7].age = 18;
    reservations[7].statut = 3;
    reservations[7].id = 8;
    strcpy(reservations[8].nom, "Nigor");
    strcpy(reservations[8].prenom, "Ilyass");
    strcpy(reservations[8].tel, "0642066420");
    strcpy(reservations[8].date, "2017-11-21");
    reservations[8].age = 27;
    reservations[8].statut = 4;
    reservations[8].id = 9;
    strcpy(reservations[9].nom, "Lefkih");
    strcpy(reservations[9].prenom, "Saad");
    strcpy(reservations[9].tel, "0742042066");
    strcpy(reservations[9].date, "2021-03-24");
    reservations[9].age = 25;
    reservations[9].statut = 3;
    reservations[9].id = 10;
}