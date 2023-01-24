#include <stdlib.h>




//сканер хранит всё для связи с потоком который занимается поиском пользователей
struct scaner
{
    int i;
};
//юзер хранит всю инфу о пользователе, его ip и ник
struct user
{
    int i;
};

enum result
{
    READY = 0,
    WORK = 1,
    FAIL = -1
};

struct user_list
{
    struct user user;
    struct user_list *next;
};

static struct user_list *new_user_list(struct user user)
{
    struct user_list *res = (struct user_list *)malloc(sizeof(struct user_list));
    res->next = NULL;
    res->user = user;
    return res;
}

static void user_list_add(struct user_list **head, struct user user)
{
    if (*head == NULL)
    {
        *head = new_user_list(user);
        return;
    }
    struct user_list *temp = *head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = new_user_list(user);
    return;
}

int size_user_list(struct user_list **head)
{
    if (*head == NULL)
    {
        return 0;
    }
    int i = 1;
    struct user_list *temp = *head;
    while (temp->next != NULL)
    {
        temp = temp->next;
        i++;
    }
    return i;
}

struct user *get_user(struct user_list **head, int num)
{
    if (*head == NULL)
    {
        return NULL;
    }
    int i = 0;
    struct user_list *temp = *head;
    while (temp != NULL)
    {
        if (num == i)
        {
            return &(temp->user);
        }
        temp = temp->next;
        i++;
    }
    return NULL;
}

void delete_user_list(struct user_list **head)
{
    if (*head == NULL)
    {
        return;
    }
    struct user_list *temp = *head;
    while (temp != NULL)
    {
        struct user_list *del = temp;
        temp = temp->next;
        free(del);
    }
    *head = NULL;
    return;
}
//убивает поток на который ссылается сканер, и 
void stop_scan(struct scaner **scaner)
{
    if (*scaner == NULL){
        return;
    }
    free(*scaner);
    *scaner = NULL;
    return;
}
//запускает поток, сохраняет в сканер всю инфу для связи с ним, если сканер уже ссылается на какой-то поток,
// то необходимо завершить этот поток
void start_scan(struct scaner **scaner)
{
    if (*scaner != NULL)
    {
        stop_scan(scaner);
    }
    static int i = 0;
    *scaner = (struct scaner *)malloc(sizeof(struct scaner));
    (*scaner)->i = i++;

    return;
}
//возвращает текущий результат работы сканера, если всё готово то сохраняет пользователй в списке,
//если список не пустой на начало запроса, то его следует очистить
enum result get_user_list(struct scaner **scaner, struct user_list **userList)
{
    delete_user_list(userList);
    static struct user user;

    for (int i = 0; i < 10; i++)
    {
        user.i++;
        user_list_add(userList, user);
    }
    return READY;

}
//тайбл хранит всё для связи с потоком который занимается получением списка доступный файлов 
//и пользователя у которого этот спискок был запрошен
struct table
{
    struct user owner;
    int i;
};
//хранит имя файла и пользователя от которого это название пришло
struct file_name
{
    struct user owner;
    int i;
    //char*
};

struct file_name_list
{
    struct file_name file_name;
    struct file_name_list *next;
};

static struct file_name_list *new_file_name_list(struct file_name file_name)
{
    struct file_name_list *res = (struct file_name_list *)malloc(sizeof(struct file_name_list));
    res->next = NULL;
    res->file_name = file_name;
    return res;
}

static void table_list_add(struct file_name_list **head, struct file_name file_name)
{
    if (*head == NULL)
    {
        *head = new_file_name_list(file_name);
        return;
    }
    struct file_name_list *temp = *head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = new_file_name_list(file_name);
    return;
}

int size_file_name_list(struct file_name_list **head)
{
    if (*head == NULL)
    {
        return 0;
    }
    int i = 1;
    struct file_name_list *temp = *head;
    while (temp->next != NULL)
    {
        temp = temp->next;
        i++;
    }
    return i;
}

struct file_name *get_file_name(struct file_name_list **head, int num)
{
    if (*head == NULL)
    {
        return NULL;
    }
    int i = 0;
    struct file_name_list *temp = *head;
    while (temp != NULL)
    {
        if (num == i)
        {
            return &(temp->file_name);
        }
        temp = temp->next;
        i++;
    }
    return NULL;
}

void delete_file_name_list(struct file_name_list **head)
{
    if (*head == NULL)
    {
        return;
    }
    struct file_name_list *temp = *head;
    while (temp != NULL)
    {
        struct file_name_list *del = temp;
        temp = temp->next;
        free(del);
    }
    *head = NULL;
    return;
}
// по анологии, закрывает поток
void stop_table(struct table **table)
{
    if(*table == NULL){
        return;
    }
    free(*table);
    *table = NULL;
    return;
}
//создает поток который запускает поток запрашивающей у заданного пользователя список файлов,
//всё для связи с потоком записывается в тэйбл
// если тэйбл уже привязан к потоку, то необходимо закрыть этот поток
void start_table(struct table **table, struct user user)
{
    if (*table != NULL)
    {
        stop_table(table);
    }
    static int i = 0;
    *table = (struct table *)malloc(sizeof(struct table));
    (*table)->i = i++;
    (*table)->owner = user;
    return;
}
//возвращает текущий результат работы потока, если готово то солхраняет имена файлов в списке
enum result get_file_name_list(struct table **table, struct file_name_list **file_nameList)
{
    delete_file_name_list(file_nameList);
    static struct file_name file_name;

    for (int i = 0; i < 10; i++)
    {
        file_name.i++;
        file_name.owner = (*table)->owner;
        table_list_add(file_nameList, file_name);
    }
    return READY;
}

//лоадер хранит всё что нужно для связи с потоком-загрузчиком 
struct loader
{
    int i;
};
//запускает поток загрузчик, принимает имя файла для скачки и в нём есть и сам пользователь
//возващает лоадер который хранит всё что нужно для связи с его потоком
struct loader *start_load(struct file_name file_name)
{
    static int i = 0;
    struct loader *loader = (struct loader *)malloc(sizeof(struct loader));
    return loader;
}
//возвращает готовность загрузски 0-100, -1 - ошибка
//должна срабатывать сразу, ничего не ждать
int load_ready(struct loader **loader)
{
    if(*loader == NULL){
        return -1;
    }
    return 100;
}
//убийство лоадера
void stop_loader(struct loader **loader)
{
    if(*loader == NULL){
        return;
    }
    free(*loader);
    *loader = NULL;
}

struct queue_loaders
{
    struct loader *loader;
    struct queue_loaders *next;
};

static struct queue_loaders *new_queue_loaders(struct loader *loader)
{
    struct queue_loaders *queue_loaders = (struct queue_loaders *)malloc(sizeof(struct queue_loaders));
    queue_loaders->loader = loader;
    queue_loaders->next = NULL;
    return queue_loaders;
}

void push_queue_loaders(struct queue_loaders **head, struct loader *loader)
{
    if (*head == NULL)
    {
        *head = new_queue_loaders(loader);
        return;
    }
    struct queue_loaders *temp = *head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = new_queue_loaders(loader);
    return;
}

struct loader * front_queue_loaders(struct queue_loaders **head){
    if(*head == NULL){
        return NULL;
    }
    return (*head)->loader;
}

void pop_queue_loaders(struct queue_loaders **head){
    if(*head == NULL){
        return;
    }
    struct queue_loaders *temp = (*head)->next;
    stop_loader(&((*head)->loader));
    free(*head);
    *head = temp;
    return;
}

void delete_queue_loaders(struct queue_loaders **head){
    while(*head != NULL){
        pop_queue_loaders(head);
    }
    return;
}