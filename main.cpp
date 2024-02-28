#include "AppWin.h"
#include <iostream>
#include "EngineStyle.h"
#include "MainWindow.h"
#include "CMEventSystem.h"
#include "CMEvents.h"
#include "CMMath.h"

using namespace std;

int main()
{
	CMVec2 v(2, 1);
	CMVec2 v2(1, -2);
	CMVec3 v3(1, 1, 1);
	CMVec3 v4(1, 1, 1);

	cout << "Vettore ruotato: " << v3.rotate(v4) << endl;

	//cout << CMVec2::dot(v2, v) << endl;
	//cout << "Prodotto per scalare: " << v3 * 3 << endl;
	//cout << "Prodotto scalare: " << v3 * v4 << endl;
	//
	//cout << CMVec3::cross(v3, v4) << endl;
	//
	//cout << v3.normalize() << endl;

	//CMQuat q(3, 2, 1, 1);

	

	MainWin* app = new MainWin();
	Style s;

	s.setStyle(app->getItem());

	app->broadcast();

	return 0;
}