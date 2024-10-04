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
int id = 11; //next unique id to be assigned

void menu() {// main menu
    printf("\n\t\t******Menu******\n");
    printf("\t1. Ajouter une reservation\n");
    printf("\t2. Modifier ou supprimer une reservation\n");
    printf("\t3. Afficher les details dune reservation\n");
    printf("\t4. Trier les reservations\n");
    printf("\t5. Rechercher des reservations\n");
    printf("\t6. Afficher les statistiques\n");
    printf("\t7. Quitter\n");
}

void editdelmenu() {//edit or delete menu
    printf("\n\t\t******Modifier ou Supprimer******\n");
    printf("\t1. Modifier une reservation\n");
    printf("\t2. Supprimer une reservation\n");
    printf("\t3. Retourner\n");
}

void showmenu() {//statistics menu
    printf("\n\t\t******Menu des Affichage******\n");
    printf("\t1. Tout afficher\n");
    printf("\t2. Affichage avec ID\n");
    printf("\t3. Retourner\n");
}

void searchmenu() {//searching menu
    printf("\n\t\t******Recherche******\n");
    printf("\t1. Par ID\n");
    printf("\t2. Par Nom\n");
    printf("\t3. Retourner\n");
}

void sortmenu() {//sorting menu
    printf("\n\t\t******Trier******\n");
    printf("\t1. Par Nom\n");
    printf("\t2. Par Statut\n");
    printf("\t3. Retourner\n");
}

void statmenu() {//statistics menu
    printf("\n\t\t******Menu des Statistiques******\n");
    printf("\t1. Moyenne dage des patients\n");
    printf("\t2. Nombre de patients par tranche dage\n");
    printf("\t3. Nombre total de reservations par statut\n");
    printf("\t4. Retourner\n");
}

void confirmmenu(int n){
    if (n == 1){
        printf("\nEtes-vous sur de vouloir modifier?");// propmting the user if he is sure to edit said reservation
        printf("\n1. Confirmer");
        printf("\n2. Retourner");}
    else if (n == 2){
        printf("\nEtes-vous sur de vouloir supprimer?");
        printf("\n1. Confirmer");
        printf("\n2. Retourner");}
}

int searchwithid();

void fgt(char input[], size_t n) { //function for easier fgets
    fgets(input, n, stdin);//we use fget to accept whitespaces and not \n
    input[strcspn(input, "\n")] = 0; //remove \n at the end
}

int getchoice(int min, int max) {//function to get the user choice because we have nested menu
    int choice;
    while (1) {
        printf("\nEntrez votre choix (%d-%d): ", min, max);//checking the correct choice
        if (scanf("%d", &choice) == 1 && choice >= min && choice <= max) {
            getchar(); // clearing new line and getting only the first digit
            return choice;
        } else {
            printf("Erreur, Entrez un choix valide.\n");
            while (getchar() != '\n'); //clearing the input
        }
    }
}

int checkage(int age) { //checking the age
    if (age > 0 && age < 100) {// le min age est 1 et le max est 99
        return 1;
    } else {
        printf("Erreur, Entrez un age valide\n");
        return 0;
    }
}

int checkname(char *nom) { //checking the name
    for (int i = 0; nom[i] != '\0'; i++) {
        if (!isalpha(nom[i]) && !isspace(nom[i])) { //making sure the name doesnt contain numbers with spaces
            printf("Erreur, Entrer un nom valide.\n");
            return 0;
        }
    }
    return 1;
}

int checknum(char *num, int idx) { //checking the numbers
    for (int i = 0; i < count; i++) {
        if (i != idx && strcmp(reservations[i].tel, num) == 0) { //making sure that the number dont already exists
            printf("Erreur, Ce numero existe deja\n");
            return 0;
        }  
    }
        
    if (strlen(num) != 10 || strspn(num, "0123456789") != 10) { //making sure the number is 10digits long
        printf("Erreur, Entrez un numero de telephone valide\n");
        return 0;
    }

    if (strncmp(num, "06", 2) != 0 && strncmp(num, "07", 2) != 0) {//making sure the number starts with 06 or 07
        printf("Erreur, Le numero doit commencer par 06 ou 07\n");
        return 0;
    }

    return 1;
}

int checkstatut(int statut) {//checking status
    if (statut >= 1 && statut <= 4){
        return 1;}
    else
        printf("Erreur, Entrez une status valide(1.Valide, 2.Raporte, 3.Annule, 4.Traite).");
        return 0;
}

int checkdate(char date[]) {//checking the date format
    int year, mmonth, day;

    if (sscanf(date, "%d-%d-%d", &year, &mmonth, &day) != 3) {//checking the date format is valid
        printf("Erreur, Entrez une format valide(YYYY-MM-DD)\n"); //YYYY-MM-DD
        return 0; //sscanf returns 4 is something is not valid then we return error
    } else if (mmonth < 1 || mmonth > 12 || day < 1 || day > 31) {//checking the number of days and months is correct
        printf("Erreur, Entrez un jour ou un mois valide\n");
        return 0;
    } else if (year < 1950 || year > 2050){//same thing as above making sure the year enetered is valid
        printf("Erreur, Entrez une annee valide\n");
        return 0;
    } else if (mmonth == 2 && day > 28) { //same thing for this one but this is for february 
        printf("Erreur, Entrez un jour valide\n");//because it has 28 days
        return 0;
    } else if ((mmonth == 4 || mmonth == 6 || mmonth == 9 || mmonth == 11) && day > 30) {
        printf("Erreur, Entrez un jour valide\n");//and this is for months that only have 30 days
        return 0;
    }

    return 1;
}

void getreservation(Reserv *res,int n) {//function to handle reservations info
    //adding or editing each element of a reservation while checking the input is correct
    //using a pointer so the adress wont be random
    do {
        printf("Nom: ");
        fgt(res->nom, sizeof(res->nom));
    } while (!checkname(res->nom));

    do {
        printf("Prenom: ");
        fgt(res->prenom, sizeof(res->prenom));
    } while (!checkname(res->prenom));

    do {
        printf("Telephone: ");
        fgt(res->tel, sizeof(res->tel));
    } while (!checknum(res->tel, n));    

    char inputage[10];
    do {
        printf("Age: ");
        fgt(inputage, sizeof(inputage));
        res->age = atoi(inputage);
    } while (!checkage(res->age));

    char inputstatut[10];
    do {
        printf("Statut(1.Valide, 2.Raporte, 3.Annule, 4.Traite): ");
        fgt(inputstatut, sizeof(inputstatut));
        res->statut = atoi(inputstatut);
    } while (!checkstatut(res->statut));

    do {
        printf("Date(YYYY-MM-DD): ");
        fgt(res->date, sizeof(res->date));
    } while (!checkdate(res->date));
}

void add() {//for adding a reservation
    if (count >= MAX) {
        printf("les reservations sont completes.\n");//if max reservations limit is reached
        return;
    }

    Reserv new; //structer for a new reservation
    getreservation(&new,count);//because we use a pointer
    
    new.id = id++; // new id (reference)
    
    reservations[count++] = new; //adding the new reservation to the reservations array we declared earlier
    printf("Cet operation est succes!");
}

void showres(Reserv res) {//here we declare a reservation variable inside the function to print out reservations
    char statut[10];
    char *stvar[4] = {"Valide", "Raporte", "Annule", "Traite"};//variable for status names

    if (res.statut == 1){strcpy(statut, stvar[0]);}
    if (res.statut == 2){strcpy(statut, stvar[1]);}
    if (res.statut == 3){strcpy(statut, stvar[2]);}
    if (res.statut == 4){strcpy(statut, stvar[3]);}
    printf("\n+----+---+------------------------------+----------------------+--------+---------------+-----------------+");
    printf("\n| %-3d|Nom|Prenom: %-12s %-9s",res.id, res.nom, res.prenom);
    printf("|Telephone: %-11s", res.tel);
    printf("|Age: %-3d", res.age);
    printf("|Statut: %-7s", statut);
    printf("|Date: %-11s|", res.date);
}

void showresdetails() {//here we loop through each reservation and show its details
    if (count == 0){
        printf("il ny a pas de reservations.\n");
        return;
    }
    showmenu();
    int choice = getchoice(1,3);

    if (choice == 1){
        for (int i = 0; i < count; i++) { //the loop for doing this
            showres(reservations[i]);
        }
        return;
    } else if (choice == 2){
        int found = searchwithid();
        if (found == 0) printf("Aucune reservation trouvee avec cette ID.\n");
        return;
    } else if (choice == 3){
        return;
    }
    printf("Aucune reservation trouvee avec cette ID.\n");
    
}

void edit() { // function to edit a reservation with id
    if (count == 0) {
        printf("il ny a pas de reservations.\n");
        return;
    }
    int index = searchwithid(); // finding the index

    if (index == -1) { //check if no reservation was found
        return; // exit the function if not found
    }

    printf("\nModifier la reservation %d:", reservations[index].id);
    confirmmenu(1); // to show the confirmation menu
    int choice = getchoice(1, 2); // taking the choice
    if (choice == 1) { // here the user gets to edit the reservation
        getreservation(&reservations[index], index);
        printf("Reservation mise a jour avec succes!\n");
    }
}

void delete() { // function to delete a reservation with id
    if (count == 0) {
        printf("il ny a pas de reservations.\n");
        return;
    }
    int index = searchwithid(); // finding the index

    if (index == -1) { //checking if the index is found
        return; // exit the function if not found
    }

    printf("\nUne reservation avec cet ID a ete trouvee!");
    confirmmenu(2); // to get confirmation from the user
    int choice = getchoice(1, 2);
    if (choice == 1) {
        for (int j = index; j < count - 1; j++) { //move everything to the left
            reservations[j] = reservations[j + 1];
        }
        count--; // decrement the count because we deleted something
        printf("Reservation supprimee avec succes!\n"); // once the inside loop is done we print
    } else if (choice == 2) {
        return; // do nothing if user chooses to return
    }
}

void sortwithanme() { //sorting with name using bubble sort
    if (count == 0){
        printf("il ny a pas de reservations.\n");
        return;
    }
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcasecmp(reservations[j].nom, reservations[j + 1].nom) > 0) {//if the first reserve we ecnounter
                Reserv temp = reservations[j];//is bigger than the next then we swap until
                reservations[j] = reservations[j + 1];//we have an ascending order alphabetically
                reservations[j + 1] = temp;//storing in a temp variable but because we have a struct
                //we make the temp variable with our strutcer to preserve info
            }
        }
    }
    printf("Reservations triees par nom.\n");//print success

    for (int i = 0; i < count; i++) { //the loop for doing this
            showres(reservations[i]);
    }
    return;

}

void sortwithstatus() {//sorting with status
    if (count == 0){
        printf("il ny a pas de reservations.\n");
        return;
    }
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

    for (int i = 0; i < count; i++) { //the loop for doing this
            showres(reservations[i]);
    }
    return;

}

int searchwithid() {//searching with id and returning the index
    if (count == 0){
        printf("il ny a pas de reservations.\n");
        return 0;
    }

    char search[10];
    printf("Entrez lID de la reservation: ");
    fgt(search, sizeof(search));
    int inputid = atoi(search);

    for (int i = 0; i < count; i++) {//searching with linear search
        if (reservations[i].id == inputid) {
            showres(reservations[i]);//here we call the displaying function we established earlier
            return i; // return the index of the found reservation
        }
    }

    printf("Aucune reservation trouvee avec cette ID.\n");//print false
    return -1; // return -1 if not found
}

void searchwithname() {//searching with name
    if (count == 0){
        printf("il ny a pas de reservations.\n");
        return;
    }
    char name[50];
    printf("Entrez le nom de la reservation: ");
    fgt(name, sizeof(name));

    int found = 0;//variable to check if we foudn said reservation with name
    for (int i = 0; i < count; i++) {//using linear search
        if (strcasecmp(reservations[i].nom, name) == 0) {//using strcasecmp to compare between name and search input
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

    printf("Moyenne dage est: %.2f\n", total / count);
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
    printf("de 0 a 18 ans: %d\n", group[0]);
    printf("de 19 a 35 ans: %d\n", group[1]);
    printf("de 36+: %d\n", group[2]);
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
        
        choice = getchoice(1,7);//getting the choice from the user

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
    strcpy(reservations[0].nom, "Jaiti");reservations[0].age = 21;
    strcpy(reservations[0].prenom, "Taha");reservations[0].statut = 4;
    strcpy(reservations[0].tel, "0682226573");reservations[0].id = 1;
    strcpy(reservations[0].date, "2004-08-16");

    strcpy(reservations[1].nom, "Koti");reservations[1].age = 1;
    strcpy(reservations[1].prenom, "Ghali");reservations[1].statut = 4;
    strcpy(reservations[1].tel, "0683276982");reservations[1].id = 2;
    strcpy(reservations[1].date, "2023-12-03");

    strcpy(reservations[2].nom, "Serghini");reservations[2].age = 15;
    strcpy(reservations[2].prenom, "Jad");reservations[2].statut = 2;
    strcpy(reservations[2].tel, "0713654834");reservations[2].id = 3;
    strcpy(reservations[2].date, "2016-06-12");

    strcpy(reservations[3].nom, "EL Ouariachi");reservations[3].age = 20;
    strcpy(reservations[3].prenom, "Yasser");reservations[3].statut = 1;
    strcpy(reservations[3].tel, "0689342544");reservations[3].id = 4;
    strcpy(reservations[3].date, "2005-06-12");
  
    strcpy(reservations[4].nom, "Ziza");reservations[4].age = 40;
    strcpy(reservations[4].prenom, "Mohammed");reservations[4].statut = 3;
    strcpy(reservations[4].tel, "0623071034");reservations[4].id = 5;
    strcpy(reservations[4].date, "2023-08-04");

    strcpy(reservations[5].nom, "Kasir");reservations[5].age = 37;
    strcpy(reservations[5].prenom, "Imane");reservations[5].statut = 2;
    strcpy(reservations[5].tel, "0733688450");reservations[5].id = 6;
    strcpy(reservations[5].date, "2024-05-28");

    strcpy(reservations[6].nom, "Astir");reservations[6].age = 5;
    strcpy(reservations[6].prenom, "Nabil");reservations[6].statut = 1;
    strcpy(reservations[6].tel, "0698497658");reservations[6].id = 7;
    strcpy(reservations[6].date, "2024-08-30");
  
    strcpy(reservations[7].nom, "Benanaa");reservations[7].age = 18;
    strcpy(reservations[7].prenom, "Saad");reservations[7].statut = 3;
    strcpy(reservations[7].tel, "0612563544");reservations[7].id = 8;
    strcpy(reservations[7].date, "2024-09-23");
 
    strcpy(reservations[8].nom, "Nigor");reservations[8].age = 27;
    strcpy(reservations[8].prenom, "Ilyass");reservations[8].statut = 4;
    strcpy(reservations[8].tel, "0642066420");reservations[8].id = 9;
    strcpy(reservations[8].date, "2017-11-21");
 
    strcpy(reservations[9].nom, "Lefkih");reservations[9].age = 25;
    strcpy(reservations[9].prenom, "Saad");reservations[9].statut = 3;
    strcpy(reservations[9].tel, "0742042066");reservations[9].id = 10;
    strcpy(reservations[9].date, "2021-03-24");
 
}