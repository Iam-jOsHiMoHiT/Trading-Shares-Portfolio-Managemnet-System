
#include <stdio.h>
#include "tspm_header.h"

stock portfolio[MAXSTOCKS];
int count_stock = 0;

int main()
{
    if (login() != 1)
    {
        puts("Access denied. Exiting.");
        return 1;
    }
    load_portfolio();
    simulate_prices();
    view_portfolio();

    int choice;
    int TRUE = 1;

    while (TRUE)
    {
        puts("\n\nEnter choice:");
        puts("1.Buy Share\n2.Sell Share\n3.View Portfolio\n4.View Transaction History\n5.Exit\n");
        if (scanf("%d", &choice) != 1)
        {
            puts("Invalid input! Please enter a number.");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF)
                ; // clear input buffer
            continue;
        }
        switch (choice)
        {
        case 1:
            buy_share();
            save_portfolio();
            break;

        case 2:
            sell_share();
            save_portfolio();
            break;

        case 3:
            view_portfolio();
            puts("\n\n");
            view_chart();
            puts("\n\n");
            detailed_report();
            puts("\n\n");
            
            break;

        case 4:
        {
            int input;
            puts("Enter choice:");
            puts("\n1.Buy History\n2.Sale History");
            if (scanf("%d", &input) != 1)
            {
                puts("Invalid input! Please enter a number.");
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF)
                    ; // clear input buffer
                break;
            }
            switch (input)
            {
            case 1:
                load_buy();
                break;
            case 2:
                load_sale();
                break;
            default:
                puts("Invalid choice! Enter 1 or 2.");
                break;
            }
            break;
        }

        case 5:
            puts("THANK YOU!");
            save_portfolio();
            TRUE = 0;
            break;

        default:
            puts("Invalid choice! Enter 1-5.");
            break;
        }
    }

    return 0;
}