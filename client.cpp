#include <iostream>
#include <string>
#include <sys/socket.h>
#include <vector>
#include <string.h>

#include "helpers.h"
#include "requests.h"

// functie care extrage cookie-ul gasind expresia "Set-Cookie:"
string get_cookie(const char *response) {
    string header(response);
    string cookie_label = "Set-Cookie: ";
    size_t start_pos = header.find(cookie_label); // gasesc pozitia etichetei

    start_pos += cookie_label.length();
    size_t end_pos = header.find(';', start_pos); // gasesc sfarsitul cookie-ului

    return header.substr(start_pos, end_pos - start_pos); // returnez cookie-ul
}

// functie care verifica daca un string contine spatii
bool contains_space(const string& str) {
    int ok = 1;
    for (char c : str) {
        if (c == ' ') { // daca gasesc un spatiu
            ok = 0;
        }
    }
    if (ok == 1) {
        return true;
    } else {
        return false;
    }
}

// functie care extrage codul dintr-un string
string get_code(const string& str) {
    size_t first = str.find(' '); // gasesc primul spatiu
    size_t second = str.find(' ', first + 1); // gasesc al doilea spatiu
    return str.substr(first + 1, second - first - 1); // returnez codul dintre spatii
}

// functie care extrage jwt din raspuns
string get_jwt(const char *response) {
    const char *token_label = "token\":\"";
    const char *token_start = strstr(response, token_label); // gasesc inceputul token-ului

    token_start += strlen(token_label);
    const char *token_end = strchr(token_start, '\"'); // gasesc sfarsitul token-ului

    return string(token_start, token_end - token_start); // returnez token-ul
}

// functie care concateneaza doua siruri
char* concat(const char* str1, const char* str2) {
    // aloca memorie pentru noul sir
    char* result = new char[strlen(str1) + strlen(str2) + 1];
    
    // copiez primul sir in rezultatul final
    strcpy(result, str1);
    
    // adaug al doilea sir la rezultatul final
    strcat(result, str2);
    
    return result;
}

// functie care verifica daca un string contine doar cifre
bool is_number(string& str) {
    int ok = 1;
    for (char c : str) {
        if (c < '0' || c > '9') { // verific daca caracterul nu e cifra
            ok = 0;
        }
    }
    if (ok == 1) {
        return true;
    } else {
        return false;
    }
}

int main() {
    // initializez variabilele pentru ip-ul serverului si portul
    char *ip = (char *)"34.246.184.49";
    int port = 8080;
    string jwt, cookie; // string-uri pentru jwt si cookie
    char *message;
    char *response;
    int sockfd;

    while (1) {
        string command;
        getline(cin, command);

        if (command == "register") {
            string username, password;
            cout << "username=";
            getline(cin, username);
            cout << "password=";
            getline(cin, password);

            if (!contains_space(username) || !contains_space(password)) { // verific sa nu existe spatii
                cout << "nu avem voie spatii: eroare.\n";
                continue;
            }

            // deschid conexiunea cu serverul
            sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);

            // creez json-ul cu username-ul si parola
            nlohmann::json json = {
                {"username", username},
                {"password", password}
            };

            // construiesc mesajul de post request
            message = compute_post_request(ip, (char *)"/api/v1/tema/auth/register", (char *)"application/json", &json, "");

            send_to_server(sockfd, message); // trimit mesajul la server
            response = receive_from_server(sockfd); // primesc raspunsul de la server

            string number = get_code(response); // extrag codul din raspuns

            // daca am codul 201 s-a efectuat cu succes, altfel eroare
            if (stoi(number) == 201) {
                cout << "utilizator inregistrat cu succes!\n";
            } else {
                cout << "utilizatorul exista deja: eroare.\n";
            }

            close_connection(sockfd); // inchid conexiunea
        } else if (command == "login") {
            string username, password;
            cout << "username=";
            getline(cin, username);
            cout << "password=";
            getline(cin, password);

            if (!contains_space(username) || !contains_space(password)) { // verific sa nu existe spatii
                cout << "nu avem voie spatii: eroare.\n";
                continue;
            }

            // deschid conexiunea cu serverul
            sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);

            // creez json-ul cu username-ul si parola
            nlohmann::json json = {
                {"username", username},
                {"password", password}
            };

            // construiesc mesajul de post request
            message = compute_post_request(ip, (char *)"/api/v1/tema/auth/login", (char *)"application/json", &json, "");

            send_to_server(sockfd, message); // trimit mesajul la server
            response = receive_from_server(sockfd); // primesc raspunsul de la server

            string number = get_code(response); // extrag codul din raspuns

            // daca am codul 200 s-a efectuat cu succes, altfel eroare
            if (stoi(number) == 200) {
                cout << "utilizator conectat cu succes!\n";
            } else {
                cout << "utilizatorul nu s-a conectat: eroare.\n";
            }

            cookie = get_cookie(response); // extrag cookie-ul din raspuns
            close_connection(sockfd); // inchid conexiunea

        } else if (command == "enter_library") {
            if (cookie.empty()) {
                cout << "eroare - utilizatorul nu este logat: eroare.\n";
                continue;
            }
            // deschid conexiunea cu serverul
            sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);

            vector<string> cookies; // vector pentru cookies
            cookies.push_back(cookie); // adauga cookie-ul in vector
            message = compute_get_request(ip, (char *)"/api/v1/tema/library/access", cookies, ""); // construiesc mesajul de get request

            send_to_server(sockfd, message); // trimit mesajul la server
            response = receive_from_server(sockfd); // primesc raspunsul de la server

            string number = get_code(response); // extrag codul din raspuns

            // daca am codul 200 s-a efectuat cu succes, altfel eroare
            if (stoi(number) == 200) {
                cout << "succes - utilizatorul are acces la biblioteca: succes!\n";
            } else {
                cout << "utilizatorul nu are acces: eroare.\n";
            }
            jwt = get_jwt(response); // extrag token-ul din raspuns
            close_connection(sockfd); // inchid conexiunea
        } else if (command == "get_books") {
            vector<string> cookies;
            if (jwt.empty()) {
                cout << "utilizatorul nu are acces: eroare.\n";
                continue;
            }

            // deschid conexiunea cu serverul
            sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);

            // construiesc mesajul de get request
            message = compute_get_request(ip, (char *)"/api/v1/tema/library/books", cookies, jwt);

            send_to_server(sockfd, message); // trimit mesajul la server
            response = receive_from_server(sockfd); // primesc raspunsul de la server

            cout << response << endl;
            close_connection(sockfd); // inchid conexiunea
        } else if (command == "add_book") {
            if (jwt.empty()) {
                cout << "utilizatorul nu are acces: eroare.\n";
                continue;
            }

            cout << "title=";
            string title;
            getline(cin, title);
            
            string author;
            cout << "author=";
            getline(cin, author);
            
            string genre;
            cout << "genre=";
            getline(cin, genre);
            
            string publisher;
            cout << "publisher=";
            getline(cin, publisher);
            
            string page_count;
            cout << "page_count=";
            getline(cin, page_count);

            if (!is_number(page_count) || page_count == "") { // verific daca numarul de pagini este chiar numar
                cout << "nu este corect inputul: eroare.\n";
                continue;
            }

            // deschid conexiunea cu serverul
            sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);

            int num = stoi(page_count); // transform din string in int

            // creez json-ul cu caracteristicile cartii
            nlohmann::json json = {
                {"title", title},
                {"author", author},
                {"genre", genre},
                {"publisher", publisher},
                {"page_count", num}
            };

            // construiesc mesajul de post request
            message = compute_post_request(ip, (char *)"/api/v1/tema/library/books", (char *)"application/json", &json, jwt);

            send_to_server(sockfd, message); // trimit mesajul la server
            response = receive_from_server(sockfd); // primesc raspunsul de la server

            string number = get_code(response); // extrag codul din raspuns

            // daca am codul 200 s-a efectuat cu succes, altfel eroare
            if (stoi(number) == 200) {
                cout << "succes - s-a adaugat cartea: succes!\n";
            } else {
                cout << "nu s-a putut adauga: eroare.\n";
            }

            close_connection(sockfd); // inchid conexiunea
        } else if (command == "get_book") {
            vector<string> cookies;

            if (jwt.empty()) {
                cout << "utilizatorul nu are acces: eroare.\n";
                continue;
            }

            cout << "id=";
            string id;
            getline(cin, id);

            if (!is_number(id)) { // verific daca id-ul este un numar
                cout << "id-ul trebuie sa fie numar: eroare.\n";
                continue;
            }

            // deschid conexiunea cu serverul
            sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);

            // construiesc url-ul pentru request adaugand id-ul
            char *url = concat("/api/v1/tema/library/books/", id.c_str());
            // construiesc mesajul de get request
            message = compute_get_request(ip, url, cookies, jwt);

            send_to_server(sockfd, message); // trimit mesajul la server
            response = receive_from_server(sockfd); // primesc raspunsul de la server

            cout << response << endl;
            close_connection(sockfd); // inchid conexiunea
        } else if (command == "delete_book") { // verific daca comanda este "delete_book"

            if (jwt.empty()) {
                cout << "utilizatorul nu are acces: eroare.\n";
                continue;
            }

            cout << "id=";
            string id;
            getline(cin, id); // citeste id-ul cartii

            if (!is_number(id)) { // verific daca id-ul este un numar
                cout << "id-ul nu e numar: eroare.\n";
                continue;
            }

            // deschid conexiunea cu serverul
            sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);

            // construiesc url-ul pentru request
            char *url = concat("/api/v1/tema/library/books/", id.c_str());
            // construiesc mesajul de delete request
            message = compute_delete_request(ip, url, jwt);

            send_to_server(sockfd, message); // trimit mesajul la server
            response = receive_from_server(sockfd); // primesc raspunsul de la server

            string number = get_code(response); // extrag codul din raspuns

            // daca am codul 200 s-a efectuat cu succes, altfel eroare
            if (stoi(number) == 200) {
                cout << "succes - s-a sters cartea: succes!\n";
            } else {
                cout << "nu s-a putut sterge: eroare.\n";
            }

            close_connection(sockfd); // inchid conexiunea
        } else if (command == "logout") {
            vector<string> cookies;
            if (cookie.empty()) {
                cout << "eroare - utilizatorul nu este logat: eroare.\n";
                continue;
            }
            cookies.push_back(cookie);

            // deschid conexiunea cu serverul
            sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);

            // construiesc mesajul de get request
            message = compute_get_request(ip, (char *)"/api/v1/tema/auth/logout", cookies, "");
            send_to_server(sockfd, message); // trimit mesajul la server

            response = receive_from_server(sockfd); // primesc raspunsul de la server

            string number = get_code(response); // extrag codul din raspuns

            // daca am codul 200 s-a efectuat cu succes, altfel eroare
            if (stoi(number) == 200) {
                cout << "succes - utilizatorul s-a deconectat cu succes!\n";
            } else {
                cout << "nu s-a putut deconecta: eroare.\n";
            }

            // resetez cookie-ul si token-ul
            cookie = "";
            jwt = "";
            close_connection(sockfd); // inchid conexiunea

        } else if (command == "exit") {
            break;
        }
    }

    return 0;
}


