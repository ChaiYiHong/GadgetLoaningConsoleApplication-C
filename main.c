// Works in visual studio compiler
#define _CRT_SECURE_NO_WARNINGS
#ifdef _MSC_VER 
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

// System header files
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// Header files of this program
#include "menu.h"
#include "function.h"

int main() {
    int option = 0;
    bool validInput = true;
    // Create current and history struct list
    CreateList();
    do {
        ClearConsoleScreen();
        MainMenu();
        if (!validInput) {
            printf("\t(Invalid input)");
            validInput = true;
        }
        option = UserInput(5);
        // If the user input is not valid
        if (option <= 0 || option > 5) {
            validInput = false;
        }
        switch (option) {
        case 1:
            // Borrow Gadget
            BorrowGadget();
            break;
        case 2:
            // Return Gadget
            ReturnGadget();
            break;
        case 3:
            // Search Loaning
            SearchLoaning();
            break;
        case 4:
            // Generate report
            GenerateReport();
            break;
        case 5:
            // Exit
            ClearConsoleScreen();
            ExitMenu();
            break;
        default:
            break;
        }
    } while (option != 5);
    return 0;
}