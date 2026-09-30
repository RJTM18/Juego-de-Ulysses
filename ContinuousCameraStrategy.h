#include "CameraBehavior.h" 

class ContinuousCameraStrategy : public ICameraBehavior {
private:
	//variable
	int centroPantalla;
	//constantes
	int anchoPantalla; 
	int anchoMundo; 
	
public:
	ContinuousCameraStrategy(int anchoPantalla, int anchoMundo); //no se requiere introducir todos los atributos en private; solo los necesarios
	void updateCamera(int playerX, int playerY, int& camX, int& camY) override: //variables
};

//Declaracion de variables internas