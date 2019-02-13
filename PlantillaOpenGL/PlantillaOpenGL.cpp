#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GLEW_STATIC

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <math.h>

using namespace std;

float xTriangulo = 0.0;
float yTriangulo = 0.0;
float anguloTriangulo = 0.0;
float velocidadAngular = 100;

float velocidad = 1.2;
double tiempoAnterior = 0.0;
double tiempoActual = 0.0;
double tiempoDiferencial = 0.0;

double xCuadrado = 0.6;
double yCuadrado = 0.0;

double rTriangulo = 1;
double gTriangulo = 1;
double bTriangulo = 1;


//Declaramos
GLFWwindow * window;

void dibujarTriangulo() {
	glPushMatrix();
	//Transformaciones
	glTranslatef(xTriangulo, yTriangulo, 0);
	glRotatef(anguloTriangulo, 0, 0, 1);
	//Triangulo principal-
	glBegin(GL_TRIANGLES);
	glColor3f(rTriangulo, gTriangulo, bTriangulo);
	glVertex3f(0, .08, 0);
	glVertex3f(-.05, -.08, 0);
	glVertex3f(.05, -.08, 0);
	glEnd();
	glPopMatrix();
}

void dibujarCuadrado() {
	glPushMatrix();
	//Transformaciones
	glTranslatef(xCuadrado, yCuadrado, 0);
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glVertex2f(-0.1, 0.4);
	glVertex2f(-0.1, -0.4);
	glVertex2f(0.1, -0.4);
	glVertex2f(0.1, 0.4);
	glEnd();
	glPopMatrix();
}

void moverTriangulo() {
	int estadoTeclaDerecha = glfwGetKey(window, GLFW_KEY_RIGHT);
	if (estadoTeclaDerecha == GLFW_PRESS) {
		anguloTriangulo -= velocidadAngular * tiempoDiferencial;
		if (anguloTriangulo > 360) {
			anguloTriangulo -= 360;
		}
	}

	int estadoTeclaIzquierda = glfwGetKey(window, GLFW_KEY_LEFT);
	if (estadoTeclaIzquierda == GLFW_PRESS) {
		anguloTriangulo += velocidadAngular * tiempoDiferencial;
		if (anguloTriangulo < 0) {
			anguloTriangulo += 360;
		}
	}

	int estadoTeclaArriba = glfwGetKey(window, GLFW_KEY_UP);
	if (estadoTeclaArriba == GLFW_PRESS) {
		xTriangulo += cos(anguloTriangulo) * velocidad * tiempoDiferencial;
		yTriangulo += sin(anguloTriangulo) * velocidad * tiempoDiferencial;
	}


	/*
	//Mover el triangulo con las teclas
	int estadoTeclaDerecha = glfwGetKey(window, GLFW_KEY_RIGHT);
	if (estadoTeclaDerecha == GLFW_PRESS) {
		xTriangulo += velocidad;
	}

	int estadoTeclaIzquierda = glfwGetKey(window, GLFW_KEY_LEFT);
	if (estadoTeclaIzquierda == GLFW_PRESS) {
		xTriangulo -= velocidad;
	}

	int estadoTeclaArriba = glfwGetKey(window, GLFW_KEY_UP);
	if (estadoTeclaArriba == GLFW_PRESS) {
		yTriangulo += velocidad;
	}
	int estadoTeclaAbajo = glfwGetKey(window, GLFW_KEY_DOWN);
	if (estadoTeclaAbajo == GLFW_PRESS) {
		yTriangulo -= velocidad;
	}*/
}

void dibujar() {

	dibujarTriangulo();
	dibujarCuadrado();
}

void checarColisiones() {
	//Ancho, alto y coordenadas de X y Y
	//Se le suma el .1 porque es la mitad del cuadrado, asi es la orilla del cuadrado
	if (xTriangulo - 0.2 < xCuadrado + 0.1 && 
		xTriangulo + 0.2 > xCuadrado - 0.1 && 
		yTriangulo - 0.2 < yCuadrado + 0.1 &&
		yTriangulo + 0.2 > yCuadrado - 0.1) {
		
		gTriangulo = 0;
		bTriangulo = 0;

		xCuadrado = (rand() % 2) - 0.8;
		yCuadrado = (rand() % 2) - 0.8;
	}

	else
	{
		gTriangulo = 1;
		bTriangulo = 1;
	}

	/*if (abs(xTriangulo - xCuadrado) + abs(yTriangulo - yCuadrado) < 0.015) {
		cout << "Colision" << endl;
	}*/
}

void actualizar(){
	tiempoActual = glfwGetTime();
	tiempoDiferencial = tiempoActual - tiempoAnterior;

	moverTriangulo();
	checarColisiones();
	tiempoAnterior = tiempoActual;

	if (xTriangulo > 0.8) {
		xTriangulo = 0.8;
	}

	if (xTriangulo < -0.8) {
		xTriangulo = -0.8;
	}

	if (yTriangulo > 0.8) {
		yTriangulo = 0.8;
	}

	if (yTriangulo < -0.8) {
		yTriangulo = -0.8;
	}

}



void teclado_callback(GLFWwindow * window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS )) {
		xTriangulo += velocidad * tiempoDiferencial;
	}

	if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
		xTriangulo -= velocidad * tiempoDiferencial;
	}

	if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
		yTriangulo += velocidad * tiempoDiferencial;
	}

	if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
		yTriangulo -= velocidad * tiempoDiferencial;
	}

}

int main()
{

	// Si no se pudo iniciar GLFW terminamos la ejecucion
	if (!glfwInit()) { exit(EXIT_FAILURE); } // Si se pudo iniciar GLFW entonces inicializamos la ventana
	
	window = glfwCreateWindow(1024, 768, "Ventana", NULL, NULL);

	//SI no pudimos iniciar la ventana, entonces terminamos la ejecucion
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	//Establecemos el contexto
	glfwMakeContextCurrent(window);

	//Una vez establecido el contexto, activamos funciones modernas
	glewExperimental = true;
	GLenum errorGlew = glewInit();
	if (errorGlew != GLEW_OK) {
		cout << glewGetErrorString(errorGlew);
	}

	const GLubyte *versionGL = glGetString(GL_VERSION);
	cout << "Version OpenGL: " << versionGL;
	cout << "\n Hecho por Javier Eliud \n Matricula: 169171";

	//Para reconocer las teclas
	/*glfwSetKeyCallback(window, teclado_callback);*/
	
	tiempoAnterior = glfwGetTime();


	//ciclo de dibujo (Draw loop)
	while (!glfwWindowShouldClose(window)) {
		//Establecer region de dibujo
		glViewport(0, 0, 1020, 768);
		//Establece el color de borrado
		glClearColor(0, 0, 0, 0);
		//Clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Rutina de dibujo
		actualizar();
		dibujar();
		
		//Cambiar los buffer
		glfwSwapBuffers(window);
		//Reconocer si hay entradas
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}