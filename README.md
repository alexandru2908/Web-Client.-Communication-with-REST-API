# Web-Client.-Communication-with-REST-API

- "register" - am citit numele si parola, am creat un json cu acestea si l-am trimis la server (POST).
Daca am primit raspunsul "200 OK", am afisat mesajul de succes, iar in caz contrar, am afisat eroarea.
- "login" - am citit numele si parola, parte pe care deja o fac daca se trimite comanda "register", creez json-ul si apoi trimit
la server . Daca cookie-ul primit este diferit de NULL, inseamna ca am primit un cookie si il salvez. Dupa ce il salvez, voi verifica iar daca e diferit de NULL si daca da, afisez mesajul de succes. Daca cookie-ul este NULL din prima, inseamna ca am primit datele incorecte si afisez eroarea. De asemenea, cand un user se logheaza, il voi retine ca si "connected" si ca acesta nu este in nicio librarie.
- "logout" - daca userul este logat, voi trimite la server un mesaj de tip GET cu cookie-ul si voi primi raspunsul. In caz contrar, voi afisa eroarea ca acesta nu este conectat pentru a putea fi decconectat.
- "enter_library" - daca userul este logat si nu este deja in librarie, voi trimite la server un mesaj de tip GET cu cookie-ul si voi primi raspunsul. In caz contrar, voi afisa eroarea ca acesta nu este conectat pentru a putea intra in librarie.
Daca am primit raspunsul afirmativ, voi retine ca userul este in librarie si voi salva token-ul primit.
Daca user-ul nu este logat, voi afisa ca acesta nu este conectat.
Iar daca acesta este deja in librarie(conectat si paramatrul in_library este 1), voi afisa ca deja este in librarie.
- "add_book" - daca userul este in librarie, voi citi de la tastatura informatiile despre carte, le vom srializa si le voi salva in addbook. Dupa ce am citit, voi crea un json cu acestea si le voi trimite la server. In cazul in cafre user-ul nu este in librarie, voi afisa eroarea.
- "get_books" - daca userul este in librarie, voi trimite la server un mesaj de tip GET cu cookie-ul si voi primi raspunsul. In caz contrar, voi afisa eroarea ca acesta nu este conectat pentru a putea vedea cartile.
- "get_book" - daca userul este in librarie, voi citi de la tastatura id-ul cartii pe care vreau sa o vad, verifica daca id-ul este cumva negatic, car in care nu se poate si il voi trimite la server, adaugandu-l la ruta corecta si voi afisa cartea. In cazul in care user-ul nu este in librarie, voi afisa eroarea.
- "delete_book" - daca userul este in librarie, voi citi de la tastatura id-ul cartii pe care vreau sa o sterg, verifica daca id-ul este cumva negativ, car in care nu se poate si il voi trimite la server, adaugandu-l la ruta corecta si voi primi raspunsul. In cazul in care user-ul nu este in librarie, voi afisa eroarea.
