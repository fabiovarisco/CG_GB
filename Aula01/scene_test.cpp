
#include "Scene.h"
#include "CurveDesigner.h"
#include <string>>
#include <vector>
#include <iostream>
using namespace std;

void openScene(string filename) {
	Scene* scene = new Scene();
	scene->init(".\\", filename + ".obj", filename + ".curve");
	scene->draw();
}

int main(){

	cout << "Selecione a opção desejada: \n";
	cout << "1 - Desenhar pista\n";
	cout << "2 - Ler arquivo OBJ\n";

	int opcao = 1;
	cin >> opcao;

	if (opcao == 1) {
		cout << "\n\nNome do arquivo para exportar a curva: ";
		string filename;
		cin >> filename;

		CurveDesigner* curveDesigner = new CurveDesigner(filename);
		curveDesigner->init();
		curveDesigner->loop();

		openScene(filename);
	}
	else if (opcao == 2) {
		cout << "\n\nNome do arquivo para ler o OBJ: ";
		string filename;
		cin >> filename;

		openScene(filename);
		/*Scene* scene = new Scene();

		scene->init("C:\\Users\\I844141\\Documents\\Unisinos\\2018-2\\computacao_grafica\\obj_files\\paintball\\", "cenaPaintball.obj");
		//scene->init("C:\\Users\\I844141\\Documents\\Unisinos\\2018-2\\computacao_grafica\\obj_files\\mesa\\", "mesa01.obj");
		//scene->init("C:\\Users\\I844141\\Documents\\Unisinos\\2018-2\\computacao_grafica\\obj_files\\pokemon\\", "Dragonite.obj");

		scene->draw();*/
	}

	return 0;
	
}

