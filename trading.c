#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "tspm_header.h"

// function to simulate current prices of listed companies
void simulate_prices()
{
    srand(time(NULL));
    for (int i = 0; i < TOTAL_COMPANIES; i++)
    {
        float range = listed[i].max_price - listed[i].min_price;
        listed[i].current_price = listed[i].min_price + ((float)rand() / (RAND_MAX + 1.0f)) * range;
    }

    for (int i = 0; i < count_stock; i++)
    {
        for (int j = 0; j < TOTAL_COMPANIES; j++)
        {
            if (strcmp(portfolio[i].id, listed[j].id) == 0)
            {
                portfolio[i].current_price = listed[j].current_price;
                break;
            }
        }
    }
}

// function to show companies available for trading
void show_companies()
{
    printf("\n");
    printf("  %-12s  %-4s  %-30s  %-8s  %s\n",
           "Category", "ID", "Company", "Ticker", "Current Price");
    printf("  %-12s  %-4s  %-30s  %-8s  %s\n",
           "------------", "----", "------------------------------",
           "--------", "----------");
    for (int i = 0; i < TOTAL_COMPANIES; i++)
    {
        printf("  %-12s  %-4s  %-30s  %-8s  Rs.%.2f\n",
               listed[i].category,
               listed[i].id,
               listed[i].company,
               listed[i].ticker,
               listed[i].current_price);
    }
}

// function to calculate profit/loss of a stock
float calculate_PL(stock s)
{
    return (s.current_price - s.buy_price) * s.quantity;
}

// function to calculate profit/loss percentage of a stock
float calculate_PL_percentage(stock s)
{
    if (s.buy_price == 0)
        return 0;
    return ((s.current_price - s.buy_price) / s.buy_price) * 100;
}

// function to record the sale transaction
void record_sale(stock s, int q)
{
    FILE *f;
    f = fopen(SALEHISTORY, "a");
    if (f == NULL)
    {
        puts("ERROR! couldn't open file");
        exit(EXIT_FAILURE);
    }
    else
    {
        sale sale_entry;
        time_t t = time(NULL);
        strcpy(sale_entry.id, s.id);
        strcpy(sale_entry.company, s.company);
        sale_entry.quantity = q;
        strftime(sale_entry.buy_date, sizeof(sale_entry.buy_date), "%d/%m/%Y", localtime(&t));
        fwrite(&sale_entry, sizeof(sale_entry), 1, f);
        fclose(f);
    }
}

// function to record the buy transactions
void record_buy(company_listed c, int q)
{
    FILE *f;
    f = fopen(BUYHISTORY, "a");
    if (f == NULL)
    {
        puts("ERROR! couldn't open file");
        exit(EXIT_FAILURE);
    }
    else
    {
        buy buy_entry;
        time_t t = time(NULL);
        strcpy(buy_entry.id, c.id);
        strcpy(buy_entry.company, c.company);
        strftime(buy_entry.buy_date, sizeof(buy_entry.buy_date), "%d/%m/%Y", localtime(&t));
        buy_entry.quantity = q;
        fwrite(&buy_entry, sizeof(buy_entry), 1, f);
        fclose(f);
    }
    return;
}

// function to buy shares of a company
void buy_share()
{
    if (count_stock >= MAXSTOCKS)
    {
        puts("Portfolio is full! Cannot add more stocks.");
        return;
    }
    show_companies();
    time_t t = time(NULL);
    stock new_stock;
    int found = -1, qty;
    char id[15], choice;
    printf("\nEnter the ID of stock you want to purchase: ");
    scanf("%14s", id);
    for (int i = 0; i < TOTAL_COMPANIES; i++)
    {
        if (strcmp(id, listed[i].id) == 0)
        {
            found = i;
            break;
        }
    }
    if (found == -1)
    {
        puts("Invalid Id! Enter valid Id for purchase.");
        return;
    }
    printf("Enter the quantity to be Purchased: ");
    scanf("%d", &qty);
    if (qty <= 0)
    {
        puts("Invalid quantity! Enter non-zero positive quantity");
        return;
    }
    printf("\nPurchase Description:\nCompany Name: %s\nShare Price: %.2f\nQuantity: %d\nTotal Price: %.2f", listed[found].company, listed[found].current_price, qty, (qty * listed[found].current_price));
    puts("\nContinue Purchase? (y/n)");
    scanf(" %c", &choice);
    if (tolower(choice) != 'y')
    {
        puts("Transaction Cancelled!");
        return;
    }
    for (int i = 0; i < count_stock; i++)
    {
        if (strcmp(listed[found].id, portfolio[i].id) == 0)
        {
            portfolio[i].buy_price = ((portfolio[i].buy_price * portfolio[i].quantity) + (listed[found].current_price * qty)) / (portfolio[i].quantity + qty);
            portfolio[i].quantity += qty;
            record_buy(listed[found], qty);
            puts("Purchase Successful, Portfolio updated successfully!");
            return;
        }
    }
    strcpy(new_stock.company, listed[found].company);
    strcpy(new_stock.ticker, listed[found].ticker);
    strcpy(new_stock.id, listed[found].id);
    strftime(new_stock.buy_date, sizeof(new_stock.buy_date), "%d/%m/%Y", localtime(&t));
    new_stock.quantity = qty;
    new_stock.buy_price = listed[found].current_price;
    new_stock.current_price = listed[found].current_price;
    record_buy(listed[found], qty);
    portfolio[count_stock] = new_stock;
    count_stock++;
    puts("Purchase Successful !");
    return;
    
}

// functions to sell shares of a company
void sell_share()
{
    view_portfolio(); // p/l to be shown
    int found = -1, qty;
    char id[15], choice;
    printf("Enter the Id of stock to sell: ");
    scanf("%14s", id);
    for (int i = 0; i < count_stock; i++)
    {
        if (strcmp(id, portfolio[i].id) == 0)
        {
            found = i;
            break;
        }
    }
    if (found == -1)
    {
        puts("Invalid Id! Enter valid Id for Sale.");
        return;
    }
    printf("Enter the quantity to be Sold: ");
    scanf("%d", &qty);
    if (qty <= 0 || qty > portfolio[found].quantity)
    {
        puts("Invalid quantity! Enter valid value");
        return;
    }
    printf("\nSelling Description:\nCompany Name: %s\nShare Price: %.2f\nQuantity: %d\nTotal Price: %.2f\n", portfolio[found].company, portfolio[found].current_price, qty, (qty * portfolio[found].current_price));
    puts("\nContinue Sale? (y/n)");
    scanf(" %c", &choice);
    if (tolower(choice) != 'y')
    {
        puts("Transaction Cancelled!");
        return;
    }
    float realized_pl = (portfolio[found].current_price - portfolio[found].buy_price) * qty;
    if (realized_pl > 0)
    {
        printf("PROFIT EARNED!\nAmount: Rs.%.2f\n", realized_pl);
    }
    else if (realized_pl == 0)
    {
        puts("NEITHER PROFIT NOR LOSS!");
    }
    else
    {
        printf("LOSS INCURRED!\nAmount: Rs.%.2f\n", (realized_pl * -1));
    }

    record_sale(portfolio[found], qty);

    if (qty == portfolio[found].quantity)
    {
        for (int i = found; i < (count_stock - 1); i++)
        {
            portfolio[i] = portfolio[i + 1];
        }
        count_stock--;
    }
    else
    {
        portfolio[found].quantity -= qty;
    }
    puts("Sale Successful");
}
