#include "Engine/LogSystem/LogSystem.h"
#include "GUI/CMWindow.h"
#include "GUI/CMSlider.h"

int main()
{
	//CMVec2 v(2, 1);
	//CMVec2 v2(1, -2);
	//CMVec3 v3(1, 0, 1);
	//CMVec3 v4(0, 1, 0);
	//
	//v3 = v3.rotate(180, 0, 0);
	//cout << "Vettore ruotato: " << v3 << endl;
	
	//cout << CMVec2::dot(v2, v) << endl;
	//cout << "Prodotto per scalare: " << v3 * 3 << endl;
	//cout << "Prodotto scalare: " << v3 * v4 << endl;
	//	
	//cout << CMVec3::cross(v3, v4) << endl;
	//
	//cout << v3.normalize() << endl;

	//CMQuat q(3, 2, 1, 1);

	CMLog::Debug("Debug");
	CMLog::Warning("Warning");
	CMLog::Error("Error");
	CMLog::Succeed("Succeed");

	cout << endl;

	//MainWin* app = new MainWin();
	//Style s;
	//
	//s.setStyle(app->getItem());

	//app->broadcast();

	CMWindow w;

    CMSlider s1(w);
    s1.show();

    w.run();

	return 0;
}