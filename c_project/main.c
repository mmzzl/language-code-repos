#include <stdio.h>
#include <string.h>
#define MAX_NAME_LENGTH 50
#define MAX_PHONE_LENGTH 20

typedef struct {
    char name[MAX_NAME_LENGTH];
    char phone[MAX_PHONE_LENGTH];
} Contact;

void add_contact();
void view_contacts();
void delete_contact();
void update_contact();
void search_contact();
void save_contacts();

int main()
{
    int choice;
    while(1) {
        
        printf("1. 添加联系人\n");
        printf("2. 删除联系人\n");
        printf("3. 展示联系人\n");
        printf("4. 修改联系人\n");
        printf("5. 搜索联系人\n");
        printf("6. 保存联系人\n");
        scanf("%d", &choice);
        getchar(); //清除缓冲区的换行符
        switch (choice)
        {
        case 1/* constant-expression */:
            /* code */
            add_contact();
            break;
        case 2:
            delete_contact();
            break;
        case 3:
            view_contacts();
            break;
        case 4:
            update_contact();
            break;
        case 5:
            search_contact();
        case 6:
            save_contacts();
        default:
            break;
        }
    }
}

void add_contact()
{
    FILE *file = fopen("contacts.txt", "a");
    if (file == NULL) {
        printf("Failed to open file. \n");
        return ;
    }
    Contact contact;
    printf("Enter name:");
    fgets(contact.name, MAX_NAME_LENGTH, stdin);
    contact.name[strcspn(contact.name, "\n")] = '\0'; // 去除换行符
    printf("Enter phone:");
    fgets(contact.phone, MAX_PHONE_LENGTH, stdin);
    contact.phone[strcspn(contact.phone, "\n")] = '\0';
    fprintf(file, "%s|%s\n", contact.name, contact.phone);
    fclose(file);
    printf("Contact added successfully.\n");
}

void view_contacts()
{
    FILE *file = fopen("contacts.txt", "r");
    if (file == NULL) {
        printf("Failed to open file. \n");
        return ;
    }
    char buffer[1024];
    while(fgets(buffer, sizeof(buffer), file)){
        printf("%s", buffer); // 打印每行的数据
    }
    fclose(file);
}

void delete_contact()
{
    char *name;
    FILE *file = fopen("contacts.txt", "w");
    if (file == NULL) {
        printf("Failed to open file. \n");
        return ;
    }
    // 创建一个临时文件来存储修改后的内容
    FILE *temp_file = fopen("temp_contacts.txt","w");
    if (temp_file == NULL) {
        printf("Failed to create temp file");
        fclose(file);
        return;
    }
    printf("输入要删除的姓名: ");
    scanf("%s", &name);
    char buffer[1024];    
    while(fgets(buffer,sizeof(buffer), file)) {
        printf("buffer %s", buffer);
        if (strstr(buffer,name) == NULL) {
            fputs(buffer, temp_file);
        }
    }

    fclose(file);
    fclose(temp_file);
    // 删除原文件
    if (remove("contacts.txt") != 0) {
        printf("Failed to remove original file\n");
        return;
    }

    if (rename("temp_contacts.txt", "contacts.txt") != 0) {
        printf("Failed to rename temp file\n");
    } else {
        printf("delete contact success");
    }
    
}

void update_contact()
{

}

void search_contact()
{

}

void save_contacts()
{

}