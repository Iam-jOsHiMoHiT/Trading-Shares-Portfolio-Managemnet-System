#include "tspm_header.h"
#include <stdio.h>
void view_portfolio()
{
    // check if count_stock is 0 then print "No stocks in portfolio" and return
    if (count_stock == 0)
    {
        puts("No stocks in portfolio");
        return;
    }
    printf(" %-5s %-30s %-10s %-5s %-15s %-15s %-15s %-10s %-12s\n", "ID", "COMPANY NAME", "TICKER", "QTY", "BUY PRICE", "CURRENT PRICE", "BUY DATE", "P/L", "P/L%%");
    printf(" %-5s %-30s %-10s %-5s %-15s %-15s %-15s %-10s %-12s\n", "-----", "-----------------------------", "----------", "-----", "---------------", "---------------", "---------------", "----------", "----------");
    for (int i = 0; i < count_stock; i++)
    {
        printf(" %-5s %-30s %-10s %-5d Rs.%-12.2f Rs.%-12.2f %-15s Rs.%-9.2f %-9.2f\n", portfolio[i].id, portfolio[i].company, portfolio[i].ticker, portfolio[i].quantity, portfolio[i].buy_price, portfolio[i].current_price, portfolio[i].buy_date, calculate_PL(portfolio[i]), calculate_PL_percentage(portfolio[i]));
    }
    return;
    // else print the portfolio in a tabular format with Id, company name, ticker, quantity, buy price, current price, buy date, profit/loss and profit/loss percentage
}

void view_chart()
{
    if (count_stock == 0)
    {
        printf("No stocks in portfolio.\n");
        return;
    }

    printf("\nPROFIT / LOSS BAR CHART\n");

    // ── STEP 1: find the largest absolute P&L% in the portfolio ──
    // This becomes our scale anchor — the biggest mover gets a full bar,
    // everything else is drawn proportionally to it.
    float max_pnl = 0.0f;
    for (int i = 0; i < count_stock; i++)
    {
        float pnl = ((portfolio[i].current_price - portfolio[i].buy_price) / portfolio[i].buy_price) * 100.0f;
        if (pnl < 0)
            pnl = -pnl; // take absolute value
        if (pnl > max_pnl)
            max_pnl = pnl;
    }

    // edge case — if all stocks are exactly at buy price, nothing to show
    if (max_pnl == 0.0f)
    {
        printf("  All stocks at break-even. No P/L to display.\n");
        return;
    }

    // ── STEP 2: print header ──────────────────────────────────────
    //   "LOSS  <<<<  0  >>>>  PROFIT"
    printf("  %-10s  %*s          |%*s             %s\n", "", HALF_BARS / 2, "LOSS", HALF_BARS / 2, "PROFIT", "");
    printf("  %-10s  ", "TICKER");
    for (int i = 0; i < 2 * HALF_BARS + 1; i++)
        printf("-");
    printf("  P/L%%\n");

    // ── STEP 3: print one row per stock ──────────────────────────
    for (int i = 0; i < count_stock; i++)
    {
        float pnl_percent = ((portfolio[i].current_price - portfolio[i].buy_price) / portfolio[i].buy_price) * 100.0f;

        // how many blocks does this stock get?
        // scale it: biggest mover gets HALF_BARS blocks, rest are proportional
        int blocks = (int)((pnl_percent / max_pnl) * HALF_BARS);
        // blocks is positive for profit, negative for loss

        // print ticker label
        printf("  %-10s  ", portfolio[i].ticker);

        // print LEFT side (loss side) — 20 characters wide
        for (int b = -HALF_BARS; b < 0; b++)
        {
            if (blocks < 0 && b >= blocks)
                printf("#"); // loss bar fills from center leftward
            else
                printf(" "); // empty space
        }

        printf("|"); // center zero line

        // print RIGHT side (profit side) — 20 characters wide
        for (int b = 1; b <= HALF_BARS; b++)
        {
            if (blocks > 0 && b <= blocks)
                printf("#"); // profit bar fills from center rightward
            else
                printf(" "); // empty space
        }

        // print the actual percentage number at the end
        if (pnl_percent >= 0)
            printf("  +%.2f%%\n", pnl_percent);
        else
            printf("  %.2f%%\n", pnl_percent);
    }

    // ── STEP 4: bottom border ─────────────────────────────────────
    printf("  %-10s  ", "");
    for (int i = 0; i < HALF_BARS * 2 + 1; i++)
        printf("-");
    printf("\n");
    printf("  Scale: 1 block = %.2f%%\n\n", max_pnl / HALF_BARS);
}

void detailed_report()
{
    // check if count_stock is 0 then print "No stocks in portfolio" and return
    // else use a loop to iterate through the portfolio and calculate the total investment, current value, profit/loss and profit/loss percentage for whole portfolio.

    if (count_stock == 0)
    {
        printf("no stocks in potfolio");
    }
    else
    {

        float total_investment = 0, total_current_value = 0, profit_loss = 0, profit_loss_percentage = 0.0f;
        for (int i = 0; i < count_stock; i++)
        {
            total_investment += portfolio[i].quantity * portfolio[i].buy_price;
            total_current_value += portfolio[i].quantity * portfolio[i].current_price;
        }
        profit_loss = total_current_value - total_investment;

        if (total_investment != 0)
            profit_loss_percentage = (profit_loss / total_investment) * 100;
        else
            printf(" Rs.%.2f Rs.%.2f Rs.%.2f Rs.%.2f\n", total_investment, total_current_value, profit_loss, profit_loss_percentage);
        printf(" %-20s %-20s %-20s %-20s\n", "TOTAL INVESTMENT", "CURRENT VALUE", "PROFIT/LOSS", "P/L PERCENTAGE");
        printf(" %-20s %-20s %-20s %-20s\n", "________________", "_____________", "___________", "______________");
        printf(" Rs.%-17.2f Rs.%-17.2f Rs.%-17.2f Rs.%-17.2f", total_investment, total_current_value, profit_loss, profit_loss_percentage);
    }
    return;
}

void load_buy()
{
    // open file in read mode
    FILE *f;
    buy buy_record;
    f = fopen(BUYHISTORY, "rb");
    if (f == NULL)
    {
        puts("No buy history found.");
        return;
    }
    printf(" %-5s %-30s %-5s %-10s\n", "ID", "COMPANY NAME", "QUANTITY", "BUY DATE");
    printf(" %-5s %-30s %-5s %-10s\n", "-----", "-------------------------------", "-----", "----------");
    // read data from file till EOF
    while (fread(&buy_record, sizeof(buy), 1, f))
    {
        printf(" %-5s %-30s %-5d %-10s\n", buy_record.id, buy_record.company, buy_record.quantity, buy_record.buy_date);
    }
    fclose(f);
    return;
    // print data in table format
}

void load_sale()
{
    // open file in read mode
    FILE *f;
    sale sale_record;
    f = fopen(SALEHISTORY, "rb");
    if (f == NULL)
    {
        puts("No sale history found.");
        return;
    }
    printf(" %-5s %-30s %-5s %-10s\n", "ID", "COMPANY NAME", "QUANTITY", "BUY DATE");
    printf(" %-5s %-30s %-5s %-10s\n", "-----", "-------------------------------", "-----", "----------");
    // read data from file till EOF
    while (fread(&sale_record, sizeof(sale), 1, f))
    {
        printf(" %-5s %-30s %-5d %-10s\n", sale_record.id, sale_record.company, sale_record.quantity, sale_record.buy_date);
    }
    fclose(f);
    return;
    // print data in table format
}