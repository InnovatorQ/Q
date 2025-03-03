#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX_FLIGHTS 100
#define MAX_CLIENTS 500
#define MAX_ORDERS 1000

typedef struct{
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

void add_flight(){
    if(flight_count > MAX_FLIGHTS){
        printf("航班数量已达上限! \n");
        return;
    }

    Flight f;
    printf("\n输入航班号：");
    scanf("%s",f.flight_id);
    for(int i=0; i<flight_count; i++){
        if(strcmp(f.flight_id, flights[i].flight_id) == 0){
            printf("航班号已存在! \n");
            printf("重新输入航班号：");
            scanf("%s",f.flight_id);
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
//文件存储
void save_flights()
{
    FILE* fp = fopen(flight_file,"wb");
    if(!fp){
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
    FILE* fp = fopen(flight_file,"rb");
    if(!fp){
        perror("无法打开航班文件！");
        return;
    }

    fread(&flight_count, sizeof(int), 1, fp);
    fread(flights, sizeof(Flight), flight_count, fp);
    fclose(fp);
    printf("航班数据加载成功！\n");
}


//航班查询（按航班号）

void query_by_id(){
    char target[10];
    printf("输入查询航班号：");
    scanf("%s",target);

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
    scanf("%s",flight_id);

    //查找航班
    for(int i=0; i<flight_count; i++){
        if(strcmp(flight_id,flights[i].flight_id) == 0){
            if(flights[i].available_seats <= 0){
                printf("该航班以无余票！\n");

                return;
            }
            printf("当前余票：%d\n",flights[i].available_seats);
            printf("输入订票数量：");
            scanf("%d",&quantity);
            if(quantity > flights[i].available_seats){
                printf("余票不足！\n");
                return;
            }
            //记录订单
            Order o;
            printf("输入证件号：");
            scanf("%s",o.cline_id);
            printf("输入姓名：");
            scanf("%s",o.cline_name);
            o.tickets_num = quantity;
            strcpy(o.flight_id,flight_id);
            sprintf(o.order_id, "%ld%d", time(0),order_count);      //生成唯一订单号

            orders[order_count++] = o;
            flights[i].available_seats -= quantity;

            printf("订票成功！ 订单号：%s\n",o.order_id);
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
    scanf("%s",order_id);

    for(int i=0; i<order_count; i++){
        if(strcmp(orders[i].order_id,order_id) == 0){
            for (int j = 0; j < flight_count; j++) {
                if (strcmp(flights[j].flight_id, orders[i].flight_id) == 0) {
                    // 恢复座位数
                    flights[j].available_seats += orders[i].tickets_num;
                    printf("成功恢复 %d 张票，航班号：%s，余票：%d\n", 
                           orders[i].tickets_num, flights[j].flight_id, flights[j].available_seats);
                    break;
                }
            }        //恢复座位数
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

int main()
{
    int choice;
    load_flights();
    do{
        main_menu();
        scanf("%d",&choice);

        switch (choice)
        {
            case 1: add_flight(); break;
            case 3: save_flights(); break;
            case 4: load_flights(); break;
            case 5: query_by_id(); break;
            case 6: book_ticket(); break;
            case 7: refund_ticket();break;
            // 其他功能实现...
            case 0: save_flights(); break; // 退出前保存
        }
        system("pause");
    }while (choice != 0);
    

    return 0;
}