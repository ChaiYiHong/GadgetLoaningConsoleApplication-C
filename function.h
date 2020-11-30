// Declare struct named Loaning
struct Loaning {
    char loaningItem[20];
    char lecturerID[50];
    char lecturerName[50];
    char serialNumber[10];
    char loaningDate[20];
    char loaningTime[20];
    char returnDate[20];
    char returnTime[20];
    struct Loaning* next;
};

// ListHead is the head of current loaning list, HistoryHead is the head of loaning history list
struct Loaning* ListHead, * HistoryHead, * NewLoaning, * temp, * temp2, * prev;

// Declare gloabal variables and pointers
char lecturerID[50], lecturerName[50], digitalPadSN[50], wirelessMicrophoneSN[50];
char* currentTime, * currentDate;
bool wirelessMicrophone = false, digitalPad = false;
int wmLoanedAmount, dpLoanedAmount;

// Clear terminal or command prompt screen
void ClearConsoleScreen() {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#elif defined(__APPLE__) || defined(__linux__) || defined(__unix__)
    system("clear");
#endif
}

// Convert string to int and return 0 if it is a string
int ConvertStrToInt(char input[10]) {
    char* temp;
    // Convert str to long int and stores the rest of string in temp
    // It also ignores whitespaces at the start
    long convertedInput = strtol(input, &temp, 10);
    return (int)convertedInput;
}

// Used to accept user input and check whether the input is within the range that parameter passed in
int UserInput(int optionRange) {
    char userInput[3];
    int convertedInput = 0;
    if (optionRange > 1) {
        printf("\tPlease input option from 1 to %d \n\t> ", optionRange);
    }
    else {
        printf("\tPlease input %d to exit\n\t> ", optionRange);
    }
    // Receive keyboard input with specified char array size and store into userInput
    // It also accept string with space
    fgets(userInput, 3, stdin);
    printf("\n");
    rewind(stdin); // Flush output buffer

    // Convert str to int and stored in convertedInput
    convertedInput = ConvertStrToInt(userInput);

    return convertedInput;
}

// Get current date and time by using function in time.h
char* GetDateTime(char option[5]) {
    char* tempDate = malloc(20);
    char* tempTime = malloc(20);

    // struct tm from time.h holds the date and time
    struct tm* local;

    // time_t represents the clock time as integer
    // time(NULL) returns the number of seconds elapsed
    time_t t = time(NULL);

    // Get the local time and date
    local = localtime(&t);

    // Format time as string
    strftime(tempDate, 256, "%F", local); // Date format: YYYY-MM-DD
    strftime(tempTime, 256, "%R", local); // Time format: HH:MM (24H)

    char* tempReturn = NULL;

    // strcmp is used to compare two string
    if (strcmp(option, "date") == 0) {
        tempReturn = tempDate;
    }
    else if (strcmp(option, "time") == 0) {
        tempReturn = tempTime;
    }
    return tempReturn;
}

// Check for the date format
bool CheckDate(char date[50]) {
    bool valid = true, isLeapYear = false;
    int dateArray[5], count = 0;
    char tempDate[50], * RemainString;

    strcpy(tempDate, date);
    char* datePtr = strtok(tempDate, "-");
    while (datePtr != NULL) {
        long convertedInput = strtol(datePtr, &RemainString, 10);
        dateArray[count] = (int)convertedInput;
        datePtr = strtok(NULL, "-");
        if (count < 5) {
            count++;
        }
        if (count > 3 || strcmp(RemainString, "") != 0) {
            valid = false;
        }
    }
    if (valid) {
        // Check leap year
        if (dateArray[0] % 400 == 0 || (dateArray[0] % 4 == 0 && dateArray[0] % 100 != 0)) {
            isLeapYear = true;
        }
        // Check year
        if (dateArray[0] >= 1900 && dateArray[0] <= 9999) {
            // Check month
            if (dateArray[1] >= 1 && dateArray[1] <= 12) {
                // Check days
                // Check for days in Feb
                if (dateArray[2] >= 1) {
                    if (dateArray[1] == 2) {
                        if (isLeapYear && dateArray[2] == 29) {
                            valid = true;
                        }
                        else if (dateArray[2] > 28) {
                            valid = false;
                        }
                    }
                    else if (dateArray[1] == 4 || dateArray[1] == 6 || dateArray[1] == 9 || dateArray[1] == 11) {
                        // Check for days in April, June, September, and November
                        if (dateArray[2] > 30) {
                            valid = false;
                        }
                    }
                    else if (dateArray[2] > 31) {
                        // Check for days in rest of the months
                        valid = false;
                    }
                }
                else {
                    valid = false;
                }
            }
            else {
                valid = false;
            }
        }
        else {
            valid = false;
        }
    }
    return valid;
}

// Check for the time format
bool CheckTime(char time[50]) {
    bool valid = true;
    int timeArray[4], count = 0;
    char tempTime[50], * RemainString;

    strcpy(tempTime, time);
    char* timePtr = strtok(tempTime, ":");
    while (timePtr != NULL) {
        long convertedInput = strtol(timePtr, &RemainString, 10);
        timeArray[count] = (int)convertedInput;
        timePtr = strtok(NULL, "-");
        if (count < 4) {
            count++;
        }
        if (count > 2 || strcmp(RemainString, "") != 0) {
            valid = false;
        }
    }
    if (valid) {
        if (timeArray[0] >= 0 && timeArray[0] < 24) {
            if (timeArray[1] < 0 || timeArray[1] > 59) {
                valid = false;
            }
        }
        else {
            valid = false;
        }
    }
    return valid;
}

// Assign NULL pointer to current and history list when the program starts
void CreateList() {
    ListHead = NULL;
    HistoryHead = NULL;
}

// Count the total amount of digital pad and wireless microphone
void Count(char countList[50]) {
    char Item[50];
    wmLoanedAmount = 0;
    dpLoanedAmount = 0;

    if (strcmp(countList, "currentList") == 0) {
        temp = ListHead;
    }
    else if (strcmp(countList, "historyList") == 0) {
        temp = HistoryHead;
    }
    while (temp != NULL) {
        strcpy(Item, temp->loaningItem);
        if (strcasecmp(Item, "Wireless Microphone") == 0) {
            wmLoanedAmount++;
        }
        else if (strcasecmp(Item, "Digital Pad") == 0) {
            dpLoanedAmount++;
        }
        temp = temp->next;
    }
}

// Check if the lecturer ID or serial number is already recorded in the system
bool Check(char itemType[50], char searchItem[50], char searchList[50]) {
    bool Found = false;

    // Switch the checking list between current and history list
    if (strcmp(searchList, "currentList") == 0) {
        temp = ListHead;
    }
    else if (strcmp(searchList, "historyList") == 0) {
        temp = HistoryHead;
    }

    if (temp != NULL) {
        // Loop until last record is reached
        while (temp != NULL) {
            if (strcmp(itemType, "lecturerID") == 0) {
                if (strcasecmp(temp->lecturerID, searchItem) == 0) {
                    if (strcmp(temp->loaningItem, "Digital Pad") == 0) {
                        digitalPad = true;
                    }
                    else if (strcmp(temp->loaningItem, "Wireless Microphone") == 0) {
                        wirelessMicrophone = true;
                    }
                    Found = true;
                }
            }
            else if (strcmp(itemType, "serialNumber") == 0) {
                if (strcasecmp(temp->serialNumber, searchItem) == 0) {
                    Found = true;
                    break;
                }
            }
            // Swap to the next record in the list
            temp = temp->next;
        }
    }
    return Found;
}

// Display Loaning for current list and history list
void DisplayLoaning(char itemType[50], char displayItem[50], char displayList[50], char displayStyle[50]) {
    wirelessMicrophone = false, digitalPad = false;
    int index = 1, length;
    bool display = false, displayNameID = false;
    // indent contains 20 spaces
    char indent[50] = "                    ";

    if (strcmp(displayList, "currentList") == 0) {
        temp = ListHead;
        printf("\n\t                          - Current loaning -                        \n");
        if (strcmp(displayStyle, "horizontal") == 0) {
            printf("\t==================================================================================================\n");
            printf("\t| No. | Lecturer ID | Lecturer Name | Loaning Item | Serial Number | Loaning Date | Loaning Time |\n");
            printf("\t==================================================================================================\n");
        }
    }
    else if (strcmp(displayList, "historyList") == 0) {
        temp = HistoryHead;
        printf("\n\t                          - History loaning -                        \n");
        if (strcmp(displayStyle, "horizontal") == 0) {
            printf("\t==============================================================================================================================\n");
            printf("\t| No. | Lecturer ID | Lecturer Name | Loaning Item | Serial Number | Loaning Date | Loaning Time | Return Date | Return Time |\n");
            printf("\t==============================================================================================================================\n");
        }
    }
    while (temp != NULL) {
        display = false;
        displayNameID = true;

        if (strcmp(itemType, "lecturerID") == 0) {
            if (strcasecmp(temp->lecturerID, displayItem) == 0) {
                if (wirelessMicrophone || digitalPad) {
                    displayNameID = false;
                }
                display = true;
            }
        }
        else if (strcmp(itemType, "serialNumber") == 0) {
            if (strcasecmp(temp->serialNumber, displayItem) == 0) {
                display = true;
            }
        }
        else if (strcmp(itemType, "all") == 0) {
            display = true;
        }

        // Vertical display
        if (display && (strcmp(displayStyle, "vertical") == 0)) {
            if (displayNameID) {
                printf("\t      Lecture ID: %s\n", temp->lecturerID);
                printf("\t      Lecture name: %s\n\n", temp->lecturerName);
            }
            printf("\t      Loaning Item: %s (S/N: %s)\n", temp->loaningItem, temp->serialNumber);
            printf("\t      Loaning Date: %s\n", temp->loaningDate);
            printf("\t      Loaning Time: %s\n\n", temp->loaningTime);
            if (strcmp(displayList, "historyList") == 0) {
                printf("\t      Return Date: %s\n", temp->returnDate);
                printf("\t      Return Time: %s\n\n", temp->returnTime);
            }
        }

        // Horizontal display
        if (display && (strcmp(displayStyle, "horizontal") == 0)) {
            if (index >= 10) {
                length = 2;
            }
            else {
                length = 3;
            }
            printf("\t| %d", index);
            printf("%s| %s", &indent[20 - length], temp->lecturerID);
            length = 12 - (int)strlen(temp->lecturerID);
            printf("%s| %s", &indent[20 - length], temp->lecturerName);
            length = 14 - (int)strlen(temp->lecturerName);
            printf("%s| ", &indent[20 - length]);
            if (strcmp(temp->loaningItem, "Digital Pad") == 0) {
                printf("DP");
            }
            else if (strcmp(temp->loaningItem, "Wireless Microphone") == 0) {
                printf("WM");
            }
            length = 11;
            printf("%s| %s", &indent[20 - length], temp->serialNumber);
            length = 14 - (int)strlen(temp->serialNumber);
            printf("%s| %s", &indent[20 - length], temp->loaningDate);
            length = 13 - (int)strlen(temp->loaningDate);
            printf("%s| %s", &indent[20 - length], temp->loaningTime);
            length = 13 - (int)strlen(temp->loaningTime);
            if (strcmp(displayList, "historyList") == 0) {
                printf("%s| %s", &indent[20 - length], temp->returnDate);
                length = 12 - (int)strlen(temp->returnDate);
                printf("%s| %s", &indent[20 - length], temp->returnTime);
                length = 12 - (int)strlen(temp->returnTime);
                printf("%s|\n", &indent[20 - length]);
                printf("\t------------------------------------------------------------------------------------------------------------------------------\n");
            }
            else {
                printf("%s|\n", &indent[20 - length]);
                printf("\t--------------------------------------------------------------------------------------------------\n");
            }
            index++;
        }

        if (display && !(strcmp(displayList, "all") == 0)) {
            strcpy(lecturerName, temp->lecturerName);
            strcpy(lecturerID, temp->lecturerID);
            if (strcmp(temp->loaningItem, "Wireless Microphone") == 0) {
                strcpy(wirelessMicrophoneSN, temp->serialNumber);
                wirelessMicrophone = true;
            }
            else if (strcmp(temp->loaningItem, "Digital Pad") == 0) {
                strcpy(digitalPadSN, temp->serialNumber);
                digitalPad = true;
            }
        }
        temp = temp->next;
    }
}

// Set the date and time automatically or accept input from the user
void SetDateTime(char type[50], int option) {
    bool valid = true, validInput = true;
    int subOption = 0;
    do {
        currentDate = GetDateTime("date");

        ClearConsoleScreen();
        if (strcmp(type, "Loaning") == 0) {
            BorrowMenu();
        }
        else if (strcmp(type, "Return") == 0) {
            ReturnMenu();
        }
        printf("\n\t      Lecturer ID: %s\n", lecturerID);
        printf("\t      Lecturer Name: %s\n", lecturerName);

        if (option == 1 || option == 3) {
            printf("\t      %s Item: Digital pad (S/N: %s)\n", type, digitalPadSN);
        }
        if (option == 2 || option == 3) {
            printf("\t      %s Item: Wireless microphone (S/N: %s)\n", type, wirelessMicrophoneSN);
        }
        printf("\n\t      The current date is %s\n", currentDate);
        printf("\t      1. Use the current date\n");
        printf("\t      2. Manually enter the date as YYYY-MM-DD format\n\n");
        printf("\t---------------------------------------------------------------------\n");
        if (!validInput) {
            printf("\t(Invalid Input)");
        }
        subOption = UserInput(2);
        if (subOption <= 0 || subOption > 2) {
            validInput = false;
        }
        if (subOption == 2) {
            do {
                ClearConsoleScreen();
                if (strcmp(type, "Loaning") == 0) {
                    BorrowMenu();
                }
                else if (strcmp(type, "Return") == 0) {
                    ReturnMenu();
                }
                printf("\n\t      Lecturer ID: %s\n", lecturerID);
                printf("\t      Lecturer Name: %s\n", lecturerName);
                if (option == 1 || option == 3) {
                    printf("\t      %s Item: Digital pad (S/N: %s)\n", type, digitalPadSN);
                }
                if (option == 2 || option == 3) {
                    printf("\t      %s Item: Wireless microphone (S/N: %s)\n", type, wirelessMicrophoneSN);
                }
                printf("\n\t---------------------------------------------------------------------\n");
                if (valid) {
                    printf("\n\tPlease enter the date as YYYY-MM-DD format\n\t> ");
                }
                else {
                    printf("\t(Invalid format) Please enter the date as YYYY-MM-DD format\n\t> ");
                }
                scanf("%s", currentDate);
                valid = CheckDate(currentDate);
                rewind(stdin);
            } while (!valid);
        }
    } while (subOption <= 0 || subOption > 2);

    validInput = true;
    do {
        currentTime = GetDateTime("time");
        ClearConsoleScreen();
        if (strcmp(type, "Loaning") == 0) {
            BorrowMenu();
        }
        else if (strcmp(type, "Return") == 0) {
            ReturnMenu();
        }
        printf("\n\t      Lecturer ID: %s\n", lecturerID);
        printf("\t      Lecturer Name: %s\n", lecturerName);
        if (option == 1 || option == 3) {
            printf("\t      %s Item: Digital pad (S/N: %s)\n", type, digitalPadSN);
        }
        if (option == 2 || option == 3) {
            printf("\t      %s Item: Wireless microphone (S/N: %s)\n", type, wirelessMicrophoneSN);
        }
        printf("\t      %s Date: %s\n\n", type, currentDate);
        printf("\t      The current time is %s\n", currentTime);
        printf("\t      1. Use the current time\n");
        printf("\t      2. Manually enter the time\n\n");
        printf("\t---------------------------------------------------------------------\n");
        if (!validInput) {
            printf("\t(Invalid Input)");
        }
        subOption = UserInput(2);
        if (subOption <= 0 || subOption > 2) {
            validInput = false;
        }
        if (subOption == 2) {
            do {
                ClearConsoleScreen();
                if (strcmp(type, "Loaning") == 0) {
                    BorrowMenu();
                }
                else if (strcmp(type, "Return") == 0) {
                    ReturnMenu();
                }
                printf("\n\t      Lecturer ID: %s\n", lecturerID);
                printf("\t      Lecturer Name: %s\n", lecturerName);
                if (option == 1 || option == 3) {
                    printf("\t      %s Item: Digital pad (S/N: %s)\n", type, digitalPadSN);
                }
                if (option == 2 || option == 3) {
                    printf("\t      %s Item: Wireless microphone (S/N: %s)\n", type, wirelessMicrophoneSN);
                }
                printf("\t      %s Date: %s\n\n", type, currentDate);
                printf("\n\t---------------------------------------------------------------------\n");
                if (valid) {
                    printf("\tPlease enter the time as HH:MM (24H) format\n\t> ");
                }
                else {
                    printf("\t(Invalid format) Please enter the time as HH:MM (24H) format\n\t> ");
                }
                scanf("%s", currentTime);
                valid = CheckTime(currentTime);
                rewind(stdin);
            } while (!valid);
        }
    } while (subOption <= 0 || subOption > 2);
}

// Accept input from the user and insert the node into the current lonaing list
void BorrowGadget() {
    int option = 0, subOption = 0, subOption2 = 0;
    bool secondInsert = false, validInput = true, status = true;
    wirelessMicrophone = false, digitalPad = false;

    // Accept input for Lecturer ID
    ClearConsoleScreen();
    BorrowMenu();
    printf("\n\t---------------------------------------------------------------------\n");
    printf("\tPlease enter the lecturer ID\n\t> ");
    scanf("%s", lecturerID);
    rewind(stdin);
    ClearConsoleScreen();

    // Check for the lecturer ID and list the current loaning of the lecturer ID
    Check("lecturerID", lecturerID, "currentList");

    // wirelessMircrophone and digitalPad condition are checked in Check() fucntion
    if (!wirelessMicrophone || !digitalPad) {
        do {
            // Clear console screen and display borrow menu if the user enters invalid input
            if (status) {
                ClearConsoleScreen();
                BorrowMenu();
            }
            else {
                status = true;
            }
            if (Check("lecturerID", lecturerID, "currentList")) {
                DisplayLoaning("lecturerID", lecturerID, "currentList", "vertical");
            }
            else {
                printf("\n\t ( There is no current loaning for Lecturer ID: %s in the system )\n\n", lecturerID);
            }
            printf("\n\t      1. Digital Pad                            \n");
            printf("\t      2. Wireless Microphone                     \n");
            printf("\t      3. Digital Pad and Wireless Microphone    \n");
            printf("\t      4. Exit                                    \n\n");
            printf("\t---------------------------------------------------------------------\n");
            if (!validInput) {
                printf("\t(Invalid input)");
            }
            option = UserInput(4);
            rewind(stdin);

            // Get the total amount of current digital pad and wireless microphone loaning
            Count("currentList");

            // If the user choose for digital pad or both gadgets
            if (option == 1 || option == 3) {
                if (dpLoanedAmount < 20) {
                    ClearConsoleScreen();
                    BorrowMenu();
                    printf("\n\t      Lecturer ID: %s\n\n", lecturerID);
                    printf("\t---------------------------------------------------------------------\n");
                    // If the user does not have current digital pad loaning
                    if (!digitalPad) {
                        printf("\tPlease enter the digital pad's serial number\n\t> ");
                        scanf("%s", digitalPadSN);
                        // If the serial number entered are matching any record in the current list
                        if (Check("serialNumber", digitalPadSN, "currentList")) {
                            ClearConsoleScreen();
                            BorrowMenu();
                            printf("\n\t     Gadget with S/N: %s is already loaned out from the system\n", digitalPadSN);
                            // loop again to enter serial number again
                            option = 0;
                            status = false;
                        }
                    }
                    // If the user have current digital pad loaning record
                    else {
                        ClearConsoleScreen();
                        BorrowMenu();
                        printf("\n\t     Failed: lecterer ID (%s) had loaned out one digital pad\n", lecturerID);
                        status = false;
                        option = 0;
                    }
                }
                else {
                    ClearConsoleScreen();
                    BorrowMenu();
                    printf("\n\t     Failed: There are 20 digital pads are currently loaned\n", lecturerID);
                    status = false;
                    option = 0;
                }
            }
            // If the user choose for wireless microphone or both gadgets
            if (option == 2 || option == 3) {
                if (wmLoanedAmount < 20) {
                    ClearConsoleScreen();
                    BorrowMenu();
                    printf("\n\t      Lecturer ID: %s\n", lecturerID);
                    if (option == 3) {
                        printf("\t      Loaning Item: Digital pad (S/N: %s)\n", digitalPadSN);
                    }
                    printf("\n\t---------------------------------------------------------------------\n");
                    // If the user does not have current wireless microphone loaning
                    if (!wirelessMicrophone) {
                        printf("\tPlease enter the wireless microphone's serial number\n\t> ");
                        scanf("%s", wirelessMicrophoneSN);
                        // If the serial number entered are matching any record in the current list
                        if (Check("serialNumber", wirelessMicrophoneSN, "currentList")) {
                            ClearConsoleScreen();
                            BorrowMenu();
                            printf("\n\t     Gadget with S/N: %s is already loaned out from the system\n", wirelessMicrophoneSN);
                            status = false;
                            // loop again to enter serial number again
                            option = 0;
                        }
                        else if (strcasecmp(wirelessMicrophoneSN, digitalPadSN) == 0) {
                            ClearConsoleScreen();
                            BorrowMenu();
                            printf("\n\t     Gadget with S/N: %s is duplicated with the Digital Pad SN\n", wirelessMicrophoneSN);
                            status = false;
                            option = 0;
                        }
                    }
                    // If the user have current digital pad loaning record
                    else {
                        ClearConsoleScreen();
                        BorrowMenu();
                        printf("\n\t  Failed: lecterer ID (%s) had loaned out one wireless microphone\n", lecturerID);
                        status = false;
                        option = 0;
                    }
                }
                else {
                    ClearConsoleScreen();
                    BorrowMenu();
                    printf("\n\t     Failed: There are 20 wireless microphones are currently loaned\n", lecturerID);
                    status = false;
                    option = 0;
                }
            }

            if (option <= 0 || option > 4) {
                if (!status) {
                    validInput = true;
                }
                else {
                    validInput = false;
                }
            }
            else {
                status = true;
                validInput = true;
            }
            rewind(stdin);
            // Loop if the user enters invalid input
        } while (option <= 0 || option > 4);

        // If the user chooses to loan one of the gadget
        if (option > 0 && option < 4) {
            ClearConsoleScreen();
            BorrowMenu();
            printf("\n\t      Lecturer ID: %s\n", lecturerID);
            if (option == 1 || option == 3) {
                printf("\t      Loaning Item: Digital pad (S/N: %s)\n", digitalPadSN);
            }
            if (option == 2 || option == 3) {
                printf("\t      Loaning Item: Wireless microphone (S/N: %s)\n", wirelessMicrophoneSN);
            }
            // Accept input for lecturer name
            printf("\n\t---------------------------------------------------------------------\n");
            printf("\tPlease enter the lecturer name\n\t> ");
            scanf("%[^\n]%*c", lecturerName);
            rewind(stdin);

            // Set the loaning date and time
            SetDateTime("Loaning", option);

            // Confirm borrow detail
            validInput = true;
            do {
                ClearConsoleScreen();
                BorrowMenu();
                printf("\n\t      Lecturer ID: %s\n", lecturerID);
                printf("\t      Lecturer Name: %s\n", lecturerName);
                if (option == 1 || option == 3) {
                    printf("\t      Loaning Item: Digital pad (S/N: %s)\n", digitalPadSN);
                }
                if (option == 2 || option == 3) {
                    printf("\t      Loaning Item: Wireless microphone (S/N: %s)\n", wirelessMicrophoneSN);
                }
                printf("\t      Loaning Date: %s\n", currentDate);
                printf("\t      Loaning Time: %s\n", currentTime);
                printf("\n\t      Please confirm the loaning details\n");
                printf("\t      1. Yes                              \n");
                printf("\t      2. No                               \n");
                printf("\n\t---------------------------------------------------------------------\n");
                if (!validInput) {
                    printf("\t(Invaid Input)");
                }
                subOption = UserInput(2);
                if (subOption < 1 || subOption > 2) {
                    validInput = false;
                }
            } while (subOption < 1 || subOption > 2);

            // Insert the new node into the current loaning list
            if (subOption == 1) {
                do {
                    // Allocate a new memory block and return a pointer to NewLoaning
                    NewLoaning = (struct Loaning*)malloc(sizeof(struct Loaning));
                    if (option == 1 || option == 3) {
                        if (!secondInsert) {
                            strcpy(NewLoaning->loaningItem, "Digital Pad");
                            strcpy(NewLoaning->serialNumber, digitalPadSN);
                            if (option == 3) {
                                secondInsert = true;
                            }
                        }
                        else {
                            strcpy(NewLoaning->loaningItem, "Wireless Microphone");
                            strcpy(NewLoaning->serialNumber, wirelessMicrophoneSN);
                            secondInsert = false;
                        }
                    }
                    else if (option == 2) {
                        strcpy(NewLoaning->loaningItem, "Wireless Microphone");
                        strcpy(NewLoaning->serialNumber, wirelessMicrophoneSN);
                    }
                    strcpy(NewLoaning->lecturerID, lecturerID);
                    strcpy(NewLoaning->lecturerName, lecturerName);
                    strcpy(NewLoaning->loaningDate, currentDate);
                    strcpy(NewLoaning->loaningTime, currentTime);

                    NewLoaning->next = NULL;

                    if (ListHead == NULL) {
                        ListHead = NewLoaning;
                    }
                    else {
                        temp = ListHead;
                        while (temp->next != NULL) {
                            temp = temp->next;
                        }
                        temp->next = NewLoaning;
                    }
                } while (secondInsert);

                validInput = true;
                do {
                    ClearConsoleScreen();
                    BorrowMenu();
                    printf("\n\t      The loaning has been recorded into the system\n\n");
                    printf("\t      1. Exit                              \n\n");
                    printf("\t---------------------------------------------------------------------\n");
                    if (!validInput) {
                        printf("\t(Invalid Input)");
                    }
                    subOption2 = UserInput(1);
                    if (subOption2 != 1) {
                        validInput = false;
                    }
                } while (subOption2 != 1);
            }
        }
    }
    // If the user already loaned out both gadgets
    else {
        while (option != 1) {
            BorrowMenu();
            DisplayLoaning("lecturerID", lecturerID, "currentList", "vertical");
            printf("\n\t    1. Exit\n");
            printf("\t---------------------------------------------------------------------\n");
            option = UserInput(1);
        }
    }
}

// Remove the selected serial number from the loaning list and insert into history list
void Deletion(char serialNumber[50]) {
    if (ListHead != NULL) {
        if (strcmp(ListHead->serialNumber, serialNumber) == 0) {
            temp = ListHead;
            ListHead = ListHead->next;
        }
        else {
            temp = ListHead;
            while (strcasecmp(temp->serialNumber, serialNumber) != 0) {
                prev = temp;
                temp = temp->next;
            }
            prev->next = prev->next->next;
        }
        temp->next = NULL;
        strcpy(temp->returnTime, currentTime);
        strcpy(temp->returnDate, currentDate);

        // Insertion
        if (HistoryHead == NULL) {
            HistoryHead = temp;
        }
        else {
            prev = HistoryHead;
            while (prev->next != NULL) {
                prev = prev->next;
            }
            prev->next = temp;
        }
    }
}

// Accept input and search for the lecturer ID or serial number that want to be returned
void ReturnGadget() {
    int option = 0, subOption = 0;
    char returnItem[50], returnItemType[50], displayItemType[50];
    bool validInput = true, status = true, isReturned = false;
    wirelessMicrophone = false, digitalPad = false;
    prev = NULL;

    // Accept input of return by lecturer ID or gadget serial number
    do {
        ClearConsoleScreen();
        ReturnMenu();
        printf("\n\t      1. Return by LecturerID                    \n");
        printf("\t      2. Return by Gadget S/N                    \n");
        printf("\t      3. Exit                                    \n\n");
        printf("\t---------------------------------------------------------------------\n");
        if (!validInput) {
            printf("\t(Invalid Input)");
        }
        option = UserInput(3);
        if (option < 1 || option > 3) {
            validInput = false;
        }
    } while (option < 1 || option > 3);

    validInput = true;
    if (option == 1 || option == 2) {
        if (option == 1) {
            strcpy(returnItemType, "lecturerID");
            strcpy(displayItemType, "lecturer ID");
        }
        else if (option == 2) {
            strcpy(returnItemType, "serialNumber");
            strcpy(displayItemType, "serial number");
        }

        ClearConsoleScreen();
        ReturnMenu();
        printf("\n\t---------------------------------------------------------------------\n");
        printf("\tPlease enter the %s\n\t>", displayItemType);
        scanf("%s", returnItem);
        rewind(stdin);

        validInput = true;
        if (Check(returnItemType, returnItem, "currentList")) {
            switch (option) {
            case 1:
                do {
                    if (status) {
                        ClearConsoleScreen();
                        ReturnMenu();
                    }
                    else {
                        status = true;
                    }
                    DisplayLoaning(returnItemType, returnItem, "currentList", "vertical");
                    printf("\n\t      1. Return digital pad                                  \n");
                    printf("\t      2. Return wireless microphone                          \n");
                    printf("\t      3. Return wireless microphone and digital pad          \n");
                    printf("\t      4. Exit                                                \n");
                    printf("\n\t---------------------------------------------------------------------\n");
                    if (!validInput) {
                        printf("\t(Invalid Input)");
                        validInput = true;
                    }
                    subOption = UserInput(4);
                    switch (subOption) {
                    case 1:
                        if (digitalPad) {
                            SetDateTime("Return", 1);
                            Deletion(digitalPadSN);
                            isReturned = true;
                        }
                        else {
                            ClearConsoleScreen();
                            ReturnMenu();
                            printf("\n\t Failed: lecterer ID (%s) does not have current digital pad loaning\n", returnItem);
                            status = false;
                            subOption = 0;
                        }
                        break;
                    case 2:
                        if (wirelessMicrophone) {
                            SetDateTime("Return", 2);
                            Deletion(wirelessMicrophoneSN);
                            isReturned = true;
                        }
                        else {
                            ClearConsoleScreen();
                            ReturnMenu();
                            printf("\n\tFailed: lecterer ID (%s) does not have current wireless microphone loaning\n", returnItem);
                            status = false;
                            subOption = 0;
                        }
                        break;
                    case 3:
                        if (digitalPad && wirelessMicrophone) {
                            SetDateTime("Return", 3);
                            Deletion(digitalPadSN);
                            Deletion(wirelessMicrophoneSN);
                            isReturned = true;
                        }
                        else {
                            if (!digitalPad) {
                                ClearConsoleScreen();
                                ReturnMenu();
                                printf("\n\t Failed: lecterer ID (%s) does not have current digital pad loaning\n", returnItem);
                            }
                            else if (!wirelessMicrophone) {
                                ClearConsoleScreen();
                                ReturnMenu();
                                printf("\n\tFailed: lecterer ID (%s) does not have current wireless microphone loaning\n", returnItem);
                            }
                            status = false;
                            subOption = 0;
                        }
                        break;
                    }
                    if (subOption <= 0 || subOption > 4) {
                        if (!status) {
                            validInput = true;
                        }
                        else {
                            validInput = false;
                        }
                    }
                    else {
                        validInput = true;
                    }
                } while (subOption <= 0 || subOption > 4);
                break;
            case 2:
                do {
                    ClearConsoleScreen();
                    ReturnMenu();
                    DisplayLoaning(returnItemType, returnItem, "currentList", "vertical");
                    printf("\t      1. Return Gadget                                       \n");
                    printf("\t      2. Exit                                                \n");
                    printf("\t---------------------------------------------------------------------\n");
                    if (!validInput) {
                        printf("\t(Invalid Input)");
                        validInput = true;
                    }
                    subOption = UserInput(2);
                    if (subOption == 1) {
                        SetDateTime("Return", 0);
                        Deletion(returnItem);
                        isReturned = true;
                    }
                    if (subOption <= 0 || subOption > 2) {
                        validInput = false;
                    }
                } while (subOption <= 0 || subOption > 2);
                break;
            }
            validInput = true;
            if (isReturned) {
                do {
                    ClearConsoleScreen();
                    ReturnMenu();
                    printf("\n\tThe loaning record below has been removed from the current loaning list\n");
                    printf("\n\t      Lecturer ID: %s\n", lecturerID);
                    printf("\t      Lecturer Name: %s\n", lecturerName);
                    if (option == 1 || option == 3) {
                        printf("\t      Return Item: Digital pad (S/N: %s)\n", digitalPadSN);
                    }
                    if (option == 2 || option == 3) {
                        printf("\t      Return Item: Wireless microphone (S/N: %s)\n", wirelessMicrophoneSN);
                    }
                    printf("\t      Return date: %s\n", currentDate);
                    printf("\t      Return time: %s\n\n", currentTime);
                    printf("\t---------------------------------------------------------------------\n");
                    if (!validInput) {
                        printf("\t(Invalid Input)");
                    }
                    subOption = UserInput(1);
                    if (subOption != 1) {
                        validInput = false;
                    }
                } while (subOption != 1);

            }
        }
        else {
            do {
                ClearConsoleScreen();
                ReturnMenu();
                printf("\n\t         ( There is no current loaning on %s: %s)\n\n", displayItemType, returnItem);
                printf("\t      1. Exit\n");
                printf("\t---------------------------------------------------------------------\n");
                if (!validInput) {
                    printf("\t(Invalid Input)");
                }
                option = UserInput(1);
                if (option != 1) {
                    validInput = false;
                }
            } while (option != 1);
        }
    }
}

// Bubble sort and insertion sort to sort the node address by lecturer ID or loan date
void SwapList(char itemType[50], char swapList[50]) {
    bool status, swap = false;
    struct Loaning* tempHead = NULL, * lastPtr = NULL, * ptr = NULL, * prevPtr = NULL, * maxPtr = NULL, * prevMaxPtr = NULL;

    if (strcmp(itemType, "lecturerID") == 0) {
        // Selection sort
        do {
            status = false;
            if (strcmp(swapList, "currentList") == 0) {
                ptr = ListHead;
                prevPtr = ListHead;
                tempHead = ListHead;
            }
            else if (strcmp(swapList, "historyList") == 0) {
                ptr = HistoryHead;
                prevPtr = HistoryHead;
                tempHead = HistoryHead;
            }
            if (ptr->next != NULL) {
                maxPtr = ptr;
                prevMaxPtr = prevPtr;
                while (ptr->next != lastPtr) {
                    if (strcmp(maxPtr->lecturerID, ptr->next->lecturerID) < 0) {
                        prevMaxPtr = ptr;
                        maxPtr = ptr->next;
                    }
                    status = true;
                    prevPtr = ptr;
                    ptr = ptr->next;
                }

                if (strcasecmp(maxPtr->lecturerID, ptr->lecturerID) != 0) {
                    // If last node pointer is the max node pointer, no swaping needed
                    // E.g. [3, 2, 4, 5(maxPtr, ptr), 8(lastPtr)]
                    if (maxPtr == ptr) {
                        // last pointer points to the previous node
                        // E.g. [3, 2, 4, 5, 8(lastPtr)] --> [3, 2, 4, 5(lastPtr), 8)]
                        lastPtr = prevPtr->next;
                    }
                    // If the max is on the previous pointer index
                    // E.g. [3, 2(prevMaxPtr), 5(maxPtr, prevPtr), 4(ptr), 8(lastPtr)]
                    else if (maxPtr == prevPtr) {
                        temp = ptr->next;
                        // If the max is on the head node
                        // E.g. [3(maxPtr, prevPtr), 2(ptr), 5(lastPtr)]
                        if (tempHead == maxPtr) {
                            // Assign address of ptr to the head pointer
                            // E.g. [3(ptr), 2, 5] --> [3, 2(ptr), 5]
                            if (strcmp(swapList, "currentList") == 0) {
                                ListHead = ptr;
                            }
                            else if (strcmp(swapList, "historyList") == 0) {
                                HistoryHead = ptr;
                            }
                        }
                        else {
                            // The next node of previous max pointer points to ptr;
                            prevMaxPtr->next = ptr;
                        }
                        // E.g. 1 [3(maxPtr, prevPtr), 2(ptr), 5(lastPtr)] --> [2, 3, 5]
                        // E.g. 2 [3, 2(prevMaxPtr), 5(maxPtr, prevPtr), 4(ptr), 8(lastPtr)] --> [3, 2, 4, 5, 8]
                        ptr->next = maxPtr;
                        maxPtr->next = temp;
                        lastPtr = prevPtr;
                    }
                    // If the max value is on the header pointer
                    // E.g. [5(prevPtr, prevMaxPtr, maxPtr), 4, 2(ptr), 9(lastPtr)]
                    else if (prevMaxPtr == maxPtr) {
                        // E.g. temp = 1, temp2 = 4
                        temp = ptr->next;
                        temp2 = maxPtr->next;
                        ptr->next = temp2;
                        maxPtr->next = temp;
                        prevPtr->next = maxPtr;

                        // E.g. [2(ptr), 4, 5(ListHead), 9(lastPtr)] --> [2(ptr, ListHead), 4, 5, 9(lastPtr)]
                        if (strcmp(swapList, "currentList") == 0) {
                            ListHead = ptr;
                        }
                        else if (strcmp(swapList, "historyList") == 0) {
                            HistoryHead = ptr;
                        }
                        // E.g. [2, 4(prevPtr), 5, 9(lastPtr)] --> [2, 4(prevPtr), 5(lastPtr), 9]
                        lastPtr = prevPtr->next;
                    }
                    else {
                        // E.g. [2(prevMaxPtr), 5(maxPtr), 4, 3(prevPtr), 6(ptr), 9(lastPtr)] --> [2, 3, 4, 5, 6, 9]
                        temp = ptr->next;
                        temp2 = maxPtr->next;
                        prevPtr->next = maxPtr;
                        prevMaxPtr->next = ptr;
                        maxPtr->next = temp;
                        ptr->next = temp2;
                        lastPtr = prevPtr->next;
                    }
                }
                else {
                    lastPtr = prevPtr->next;
                }
            }
        } while (status);
    }
    if (strcmp(itemType, "loanDate") == 0 || strcmp(itemType, "returnDate") == 0) {
        // Bubble sort
        do {
            status = false;
            if (strcmp(swapList, "currentList") == 0) {
                ptr = ListHead;
                prevPtr = ListHead;
            }
            else if (strcmp(swapList, "historyList") == 0) {
                ptr = HistoryHead;
                prevPtr = HistoryHead;
            }
            while (ptr->next != lastPtr) {
                swap = false;

                // Checking for loaning date
                if (strcmp(itemType, "loanDate") == 0) {
                    if (strcmp(ptr->loaningDate, ptr->next->loaningDate) > 0) {
                        swap = true;
                    }
                    else if (strcmp(ptr->loaningDate, ptr->next->loaningDate) == 0) {
                        if (strcmp(ptr->loaningTime, ptr->next->loaningTime) > 0) {
                            swap = true;
                        }
                    }
                }

                // Checking for return date
                if (strcmp(itemType, "returnDate") == 0) {
                    if (strcmp(ptr->returnDate, ptr->next->returnDate) > 0) {
                        swap = true;
                    }
                    else if (strcmp(ptr->returnDate, ptr->next->returnDate) == 0) {
                        if (strcmp(ptr->returnTime, ptr->next->returnTime) > 0) {
                            swap = true;
                        }
                    }
                }

                if (swap) {
                    // Swaps between list head and the second node if list head contains max value
                    // E.g. [4(Max), 3, 1, 2] --> [3, 4(Max), 1, 2]
                    if (ptr == prevPtr) {
                        // Assign the next pointer address to head pointer
                        if (strcmp(swapList, "currentList") == 0) {
                            ListHead = ListHead->next;
                        }
                        else if (strcmp(swapList, "historyList") == 0) {
                            HistoryHead = HistoryHead->next;
                        }
                        temp = ptr->next->next;
                        ptr->next->next = ptr;
                        ptr->next = temp;
                        status = true;
                    }
                    // Swaps among nodes which is not the list head
                    // E.g. [3, 4(Max), 1, 2] --> [3, 1, 4(Max), 2]
                    else {
                        temp = ptr->next->next;
                        prevPtr->next->next->next = ptr;
                        prevPtr->next = ptr->next;
                        ptr->next = temp;
                        status = true;
                    }
                }
                // If the previous node pointer is same as the node pointer
                if (ptr == prevPtr) {
                    if (strcmp(swapList, "currentList") == 0) {
                        prevPtr = ListHead;
                    }
                    else if (strcmp(swapList, "historyList") == 0) {
                        prevPtr = HistoryHead;
                    }
                }
                else {
                    prevPtr = prevPtr->next;
                }
                // Switch to the next node address
                ptr = prevPtr->next;
            }
            lastPtr = ptr;
        } while (status);

    }
}

// Display all the outstanding or historica loaning records
void GenerateReport() {
    char displayList[50], displayItem[50];
    int option = 0, orderOption = 1, total = 0, exitOption = 3;
    bool validInput = true;

    // Accept input of generate outstanding or historical unit loan reports
    do {
        ClearConsoleScreen();
        GenerateReportMenu();
        printf("\n\t        1. Generate Outstanding Unit Loan Reports\n");
        printf("\t        2. Generate Historical Unit Loan Reports\n");
        printf("\t        3. Exit\n");
        printf("\n\t---------------------------------------------------------------------\n");
        if (!validInput) {
            printf("\t(Invalid Input)");
        }
        option = UserInput(3);
        if (option <= 0 || option > 3) {
            validInput = false;
        }
    } while (option <= 0 || option > 3);

    if (option == 1 || option == 2) {
        // Swap the list between current and history list
        if (option == 1) {
            strcpy(displayList, "currentList");
            strcpy(displayItem, "current");
        }
        else if (option == 2) {
            strcpy(displayList, "historyList");
            strcpy(displayItem, "history");
        }

        // Count the total amount of digital pad and wireless microphone
        Count(displayList);
        total = wmLoanedAmount + dpLoanedAmount;

        // Print all records if there is any record in the list
        validInput = true;
        if (total > 0) {
            do {
                ClearConsoleScreen();
                GenerateReportMenu();
                if (orderOption == 1) {
                    SwapList("lecturerID", displayList);
                }
                else if (orderOption == 2) {
                    SwapList("loanDate", displayList);
                }
                else if (strcmp(displayList, "historyList") == 0) {
                    if (orderOption == 3) {
                        SwapList("returnDate", displayList);
                    }
                }
                DisplayLoaning("all", "None", displayList, "horizontal");

                printf("\n\t        1. Ordered By Lecturer ID\n");
                printf("\t        2. Ordered By Loan Date\n");
                if (strcmp(displayList, "currentList") == 0) {
                    printf("\t        3. Exit\n");
                    exitOption = 3;
                }
                else if (strcmp(displayList, "historyList") == 0) {
                    printf("\t        3. Ordered By Return Date\n");
                    printf("\t        4. Exit\n");
                    exitOption = 4;
                }
                printf("\n\t---------------------------------------------------------------------\n");
                if (!validInput) {
                    printf("\t(Invalid Input)");
                }
                orderOption = UserInput(exitOption);
                if (orderOption <= 0 || orderOption > exitOption) {
                    validInput = false;
                    orderOption == 1;
                }
                else
                {
                    validInput = true;
                }
            } while (orderOption != exitOption);
        }
        else {
            do {
                ClearConsoleScreen();
                GenerateReportMenu();
                printf("\n\t             ( There is no %s loaning in the system )\n\n", displayItem);
                printf("\t      1. Exit                                    \n");
                printf("\t---------------------------------------------------------------------\n");
                if (!validInput) {
                    printf("\t(Invalid Input)");
                }
                orderOption = UserInput(1);
                if (orderOption != 1) {
                    validInput = false;
                }
            } while (orderOption != 1);
        }
    }
}

// Search for the specific lecturer ID or serial number given and display all the records
void SearchLoaning() {
    int option = 0, subOption = 0;
    char searchItem[50], searchItemType[50], displayItemType[50], sortBy[50];
    bool validInput = true, displayLoaning = false;
    wirelessMicrophone = false, digitalPad = false;

    do {
        ClearConsoleScreen();
        SearchLoaningMenu();
        printf("\n\t      1. Search by LecturerID                    \n");
        printf("\t      2. Search by Gadget S/N                    \n");
        printf("\t      3. Exit                                    \n\n");
        printf("\t---------------------------------------------------------------------\n");
        option = UserInput(3);
        if (!validInput) {
            printf("\n(Invalid Input)");
        }
        if (option < 1 || option > 3) {
            validInput = false;
        }
    } while (option < 1 || option > 3);

    if (option == 1 || option == 2) {
        if (option == 1) {
            strcpy(searchItemType, "lecturerID");
            strcpy(displayItemType, "lecturer ID");
            strcpy(sortBy, "lecturerID");
        }
        else if (option == 2) {
            strcpy(searchItemType, "serialNumber");
            strcpy(displayItemType, "serial number");
            strcpy(sortBy, "loanDate");
        }
        ClearConsoleScreen();
        SearchLoaningMenu();
        printf("\n\t---------------------------------------------------------------------\n");
        // Accept input for Lecturer ID and serial number
        printf("\tPlease enter the %s\n\t> ", displayItemType);
        scanf("%s", searchItem);
        rewind(stdin);

        validInput = true;
        do {
            ClearConsoleScreen();
            SearchLoaningMenu();
            if (Check(searchItemType, searchItem, "currentList")) {
                SwapList(sortBy, "currentList");
                DisplayLoaning(searchItemType, searchItem, "currentList", "horizontal");
                displayLoaning = true;
            }
            if (Check(searchItemType, searchItem, "historyList")) {
                SwapList(sortBy, "historyList");
                DisplayLoaning(searchItemType, searchItem, "historyList", "horizontal");
                displayLoaning = true;
            }
            if (!displayLoaning) {
                printf("\n\t ( There is no current and history loaning on %s: %s)\n\n", displayItemType, searchItem);
            }
            printf("\n\t      1. Exit                                    \n");
            printf("\t---------------------------------------------------------------------\n");
            if (!validInput) {
                printf("\t(Invalid Input)");
            }
            subOption = UserInput(1);
            if (subOption != 1) {
                validInput = false;
            }
        } while (subOption != 1);
    }
}