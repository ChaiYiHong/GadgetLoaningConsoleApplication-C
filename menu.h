void MenuHeader() {
    printf("\t---------------------------------------------------------------------\n");
    printf("\t<                                _____   _    _                     >\n");
    printf("\t<                         /\\    |  __ \\ | |  | |                    >\n");
    printf("\t<                        /  \\   | |__) || |  | |                    >\n");
    printf("\t<                       / /\\ \\  |  ___/ | |  | |                    >\n");
    printf("\t<                      / ____ \\ | |     | |__| |                    >\n");
    printf("\t<                     /_/    \\_\\|_|      \\____/                     >\n");
    printf("\t<                                                                   >\n");
    printf("\t---------------------------------------------------------------------\n");
    printf("\t                         Gadget Loaning System                       \n");
}

void MainMenu() {
    MenuHeader();
    printf("\t                             ( Main Menu )                           \n\n");
    printf("\t      1. Borrow gadget                                               \n");
    printf("\t      2. Return gadget                                               \n");
    printf("\t      3. Search Loaning                                              \n");
    printf("\t      4. Generate report                                             \n");
    printf("\t      5. Exit                                                        \n\n");
    printf("\t---------------------------------------------------------------------\n");
}

void BorrowMenu() {
    MenuHeader();
    printf("\t                           ( Borrow Gadget )                         \n");
}

void ReturnMenu() {
    MenuHeader();
    printf("\t                           ( Return Gadget )                         \n");
}

void ExitMenu() {
    MenuHeader();
    printf("\t                               ( Exit )                              \n\n");
    printf("\t             Thanks for using APU Gadget Loaning System              \n");
    printf("\t                           Have a nice day!                          \n");
    printf("\t---------------------------------------------------------------------\n");
}

void SearchLoaningMenu() {
    MenuHeader();
    printf("\t                          ( Search Loaning )                         \n");
}

void GenerateReportMenu() {
    MenuHeader();
    printf("\t                         ( Generate Report )                         \n");
}