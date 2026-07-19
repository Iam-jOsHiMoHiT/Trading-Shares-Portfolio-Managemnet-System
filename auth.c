
#include "tspm_header.h"
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

void get_password(char *password, int max_len)
{
    int i = 0;
    char ch;

    while (i < max_len - 1)
    {
        ch = getch();
        if (ch == '\r' || ch == '\n')
        {
            break;
        }
        else if (ch == '\b')
        { // backspace
            if (i > 0)
            {
                i--;
                printf("\b \b");
            }
        }
        else
        {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
}

void display_auth_page()
{
    printf("\n");
    printf("================================================================================\n");
    printf("                  TRADING PORTFOLIO MANAGEMENT SYSTEM (TSPM)\n");
    printf("                           Authentication Portal\n");
    printf("================================================================================\n");
    printf("\n");
    printf("                  Welcome to the Portfolio Management System\n");
    printf("                        Secure Login Required\n");
    printf("\n");
    printf("================================================================================\n");
    printf("\n");
}

int login()
{
    char username[50];
    char password[50];
    int attempts = 3;
    int login_successful = 0;

    display_auth_page();

    while (attempts > 0 && !login_successful)
    {
        printf("Enter Username: ");
        scanf("%49s", username);

        printf("Enter Password: ");
        get_password(password, sizeof(password));
        // Clear input buffer to prevent overflow if user pasted a long string
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
        {
        }
        printf("\n");

        if (strcmp(username, USERNAME) == 0 && strcmp(password, PASSWORD) == 0)
        {
            printf("\n");
            printf("================================================================================\n");
            printf("                    Login Successful! Welcome %s\n", username);
            printf("================================================================================\n");
            printf("\n");
            login_successful = 1;
            return 1;
        }
        else
        {
            attempts--;
            if (attempts > 0)
            {
                printf("\nInvalid credentials! Attempts remaining: %d\n\n", attempts);
            }
            else
            {
                printf("\nLogin failed! Maximum attempts exceeded.\n");
                printf("================================================================================\n\n");
                return 0;
            }
        }
    }

    return 0;
}

int load_portfolio()
{
    FILE *fp;

    fp = fopen(FILENAME, "rb");

    if (fp == NULL)
    {
        printf("Portfolio file not found. Starting with an empty portfolio.\n");
        count_stock = 0;
        return 0;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);
    count_stock = file_size / sizeof(stock);

    if (count_stock > 0)
    {
        if (fread(portfolio, sizeof(stock), count_stock, fp) != count_stock)
        {
            printf("Error reading portfolio data from file.\n");
            count_stock = 0;
            fclose(fp);
            return 0;
        }
    }

    fclose(fp);
    printf("Portfolio loaded successfully! (%d stocks found)\n\n", count_stock);
    return 1;
}

int save_portfolio()
{
    FILE *fp;

    fp = fopen(FILENAME, "wb");

    if (fp == NULL)
    {
        printf("Error: Could not create or open portfolio file for saving.\n");
        return 0;
    }

    if (fwrite(portfolio, sizeof(stock), count_stock, fp) != count_stock)
    {
        printf("Error writing portfolio data to file.\n");
        fclose(fp);
        return 0;
    }

    fclose(fp);
    printf("Portfolio saved successfully! (%d stocks saved)\n\n", count_stock);
    return 1;
}