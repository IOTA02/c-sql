#include <iostream>
#include <stdio.h>
#include <sqlite3.h>
#include <cstring>

using namespace std;

//переменная для ошибок (потом как-нибудь)
char *err = nullptr;

//, string name_table, string column_name[]

int create_db(string directory)
{
    setlocale(LC_ALL, "ALL");
    //объявление объекта бд
    sqlite3 *db = nullptr;
    
    //создание базы/открытие соеденения
    sqlite3_open(directory.c_str(), &db);

    //закрытие соеденения
    sqlite3_close(db);
    
    return 1; // Вернем 1, чтобы показать успешное выполнение
}

int create_tables(string directory, string table_name, vector<string> column_name)
{
    //открываем базу
    sqlite3 *db = nullptr;
    sqlite3_open(directory.c_str(), &db);
    
    string q_prepare;
    
    for(const string& str : column_name)
    {
        q_prepare += str + ", ";
    }
    if (!q_prepare.empty())
    {
        q_prepare.pop_back();
        q_prepare.pop_back();
    }
    
    string q_create_table = "CREATE TABLE IF NOT EXISTS " + table_name + "(" + q_prepare + "); ";
    
    sqlite3_exec(db, q_create_table.c_str(), 0, 0, &err);

    //закрытие соеденения
    sqlite3_close(db);
    
    return 1;
}

int insert_data(string directory)
{
    sqlite3 *db = nullptr;
    
    sqlite3_open(directory.c_str(), &db);
    
    const char* q_insert = "INSERT INTO users(login, pass, acces_lvl) VALUES ('user', 'user123', 1)";
    
    sqlite3_exec(db, q_insert, 0, 0, &err);
    
    sqlite3_close(db);
    
    return 1;
}

int q_sql(string directory, string table, string where)
{
    sqlite3 *db = nullptr;
    
    int database = sqlite3_open(directory.c_str(), &db);
    
    string querry_1pt = "select * from ";
    string querry_2pt = " where login = ";
    string querry = querry_1pt + table + querry_2pt + "\"" + where + "\"";
        
    const char *q_select = querry.c_str();
        sqlite3_stmt *q_sucsess;
    
    database = sqlite3_prepare_v2(db, q_select, -1, &q_sucsess, 0);
    while((database = sqlite3_step(q_sucsess)) == SQLITE_ROW)
    {
        int column_count = sqlite3_column_count(q_sucsess);
        
        for(int i =0; i <column_count; i++)
        {
            const char *column = reinterpret_cast<const char *>(sqlite3_column_text(q_sucsess, i));
            cout<< sqlite3_column_name(q_sucsess, i)<<": "<<column<<endl;
        }
    }
    sqlite3_finalize(q_sucsess);
    
    sqlite3_close(db);
    
    return 1;
}

int main(int argc, const char * argv[])
{
    //путь к базе
    string directory = "/Users/yuriya/Desktop/c_learn/";
    string db_name = "";
    //создание переменной для вопросов к пользователю и соответствующие вопросы
    char db_answer = 'n';
    cout<<"Создать базу? y/n"<<endl;
    cin>>db_answer;
    if(db_answer == 'y')
    {
        cout<<"Введите имя базы данных"<<endl;
        cin>>db_name;
        directory += db_name + ".db";
        if(create_db(directory) == 1)
        {
            cout<<"База создана!"<<endl;
        }
    }
    else if(db_answer == 'n')
    {
        cout<<"введите имя базы к которой нужно подключиться: "<<endl;
        cin>>db_name;
        directory += db_name + ".db";
    }
    
    db_answer = NULL;
    cout<<"создать таблицу? y/n"<<endl;
    cin>>db_answer;
    if(db_answer == 'y')
    {
        string table_name, user_insert;
        cout<<"введите название таблицы (если таблицы больше не нужны введите ts): ";
        while (table_name != "ts")
        {
            cin>>table_name;
            if(table_name != "ts")
            {
                cout<<"введите поля(через Enter): "<<endl;
                string data_type; //для проверки того что вводит пользователь
                vector<string> column_name;
                while (user_insert != "stop")
                {
                    cin>>user_insert;
                    if(user_insert != "stop")
                    {
                        cout<<"тип данных: ";
                        cin>>data_type;
                        user_insert += " " + data_type;
                        column_name.push_back(user_insert);
                        cout<<"\nполе готово, введите следующее: ";
                    }
                }
                
                if(create_tables(directory, table_name, column_name) == 1)
                {
                    cout<<"\nтаблица создана, введите следущую: "<<endl;
                }
            }
        }
        
        
    }
    else if(db_answer == 'n')
    {
        cout<<"скипаем..."<<endl;
    }
    
    db_answer = NULL;
    cout<<"вставить информацию? y/n"<<endl;
    cin>>db_answer;
    if(db_answer == 'y')
    {
        cout<<"будет реализованно только в графическом интерфейсе"<<endl;
    }
    else if(db_answer == 'n')
    {
        cout<<"скипаем..."<<endl;
    }
    
    //вывод инфы из базы и открытие базы
    sqlite3 *db = nullptr;
    int database = sqlite3_open(directory.c_str(), &db);
    
    cout<<"вывод информации из таблицы. выберите нужную таблицу: "<<endl;
    
    const char *q_select = "SELECT name FROM sqlite_master WHERE type='table';";
        sqlite3_stmt *q_sucsess;
    
    database = sqlite3_prepare_v2(db, q_select, -1, &q_sucsess, 0);
    while((database = sqlite3_step(q_sucsess)) == SQLITE_ROW)
    {
        const char *table_name = reinterpret_cast<const char *>(sqlite3_column_text(q_sucsess, 0));
        cout<<table_name<<" ";
    }
    cout<<"\n";
    sqlite3_finalize(q_sucsess);
    
    //вывод инфы из таблицы
    string selected_table;
    cin>>selected_table;
    
    if(selected_table == "users")
    {
        string user_login;
        cout<<"введите логин пользователя: ";
        cin>>user_login;
        q_sql(directory, selected_table, user_login);
    }
    
    sqlite3_close(db);
    return 0;
}
