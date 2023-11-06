#include <stdio.h>		/* printf, sprintf */
#include <stdlib.h>		/* exit, atoi, malloc, free */
#include <unistd.h>		/* read, write, close */
#include <string.h>		/* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>		/* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <stdbool.h>
#include <ctype.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"

void get_book_func(char *res)
{
	if (strstr(res, "No book was found!") != NULL)
		printf("NO BOOK WAS FOUND!\n");
	else
		printf("%s\n", strstr(res, "{"));
}

void JSON_book(JSON_Object *object, char title[BUFLEN], char author[BUFLEN], char genre[BUFLEN], char *pg, char publisher[BUFLEN], char *adbook[1])
{
	json_object_set_string(object, "title", title);
	json_object_set_string(object, "author", author);
	json_object_set_string(object, "genre", genre);
	json_object_set_string(object, "page_count", pg);
	json_object_set_string(object, "publisher", publisher);
}
char *user_input()
{

	char *input = calloc(BUFLEN, sizeof(char));
	printf("username = ");
	scanf("%s", input);
	return input;
}
char *user_passwd()
{

	char *input = calloc(BUFLEN, sizeof(char));
	printf("password = ");
	scanf("%s", input);
	return input;
}

void JSON_user(JSON_Object *object, char *username, char *password)
{
	json_object_set_string(object, "username", username);
	json_object_set_string(object, "password", password);
}
void verify_deleted(char *deleted)
{
	if (deleted != NULL)
		printf("NO BOOK! Entered id is not valid!\n");
	else
		printf("Success!\n");
}
char *get_route(char *route, int book_id)
{
	sprintf(route, "%s/%d", BOOKS, book_id);
	return route;
}
void print_res(char*res){
	printf("%s\n", strstr(res, "["));
}
int user_id()
{
	char carte[BUFLEN];
	int book_id;
	printf("id=");
	scanf("%s", carte);
	book_id = atoi(carte);
	return book_id;
}
int verify_command(char command[BUFLEN])
{
	return (strncmp(command, "register", 8) == 0) || (strncmp(command, "login", 5) == 0);
}
void success_library(int sockfd){
	printf("SUCCESS! You are now in the library!\n");
	close_connection(sockfd);
}
int verify_tok(char*tok){
	if(tok==NULL)
	{
		printf("NO ACCESS IN LIBRARY!\n");
		return 0;
	}
	return 1;
}

int main(int argc, char *argv[])
{
	int connected = 0, in_library = 0;
	int exit = 0;
	int port = 8080;
	char host[16] = "34.254.242.81";
	int sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
	char citeste[BUFLEN];
	char *user[1];
	char *cookies[1];
	char token[BUFLEN];
	char *addbook[1];
	while (exit == 0)
	{
		fgets(citeste, BUFLEN, stdin);
		sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);

		if (strncmp(citeste, "exit", 4) == 0)
		{
			exit = 1;
			close_connection(sockfd);
		}
		else if (verify_command(citeste) == 1)
		{

			char *username = calloc(BUFLEN, sizeof(char));
			char *password = calloc(BUFLEN, sizeof(char));
			username = user_input();
			password = user_passwd();

			JSON_Value *value = json_value_init_object();
			JSON_Object *object = json_value_get_object(value);
			JSON_user(object, username, password);
			user[0] = json_serialize_to_string(value);

			if (strncmp(citeste, "register", 8) == 0)
			{
				if (strstr(recv_post_req(sockfd, host, REGISTER, user, NULL), "is taken") != NULL)
					printf("TAKEN USERNAME! Try something else.\n");
				else
					printf("SUCCESS! You are now registred.\n");
				close_connection(sockfd);
			}
			else
			{
				char *cookie = strstr(recv_post_req(sockfd, host, LOGIN, user, NULL), "Set-Cookie: ");
				if (cookie == NULL)
				{
					printf("WRONG !\n");
					connected = 0;
					in_library = 0;
					close_connection(sockfd);
					continue;
				}

				strtok(cookie, ";");
				cookie = cookie + strlen("Set-Cookie: ");
				cookies[0] = cookie;

				if (cookie != NULL)
				{
					printf("SUCCESS! You are now logged in %s %s!\n", username, password);
					connected = 1;
					in_library = 0;
					close_connection(sockfd);
				}
			}
		}
		else if (strncmp(citeste, "logout", 6) == 0)
		{
			if (connected == 1)
			{
				connected = 0;
				in_library = 0;
				recv_get_req(sockfd, host, LOGOUT, token, cookies, "get");
				success_library(sockfd);
			}
			else
				printf("You are not logged in!\n");
			close_connection(sockfd);
		}
		else if (strncmp(citeste, "enter_library", 13) == 0)
		{

			if (connected && !in_library)
			{
				// Daca suntem logati si nu suntem in librarie, se retine tokenul pentru verificarea accesului
				char *tok = strstr(recv_get_req(sockfd, host, ACCESS, token, cookies, "get"), "token");
				// Daca nu avem acces, se afiseaza mesajul corespunzator
				if (verify_tok(tok) == 0)
					printf("NO ACCESS IN LIBRARY!\n");
				// Altfel, se retine tokenul
				else
				{
					tok += 8;
					memset(token, 0, BUFLEN);
					strcpy(token, tok);
					token[strlen(token) - 2] = '\0';
					in_library = 1;
					success_library(sockfd);
				}
			}
			else if (!connected)
				printf("You are not logged in.\n");
			else
				printf("You are already in library.\n");
			close_connection(sockfd);
		}
		else if (strncmp(citeste, "get_books", 9) == 0)
		{
			if (in_library == 1)
			{
				// Daca suntem in librarie, se cauta cartile retinute
				char *res = recv_get_req(sockfd, host, BOOKS, token, cookies, "get");
				print_res(res);
			}
			else
				printf("You are not in library.\n");
		}
		else if (strncmp(citeste, "get_book", 8) == 0)
		{
			if (in_library == 1)
			{
				// Daca suntem in librarie, se preia id-ul
				char route[BUFLEN];
				int book_id = 0;

				printf("id=");
				book_id = user_id();
				if (book_id <= 0)
				{
					printf("WRONG FORMAT! Try again!\n");
					close_connection(sockfd);
					continue;
				}

				sprintf(route, "%s/%d", BOOKS, book_id);
				// Se cauta cartile retinute cartile retinute
				char *res = recv_get_req(sockfd, host, route, token, cookies, "get");

				get_book_func(res);
				close_connection(sockfd);
			}
			else
				printf("You are not in library.\n");
		}
		else if (strncmp(citeste, "add_book", 8) == 0)
		{
			if (in_library == 1)
			{
				// Daca suntem in librarie, se preiau informatiile despre carte

				char title[BUFLEN], author[BUFLEN], genre[BUFLEN], publisher[BUFLEN];
				char *pg = calloc(BUFLEN, sizeof(char));
				int pages;

				printf("title =");
				scanf("%[^\n]", title);
				scanf("%*c");
				printf("author =");
				scanf("%[^\n]", author);
				scanf("%*c");
				printf("genre =");
				scanf("%s", genre);
				scanf("%*c");
				printf("publisher =");
				scanf("%[^\n]", publisher);
				scanf("%*c");
				printf("page_count =");
				scanf("%s", pg);
				scanf("%*c");

				pages = atoi(pg);
				if (pages <= 0)
				{
					printf("WRONG FORMAT! Try again!\n");
					continue;
				}
				// Se construieste payload si se trimite request

				JSON_Value *value = json_value_init_object();
				JSON_Object *object = json_value_get_object(value);
				JSON_book(object, title, author, genre, pg, publisher, addbook);

				addbook[0] = json_serialize_to_string(value);

				recv_post_req(sockfd, host, BOOKS, addbook, token);
				printf("Success!\n");
				close_connection(sockfd);
			}
			else
				printf("You are not in library.\n");
			close_connection(sockfd);
		}
		else if (strncmp(citeste, "delete_book", 11) == 0)
		{
			if (in_library == 1)
			{
				// Daca suntem in librarie, se preia id-ul

				char *route = calloc(BUFLEN, sizeof(char));
				int book_id = 0;
				book_id = user_id();
				if (book_id <= 0)
				{
					printf("WRONG FORMAT! Try again!\n");
					continue;
				}
				route = get_route(route, book_id);

				// Se sterge cartea cu id-ul dat(daca exista)
				char *deleted = strstr(recv_get_req(sockfd, host, route, token, cookies, "delete"), "No book was deleted!");
				verify_deleted(deleted);
			}
			else
				error("You are not in library.\n");
			close_connection(sockfd);
		}

		close_connection(sockfd);
	}
	return 0;
}
