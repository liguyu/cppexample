#include <stdio.h>
#include <winsock.h>
#include <mysql.h>
#include <windows.h>
#pragma comment(lib, "libmysql.lib")


int main(int argc, char* argv[])
{
	unsigned short Port = 3306;
	char *IPAddress = "192.168.10.147";
	char *UserName = "root";
	char *Password = "djtest";
	char *DBName = "activeCode";
	unsigned long i;
	printf("Start... \n");
	
	MYSQL *ssock;
	MYSQL_RES   *res;  	
	MYSQL_ROW   row;   
	//char execsql[500];
	ssock = (MYSQL *)malloc(sizeof(MYSQL));  
	//在某些版本中，不需要该初始化工作，可观看mysql.H以及readme
	mysql_init(ssock);
	if(ssock == NULL)
	{
		printf("EROR: MySQL ssock init error. \n");
		return FALSE;
	}
	printf("MySQL ssock init OK. \n");
	
	//连接到指定的数据库
	ssock = mysql_real_connect(ssock, IPAddress, UserName, Password, NULL, Port, NULL, 0);
	if(!ssock)
	{
		printf("conn fail... \n");
		//memcpy(eee, mysql_error(ssock), 20);
		//fprintf(stderr, "Failed to connect to database: Error: %s ", mysql_error(ssock));
		
		//printf("%c ", eee);
		unsigned int mtint = mysql_errno(ssock);
		//printf("%d ");
		return FALSE;
	}
	printf("MySQL connnect OK... \n");
	
	if(mysql_select_db(ssock, DBName) != 0)
	{
		printf("select db error. \n");
		return FALSE;
	}
	printf("select db OK. \n");
	printf("version=%d \n", mysql_get_server_version(ssock));
	//SQL查询语句
	if(mysql_query( ssock,"SELECT * FROM  userinfo"))  
	{
		printf("mysql_query() Error, %s\n", mysql_error(ssock));  
	}
	
	if( !(res = mysql_store_result(ssock)) )  
	{
		printf("mysql_store_result() Error, %s\n", mysql_error(ssock));  
	} 
	
	while( (row = mysql_fetch_row(res)) )
	{  		
		for(i=0 ; i<mysql_num_fields(res); i++)  
		{
			printf("%s ",row[i]);
		}	
		printf("\n");
	}  	
	mysql_free_result(res);   

	//执行SQL插入语句,第一列自增
	if(mysql_query( ssock,"insert into UserInfo SET userName=\'lmy4\', userLogName=\'lmy4\', userLogPWD=\'000000\', userRole=1,userState=0 " ))  
	{
		printf("mysql_query() Error, %s\n", mysql_error(ssock));  
	}

	mysql_close(ssock);
	printf("End... \n");
	
	return TRUE;
}
