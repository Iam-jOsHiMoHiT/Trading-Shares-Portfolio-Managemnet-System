#ifndef TSPM_HEADER
#define TSPM_HEADER

#define MAXSTOCKS 100
#define HALF_BARS 20
#define TOTAL_COMPANIES 10
#define FILENAME "portfolio.dat"
#define BUYHISTORY "buy_history.dat"
#define SALEHISTORY "sale_history.dat"
#define USERNAME "admin"
#define PASSWORD "password"

typedef struct
{
    char id[10];
    char company[50];
    int quantity;
    char buy_date[15];
} buy;

typedef struct
{
    char id[10];
    char company[50];
    int quantity;
    char buy_date[15];
} sale;

typedef struct
{
    char id[10];
    char company[50];
    char ticker[20];
    int quantity;
    float buy_price;
    float current_price;
    char buy_date[15];

} stock;

typedef struct
{
    char category[50];
    char id[10];
    char company[50];
    char ticker[20];
    float min_price;
    float max_price;
    float current_price;
} company_listed;

extern stock portfolio[MAXSTOCKS];
extern int count_stock;
extern company_listed listed[TOTAL_COMPANIES];

// function declaration for login module
int login();
int load_portfolio();
int save_portfolio();

// function declaration for trading module
void buy_share();
void sell_share();
float calculate_PL(stock s);
float calculate_PL_percentage(stock s);
void simulate_prices();
void show_companies();
void record_buy(company_listed c, int q);
void record_sale(stock s, int q);

// function declaration for display module
void view_portfolio();
void view_chart();
void detailed_report();
void load_buy();
void load_sale();

#endif