#include <iostream>
using namespace std;
#include "babel_headers.h"

int main()
{
#if PLATFORM == 1
	system(" title \"Babel-Bubel\" Dorian Janiak (C) 2014 Wersja 0.2");
#endif
	Interface::Intoduce(cout);
	cout << "\n\nJakich rozmiarow plansze chcesz stworzyc: \n(podaj szerokosc i wysokosc po czym nacisnij ENTER)\n" <<
		"(przyklad: 18 14 ENTER stworzy mape o szerokosci 18, a wysokosci 14)\n(zalecam rozmiar 14 14)\n";
	int width, height;
	cin >> width >> height;
	Interface::refreshConsole(cout);
	Game game_obj(width,height);
	cin.ignore();
#if PLATFORM == 1
	getchar();
#endif
	return 0;
}