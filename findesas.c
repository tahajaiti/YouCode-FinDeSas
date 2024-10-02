#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX 100

typedef struct {
    char nom[50]; 
    char prenom[50]; 
    char tel[15];
    int age;
    int statut; 
    int id; 
    char date[20];
} Reserv;

Reserv reservations[MAX];
int count = 0;
int id = 1;

void menu() {
    printf("\nMenu:\n");
    printf("1. Ajouter une reservation\n");
    printf("2. Modifier ou supprimer une reservation\n");
    printf("3. Afficher les details dune reservation\n");
    printf("4. Trier les réservations\n");
    printf("5. Rechercher des reservations\n");
    printf("6. Afficher les statistiques\n");
    printf("7. Quitter\n");
}

void editdelmenu() {
    printf("\nModifier ou Supprimer:\n");
    printf("1. Modifier une reservation\n");
    printf("2. Supprimer une reservation\n");
    printf("3. Retourner\n");
}

void searchmenu() {
    printf("\nRecherche:\n");
    printf("1. Par ID\n");
    printf("2. Par Nom\n");
    printf("3. Retourner\n");
}

void sortmenu() {
    printf("\nTrier par:\n");
    printf("1. Nom\n");
    printf("2. Statut\n");
    printf("3. Retourner\n");
}

void statmenu() {
    printf("\nMenu des Statistiques:\n");
    printf("1. Moyenne dage des patients\n");
    printf("2. Nombre de patients par tranche dage\n");
    printf("3. Nombre total de reservations par statut\n");
    printf("4. Quitter\n");
}

void fgt(char input[], size_t n) {
    fgets(input, n, stdin);
    input[strcspn(input, "\n")] = 0; //remove \n
}

int getchoice(int min, int max) {
    int choice;
    while (1) {
        printf("Entrez votre choix (%d-%d): ", min, max);
        if (scanf("%d", &choice) == 1 && choice >= min && choice <= max) {
            getchar(); // clear new line from the buffer
            return choice;
        } else {
            printf("Erreur, entrez un nombre valide.\n");
            while (getchar() != '\n'); // clearing the invalid input
        }
    }
}

int checkage(int age) {
    if (age > 0 && age < 90) {
        return 1;
    } else {
        printf("Erreur, Entrez un age valide\n");
        return 0;
    }
}

int checkname(char *nom) {
    for (int i = 0; nom[i] != '\0'; i++) {
        if (!isalpha(nom[i]) && !isspace(nom[i])) {
            printf("Erreur, Entrer un nom valide.\n");
            return 0;
        }
    }
    return 1;
}

int checknum(char *num, int idx) {
    for (int i = 0; i < count; i++) {
        if (i != idx && strcmp(reservations[i].tel, num) == 0) {
            printf("Erreur, ce numero existe deja\n");
            return 0;
        }  
    }
        
    if (strlen(num) != 10 || strspn(num, "0123456789") != 10) {
        printf("Erreur, Entrez un numero de telephone valide\n");
        return 0; // return false
    }

    if (strncmp(num, "06", 2) != 0 && strncmp(num, "07", 2) != 0) {
        printf("Erreur, le numero doit commencer par 06 ou 07\n");
        return 0; // return false
    }

    return 1; // return true
}

int checkstatut(int statut) {
    return (statut >= 1 && statut <= 4);
}

int checkdate(char date[]) {
    int anne, moi, jour;
    if (sscanf(date, "%4d-%2d-%2d", &anne, &moi, &jour) != 3) {
        printf("Erreur, Entrez une format valide(YYYY-MM-DD)\n");
        return 0;
    }

    if (moi < 1 || moi > 12 || jour < 1 || jour > 31) {
        printf("Erreur, Entrez jour ou mois valide\n");
        return 0;
    }

    if (moi == 2 && jour > 28) {
        printf("Erreur, Entrez jour valide\n");
        return 0;
    } else if ((moi == 4 || moi == 6 || moi == 9 || moi == 11) && jour > 30) {
        printf("Erreur, Entrez jour valide\n");
        return 0;
    }

    return 1;
}

void add() {
    if (count >= MAX) {
        printf("les réservations sont completes.\n");
        return;
    }

    Reserv new;

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

    char inputage[10];
    do {
        printf("Age: ");
        fgt(inputage, sizeof(inputage));
        new.age = atoi(inputage);
    } while (!checkage(new.age));

    char inputstatut[10];
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

    reservations[count++] = new;
    printf("Cet operation est succes!");
}

void showres(Reserv r) {
    printf("\nDetails de la reservation:\n");
    printf("ID: %d\n", r.id);
    printf("Nom: %s\n", r.nom);
    printf("Prenom: %s\n", r.prenom);
    printf("Telephone: %s\n", r.tel);
    printf("Age: %d\n", r.age);
    printf("Statut: %d\n", r.statut);
    printf("Date: %s\n", r.date);
}

void showresdetails() {
    char search[10];
    printf("Entrez lID de la reservation: ");
    fgt(search, sizeof(search));
    int inputsearch = atoi(search);
    
    for (int i = 0; i < count; i++) {
        if (reservations[i].id == inputsearch) {
            showres(reservations[i]);
            return;
        }
    }
    printf("Aucune reservation trouvee avec cette ID.\n");
}

void edit() {
    char search[10];
    printf("Entrez l'ID de la reservation: ");
    fgt(search, sizeof(search));
    int idSearch = atoi(search);

    for (int i = 0; i < count; i++) {
        if (reservations[i].id == idSearch) {
            printf("Modifier la reservation %d:\n", reservations[i].id);
            do {
                printf("Nom(Actuel: %s): ", reservations[i].nom);
                fgt(reservations[i].nom, sizeof(reservations[i].nom));
            } while (!checkname(reservations[i].nom));

            do {
                printf("Prenom(Actuel: %s): ", reservations[i].prenom);
                fgt(reservations[i].prenom, sizeof(reservations[i].prenom));
            } while (!checkname(reservations[i].prenom));

            do {
                printf("Telephone(Actuel: %s): ", reservations[i].tel);
                fgt(reservations[i].tel, sizeof(reservations[i].tel));
            } while (!checknum(reservations[i].tel, count));

            char inputage[10];
            do {
                printf("Age(Actuel: %d): ", reservations[i].age);
                fgt(inputage, sizeof(inputage));
                reservations[i].age = atoi(inputage);
            } while (!checkage(reservations[i].age));

            char inputstatut[10];
            do {
                printf("Statut(Actuel: %d): ", reservations[i].statut);
                fgt(inputstatut, sizeof(inputstatut));
                reservations[i].statut = atoi(inputstatut);
            } while (!checkstatut(reservations[i].statut));

            do {
                printf("Date de reservation(Actuelle: %s): ", reservations[i].date);
                fgt(reservations[i].date, sizeof(reservations[i].date));
            } while (!checkdate(reservations[i].date));

            printf("Reservation mise a jour avec succes!\n");
            return;
        }
    }
    printf("Aucune reservation trouvee avec cette ID.\n");
}

void delete() {
    char search[10];
    printf("Entrez l'ID de la reservation: ");
    fgt(search, sizeof(search));
    int idSearch = atoi(search);

    for (int i = 0; i < count; i++) {
        if (reservations[i].id == idSearch) {
            for (int j = i; j < count - 1; j++) {
                reservations[j] = reservations[j + 1];
            }
            count--;
            printf("Reservation supprimee avec succes!\n");
            return;
        }
    }
    printf("Aucune reservation trouvee avec cette ID.\n");
}

void sortwithanme() {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(reservations[j].nom, reservations[j + 1].nom) > 0) {
                Reserv temp = reservations[j];
                reservations[j] = reservations[j + 1];
                reservations[j + 1] = temp;
            }
        }
    }
    printf("Reservations triees par nom.\n");
}

void sortwithstatus() {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (reservations[j].statut > reservations[j + 1].statut) {
                Reserv temp = reservations[j];
                reservations[j] = reservations[j + 1];
                reservations[j + 1] = temp;
            }
        }
    }
    printf("Reservations triees par statut.\n");
}

void searchwithid() {
    char search[10];
    printf("Entrez l'ID de la reservation: ");
    fgt(search, sizeof(search));
    int idSearch = atoi(search);

    for (int i = 0; i < count; i++) {
        if (reservations[i].id == idSearch) {
            showres(reservations[i]);
            return;
        }
    }
    printf("Aucune reservation trouvee avec cette ID.\n");
}

void searchwithname() {
    char name[50];
    printf("Entrez le nom de la reservation: ");
    fgt(name, sizeof(name));

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(reservations[i].nom, name) == 0) {
            showres(reservations[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("Aucune reservation trouvee avec ce nom.\n");
    }
}

void calculmoyenne() {
    if (count == 0) {
        printf("Aucune reservation a afficher.\n");
        return;
    }

    int total = 0;
    for (int i = 0; i < count; i++) {
        total += reservations[i].age;
    }

    printf("Moyenne dage: %.2f\n", (double)total / count);
}

void countages() {
    if (count == 0) {
        printf("Aucune reservation a afficher.\n");
        return;
    }

    int group[3] = {0}; // 0-18, 19-35, 36+

    for (int i = 0; i < count; i++) {
        if (reservations[i].age <= 18) group[0]++;
        else if (reservations[i].age <= 35) group[1]++;
        else group[2]++;
    }

    printf("Nombre de patients par tranche d'age:\n");
    printf("0-18: %d\n", group[0]);
    printf("19-35: %d\n", group[1]);
    printf("36+: %d\n", group[2]);
}

void countbystatus() {
    if (count == 0) {
        printf("Aucune reservation a afficher.\n");
        return;
    }

    int status[4] = {0}; // 1: Valide, 2: Reporte, 3: Annule, 4: Traite

    for (int i = 0; i < count; i++) {
        if (reservations[i].statut >= 1 && reservations[i].statut <= 4) {
            status[reservations[i].statut - 1]++;
        }
    }

    printf("Nombre total de reservations par statut:\n");
    printf("Valide: %d\n", status[0]);
    printf("Reporte: %d\n", status[1]);
    printf("Annule: %d\n", status[2]);
    printf("Traite: %d\n", status[3]);
}

int main() {
    int choice;

    do {
        menu();
        
        choice = getchoice(1, 7);

        switch (choice) {
            case 1: add(); break;
            case 2: editdelmenu();
                int choice2 = getchoice(1, 3);
                if (choice2 == 1) { edit(); } 
                else if (choice2 == 2) { delete(); } break;
            case 3: showresdetails(); break;
            case 4: sortmenu();
                int choice3 = getchoice(1, 3);
                if (choice3 == 1) { sortwithanme(); } 
                else if (choice3 == 2) { sortwithstatus(); } break;
            case 5: searchmenu();
                int choice4 = getchoice(1, 3);
                if (choice4 == 1) { searchwithid(); } 
                else if (choice4 == 2) { searchwithname(); } break;
            case 6: statmenu();
                int choice5 = getchoice(1, 4);
                if (choice5 == 1) { calculmoyenne(); } 
                else if (choice5 == 2) { countages(); } 
                else if (choice5 == 3) { countbystatus(); } break;
            case 7: printf("Passez une bonne journee.\n"); break;
            default: break;
        }
    } while (choice != 7);

    return 0;
}
