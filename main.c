#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX_FLIGHTS 100
#define MAX_CLIENTS 500
#define MAX_ORDERS 1000

#pragma warning(disable:4996)
typedef struct {
    char flight_id[10];     //航班号
    char depart_city[30];   //出发城市
    char arrive_city[30];   //到达城市
    char airline[20];       //航空公司
    char depart_time[6];    //起飞时间 HH:MM
    char arrive_time[6];    //到达时间
    float price;            //基础票价
    float discount;         //折扣率
    int total_seats;        //总座位数
    int available_seats;    //剩余座位    
}Flight;

typedef struct
{
    char order_id[15];      //订单编号
    char cline_id[20];      //客户证件号
    char cline_name[30];    //客户姓名
    char flight_id[10];     //航班号
    int tickets_num;        //订票数量
}Order;

//全局数据存储
Flight flights[MAX_FLIGHTS];
Order orders[MAX_ORDERS];
int flight_count = 0;
int order_count = 0;

//文件路径
const char* flight_file = "flights.dat";
const char* order_file = "orders.dat";

void main_menu()
{
    system("cls");  // 清屏
    printf("\n========= 航班订票系统 =========\n");
    printf("1. 录入航班信息\n");
    printf("2. 修改航班信息\n");
    printf("3. 航班信息存盘\n");
    printf("4. 航班信息导入\n");
    printf("5. 航班查询\n");
    printf("6. 订票服务\n");
    printf("7. 退票服务\n");
    printf("8. 客户资料查询\n");
    printf("9. 订单查询\n");
    printf("0. 退出系统\n");
    printf("=================================\n");
    printf("请输入选择：");
}

void add_flight() {
    if (flight_count > MAX_FLIGHTS) {
        printf("航班数量已达上限! \n");
        return;
    }

    Flight f;
    printf("\n输入航班号：");
    scanf("%s", f.flight_id);
    for (int i = 0; i < flight_count; i++) {
        if (strcmp(f.flight_id, flights[i].flight_id) == 0) {
            printf("航班号已存在! \n");
            printf("重新输入航班号：");
            scanf("%s", f.flight_id);
        }
    }                                // 检查航班号是否已存在...
    printf("出发城市：");
    scanf("%s", f.depart_city);
    printf("到达城市：");
    scanf("%s", f.arrive_city);
    printf("航空公司：");
    scanf("%s", f.airline);
    printf("起飞时间（HH:MM）：");
    scanf("%s", f.depart_time);
    printf("到达时间（HH:MM）：");
    scanf("%s", f.arrive_time);
    printf("基础票价：");
    scanf("%f", &f.price);
    printf("折扣率（0-1）：");
    scanf("%f", &f.discount);
    printf("总座位数：");
    scanf("%d", &f.total_seats);
    f.available_seats = f.total_seats;

    flights[flight_count++] = f;
    printf("航班添加成功！\n");

}
void modify_flight()
{
    char target_id[10];
    printf("\n输入要修改的航班号：");
    scanf("%s", target_id);

    // 查找航班
    int index = -1;
    for (int i = 0; i < flight_count; i++) {
        if (strcmp(flights[i].flight_id, target_id) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("未找到该航班！\n");
        return;
    }

    // 显示当前航班信息
    printf("\n当前航班信息：\n");
    printf("航班号：%s\n", flights[index].flight_id);
    printf("出发城市：%s\n", flights[index].depart_city);
    printf("到达城市：%s\n", flights[index].arrive_city);
    printf("航空公司：%s\n", flights[index].airline);
    printf("起飞时间：%s\n", flights[index].depart_time);
    printf("到达时间：%s\n", flights[index].arrive_time);
    printf("基础票价：%.2f\n", flights[index].price);
    printf("折扣率：%.2f\n", flights[index].discount);
    printf("总座位数：%d\n", flights[index].total_seats);
    printf("剩余座位：%d\n", flights[index].available_seats);

    // 输入新的航班信息
    printf("\n开始修改航班信息：\n");
    printf("输入新的出发城市：");
    scanf("%s", flights[index].depart_city);
    printf("输入新的到达城市：");
    scanf("%s", flights[index].arrive_city);
    printf("输入新的航空公司：");
    scanf("%s", flights[index].airline);
    printf("输入新的起飞时间（HH:MM）：");
    scanf("%s", flights[index].depart_time);
    printf("输入新的到达时间（HH:MM）：");
    scanf("%s", flights[index].arrive_time);
    printf("输入新的基础票价：");
    scanf("%f", &flights[index].price);
    printf("输入新的折扣率（0-1）：");
    scanf("%f", &flights[index].discount);
    printf("输入新的总座位数：");
    scanf("%d", &flights[index].total_seats);

    // 确保剩余座位不超过总座位数
    if (flights[index].available_seats > flights[index].total_seats) {
        flights[index].available_seats = flights[index].total_seats;
    }

    printf("\n航班信息修改成功！\n");
    printf("更新后的信息：\n");
    printf("出发城市：%s\n", flights[index].depart_city);
    printf("到达城市：%s\n", flights[index].arrive_city);
    printf("航空公司：%s\n", flights[index].airline);
    printf("起飞时间：%s\n", flights[index].depart_time);
    printf("到达时间：%s\n", flights[index].arrive_time);
    printf("基础票价：%.2f\n", flights[index].price);
    printf("折扣率：%.2f\n", flights[index].discount);
    printf("总座位数：%d\n", flights[index].total_seats);
    printf("剩余座位：%d\n", flights[index].available_seats);

    // 保存修改到文件
    save_flights();
}
//文件存储
void save_flights()
{
    FILE* fp = fopen(flight_file, "wb");
    if (!fp) {
        perror("无法打开航班文件！");
        return;
    }

    fwrite(&flight_count, sizeof(int), 1, fp);
    fwrite(flights, sizeof(Flight), flight_count, fp);
    fclose(fp);
    printf("航班数据保存成功！\n");
}

//文件加载
void load_flights()
{
    FILE* fp = fopen(flight_file, "rb");
    if (!fp) {
        perror("无法打开航班文件！");
        return;
    }

    fread(&flight_count, sizeof(int), 1, fp);
    fread(flights, sizeof(Flight), flight_count, fp);
    fclose(fp);
    printf("航班数据加载成功！\n");
}


//航班查询（按航班号）

void query_by_id() {
    char target[10];
    printf("输入查询航班号：");
    scanf("%s", target);

    for (int i = 0; i < flight_count; i++) {
        if (strcmp(flights[i].flight_id, target) == 0) {
            printf("\n航班号：%s\n", flights[i].flight_id);
            printf("航线：%s-%s\n", flights[i].depart_city, flights[i].arrive_city);
            printf("时间：%s-%s\n", flights[i].depart_time, flights[i].arrive_time);
            printf("票价：%.2f（%.0f折）\n",
                flights[i].price * flights[i].discount,
                flights[i].discount * 100);
            printf("余票：%d/%d\n", flights[i].available_seats, flights[i].total_seats);
            return;
        }
    }
    printf("未找到该航班！\n");
}

//订票功能
void book_ticket()
{
    char flight_id[10], client_id[20], name[30];
    int quantity;

    printf("输入航班号：");
    scanf("%s", flight_id);

    //查找航班
    for (int i = 0; i < flight_count; i++) {
        if (strcmp(flight_id, flights[i].flight_id) == 0) {
            if (flights[i].available_seats <= 0) {
                printf("该航班以无余票！\n");
                printf("为您推荐以下有余票的航班：\n");
                int found = 0;
                for (int j = 0; j < flight_count; j++) {
                    if (flights[j].available_seats > 0) {
                        // 推荐条件：目的地相同，或者出发城市相同
                        if (strcmp(flights[i].depart_city, flights[j].depart_city) == 0 ||
                            strcmp(flights[i].arrive_city, flights[j].arrive_city) == 0) {
                            printf("航班号：%s， 出发城市：%s， 到达城市：%s， 余票：%d， 票价：%.2f\n",
                                flights[j].flight_id,
                                flights[j].depart_city,
                                flights[j].arrive_city,
                                flights[j].available_seats,
                                flights[j].price * flights[j].discount);
                            found = 1;
                        }
                    }
                }
                if (!found) {
                    printf("抱歉，当前没有符合推荐条件的航班。\n");
                }//推荐其他航班
                return;
            }
            printf("当前余票：%d\n", flights[i].available_seats);
            printf("输入订票数量：");
            scanf("%d", &quantity);
            if (quantity > flights[i].available_seats) {
                printf("余票不足！\n");
                return;
            }
            //记录订单
            Order o;
            printf("输入证件号：");
            scanf("%s", o.cline_id);
            printf("输入姓名：");
            scanf("%s", o.cline_name);
            o.tickets_num = quantity;
            strcpy(o.flight_id, flight_id);
            sprintf(o.order_id, "%ld%d", time(0), order_count);      //生成唯一订单号

            orders[order_count++] = o;
            flights[i].available_seats -= quantity;

            printf("订票成功！ 订单号：%s\n", o.order_id);
            return;
        }

    }
    printf("未找到该航班！\n");
}

//退票功能
void refund_ticket()
{
    char order_id[15];
    printf("输入订单号：");
    scanf("%s", order_id);

    for (int i = 0; i < order_count; i++) {
        if (strcmp(orders[i].order_id, order_id) == 0) {
            for (int j = 0; j < flight_count; j++) {
                if (strcmp(flights[j].flight_id, orders[i].flight_id) == 0) {
                    // 恢复座位数
                    flights[j].available_seats += orders[i].tickets_num;
                    printf("成功恢复 %d 张票，航班号：%s，余票：%d\n",
                        orders[i].tickets_num, flights[j].flight_id, flights[j].available_seats);
                    break;
                }
            }        
            for (int k = i; k < order_count - 1; k++) {
                orders[k] = orders[k + 1];
            }
            order_count--;  // 更新订单数量
            printf("退票成功！\n");
            return;
        }
    }
    printf("未找到该订单！ \n");
}


//客户资料查询
void query_customer()
{
    char target_id[20];
    char target_name[30];
    int choice;

    printf("\n-----客户资料查询-----\n");
    printf("1.按证件号查询\n");
    printf("2.按姓名查询\n");
    printf("请选择查询方式：");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("\n输入客户证件号: ");
        scanf("%s", target_id);
        for (int i = 0; i < order_count; i++) {
            if (strcmp(orders[i].cline_id, target_id) == 0) {
                printf("\n客户姓名: %s\n", orders[i].cline_name);
                printf("证件号: %s\n", orders[i].cline_id);
                printf("订单编号: %s\n", orders[i].order_id);
                printf("所购航班号: %s\n", orders[i].flight_id);
                printf("订票数量: %d\n", orders[i].tickets_num);
                return;
            }
        }
        printf("未找到该客户！\n");
    }
    else if (choice == 2) {
        printf("\n输入客户姓名: ");
        scanf("%s", target_name);
        for (int i = 0; i < order_count; i++) {
            if (strcmp(orders[i].cline_name, target_name) == 0) {
                printf("\n客户姓名: %s\n", orders[i].cline_name);
                printf("证件号: %s\n", orders[i].cline_id);
                printf("订单编号: %s\n", orders[i].order_id);
                printf("所购航班号: %s\n", orders[i].flight_id);
                printf("订票数量: %d\n", orders[i].tickets_num);
                return;
            }
        }
        printf("未找到该客户！\n");
    }
    else {
        printf("无效输入！\n");
    }
}

//订单查询功能
void query_order()
{
    char target_order_id[15];
    char target_c_id[20];
    int choice;

    printf("\n----- 订单查询 -----\n");
    printf("1. 按订单编号查询\n");
    printf("2. 按客户证件号查询\n");
    printf("请选择查询方式: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("\n输入订单编号: ");
        scanf("%s", target_order_id);
        for (int i = 0; i < order_count; i++) {
            if (strcmp(orders[i].order_id, target_order_id) == 0) {
                printf("\n订单编号: %s\n", orders[i].order_id);
                printf("客户姓名: %s\n", orders[i].cline_name);
                printf("客户证件号: %s\n", orders[i].cline_id);
                printf("所购航班号: %s\n", orders[i].flight_id);
                printf("订票数量: %d\n", orders[i].tickets_num);
                return;
            }
        }
        printf("未找到该订单！\n");
    }
    else if (choice == 2) {
        printf("\n输入客户证件号: ");
        scanf("%s", target_c_id);
        printf("\n以下是该客户的订单信息：\n");
        for (int i = 0; i < order_count; i++) {
            if (strcmp(orders[i].cline_id, target_c_id) == 0) {
                printf("\n订单编号: %s\n", orders[i].order_id);
                printf("客户姓名: %s\n", orders[i].cline_name);
                printf("客户证件号: %s\n", orders[i].cline_id);
                printf("所购航班号: %s\n", orders[i].flight_id);
                printf("订票数量: %d\n", orders[i].tickets_num);
                return;
            }
        }
        printf("未找到该客户的订单！\n");
    }
    else {
        printf("无效输入！\n");
    }
}
int main()
{
    int choice;
    load_flights();
    do {
        main_menu();
        scanf("%d", &choice);

        switch (choice)
        {
        case 1: add_flight(); break;
        case 2: modify_flight(); break;
        case 3: save_flights(); break;
        case 4: load_flights(); break;
        case 5: query_by_id(); break;
        case 6: book_ticket(); break;
        case 7: refund_ticket(); break;
        case 8: query_customer(); break;
        case 9: query_order(); break;
            // 其他功能实现...
        case 0: save_flights(); break; // 退出前保存
        }
        system("pause");
    } while (choice != 0);


    return 0;
}