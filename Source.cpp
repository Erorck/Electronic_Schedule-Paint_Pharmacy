
#include "resource.h" 
#include"LinkedList.h"
#include "TreeMed.h"
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <commctrl.h>

using namespace std;

#define TM_RELOJ 3000

#pragma region VARIABLES GLOBALES
char chDirLogo[MAX_PATH] = "Bitmaps\\Logo consultorio.bmp";

HWND hLblReloj;

//PUNTEROS HANDLER A VENTANAS
HWND hVentanaLogin;
HWND hVentanaMenu;
HWND hVentanaTrasMed;
HWND hVentanaRegistroMed;
HWND hVentanaRegistroPac;
HWND hVentanaRegistroCita;
HWND hVentanaRegistroEsp;
HWND hVentanaConsulta;
HWND hVentanaSalir, hVentanaCancelar;

#pragma region HANDLERS A COMPLEMENTOS
	//Handlers Login
		HWND hEcUsuario, hEcPassword;

	//Handlers Menu
		HWND hLogoFarmaciaPc;
		HBITMAP hLogoFarmacia;

	//Handlers Consulta
		HWND hLbConsulta;
		HWND hCbCEsp;
		HWND hEcCSemana1, hEcCSemana2, hEcCMesMed, hEcCMesEsp, hEcCCedula, hEcCMot, hEcCId;
		HWND hCFotoDocPc;
		HBITMAP hCFotoDoc;

	//Handlers Registro de medico
		HWND hEcReMCedula, hEcReMNombre, hEcReMHora1, hEcReMMin1, hEcReMMin2, hEcReMHora2, hEcReMTel;
		HWND hReMFotoDocPc;
		HWND hLbReMLista;
		HWND hCbReMEsp, hCbReMConsul;
		HBITMAP hReMFotoDoc;

	//Handlers Registro de paciente
		HWND hCbRePPMed, hCbRePGenero;
		HWND hEcRePID,hEcRePNom, hEcRePApe, hEcRePTel, hEcRePRef, hEcRePDia, hEcRePMes, hEcRePAnio, hEcRePEdad;
		HWND hLbRePLista;

	//Handlers Registro de cita
		HWND hLbReCLista;
		HWND hCbReCPac, hCbReCEsp, hCbReCMed;
		HWND hDtpReC;
		HWND hReCFotoDocPc;
		HWND hEcReCID, hEcReCTel, hEcReCHora, hEcReCMin;

	//Handlers Registro de especialidad
		HWND hLbReELista;
		HWND hEcReEID, hEcReENom, hEcReEDesc;

#pragma endregion


char _path[MAX_PATH] = "";
char nomUsuario[50], nick[15] = "erickcisho";
char archEsp[] = "ListaEsp.dat";
char archPac[] = "ListaPac.dat";
char archCitas[] = "ListaCitas.dat";
char dirFotoTemp[MAX_PATH] = "";
char dirFotoEd[MAX_PATH] = "";
bool editOrDelEsp = false;
bool editOrDelMed = false;
bool editOrDelPac = false;
bool editOrDelCita = false;
bool repetido = false;
fstream archivo;
ofstream reporte;

//ESTRUCTURA TIEMPO
time_t segundosTotales;
struct tm* tiempoActual;

//ESTRUCTURAS
struct user {
	char nick[30];
	char nombre[50];
	char password[30];
};

user arrUsuarios[5];

Node<NodeEsp>* auxEsp;
LinkedList<NodeEsp> *ListaEsp = new LinkedList<NodeEsp>(archEsp);
Node<NodePac>* auxPac;
LinkedList<NodePac>* ListaPac = new LinkedList<NodePac>(archPac);
Node<NodeCita>* auxCita, *auxCita2, *auxCita3, *auxCita4;
LinkedList<NodeCita>* ListaCita = new LinkedList<NodeCita>(archCitas);

NodeMed* auxM, *auxM2;
TreeMed ArbolMed;

#pragma endregion

//PROTOTIPOS DE FUNCIONES

void cargarID();
bool idCmp(char temp[]);
bool passCmp(char temp[]);
bool textoConLetras(char strTemp[100], int longitud);
bool textoIncorrecto(char strTemp[100], int longitud);
void updateMostrar(NodeMed* node);
void ordenarPacApe();
template <typename U> 
void agregarEspacios(U data,short dig, short digTotal);
void crearArchivoCita(NodeCita temp, short tempId, NodeMed* M);
bool citaPasada(SYSTEMTIME temp, short tempH, short tempM);
void titleCreator(char* textoTemp);
void addNodeInfo(Node<NodeCita>* auxC);
void copyCita(NodeCita* des, NodeCita ori);
void updateCitaMostrar(Node<NodeCita>* aux);

#pragma region FUNCIONES DE VENTANAS

BOOL CALLBACK funcionTrasMed
(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK funcionRegistroMed
(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK funcionRegistroPac
(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK funcionRegistroCita
(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK funcionRegistroEsp
(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK funcionConsulta
(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK funcionLogin
(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK funcionMenu
(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK funcionSalir
(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


#pragma endregion


//WINMAIN
	int WinMain(HINSTANCE hInst,
		HINSTANCE hPrevInstance,
		PSTR cmdLine, int showCmd)
	{

		#pragma region CREACION DE DIALOGOS


		//VENTANA LOGIN
		hVentanaLogin = CreateDialog(
			hInst,	//Instancia a la cual va a pertenecer nuestro dialogo
			MAKEINTRESOURCE(IDD_LOGIN), //Crearemos un recurso basado en el ID que conocemos
			NULL,	//Tiene una ventana padre este dialogo? no? NULL
			funcionLogin //Funcion que va a recibir los mensajes que envie el DispatchMessage
		);

		//VENTANA MENU
		hVentanaMenu = CreateDialog(
			hInst,
			MAKEINTRESOURCE(IDD_CENTRO),
			NULL,
			funcionMenu
		);

		//VENTANA REGISTRO CITAS
		hVentanaRegistroCita = CreateDialog(
			hInst,
			MAKEINTRESOURCE(IDD_REGISTRO_CITA),
			NULL,
			funcionRegistroCita
		);

		//VENTANA REGISTRO PACIENTE
		hVentanaRegistroPac = CreateDialog(
			hInst,
			MAKEINTRESOURCE(IDD_REGISTRO_PAC),
			NULL,
			funcionRegistroPac
		);

		//VENTANA TRASPAPELE MEDICO
		hVentanaTrasMed = CreateDialog(
			hInst,
			MAKEINTRESOURCE(IDD_TRAS_MED),
			NULL,
			funcionTrasMed
		);

		//VENTANA REGISTRO MEDICO
		hVentanaRegistroMed = CreateDialog(
			hInst,
			MAKEINTRESOURCE(IDD_REGISTRO_MED),
			NULL,
			funcionRegistroMed
		);

		//VENTANA CONSULTA CITAS
		hVentanaConsulta = CreateDialog(
			hInst,
			MAKEINTRESOURCE(IDD_CONSULTA),
			NULL,
			funcionConsulta
		);


		//VENTANA REGISTRO ESPECIALIDADES
		hVentanaRegistroEsp = CreateDialog(
			hInst,
			MAKEINTRESOURCE(IDD_REGISTRO_ESP),
			NULL,
			funcionRegistroEsp
		);

		
		//VENTANA SALIR
		hVentanaSalir = CreateDialog(
			hInst,
			MAKEINTRESOURCE(IDD_SALIR),
			NULL,
			funcionSalir
		);

		//TIMER
		SetTimer(
			hVentanaRegistroCita,
			TM_RELOJ,
			1000,
			NULL
		);
		#pragma endregion


		#pragma region LIMPIEZA DE MSG


		MSG msg; //Variable que almacenara los mensajes que reciban las ventanas.
		ZeroMemory(&msg, sizeof(MSG)); //Limpiamos la variable
		ShowWindow(hVentanaLogin, SW_SHOW); //Mostramos el dialogo

		MSG msgMenu; 
		ZeroMemory(&msgMenu, sizeof(MSG)); 
		ShowWindow(hVentanaMenu, SW_HIDE); 

		MSG msgConsulta;
		ZeroMemory(&msgConsulta, sizeof(MSG));
		ShowWindow(hVentanaConsulta, SW_HIDE);

		MSG msgTrasMed;
		ZeroMemory(&msgTrasMed, sizeof(MSG));
		ShowWindow(hVentanaTrasMed, SW_HIDE);

		MSG msgRegistroMed;
		ZeroMemory(&msgRegistroMed, sizeof(MSG));
		ShowWindow(hVentanaRegistroMed, SW_HIDE);

		MSG msgRegistroPac;
		ZeroMemory(&msgRegistroPac, sizeof(MSG));
		ShowWindow(hVentanaRegistroPac, SW_HIDE);

		MSG msgRegistroCita;
		ZeroMemory(&msgRegistroCita, sizeof(MSG));
		ShowWindow(hVentanaRegistroCita, SW_HIDE);

		MSG msgRegistroEsp;
		ZeroMemory(&msgRegistroEsp, sizeof(MSG));
		ShowWindow(hVentanaRegistroEsp, SW_HIDE);

		MSG msgSalir;
		ZeroMemory(&msgSalir, sizeof(MSG));
		ShowWindow(hVentanaSalir, SW_HIDE);

		#pragma endregion


		#pragma region CICLO DE VENTANAS

		//CICLO VENTANA LOGIN
		while (GetMessage(&msg, 0, 0, 0)) //Siempre devolvera TRUE hasta que reciba un WM_QUIT
		{
			TranslateMessage(&msg); //Traducimos el mensaje
			DispatchMessage(&msg); //Enviamos el mensaje a quien corresponde
		}
		return msg.wParam; //Recuperamos el valor que se envio en el PostQuitMessage()

		//CICLO VENTANA MENU
		while (GetMessage(&msgMenu, 0, 0, 0)) 
		{
			TranslateMessage(&msgMenu); 
			DispatchMessage(&msgMenu); 
		}
		return msgMenu.wParam; 

		// CICLO VENTANA CONSULTA CITAS
			while (GetMessage(&msgConsulta, 0, 0, 0))
			{
				TranslateMessage(&msgConsulta);
				DispatchMessage(&msgConsulta);
			}
		return msgConsulta.wParam;

		//CICLO VENTANA REGISTRO MEDICO
		while (GetMessage(&msgTrasMed, 0, 0, 0))
		{
			TranslateMessage(&msgTrasMed);
			DispatchMessage(&msgTrasMed);
		}
		return msgTrasMed.wParam;

		//CICLO VENTANA REGISTRO MEDICO
		while (GetMessage(&msgRegistroMed, 0, 0, 0))
		{
			TranslateMessage(&msgRegistroMed);
			DispatchMessage(&msgRegistroMed);
		}
		return msgRegistroMed.wParam;

		//CICLO VENTANA REGISTRO PACIENTE
		while (GetMessage(&msgRegistroPac, 0, 0, 0))
		{
			TranslateMessage(&msgRegistroPac);
			DispatchMessage(&msgRegistroPac);
		}
		return msgRegistroPac.wParam;

		//CICLO VENTANA REGISTRO CITA
		while (GetMessage(&msgRegistroCita, 0, 0, 0))
		{
			TranslateMessage(&msgRegistroCita);
			DispatchMessage(&msgRegistroCita);
		}
		return msgRegistroCita.wParam;

		//CICLO VENTANA REGISTRO ESPECIALIDAD
		while (GetMessage(&msgRegistroEsp, 0, 0, 0))
		{
			TranslateMessage(&msgRegistroEsp);
			DispatchMessage(&msgRegistroEsp);
		}
		return msgRegistroEsp.wParam;

		//CICLO VENTANA SALIR
		while (GetMessage(&msgSalir, 0, 0, 0))
		{
			TranslateMessage(&msgSalir);
			DispatchMessage(&msgSalir);
		}
		return msgSalir.wParam;

		#pragma endregion 

	}

	#pragma region MENSAJES

		//MENSAJE LOGIN
	BOOL CALLBACK funcionLogin
	(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

		switch (msg) {
		case WM_INITDIALOG:
			cargarID();
			hEcUsuario = GetDlgItem(hWnd, IDC_ECUSUARIO);
			SendMessage(hEcUsuario, EM_SETLIMITTEXT, 30, 0);
			hEcPassword = GetDlgItem(hWnd, IDC_ECCONTRA);
			SendMessage(hEcPassword, EM_SETLIMITTEXT, 47, 0);
			break;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDC_BNINGRESAR && HIWORD(wParam) == BN_CLICKED)
			{
				char textoTemp[50], textoTemp2[50];
				int lenght = GetWindowTextLength(hEcUsuario);
				int lenght2 = GetWindowTextLength(hEcPassword);
				GetWindowText(hEcUsuario, textoTemp, lenght + 1);
				GetWindowText(hEcPassword, textoTemp2, lenght2 + 1);
				
				if (lenght == 0 && lenght2 == 0)
				{
					MessageBox(hWnd, "NO HA INGRESADO NINGUN DATO", "", MB_ICONEXCLAMATION);
					break;
				}
				else {
					if (lenght == 0)
					{
						MessageBox(hWnd, "NO HA INGRESADO NINGUN USUARIO", "", MB_ICONEXCLAMATION);
						break;
					}
					else if (lenght2 == 0) {
						MessageBox(hWnd, "NO HA INGRESADO LA CONTRASEÑA", "", MB_ICONEXCLAMATION);
						break;
					}
					else if (idCmp(textoTemp))
					{
						MessageBox(hWnd,"USUARIO INCORRECTO", "", MB_ICONEXCLAMATION);
						break;
					}
					else if (passCmp(textoTemp2))
					{
						MessageBox(hWnd, "CONTRASEÑA INCORRECTA", "", MB_ICONEXCLAMATION);
						break;
					}
				}
				
				SetDlgItemText(hVentanaConsulta, IDC_NOM_USUARIO, nomUsuario);
				SetDlgItemText(hVentanaTrasMed, IDC_NOM_USUARIO, nomUsuario);
				SetDlgItemText(hVentanaRegistroMed, IDC_NOM_USUARIO, nomUsuario);
				SetDlgItemText(hVentanaRegistroPac, IDC_NOM_USUARIO, nomUsuario);
				SetDlgItemText(hVentanaRegistroCita, IDC_NOM_USUARIO, nomUsuario);
				SetDlgItemText(hVentanaRegistroEsp, IDC_NOM_USUARIO, nomUsuario);
				SetDlgItemText(hVentanaMenu, IDC_NOM_USUARIO, nomUsuario);
				SetDlgItemText(hVentanaSalir, IDC_NOM_USUARIO, nomUsuario);

				ShowWindow(hVentanaLogin, SW_HIDE);
				ShowWindow(hVentanaMenu, SW_SHOW);
			}
			break;
		case WM_DESTROY: 
			PostQuitMessage(10);   
			break;
		case WM_CLOSE: 
			//ShowWindow(hWnd, SW_HIDE);
			//ShowWindow(hVentanaSalir, SW_SHOW);
			//DestroyWindow(hWnd);  //Destruimos el dialogo
			break;
		}
		return FALSE; 
	}


	//MENSAJE MENU
	BOOL CALLBACK funcionMenu
	(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

		switch (msg) {
		case WM_INITDIALOG:
			hLogoFarmaciaPc = GetDlgItem(hWnd, IDC_BMP_MENU_LOGO);
			hLogoFarmacia = (HBITMAP)LoadImage(NULL, chDirLogo, IMAGE_BITMAP, 400, 266, LR_LOADFROMFILE);
			SendMessage(hLogoFarmaciaPc, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLogoFarmacia);
			break;

		case WM_COMMAND:
			
			if (LOWORD(wParam) == ID_BM_RP_CONSUL && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaMenu, SW_HIDE);
				ShowWindow(hVentanaConsulta, SW_SHOW);
			}
			if (LOWORD(wParam) == ID_BM_R_MEDICO && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaMenu, SW_HIDE);
				ShowWindow(hVentanaRegistroMed, SW_SHOW);
			}
			if (LOWORD(wParam) == ID_BM_R_PACIENTE && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaMenu, SW_HIDE);
				ShowWindow(hVentanaRegistroPac, SW_SHOW);
			}
			if (LOWORD(wParam) == ID_BM_R_CITA && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaMenu, SW_HIDE);
				ShowWindow(hVentanaRegistroCita, SW_SHOW);
			}
			if (LOWORD(wParam) == ID_BM_R_ESPECIALIDAD && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaMenu, SW_HIDE);
				ShowWindow(hVentanaRegistroEsp, SW_SHOW);
			}

			break;
		case WM_DESTROY: 
			PostQuitMessage(10);   
			break;
		case WM_CLOSE: // X
			hVentanaCancelar = hVentanaMenu;
			ShowWindow(hWnd, SW_HIDE);
			ShowWindow(hVentanaSalir, SW_SHOW);
			//DestroyWindow(hWnd);  
			break;
		}
		return FALSE; 
	}


	//MENSAJE CONSULTA CITAS
	BOOL CALLBACK funcionConsulta
	(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

		switch (msg) {
		case WM_INITDIALOG: {
			hCFotoDocPc = GetDlgItem(hWnd, IDC_BMP_C_FOTODOC);
			hLbConsulta = GetDlgItem(hWnd, IDC_LB_C_CITAS);
			hEcCId = GetDlgItem(hWnd, IDC_EC_C_ID);
			SendMessage(hEcCId, EM_SETLIMITTEXT, 5, 0);
			hEcCSemana1 = GetDlgItem(hWnd, IDC_EC_SEM1);
			SendMessage(hEcCSemana1, EM_SETLIMITTEXT, 2, 0);
			hEcCSemana2 = GetDlgItem(hWnd, IDC_EC_SEM2);
			SendMessage(hEcCSemana2, EM_SETLIMITTEXT, 2, 0);
			hEcCMesMed = GetDlgItem(hWnd, IDC_ECMED_MES);
			SendMessage(hEcCMesMed, EM_SETLIMITTEXT, 2, 0);
			hEcCMesEsp = GetDlgItem(hWnd, IDC_ECSEM_MES);
			SendMessage(hEcCMesEsp, EM_SETLIMITTEXT, 2, 0);
			hEcCCedula = GetDlgItem(hWnd, IDC_ECMED_CEDULAMED);
			SendMessage(hEcCCedula, EM_SETLIMITTEXT, 8, 0);
			hCbCEsp = GetDlgItem(hWnd, IDC_CB_ESP);
			SetDlgItemText(hWnd, IDC_STSEM_ID, " ");
			SetDlgItemText(hWnd, IDC_STSEM_FECHA, " ");
			SetDlgItemText(hWnd, IDC_STSEM_HORA, " ");
			SetDlgItemText(hWnd, IDC_STSEM_NCONSULT, " ");
			SetDlgItemText(hWnd, IDC_STSEM_ESPECIALIDAD, " ");
			SetDlgItemText(hWnd, IDC_STSEM_MEDICO, " ");
			SetDlgItemText(hWnd, IDC_STSEM_PACIENTE, " ");
			SetDlgItemText(hWnd, IDC_STSEM_TELPAC, " ");
			SetDlgItemText(hWnd, IDC_STSEM_ATENDIDA, " ");
			SetDlgItemText(hWnd, IDC_STSEM_MOTIVO, " ");

			EnableWindow(hEcCSemana1, FALSE);
			EnableWindow(hEcCSemana2, FALSE);
			EnableWindow(hEcCMesEsp, FALSE);
			EnableWindow(hCbCEsp, FALSE);
			EnableWindow(hEcCCedula, FALSE);
			EnableWindow(hEcCMesMed, FALSE);
			EnableWindow(GetDlgItem(hWnd, IDC_BN_CANCELAR), FALSE);
			
			hEcCMot = GetDlgItem(hWnd, IDC_EC_C_MC);
			SendMessage(hEcCMot, EM_SETLIMITTEXT, 49, 0);
			ShowWindow(hEcCMot, SW_HIDE);
			ShowWindow(GetDlgItem(hWnd, IDC_BN_GUARDAR), SW_HIDE);
			ShowWindow(GetDlgItem(hWnd, IDC_BN_REG), SW_HIDE);
			SetWindowText(hEcCMot, "");
		}
			break;

		case WM_COMMAND: {

			if (LOWORD(wParam) == IDC_BN_FILTRAR && HIWORD(wParam) == BN_CLICKED) {
				if (SendMessage(GetDlgItem(hWnd, IDC_RB_C_ESPSEM), BM_GETSTATE, 0, 0) == BST_UNCHECKED && SendMessage(GetDlgItem(hWnd, IDC_RB_C_MEDMES), BM_GETSTATE, 0, 0) == BST_UNCHECKED)
					MessageBox(hWnd, "SELECCIONE UN FILTRO", "", MB_ICONEXCLAMATION);

				Node<NodeCita>* aC;
				aC = ListaCita->getHead();
				EnableWindow(GetDlgItem(hWnd, IDC_BN_CANCELAR), FALSE);
				if (SendMessage(GetDlgItem(hWnd, IDC_RB_C_ESPSEM), BM_GETSTATE, 0, 0) == BST_CHECKED) {
					
					#pragma region VALIDACIONES FILTRO 1
					char textoTemp[70];

					//ID DE ESPECIALIDAD ELEGIDA
					short lenght = GetWindowTextLength(hCbCEsp);
					GetWindowText(hCbCEsp, textoTemp, lenght + 1);
					short Esp;
					if (strcmp(textoTemp, "TODOS") == 0)
						Esp = 0;

					Node<NodeEsp>* auxEsp2;
					auxEsp2 = ListaEsp->getHead();
					while (auxEsp2 != 0) {
						if (strcmp(auxEsp2->getData().nombre, textoTemp) == 0) {
							Esp = auxEsp2->getId();
							break;
						}
						auxEsp2 = auxEsp2->getNext();
					}

					//VALIDACIONES MES
					 lenght = GetWindowTextLength(hEcCMesEsp);
					GetWindowText(hEcCMesEsp, textoTemp, lenght + 1);
					short mes = atoi(textoTemp);

					if (lenght == 0)
					{
						MessageBox(hWnd, "INGRESE EL MES", "", MB_ICONEXCLAMATION);
						break;
					}
					else {
						if (textoConLetras(textoTemp, lenght))
						{
							MessageBox(hWnd, "SOLO NUMEROS EN EL MES POR FAVOR", "", MB_ICONEXCLAMATION);
							break;
						}

						if (mes < 1 || mes > 12) {
							MessageBox(hWnd, "EN EL MES INGRESE VALORES DESDE 1-12", "", MB_ICONEXCLAMATION);
							break;
						}
					}

					//VALIDACIONES SEMANA 1
					lenght = GetWindowTextLength(hEcCSemana1);
					GetWindowText(hEcCSemana1, textoTemp, lenght + 1);
					short Sem1 = atoi(textoTemp);

					if (lenght == 0)
					{
						MessageBox(hWnd, "INGRESE EL DIA 1 POR FAVOR", "", MB_ICONEXCLAMATION);
						break;
					}
					else {
						if (textoConLetras(textoTemp, lenght))
						{
							MessageBox(hWnd, "SOLO NUMEROS EN EL DIA 1 POR FAVOR", "", MB_ICONEXCLAMATION);
							break;
						}
						if (mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 || mes == 10 || mes == 12)
						{
							if (Sem1 < 1 || Sem1 > 31) {
								MessageBox(hWnd, "EN EL DIA INGRESE VALORES DESDE 1-31", "", MB_ICONEXCLAMATION);
								break;
							}
						}
						else if (mes == 2) {
								if (Sem1 < 1 || Sem1 > 29) {
									MessageBox(hWnd, "EN EL DIA INGRESE VALORES DESDE 1-29", "", MB_ICONEXCLAMATION);
									break;
								}
						}
						else if (Sem1 < 1 || Sem1 > 30) {
							MessageBox(hWnd, "EN EL DIA INGRESE VALORES DESDE 1-30", "", MB_ICONEXCLAMATION);
							break;
						}
					}
				
					//VALIDACIONES SEMANA 2
					lenght = GetWindowTextLength(hEcCSemana2);
					GetWindowText(hEcCSemana2, textoTemp, lenght + 1);
					short Sem2 = atoi(textoTemp);

					if (lenght == 0)
					{
						MessageBox(hWnd, "INGRESE EL DIA 2 POR FAVOR", "", MB_ICONEXCLAMATION);
						break;
					}
					else {
						if (textoConLetras(textoTemp, lenght))
						{
							MessageBox(hWnd, "SOLO NUMEROS EN EL DIA 2 POR FAVOR", "", MB_ICONEXCLAMATION);
							break;
						}
						if (Sem2 < Sem1) {
							MessageBox(hWnd, "RANGO DE DIAS NO VALIDO, DIA 1 MAYOR QUE DIA 2", "", MB_ICONEXCLAMATION);
							break;
						}
						if (mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 || mes == 10 || mes == 12)
						{
							if (Sem2 < 1 || Sem2 > 31) {
								MessageBox(hWnd, "EN EL DIA INGRESE VALORES DESDE 1-31", "", MB_ICONEXCLAMATION);
								break;
							}
						}
						else if (mes == 2) {
							if (Sem2 < 1 || Sem2 > 29) {
								MessageBox(hWnd, "EN EL DIA INGRESE VALORES DESDE 1-29", "", MB_ICONEXCLAMATION);
								break;
							}
						}
						else if (Sem2 < 1 || Sem2 > 30) {
							MessageBox(hWnd, "EN EL DIA INGRESE VALORES DESDE 1-30", "", MB_ICONEXCLAMATION);
							break;
						}
					}
				#pragma endregion

					bool vacio = true;
					
					while (aC != 0) {
						if (aC->getData().fecha.wMonth == mes && (aC->getData().fecha.wDay >= Sem1&& aC->getData().fecha.wDay <= Sem2)) {
							if (ArbolMed.findNodeCed(ArbolMed.getRaiz(), aC->getData().medico)->especialidad == Esp || Esp == 0) {
								vacio = false;
								break;
							}
						}
						aC = aC->getNext();
					}

					if (vacio) {
						MessageBox(hWnd, "LA CONSULTA NO ARROJO NINGUNA COINCIDENCIA", "", MB_ICONEXCLAMATION);
						break;
					}

					SendMessage(hLbConsulta, LB_RESETCONTENT, 0, 0);
					char fecha[4];
					
					titleCreator(textoTemp);

					if (Esp == 0)
						strcat(textoTemp, "TODOS");
					else
						strcat(textoTemp,ListaEsp->findNodeId(Esp)->getData().nombre);
					
					strcat(textoTemp , " - SEMANA ");

					if (Sem1 < 10)
						strcat(textoTemp, "0");
					_itoa(Sem1, fecha, 10);
					strcat(textoTemp, fecha);
					strcat(textoTemp, "-");

					if (Sem2 < 10)
						strcat(textoTemp, "0");
					_itoa(Sem2, fecha, 10);
					strcat(textoTemp, fecha);
					
					strcat(textoTemp, ".txt");

					strcat(_path, textoTemp);

					reporte.open(textoTemp, ios::out | ios::trunc);
					if (!reporte.is_open()) {
						MessageBox(0, "NO SE HA PODIDO CREAR EL ARCHIVO PARA CONSULTA", " ", MB_ICONEXCLAMATION);
						break;
					}

					while (aC != 0) {
						if (aC->getData().fecha.wMonth == mes && (aC->getData().fecha.wDay >= Sem1 && aC->getData().fecha.wDay <= Sem2)) {
							if (ArbolMed.findNodeCed(ArbolMed.getRaiz(), aC->getData().medico)->especialidad == Esp || Esp == 0) {
								SendMessage(hLbConsulta, LB_ADDSTRING, 0, (LPARAM)aC->getData().mostrar);
								addNodeInfo(aC);
							}
						}
						aC = aC->getNext();
					}

					MessageBox(0, _path, "ARCHIVO DE CONSULTA GENERADO EN", MB_OK);

					reporte.close();

					SetDlgItemText(hWnd, IDC_STSEM_ID, " ");
					SetDlgItemText(hWnd, IDC_STSEM_FECHA, " ");
					SetDlgItemText(hWnd, IDC_STSEM_HORA, " ");
					SetDlgItemText(hWnd, IDC_STSEM_NCONSULT, " ");
					SetDlgItemText(hWnd, IDC_STSEM_ESPECIALIDAD, " ");
					SetDlgItemText(hWnd, IDC_STSEM_MEDICO, " ");
					SetDlgItemText(hWnd, IDC_STSEM_PACIENTE, " ");
					SetDlgItemText(hWnd, IDC_STSEM_TELPAC, " ");
					SetDlgItemText(hWnd, IDC_STSEM_ATENDIDA, " ");
					SetDlgItemText(hWnd, IDC_STSEM_MOTIVO, " ");
					SendMessage(hCFotoDocPc, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
				}

				if (SendMessage(GetDlgItem(hWnd, IDC_RB_C_MEDMES), BM_GETSTATE, 0, 0) == BST_CHECKED) {

				#pragma region VALIDACIONES FILTRO 2
					char textoTemp[100];

					//ID DEL MEDICO ELEGIDO
					short lenght = GetWindowTextLength(hEcCCedula);
					GetWindowText(hEcCCedula, textoTemp, lenght + 1);

					if (lenght != 8 && lenght != 0) {
						MessageBox(hWnd, "INGRESE 8 DÍGITOS EN LA CÉDULA", "", MB_ICONEXCLAMATION);
						break;
					}

					if (lenght == 0) {
						MessageBox(hWnd, "INGRESE LA CEDULA DEL MEDICO A BUSCAR POR FAVOR", "", MB_ICONEXCLAMATION);
						break;
					}

					if (textoConLetras(textoTemp, lenght)) {
						MessageBox(hWnd, "SOLO NUMEROS EN LA CEDULA POR FAVOR", "", MB_ICONEXCLAMATION);
						break;
					}
					
					NodeMed arrTemp[' '];

					ArbolMed.ordBurb(ArbolMed.getAuxMed(), arrTemp, ArbolMed.getContMed());

					SetDlgItemText(hWnd, IDC_STMED_NOMBREMED, " ");
					SetDlgItemText(hWnd, IDC_STMED_ESPMED, " ");

					int ind = ArbolMed.binSearchCed(arrTemp, 0, ArbolMed.getContMed(), atoi(textoTemp));

					if (ind == -1) {
						MessageBox(hWnd, "MEDICO NO ENCONTRADO", "", MB_ICONEXCLAMATION);
						break;
					}

					SetDlgItemText(hWnd, IDC_STMED_NOMBREMED, arrTemp[ind].nombre);
					SetDlgItemText(hWnd, IDC_STMED_ESPMED, ListaEsp->findNodeId(arrTemp[ind].especialidad)->getData().nombre);

					//VALIDACIONES MES
					lenght = GetWindowTextLength(hEcCMesMed);
					GetWindowText(hEcCMesMed, textoTemp, lenght + 1);
					short mes = atoi(textoTemp);

					if (lenght == 0)
					{
						MessageBox(hWnd, "INGRESE EL MES", "", MB_ICONEXCLAMATION);
						break;
					}
					else {
						if (textoConLetras(textoTemp, lenght))
						{
							MessageBox(hWnd, "SOLO NUMEROS EN EL MES POR FAVOR", "", MB_ICONEXCLAMATION);
							break;
						}

						if (mes < 1 || mes > 12) {
							MessageBox(hWnd, "EN EL MES INGRESE VALORES DESDE 1-12", "", MB_ICONEXCLAMATION);
							break;
						}
					}

					
				#pragma endregion

					bool vacio = true;

					while (aC != 0) {
						if (aC->getData().fecha.wMonth == mes && aC->getData().medico == arrTemp[ind].cedula) {
							vacio = false;
							break;
						}
						aC = aC->getNext();
					}

					if (vacio) {
						MessageBox(hWnd, "LA CONSULTA NO ARROJO NINGUNA COINCIDENCIA", "", MB_ICONEXCLAMATION);
						break;
					}

					SendMessage(hLbConsulta, LB_RESETCONTENT, 0, 0);
					char fecha[4];

					titleCreator(textoTemp);

					strcat(textoTemp, "MES_");
					if (mes < 10)
						strcat(textoTemp, "0");
					_itoa(mes, fecha, 10);
					strcat(textoTemp, fecha);

					strcat(textoTemp, " - MEDICO_");
					strcat(textoTemp, arrTemp[ind].nombre);

					strcat(textoTemp, ".txt");

					strcat(_path, textoTemp);

					reporte.open(textoTemp, ios::out | ios::trunc);
					if (!reporte.is_open()) {
						MessageBox(0, "NO SE HA PODIDO CREAR EL ARCHIVO PARA CONSULTA", " ", MB_ICONEXCLAMATION);
						break;
					}

					while (aC != 0) {
						if (aC->getData().fecha.wMonth == mes && aC->getData().medico == arrTemp[ind].cedula) {
								SendMessage(hLbConsulta, LB_ADDSTRING, 0, (LPARAM)aC->getData().mostrar);
								addNodeInfo(aC);
						}
						aC = aC->getNext();
					}

					MessageBox(0, _path, "ARCHIVO DE CONSULTA GENERADO EN", MB_OK);

					reporte.close();

					SetDlgItemText(hWnd, IDC_STSEM_ID, " ");
					SetDlgItemText(hWnd, IDC_STSEM_FECHA, " ");
					SetDlgItemText(hWnd, IDC_STSEM_HORA, " ");
					SetDlgItemText(hWnd, IDC_STSEM_NCONSULT, " ");
					SetDlgItemText(hWnd, IDC_STSEM_ESPECIALIDAD, " ");
					SetDlgItemText(hWnd, IDC_STSEM_MEDICO, " ");
					SetDlgItemText(hWnd, IDC_STSEM_PACIENTE, " ");
					SetDlgItemText(hWnd, IDC_STSEM_TELPAC, " ");
					SetDlgItemText(hWnd, IDC_STSEM_ATENDIDA, " ");
					SetDlgItemText(hWnd, IDC_STSEM_MOTIVO, " ");
					SendMessage(hCFotoDocPc, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
				}
			}

			if (LOWORD(wParam) == IDC_BN_CANCELAR && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hEcCMot, SW_SHOW);
				ShowWindow(GetDlgItem(hWnd, IDC_BN_GUARDAR), SW_SHOW);
				ShowWindow(GetDlgItem(hWnd, IDC_BN_REG), SW_SHOW);
			}

			if (LOWORD(wParam) == IDC_BN_GUARDAR && HIWORD(wParam) == BN_CLICKED) {
				char textoTemp[50];

				//VALIDACION MOTIVO CANCELACION
				int lenght = GetWindowTextLength(hEcCMot);
				GetWindowText(hEcCMot, textoTemp, lenght + 1);

				if (lenght == 0) {
					MessageBox(hWnd, "INGRESE EL MOTIVO DE CANCELACION POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}

				NodeCita temp;

				copyCita(&temp, auxCita3->getData());
				temp.cancelada = true;
				strcpy(temp.motCanc, textoTemp);

				auxCita3->setData(temp);

				crearArchivoCita(auxCita3->getData(), auxCita3->getId(), ArbolMed.findNodeCed(ArbolMed.getRaiz() ,auxCita3->getData().medico));

				EnableWindow(GetDlgItem(hWnd, IDC_BN_CANCELAR), FALSE);
				ShowWindow(hEcCMot, SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDC_BN_GUARDAR), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDC_BN_REG), SW_HIDE);
				SetWindowText(hEcCMot, "");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_MOTIVO, auxCita3->getData().motCanc);

				editOrDelCita = false;
			}

			if (LOWORD(wParam) == IDC_BN_REG && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hEcCMot, SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDC_BN_GUARDAR), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDC_BN_REG), SW_HIDE);
				SetWindowText(hEcCMot, "");
			}

			if (LOWORD(wParam) == IDC_BN_BUSCAR && HIWORD(wParam) == BN_CLICKED) {
				char textoTemp[100];

				//VALIDACIONES ID
				int lenght = GetWindowTextLength(hEcCId);
				GetWindowText(hEcCId, textoTemp, lenght + 1);

				if (atoi(textoTemp) > 32767) {
					MessageBox(hWnd, "INGRESE VALORES MENORES A 32,767 EN EL ID", "", MB_ICONEXCLAMATION);
					break;
				}
				if (lenght == 0) {
					MessageBox(hWnd, "INGRESE EL ID DE LA CITA QUE QUIERE BUSCAR", "", MB_ICONEXCLAMATION);
					break;
				}
				else {
					if (textoConLetras(textoTemp, lenght)) {
						MessageBox(hWnd, "SOLO NUMEROS EN EL ID POR FAVOR", "", MB_ICONEXCLAMATION);
						break;
					}
				}

				auxCita3 = ListaCita->findNodeId(atoi(textoTemp));

				if (auxCita3 == 0) {
					MessageBox(hWnd, "CITA NO ENCONTRADA", "", MB_ICONEXCLAMATION);
					EnableWindow(GetDlgItem(hWnd, IDC_BN_CANCELAR), FALSE);
					break;
				}

				_itoa(auxCita3->getId(), textoTemp, 10);
				SetDlgItemText(hWnd, IDC_STSEM_ID, textoTemp);
				strcpy(textoTemp, "");

				char horaTemp[14];
				if (auxCita3->getData().fecha.wDay < 10)
					strcat(textoTemp, "0");
				_itoa(auxCita3->getData().fecha.wDay, horaTemp, 10);
				strcat(textoTemp, horaTemp);
				strcat(textoTemp, "/");

				if (auxCita3->getData().fecha.wMonth < 10)
					strcat(textoTemp, "0");
				_itoa(auxCita3->getData().fecha.wMonth, horaTemp, 10);
				strcat(textoTemp, horaTemp);
				strcat(textoTemp, "/");

				_itoa(auxCita3->getData().fecha.wYear, horaTemp, 10);
				strcat(textoTemp, horaTemp);
				SetDlgItemText(hWnd, IDC_STSEM_FECHA, textoTemp);
				strcpy(textoTemp, "");

				if (auxCita3->getData().hora < 10)
					strcat(textoTemp, "0");
				_itoa(auxCita3->getData().hora, horaTemp, 10);
				strcat(textoTemp, horaTemp);
				strcat(textoTemp, ":");

				if (auxCita3->getData().min < 10)
					strcat(textoTemp, "0");
				_itoa(auxCita3->getData().min, horaTemp, 10);
				strcat(textoTemp, horaTemp);
				SetDlgItemText(hWnd, IDC_STSEM_HORA, textoTemp);
				strcpy(textoTemp, "");

				NodeMed* auxM3;
				auxM3 = ArbolMed.findNodeCed(ArbolMed.getRaiz(), auxCita3->getData().medico);

				_itoa(auxM3->noCon, horaTemp, 10);
				strcat(textoTemp, horaTemp);
				SetDlgItemText(hWnd, IDC_STSEM_NCONSULT, textoTemp);
				strcpy(textoTemp, "");

				SetDlgItemText(hWnd, IDC_STSEM_ESPECIALIDAD, ListaEsp->findNodeId(auxM3->especialidad)->getData().nombre);
				SetDlgItemText(hWnd, IDC_STSEM_MEDICO, auxM3->nombre);

				Node<NodePac>* auxP;
				auxP = ListaPac->findNodeId(auxCita3->getData().paciente);

				strcat(textoTemp, auxP->getData().nombre);
				strcat(textoTemp, " ");
				strcat(textoTemp, auxP->getData().apellido);
				SetDlgItemText(hWnd, IDC_STSEM_PACIENTE, textoTemp);
				strcpy(textoTemp, "");

				SetDlgItemText(hWnd, IDC_STSEM_TELPAC, auxP->getData().tel);

				if (auxCita3->getData().cancelada) {
					EnableWindow(GetDlgItem(hWnd, IDC_BN_CANCELAR), FALSE);
					SetDlgItemText(hWnd, IDC_STSEM_ATENDIDA, "NO");
					SetDlgItemText(hWnd, IDC_STSEM_MOTIVO, auxCita3->getData().motCanc);
				}
				else {
					if (citaPasada(auxCita3->getData().fecha, auxCita3->getData().hora, auxCita3->getData().min)) {
						EnableWindow(GetDlgItem(hWnd, IDC_BN_CANCELAR), FALSE);
						SetDlgItemText(hWnd, IDC_STSEM_ATENDIDA, "SI");
					}
					else {
						EnableWindow(GetDlgItem(hWnd, IDC_BN_CANCELAR), TRUE);
						SetDlgItemText(hWnd, IDC_STSEM_ATENDIDA, "NO");
					}

					SetDlgItemText(hWnd, IDC_STSEM_MOTIVO, "N/A");
				}

				hCFotoDoc = (HBITMAP)LoadImage(NULL, auxM3->dirFoto, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				SendMessage(hCFotoDocPc, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCFotoDoc);

			}

			if (LOWORD(wParam) == IDC_LB_C_CITAS && HIWORD(wParam) == LBN_SELCHANGE) {
				
				int selIndex = (int)SendMessage(hLbConsulta, LB_GETCURSEL, 0, 0);
				char nombreEB[130];
				SendMessage(hLbConsulta, LB_GETTEXT, (WPARAM)selIndex, (LPARAM)nombreEB);

				auxCita3 = ListaCita->getHead();
				while (strcmp(auxCita3->getData().mostrar, nombreEB) != 0)
				{
					auxCita3 = auxCita3->getNext();
				}

				_itoa(auxCita3->getId(), nombreEB, 10);
				SetDlgItemText(hWnd, IDC_STSEM_ID, nombreEB);
				strcpy(nombreEB, "");

				char horaTemp[14];
				if (auxCita3->getData().fecha.wDay < 10)
					strcat(nombreEB, "0");
				_itoa(auxCita3->getData().fecha.wDay, horaTemp, 10);
				strcat(nombreEB, horaTemp);
				strcat(nombreEB, "/");

				if (auxCita3->getData().fecha.wMonth < 10)
					strcat(nombreEB, "0");
				_itoa(auxCita3->getData().fecha.wMonth, horaTemp, 10);
				strcat(nombreEB, horaTemp);
				strcat(nombreEB, "/");

				_itoa(auxCita3->getData().fecha.wYear, horaTemp, 10);
				strcat(nombreEB, horaTemp);
				SetDlgItemText(hWnd, IDC_STSEM_FECHA, nombreEB);
				strcpy(nombreEB, "");

				if (auxCita3->getData().hora < 10)
					strcat(nombreEB, "0");
				_itoa(auxCita3->getData().hora, horaTemp, 10);
				strcat(nombreEB, horaTemp);
				strcat(nombreEB, ":");

				if (auxCita3->getData().min < 10)
					strcat(nombreEB, "0");
				_itoa(auxCita3->getData().min, horaTemp, 10);
				strcat(nombreEB, horaTemp);
				SetDlgItemText(hWnd, IDC_STSEM_HORA, nombreEB);
				strcpy(nombreEB, "");

				NodeMed* auxM3;
				auxM3 = ArbolMed.findNodeCed(ArbolMed.getRaiz(), auxCita3->getData().medico);

				_itoa(auxM3->noCon, horaTemp, 10);
				strcat(nombreEB, horaTemp);
				SetDlgItemText(hWnd, IDC_STSEM_NCONSULT, nombreEB);
				strcpy(nombreEB, "");

				SetDlgItemText(hWnd, IDC_STSEM_ESPECIALIDAD, ListaEsp->findNodeId(auxM3->especialidad)->getData().nombre);
				SetDlgItemText(hWnd, IDC_STSEM_MEDICO, auxM3->nombre);
				
				Node<NodePac>* auxP;
				auxP = ListaPac->findNodeId(auxCita3->getData().paciente);

				strcat(nombreEB, auxP->getData().nombre);
				strcat(nombreEB, " ");
				strcat(nombreEB, auxP->getData().apellido);
				SetDlgItemText(hWnd, IDC_STSEM_PACIENTE, nombreEB);
				strcpy(nombreEB, "");

				SetDlgItemText(hWnd, IDC_STSEM_TELPAC, auxP->getData().tel);

				if (auxCita3->getData().cancelada) {
					EnableWindow(GetDlgItem(hWnd, IDC_BN_CANCELAR), FALSE);
					SetDlgItemText(hWnd, IDC_STSEM_ATENDIDA, "NO");
					SetDlgItemText(hWnd, IDC_STSEM_MOTIVO, auxCita3->getData().motCanc);
				}
				else {
					if (citaPasada(auxCita3->getData().fecha, auxCita3->getData().hora, auxCita3->getData().min)) {
						EnableWindow(GetDlgItem(hWnd, IDC_BN_CANCELAR), FALSE);
						SetDlgItemText(hWnd, IDC_STSEM_ATENDIDA, "SI");
					}
					else {
						EnableWindow(GetDlgItem(hWnd, IDC_BN_CANCELAR), TRUE);
						SetDlgItemText(hWnd, IDC_STSEM_ATENDIDA, "NO");
					}

					SetDlgItemText(hWnd, IDC_STSEM_MOTIVO, "N/A");
				}

				hCFotoDoc = (HBITMAP)LoadImage(NULL, auxM3->dirFoto, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				SendMessage(hCFotoDocPc, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCFotoDoc);
			}

			if (LOWORD(wParam) == IDC_RB_C_ESPSEM && HIWORD(wParam) == BN_CLICKED) {
				EnableWindow(hEcCSemana1, TRUE);
				EnableWindow(hEcCSemana2, TRUE);
				EnableWindow(hEcCMesEsp, TRUE);
				EnableWindow(hCbCEsp, TRUE);
				EnableWindow(hEcCCedula, FALSE);
				EnableWindow(hEcCMesMed, FALSE);
				
				SetDlgItemText(hWnd, IDC_STMED_NOMBREMED, " ");
				SetDlgItemText(hWnd, IDC_STMED_ESPMED, " ");
			}

			if (LOWORD(wParam) == IDC_RB_C_MEDMES && HIWORD(wParam) == BN_CLICKED) {
				EnableWindow(hEcCSemana1, FALSE);
				EnableWindow(hEcCSemana2, FALSE);
				EnableWindow(hEcCMesEsp, FALSE);
				EnableWindow(hCbCEsp, FALSE);
				EnableWindow(hEcCCedula, TRUE);
				EnableWindow(hEcCMesMed, TRUE);
			}

			if (LOWORD(wParam) == ID_BM_R_MEDICO && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaConsulta, SW_HIDE);
				ShowWindow(hVentanaRegistroMed, SW_SHOW);
			}
			if (LOWORD(wParam) == ID_BM_R_PACIENTE && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaConsulta, SW_HIDE);
				ShowWindow(hVentanaRegistroPac, SW_SHOW);
			}
			if (LOWORD(wParam) == ID_BM_R_CITA && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaConsulta, SW_HIDE);
				ShowWindow(hVentanaRegistroCita, SW_SHOW);
			}
			if (LOWORD(wParam) == ID_BM_R_ESPECIALIDAD && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaConsulta, SW_HIDE);
				ShowWindow(hVentanaRegistroEsp, SW_SHOW);
			}
		}
			break;
		case WM_DESTROY:
			PostQuitMessage(10);
			break;
		case WM_CLOSE:
			hVentanaCancelar = hVentanaConsulta;
			ShowWindow(hWnd, SW_HIDE);
			ShowWindow(hVentanaSalir, SW_SHOW);
			//DestroyWindow(hWnd);  
			break;
		}
		return FALSE;
	}


	//MENSAJE REGISTRO CITA
	BOOL CALLBACK funcionRegistroCita
	(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

		switch (msg) {
		case WM_INITDIALOG:
			hLbReCLista = GetDlgItem(hWnd, IDC_LB_CITAS);
			hEcReCID = GetDlgItem(hWnd, IDC_EC_RC_ID);
			SendMessage(hEcReCID, EM_SETLIMITTEXT, 10, 0);

			hCbReCPac = GetDlgItem(hWnd, IDC_CB_RC_PAC);
			hCbReCMed = GetDlgItem(hWnd, IDC_CB_RC_MED);
			hCbReCEsp = GetDlgItem(hWnd, IDC_CB_RC_ESP);
			hDtpReC = GetDlgItem(hWnd, IDC_DTP_RC);
			hEcReCTel = GetDlgItem(hWnd, IDC_EC_RC_TEL);
			SendMessage(hEcReCTel, EM_SETLIMITTEXT, 15, 0);

			hEcReCHora = GetDlgItem(hWnd, IDC_EC_RC_HORA);
			SendMessage(hEcReCHora, EM_SETLIMITTEXT, 2, 0);

			hEcReCMin = GetDlgItem(hWnd, IDC_EC_RC_MIN);
			SendMessage(hEcReCMin, EM_SETLIMITTEXT, 2, 0);

			hReCFotoDocPc = GetDlgItem(hWnd, IDC_BMP_RC_FOTODOC);

			auxCita = ListaCita->getHead();
			while (auxCita != NULL) {
				SendMessage(hLbReCLista, LB_ADDSTRING, 0, (LPARAM)auxCita->getData().mostrar);
				auxCita = auxCita->getNext();
			}

			break;

		case WM_COMMAND: {
			NodeCita temp;
			short tempId;
			SYSTEMTIME tempTime, timeReset;

			if (LOWORD(wParam) == IDC_BN_GUARDAR && HIWORD(wParam) == BN_CLICKED) {
				#pragma region VALIDACIONES RC
				char textoTemp[150];

				//VALIDACIONES ID
				int lenght = GetWindowTextLength(hEcReCID);
				GetWindowText(hEcReCID, textoTemp, lenght + 1);
				tempId = atoi(textoTemp);
				
				if (atoi(textoTemp) > 32767) {
					MessageBox(hWnd, "INGRESE VALORES MENORES A 32,767 EN EL ID", "", MB_ICONEXCLAMATION);
					break;
				}
				if (lenght == 0) {
					MessageBox(hWnd, "INGRESE TODOS LOS DATOS POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}
				else {
					if (textoConLetras(textoTemp, lenght)) {
						MessageBox(hWnd, "SOLO NUMEROS EN EL ID POR FAVOR", "", MB_ICONEXCLAMATION);
						break;
					}
				}

				//VALIDACION MEDICO
				lenght = GetWindowTextLength(hCbReCMed);
				GetWindowText(hCbReCMed, textoTemp, lenght + 1);

				if (lenght == 0) {
					MessageBox(hWnd, "INGRESE TODOS LOS DATOS POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}
				temp.medico = auxM2->cedula;


				//VALIDACIONES FECHA
				SendMessage(hDtpReC, DTM_GETSYSTEMTIME, 0, (LPARAM)&tempTime);
				if (tempTime.wYear < tiempoActual->tm_year + 1900) {
					MessageBox(hWnd, "ESE AÑO YA HA OCURRIDO", "", MB_ICONEXCLAMATION);
					break;
				}

				if (tempTime.wYear == tiempoActual->tm_year + 1900 && tempTime.wMonth < tiempoActual->tm_mon + 1) {
					MessageBox(hWnd, "ESE MES YA HA OCURRIDO", "", MB_ICONEXCLAMATION);
					break;
				}

				if (tempTime.wYear == tiempoActual->tm_year + 1900 && tempTime.wMonth == tiempoActual->tm_mon + 1 && tempTime.wDay < tiempoActual->tm_mday) {
					MessageBox(hWnd, "ESE DIA YA HA OCURRIDO", "", MB_ICONEXCLAMATION);
					break;
				}

				bool fueraHorario = false;

				short dias[7] = { 7,7,7,7,7,7,7 };
				for (int j = 0; j < 7; j++) {
					if (auxM2->horario[j]) {
						if (j == 6)
							dias[j] = 0;
						else
							dias[j] = j + 1;
					}
				}
				for (int k = 0; k < 7; k++) {
					if (dias[k] == tempTime.wDayOfWeek ) {
						fueraHorario = false;
						break;
					}
					else
						fueraHorario = true;
				}

				if (fueraHorario) {
					MessageBox(hWnd, "EL MEDICO ELEGIDO NO ATIENDE EN ESE DIA DE LA SEMANA", "", MB_ICONEXCLAMATION);
					break;
				}

				temp.fecha = tempTime;

				_itoa(temp.fecha.wYear, temp.mostrar, 10);
				strcat(temp.mostrar, "-");

				if (temp.fecha.wMonth < 10)
					strcat(temp.mostrar, "0");
				_itoa(temp.fecha.wMonth, textoTemp, 10);
				strcat(temp.mostrar, textoTemp);
				strcat(temp.mostrar, "-");

				if (temp.fecha.wDay < 10)
					strcat(temp.mostrar, "0");
				_itoa(temp.fecha.wDay, textoTemp, 10);
				strcat(temp.mostrar, textoTemp);
				strcat(temp.mostrar, " ");
				

				//VALIDACION HORA
				
				lenght = GetWindowTextLength(hEcReCHora);
				GetWindowText(hEcReCHora, textoTemp, lenght + 1);
				temp.hora = atoi(textoTemp);

				if (lenght == 0) {
					MessageBox(hWnd, "INGRESE TODOS LOS DATOS POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}
				else {
					if (textoConLetras(textoTemp, lenght)) {
						MessageBox(hWnd, "SOLO NUMEROS EN LA HORA POR FAVOR", "", MB_ICONEXCLAMATION);
						break;
					}
				}
				
				if (atoi(textoTemp) < 0 || atoi(textoTemp) > 23) {
					MessageBox(hWnd, "INGRESE VALORES ENTRE 0 Y 23 EN LA HORA", "", MB_ICONEXCLAMATION);
					break;
				}

				if (temp.fecha.wYear == tiempoActual->tm_year + 1900 && temp.fecha.wMonth == tiempoActual->tm_mon + 1 && temp.fecha.wDay == tiempoActual->tm_mday && atoi(textoTemp) < tiempoActual->tm_hour) {
					MessageBox(hWnd, "NO PUEDE INGRESARSE UNA HORA QUE YA HA PASADO", "", MB_ICONEXCLAMATION);
					break;
				}

				if (temp.hora < auxM2->h1 || temp.hora > auxM2->h2) {
					MessageBox(hWnd, "EL MEDICO ELEGIDO NO ATIENDE A ESA HORA", "", MB_ICONEXCLAMATION);
					break;
				}

				if (temp.hora < 10)
					strcat(temp.mostrar, "0");
				_itoa(temp.hora, textoTemp, 10);
				strcat(temp.mostrar, textoTemp);
				strcat(temp.mostrar, "-");


				//VALIDACION MINUTO

				lenght = GetWindowTextLength(hEcReCMin);
				GetWindowText(hEcReCMin, textoTemp, lenght + 1);
				temp.min = atoi(textoTemp);

				if (lenght == 0) {
					MessageBox(hWnd, "INGRESE TODOS LOS DATOS", "", MB_ICONEXCLAMATION);
					break;
				}
				else {
					if (textoConLetras(textoTemp, lenght)) {
						MessageBox(hWnd, "SOLO NUMEROS EN LOS MINUTOS POR FAVOR", "", MB_ICONEXCLAMATION);
						break;
					}
				}

				if (atoi(textoTemp) < 0 || atoi(textoTemp) > 59) {
					MessageBox(hWnd, "INGRESE VALORES ENTRE 0 Y 59 EN LOS MINUTOS", "", MB_ICONEXCLAMATION);
					break;
				}

				if (temp.fecha.wYear == tiempoActual->tm_year + 1900 && temp.fecha.wMonth == tiempoActual->tm_mon + 1 && temp.fecha.wDay == tiempoActual->tm_mday && temp.hora == tiempoActual->tm_hour && atoi(textoTemp) < tiempoActual->tm_min) {
					MessageBox(hWnd, "NO PUEDE INGRESARSE UNA HORA QUE YA HA PASADO", "", MB_ICONEXCLAMATION);
					break;
				}

				if (temp.min % 20 != 0) {
					MessageBox(hWnd, "LAS HORAS DE ATENCION SON EN LOS MINUTOS 0, 20 O 40", "", MB_ICONEXCLAMATION);
					break;
				}

				if ((temp.hora == auxM2->h1 && temp.min < auxM2->m1 ) || (temp.hora == auxM2->h2 && temp.min > auxM2->m2)) {
					MessageBox(hWnd, "EL MEDICO ELEGIDO NO ATIENDE A ESA HORA", "", MB_ICONEXCLAMATION);
					break;
				}
				
				if (temp.min < 10)
					strcat(temp.mostrar, "0");
				_itoa(temp.min, textoTemp, 10);
				strcat(temp.mostrar, textoTemp);
				strcat(temp.mostrar, " CONS. ");

				_itoa(auxM2->noCon, textoTemp, 10);
				strcat(temp.mostrar, textoTemp);
				strcat(temp.mostrar, " ");

				//VALIDACION TRASPAPELE CITA
				bool trasCita = false;
				auxCita2 = ListaCita->getHead();
				while (auxCita2 != NULL) {
					if (!(auxCita2->getId() == tempId && editOrDelCita)) {
						if (auxCita2->getData().fecha.wYear == temp.fecha.wYear && auxCita2->getData().fecha.wMonth == temp.fecha.wMonth && auxCita2->getData().fecha.wDay == temp.fecha.wDay && auxCita2->getData().hora == temp.hora && auxCita2->getData().min == temp.min && auxCita2->getData().medico == temp.medico) {
							trasCita = true;
							break;
						}
					}
					auxCita2 = auxCita2->getNext();
				}

				if (trasCita) {
					char citaOcupada[200];
					strcpy(citaOcupada, "DOCTOR: \n");
					strcat(citaOcupada, ArbolMed.findNodeCed(ArbolMed.getRaiz(), auxCita->getData().medico)->nombre);
					strcat(citaOcupada, "\n");
					strcat(citaOcupada, auxCita2->getData().mostrar);
					MessageBox(hWnd, citaOcupada, "FECHA ELEGIDA OCUPADA POR OTRO PACIENTE", MB_ICONEXCLAMATION);
					break;
				}

				//VALIDACION PACIENTE
				lenght = GetWindowTextLength(hCbReCPac);
				GetWindowText(hCbReCPac, textoTemp, lenght + 1);
				temp.paciente = atoi(textoTemp);

				if (lenght == 0) {
					MessageBox(hWnd, "INGRESE TODOS LOS DATOS POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}

				strcat(temp.mostrar, ListaPac->findNodeId(temp.paciente)->getData().nombre);
				strcat(temp.mostrar, " ");
				strcat(temp.mostrar, ListaPac->findNodeId(temp.paciente)->getData().apellido);
				

				//VALIDACION TELEFONO
				lenght = GetWindowTextLength(hEcReCTel);
				GetWindowText(hEcReCTel, textoTemp, lenght + 1);
				strcpy(temp.tel, textoTemp);

				if (lenght == 0)
				{
					MessageBox(hWnd, "INGRESE TODOS LOS DATOS POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}
				else {
					if (textoConLetras(textoTemp, lenght))
					{
						MessageBox(hWnd, "SOLO NUMEROS EN EL TELEFONO POR FAVOR", "", MB_ICONEXCLAMATION);
						break;
					}
					if (lenght != 8 && lenght != 10 && lenght != 12) {
						MessageBox(hWnd, "INGRESE TELEFONOS DE 8, 1O O 12 DIGITOS", "", MB_ICONEXCLAMATION);
						break;
					}
				}
				
				strcpy(temp.usuario, nick);

				GetCurrentDirectory(MAX_PATH, _path);
				strcat(_path, "\\Citas\\");
				strcat(_path, temp.mostrar);
				strcat(_path, ".txt");

				#pragma endregion
				if (!editOrDelCita) {
					//AGREGAR

					if (ListaCita->findNodeId(tempId) != 0) {
						MessageBox(hWnd, "EL ID YA ESTA SIENDO UTILIZADO POR OTRA CITA", "", MB_ICONEXCLAMATION);
						break;
					}

					ListaCita->add(temp, tempId);

					crearArchivoCita(temp, tempId, auxM2);

					MessageBox(hWnd, _path, "CITA REGISTRADA Y ARCHIVO GENERADO", MB_OK);
					SendMessage(hLbReCLista, LB_ADDSTRING, 0, (LPARAM)temp.mostrar);
				}
				else {//EDICION
					auxCita->setData(temp);
					crearArchivoCita(auxCita->getData(), auxCita->getId(), auxM2);
					
					MessageBox(hWnd, _path, "CITA EDITADA Y ARCHIVO GENERADO", MB_OK);

					SendMessage(hLbReCLista, LB_RESETCONTENT, 0, 0);

					auxCita = ListaCita->getHead();

					while (auxCita != NULL) {
						SendMessage(hLbReCLista, LB_ADDSTRING, 0, (LPARAM)auxCita->getData().mostrar);
						auxCita = auxCita->getNext();
					}
					EnableWindow(GetDlgItem(hWnd, IDC_BN_BORRAR), FALSE);
					SendMessage(hEcReCID, EM_SETREADONLY, FALSE, 0);
				}

				auxCita = ListaCita->getHead();

				SendDlgItemMessage(hWnd, IDC_CB_RC_ESP, CB_SETCURSEL, SendMessage(hCbReCEsp, CB_FINDSTRING, -1, (LPARAM)"TODOS"), 0);
				SendMessage(hCbReCMed, CB_RESETCONTENT, 0, 0);
				
				for (int i = 0; i < ArbolMed.getContMed(); i++)
					SendDlgItemMessage(hWnd, IDC_CB_RC_MED, CB_ADDSTRING, 0, (LPARAM)ArbolMed.getAuxMed()[i]->nombre);

				GetLocalTime(&timeReset);
				SetDlgItemText(hWnd, IDC_EC_RC_ID, "");
				SendDlgItemMessage(hWnd, IDC_DTP_RC, DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM)&timeReset);
				SetDlgItemText(hWnd, IDC_EC_RC_HORA, "");
				SetDlgItemText(hWnd, IDC_EC_RC_MIN, "");

				SendDlgItemMessage(hWnd, IDC_CB_RC_PAC, CB_SETCURSEL, -1, 0);

				SetDlgItemText(hWnd, IDC_EC_RC_TEL, "");

				ShowWindow(GetDlgItem(hWnd, IDC_ST_EDIT), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDC_ST_EDNAME), SW_HIDE);

				SetDlgItemText(hWnd, IDC_ST_DIAS, " ");
				SetDlgItemText(hWnd, IDC_ST_HORAS, " ");
				SendMessage(hReCFotoDocPc, STM_SETIMAGE, IMAGE_BITMAP, NULL);

				SetDlgItemText(hVentanaConsulta, IDC_STSEM_ID, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_FECHA, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_HORA, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_NCONSULT, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_ESPECIALIDAD, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_MEDICO, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_PACIENTE, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_TELPAC, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_ATENDIDA, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_MOTIVO, " ");
				SendMessage(hCFotoDocPc, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
				SendMessage(hLbConsulta, LB_RESETCONTENT, 0, 0);
				ShowWindow(hEcCMot, SW_HIDE);
				ShowWindow(GetDlgItem(hVentanaConsulta, IDC_BN_GUARDAR), SW_HIDE);
				ShowWindow(GetDlgItem(hVentanaConsulta, IDC_BN_REG), SW_HIDE);
				SetWindowText(hEcCMot, "");

				editOrDelCita = false;
			}

			if (LOWORD(wParam) == IDC_LB_CITAS && HIWORD(wParam) == LBN_SELCHANGE) {
				EnableWindow(GetDlgItem(hWnd, IDC_BN_GUARDAR), TRUE);
				EnableWindow(GetDlgItem(hWnd, IDC_BN_BORRAR), TRUE);
				int selIndex = (int)SendMessage(hLbReCLista, LB_GETCURSEL, 0, 0);
				char nombreEB[130];
				SendMessage(hLbReCLista, LB_GETTEXT, (WPARAM)selIndex, (LPARAM)nombreEB);
				auxCita = ListaCita->getHead();
				while (strcmp(auxCita->getData().mostrar, nombreEB) != 0)
				{
					auxCita = auxCita->getNext();
				}
				ShowWindow(GetDlgItem(hWnd, IDC_ST_EDIT), SW_SHOW);
				ShowWindow(GetDlgItem(hWnd, IDC_ST_EDNAME), SW_SHOW);
				SetDlgItemText(hWnd, IDC_ST_EDNAME, auxCita->getData().usuario);

				_itoa(auxCita->getId(), nombreEB, 10);
				SetDlgItemText(hWnd, IDC_EC_RC_ID, nombreEB);
				
				SendMessage(hCbReCMed, CB_RESETCONTENT, 0, 0);
				for (int i = 0; i < ArbolMed.getContMed(); i++) {
					SendMessage(hCbReCMed, CB_ADDSTRING, 0, (LPARAM)ArbolMed.getAuxMed()[i]->nombre);
				}

				auxM2 = ArbolMed.findNodeCed(ArbolMed.getRaiz(), auxCita->getData().medico);
				SendDlgItemMessage(hWnd, IDC_CB_RC_MED, CB_SETCURSEL, SendMessage(hCbReCMed, CB_FINDSTRING, -1, (LPARAM)auxM2->nombre), 0);
				SendDlgItemMessage(hWnd, IDC_CB_RC_ESP, CB_SETCURSEL, SendMessage(hCbReCEsp, CB_FINDSTRING, -1, (LPARAM)"TODOS"), 0);
				
				strcpy(nombreEB, "");
				if (auxM2->horario[0])
					strcat(nombreEB, "L ");
				if (auxM2->horario[1])
					strcat(nombreEB, "Ma ");
				if (auxM2->horario[2])
					strcat(nombreEB, "Mi ");
				if (auxM2->horario[3])
					strcat(nombreEB, "J ");
				if (auxM2->horario[4])
					strcat(nombreEB, "V ");
				if (auxM2->horario[5])
					strcat(nombreEB, "S ");
				if (auxM2->horario[6])
					strcat(nombreEB, "D");
				SetDlgItemText(hWnd, IDC_ST_DIAS, nombreEB);

				char horaTemp[14];

				_itoa(auxM2->h1, horaTemp, 10);
				strcat(horaTemp, ":");
				_itoa(auxM2->m1, nombreEB, 10);
				if (auxM2->m1 < 10)
					strcat(horaTemp, "0");
				strcat(horaTemp, nombreEB);
				strcat(horaTemp, " a ");
				_itoa(auxM2->h2, nombreEB, 10);
				strcat(horaTemp, nombreEB);
				strcat(horaTemp, ":");
				_itoa(auxM2->m2, nombreEB, 10);
				if (auxM2->m2 < 10)
					strcat(horaTemp, "0");
				strcat(horaTemp, nombreEB);
				SetDlgItemText(hWnd, IDC_ST_HORAS, horaTemp);

				HBITMAP BmpTemp = (HBITMAP)LoadImage(NULL, auxM2->dirFoto, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				SendMessage(hReCFotoDocPc, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)BmpTemp);

				SYSTEMTIME temp;
				temp = auxCita->getData().fecha;
				SendMessage(hDtpReC,DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM)&temp);
				_itoa(auxCita->getData().hora, nombreEB, 10);
				SetDlgItemText(hWnd, IDC_EC_RC_HORA, nombreEB);
				_itoa(auxCita->getData().min, nombreEB, 10);
				SetDlgItemText(hWnd, IDC_EC_RC_MIN, nombreEB);

				SendDlgItemMessage(hWnd, IDC_CB_RC_PAC, CB_SETCURSEL, SendMessage(hCbReCPac, CB_FINDSTRING, -1, (LPARAM)ListaPac->findNodeId(auxCita->getData().paciente)->getData().mostrar), 0);

				SetDlgItemText(hWnd, IDC_EC_RC_TEL, auxCita->getData().tel);

				GetLocalTime(&timeReset);
				
				if (auxCita->getData().cancelada) {
					MessageBox(hWnd, "CITA CANCELADA, NO SE PUEDE EDITAR", "", MB_ICONEXCLAMATION);
					EnableWindow(hCbReCEsp, FALSE);
					EnableWindow(hCbReCMed, FALSE);
					EnableWindow(hDtpReC, FALSE);
					EnableWindow(hEcReCHora, FALSE);
					EnableWindow(hEcReCMin, FALSE);
					EnableWindow(hCbReCPac, FALSE);
					EnableWindow(hEcReCTel, FALSE);
					EnableWindow(GetDlgItem(hWnd, IDC_BN_GUARDAR), FALSE);
				}
				else if (citaPasada(auxCita->getData().fecha, auxCita->getData().hora, auxCita->getData().min)) {
					MessageBox(hWnd, "CITA YA CONCLUIDA O EN PROGRESO, NO SE PUEDE EDITAR", "", MB_ICONEXCLAMATION);
					EnableWindow(hCbReCEsp, FALSE);
					EnableWindow(hCbReCMed, FALSE);
					EnableWindow(hDtpReC, FALSE);
					EnableWindow(hEcReCHora, FALSE);
					EnableWindow(hEcReCMin, FALSE);
					EnableWindow(hCbReCPac, FALSE);
					EnableWindow(hEcReCTel, FALSE);
					EnableWindow(GetDlgItem(hWnd, IDC_BN_GUARDAR), FALSE);
				}
				else {
					EnableWindow(hCbReCEsp, TRUE);
					EnableWindow(hCbReCMed, TRUE);
					EnableWindow(hDtpReC, TRUE);
					EnableWindow(hEcReCHora, TRUE);
					EnableWindow(hEcReCMin, TRUE);
					EnableWindow(hCbReCPac, TRUE);
					EnableWindow(hEcReCTel, TRUE);
					EnableWindow(GetDlgItem(hWnd, IDC_BN_GUARDAR), TRUE);
				}

				SendMessage(hEcReCID, EM_SETREADONLY, TRUE, 0);
				editOrDelCita = true;
			}

			if (LOWORD(wParam) == IDC_CB_RC_MED && HIWORD(wParam) == CBN_SELCHANGE) {
				char textoTemp[56];
				int lenght = GetWindowTextLength(hCbReCMed);
				GetWindowText(hCbReCMed, textoTemp, lenght + 1);

				auxM2 = ArbolMed.findNodeName(ArbolMed.getRaiz(), textoTemp);
				char diasTemp[17] = "";
				if (auxM2->horario[0])
					strcat(diasTemp, "L ");
				if (auxM2->horario[1])
					strcat(diasTemp, "Ma ");
				if (auxM2->horario[2])
					strcat(diasTemp, "Mi ");
				if (auxM2->horario[3])
					strcat(diasTemp, "J ");
				if (auxM2->horario[4])
					strcat(diasTemp, "V ");
				if (auxM2->horario[5])
					strcat(diasTemp, "S ");
				if (auxM2->horario[6])
					strcat(diasTemp, "D");
				SetDlgItemText(hWnd, IDC_ST_DIAS, diasTemp);
				
				char horaTemp[14];

				_itoa(auxM2->h1, horaTemp, 10);
				strcat(horaTemp, ":");
				_itoa(auxM2->m1, diasTemp, 10);
				if (auxM2->m1 < 10)
					strcat(horaTemp, "0");
				strcat(horaTemp, diasTemp);
				strcat(horaTemp, " a ");
				_itoa(auxM2->h2, diasTemp, 10);
				strcat(horaTemp, diasTemp);
				strcat(horaTemp, ":");
				_itoa(auxM2->m2, diasTemp, 10);
				if (auxM2->m2 < 10)
					strcat(horaTemp, "0");
				strcat(horaTemp, diasTemp);
				SetDlgItemText(hWnd, IDC_ST_HORAS, horaTemp);

				HBITMAP BmpTemp = (HBITMAP)LoadImage(NULL, auxM2->dirFoto, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				SendMessage(hReCFotoDocPc, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)BmpTemp);
			}

			if (LOWORD(wParam) == IDC_CB_RC_ESP && HIWORD(wParam) == CBN_SELCHANGE) {
				char textoTemp[32];
				int lenght = GetWindowTextLength(hCbReCEsp);
				GetWindowText(hCbReCEsp, textoTemp, lenght + 1);

				SendMessage(hCbReCMed, CB_RESETCONTENT, 0, 0);
				SetDlgItemText(hWnd, IDC_ST_DIAS, " ");
				SetDlgItemText(hWnd, IDC_ST_HORAS, " ");
				SendMessage(hReCFotoDocPc, STM_SETIMAGE, IMAGE_BITMAP, NULL);

				for (int j = 0; j < ArbolMed.getContMed(); j++) {
					if (strcmp(ListaEsp->findNodeId(ArbolMed.getAuxMed()[j]->especialidad)->getData().nombre, textoTemp) == 0 || strcmp("TODOS", textoTemp) == 0)
							SendDlgItemMessage(hVentanaRegistroCita, IDC_CB_RC_MED, CB_ADDSTRING, 0, (LPARAM)ArbolMed.getAuxMed()[j]->nombre);
				}

			}

			if (LOWORD(wParam) == IDC_BN_BORRAR && HIWORD(wParam) == BN_CLICKED) {

				ListaCita->deleteNode(auxCita);

				MessageBox(hWnd, "CITA ELIMINADA", "", MB_OK);
				auxCita = ListaCita->getHead();

				SendMessage(hLbReCLista, LB_RESETCONTENT, 0, 0);
				
				while (auxCita != NULL)
				{
					SendMessage(hLbReCLista, LB_ADDSTRING, 0, (LPARAM)auxCita->getData().mostrar);
					auxCita = auxCita->getNext();
				}
				auxCita = ListaCita->getHead();

				EnableWindow(GetDlgItem(hWnd, IDC_BN_BORRAR), FALSE);
				SendMessage(hEcReCID, EM_SETREADONLY, FALSE, 0);
				
				SendDlgItemMessage(hWnd, IDC_CB_RC_ESP, CB_SETCURSEL, SendMessage(hCbReCEsp, CB_FINDSTRING, -1, (LPARAM)"TODOS"), 0);
				SendMessage(hCbReCMed, CB_RESETCONTENT, 0, 0);

				for (int i = 0; i < ArbolMed.getContMed(); i++)
					SendDlgItemMessage(hWnd, IDC_CB_RC_MED, CB_ADDSTRING, 0, (LPARAM)ArbolMed.getAuxMed()[i]->nombre);

				GetLocalTime(&timeReset);
				SetDlgItemText(hWnd, IDC_EC_RC_ID, "");
				SendDlgItemMessage(hWnd, IDC_DTP_RC, DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM)&timeReset);
				SetDlgItemText(hWnd, IDC_EC_RC_HORA, "");
				SetDlgItemText(hWnd, IDC_EC_RC_MIN, "");

				SendDlgItemMessage(hWnd, IDC_CB_RC_PAC, CB_SETCURSEL, -1, 0);

				SetDlgItemText(hWnd, IDC_EC_RC_TEL, "");

				SetDlgItemText(hWnd, IDC_ST_DIAS, " ");
				SetDlgItemText(hWnd, IDC_ST_HORAS, " ");
				SendMessage(hReCFotoDocPc, STM_SETIMAGE, IMAGE_BITMAP, NULL);

				SendMessage(hLbConsulta, LB_RESETCONTENT, 0, 0);
				editOrDelCita = false;
				
				ShowWindow(GetDlgItem(hWnd, IDC_ST_EDIT), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDC_ST_EDNAME), SW_HIDE);

				SetDlgItemText(hVentanaConsulta, IDC_STSEM_ID, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_FECHA, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_HORA, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_NCONSULT, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_ESPECIALIDAD, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_MEDICO, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_PACIENTE, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_TELPAC, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_ATENDIDA, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_MOTIVO, " ");
				SendMessage(hCFotoDocPc, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
				SendMessage(hLbConsulta, LB_RESETCONTENT, 0, 0);
				ShowWindow(hEcCMot, SW_HIDE);
				ShowWindow(GetDlgItem(hVentanaConsulta, IDC_BN_GUARDAR), SW_HIDE);
				ShowWindow(GetDlgItem(hVentanaConsulta, IDC_BN_REG), SW_HIDE);
				SetWindowText(hEcCMot, "");
			}

			if (LOWORD(wParam) == ID_BM_RP_CONSUL && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaRegistroCita, SW_HIDE);
				ShowWindow(hVentanaConsulta, SW_SHOW);
			}
			if (LOWORD(wParam) == ID_BM_R_MEDICO && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaRegistroCita, SW_HIDE);
				ShowWindow(hVentanaRegistroMed, SW_SHOW);
			}
			if (LOWORD(wParam) == ID_BM_R_PACIENTE && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaRegistroCita, SW_HIDE);
				ShowWindow(hVentanaRegistroPac, SW_SHOW);
			}
			if (LOWORD(wParam) == ID_BM_R_ESPECIALIDAD && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaRegistroCita, SW_HIDE);
				ShowWindow(hVentanaRegistroEsp, SW_SHOW);
			}
		}
			break;
		case WM_TIMER:
			time(&segundosTotales);
			tiempoActual = localtime(&segundosTotales);
			char chReloj[80];
			strftime(chReloj, 80, "%d-%m-%Y %I:%M:%S", tiempoActual);
			SetDlgItemText(hWnd, IDC_LB_TIME, chReloj);
			break;
		case WM_DESTROY: // DestroyWindow();
			KillTimer(hWnd, TM_RELOJ);
			PostQuitMessage(10);   
			break;
		case WM_CLOSE: // X
			ShowWindow(hWnd, SW_HIDE);
			hVentanaCancelar = hVentanaRegistroCita;
			ShowWindow(hVentanaSalir, SW_SHOW);
			//DestroyWindow(hWnd);  
			break;
		}
		return FALSE; 
	}


	//MENSAJE REGISTRO PACIENTE
	BOOL CALLBACK funcionRegistroPac
	(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

		switch (msg) {
		case WM_INITDIALOG: {

			ArbolMed.parentLinker(ArbolMed.getRaiz());
			hLbRePLista = GetDlgItem(hWnd, IDC_LB_PAC);
			hEcRePID = GetDlgItem(hWnd, IDC_EC_RP_ID);
			SendMessage(hEcRePID, EM_SETLIMITTEXT, 10, 0);

			hEcRePNom = GetDlgItem(hWnd, IDC_EC_RP_NOMBRE);
			SendMessage(hEcRePNom, EM_SETLIMITTEXT, 40, 0);

			hEcRePApe = GetDlgItem(hWnd, IDC_EC_RP_APELLIDO);
			SendMessage(hEcRePApe, EM_SETLIMITTEXT, 40, 0);

			hCbRePGenero = GetDlgItem(hWnd, IDC_CB_RP_GENERO);

			hEcRePTel = GetDlgItem(hWnd, IDC_EC_RP_TEL);
			SendMessage(hEcRePTel, EM_SETLIMITTEXT, 15, 0);

			hEcRePRef = GetDlgItem(hWnd, IDC_EC_RP_REF);
			SendMessage(hEcRePRef, EM_SETLIMITTEXT, 90, 0);

			hEcRePDia = GetDlgItem(hWnd, IDC_EC_RP_FNDIA);
			SendMessage(hEcRePDia, EM_SETLIMITTEXT, 3, 0);

			hEcRePMes = GetDlgItem(hWnd, IDC_EC_RP_FNMES);
			SendMessage(hEcRePMes, EM_SETLIMITTEXT, 3, 0);

			hEcRePAnio = GetDlgItem(hWnd, IDC_EC_RP_FNANIO);
			SendMessage(hEcRePAnio, EM_SETLIMITTEXT, 4, 0);

			hEcRePEdad = GetDlgItem(hWnd, IDC_EC_RP_EDAD);
			SendMessage(hEcRePEdad, EM_SETLIMITTEXT, 3, 0);

			hCbRePPMed = GetDlgItem(hWnd, IDC_CB_RP_PMED);
			EnableWindow(GetDlgItem(hWnd, IDC_BN_BORRAR), FALSE);
			SendDlgItemMessage(hWnd, IDC_CB_RP_GENERO, CB_ADDSTRING, 0, (LPARAM)"MASCULINO");
			SendDlgItemMessage(hWnd, IDC_CB_RP_GENERO, CB_ADDSTRING, 0, (LPARAM)"FEMENINO");
			SendDlgItemMessage(hWnd, IDC_CB_RP_GENERO, CB_ADDSTRING, 0, (LPARAM)"OTRO");

			auxPac = ListaPac->getHead();
			while (auxPac != NULL) {
				SendMessage(hLbRePLista, LB_ADDSTRING, 0, (LPARAM)auxPac->getData().mostrar);
				SendDlgItemMessage(hVentanaRegistroCita, IDC_CB_RC_PAC, CB_ADDSTRING, 0, (LPARAM)auxPac->getData().mostrar);
				auxPac = auxPac->getNext();
			}
		}
			break;

		case WM_COMMAND: {
			
			NodePac temp;
			short tempId;
			if (LOWORD(wParam) == IDC_BN_GUARDAR && HIWORD(wParam) == BN_CLICKED) {
				#pragma region VALIDACIONES RP
				char textoTemp[150];

				//VALIDACIONES ID
				int lenght = GetWindowTextLength(hEcRePID);
				GetWindowText(hEcRePID, textoTemp, lenght + 1);
				tempId = atoi(textoTemp);
				strcpy(temp.mostrar, textoTemp);
				strcat(temp.mostrar, " ");

				if (atoi(textoTemp) > 32767) {
					MessageBox(hWnd, "INGRESE VALORES MENORES A 32,767 EN EL ID", "", MB_ICONEXCLAMATION);
					break;
				}
				if (lenght == 0) {
					MessageBox(hWnd, "INGRESE TODOS LOS DATOS POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}
				else {
					if (textoConLetras(textoTemp, lenght)) {
						MessageBox(hWnd, "SOLO NUMEROS EN EL ID POR FAVOR", "", MB_ICONEXCLAMATION);
						break;
					}
				}

				//VALIDACIONES NOMBRE
				lenght = GetWindowTextLength(hEcRePNom);
				GetWindowText(hEcRePNom, textoTemp, lenght + 1);
				strcpy(temp.nombre, textoTemp);
				strcat(temp.mostrar, textoTemp);
				strcat(temp.mostrar, " ");

				if (lenght > 40) {
					MessageBox(hWnd, "LIMITE DE CARACTERES ALCANZADO EN EL NOMBRE", "", MB_ICONEXCLAMATION);
					break;
				}

				if (lenght == 0) {
					MessageBox(hWnd, "INGRESE TODOS LOS DATOS POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}

				if (textoIncorrecto(textoTemp, lenght)) {
					break;
				}

				//VALIDACIONES APELLIDO
				lenght = GetWindowTextLength(hEcRePApe);
				GetWindowText(hEcRePApe, textoTemp, lenght + 1);
				strcpy(temp.apellido, textoTemp);
				strcat(temp.mostrar, textoTemp);

				if (lenght > 40) {
					MessageBox(hWnd, "LIMITE DE CARACTERES ALCANZADO EN EL APELLIDO", "", MB_ICONEXCLAMATION);
					break;
				}

				if (lenght == 0) {
					MessageBox(hWnd, "INGRESE TODOS LOS DATOS POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}

				if (textoIncorrecto(textoTemp, lenght)) {
					break;
				}

				//VALIDACIONES GENERO
				lenght = GetWindowTextLength(hCbRePGenero);
				GetWindowText(hCbRePGenero, textoTemp, lenght + 1);
				strcpy(temp.genero, textoTemp);

				if (lenght == 0) {
					MessageBox(hWnd, "INGRESE TODOS LOS DATOS POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}

				//VALIDACIONES TELEFONO
				lenght = GetWindowTextLength(hEcRePTel);
				GetWindowText(hEcRePTel, textoTemp, lenght + 1);
				strcpy(temp.tel, textoTemp);

				if (lenght == 0)
				{
					MessageBox(hWnd, "INGRESE TODOS LOS DATOS POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}
				else {
					if (textoConLetras(textoTemp, lenght))
					{
						MessageBox(hWnd, "SOLO NUMEROS EN EL TELEFONO POR FAVOR", "", MB_ICONEXCLAMATION);
						break;
					}
					if (lenght != 8 && lenght != 10 && lenght != 12) {
						MessageBox(hWnd, "INGRESE TELEFONOS DE 8, 1O O 12 DIGITOS", "", MB_ICONEXCLAMATION);
						break;
					}
				}

				//VALIDACIONES REFERENCIA
				lenght = GetWindowTextLength(hEcRePRef);
				GetWindowText(hEcRePRef, textoTemp, lenght + 1);
				strcpy(temp.ref, textoTemp);

				if (lenght > 79) {
					MessageBox(hWnd, "LIMITE DE CARACTERES ALCANZADO EN LA REFERENCÍA", "", MB_ICONEXCLAMATION);
					break;
				}

				if (lenght == 0)
				{
					MessageBox(hWnd, "INGRESE TODOS LOS DATOS POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}

				//VALIDACIONES AÑO
				lenght = GetWindowTextLength(hEcRePAnio);
				GetWindowText(hEcRePAnio, textoTemp, lenght + 1);
				temp.anio = atoi(textoTemp);


				if (lenght == 0) {
					MessageBox(hWnd, "INGRESE TODOS LOS DATOS POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}
				else {

					if (textoConLetras(textoTemp, lenght)) {
						MessageBox(hWnd, "SOLO NUMEROS EN EL AÑO POR FAVOR", "", MB_ICONEXCLAMATION);
						break;
					}

					if (atoi(textoTemp) < 1870) {
						MessageBox(hWnd, "NO PUEDE SER TAN VIEJO, SEA REALISTA POR FAVOR", "", MB_ICONEXCLAMATION);
						break;
					}

					if (temp.anio > (tiempoActual->tm_year + 1900)) {
						MessageBox(hWnd, "EL PACIENTE AUN NO HA NACIDO", "", MB_ICONEXCLAMATION);
						break;
					}
				}

				//VALIDACIONES MES
				lenght = GetWindowTextLength(hEcRePMes);
				GetWindowText(hEcRePMes, textoTemp, lenght + 1);
				temp.mes = atoi(textoTemp);
				if (lenght == 0)
				{
					MessageBox(hWnd, "INGRESE TODOS LOS DATOS POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}
				else {
					if (textoConLetras(textoTemp, lenght))
					{
						MessageBox(hWnd, "SOLO NUMEROS EN EL MES POR FAVOR", "", MB_ICONEXCLAMATION);
						break;
					}

					if (temp.mes < 1 || temp.mes > 12) {
						MessageBox(hWnd, "EN EL MES INGRESE VALORES DESDE 1-12", "", MB_ICONEXCLAMATION);
						break;
					}

					if (temp.mes > (tiempoActual->tm_mon + 1) && temp.anio == (tiempoActual->tm_year + 1900)) {
						MessageBox(hWnd, "EL PACIENTE AUN NO HA NACIDO", "", MB_ICONEXCLAMATION);
						break;
					}
				}

				//VALIDACIONES DIA
				lenght = GetWindowTextLength(hEcRePDia);
				GetWindowText(hEcRePDia, textoTemp, lenght + 1);
				temp.dia = atoi(textoTemp);
				if (lenght == 0)
				{
					MessageBox(hWnd, "INGRESE TODOS LOS DATOS POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}
				else {
					if (textoConLetras(textoTemp, lenght))
					{
						MessageBox(hWnd, "SOLO NUMEROS EN EL DIA POR FAVOR", "", MB_ICONEXCLAMATION);
						break;
					}
					if (temp.mes == 1 || temp.mes == 3 || temp.mes == 5 || temp.mes == 7 || temp.mes == 8 || temp.mes == 10 || temp.mes == 12)
					{
						if (temp.dia < 1 || temp.dia > 31) {
							MessageBox(hWnd, "EN EL DIA INGRESE VALORES DESDE 1-31", "", MB_ICONEXCLAMATION);
							break;
						}
					}
					else if (temp.mes == 2) {
						if (temp.anio % 4 == 0)
						{
							if (temp.dia < 1 || temp.dia > 29) {
								MessageBox(hWnd, "EN EL DIA INGRESE VALORES DESDE 1-29", "", MB_ICONEXCLAMATION);
								break;
							}
						}
						else if (temp.dia < 1 || temp.dia > 28) {
							MessageBox(hWnd, "EN EL DIA INGRESE VALORES DESDE 1-28", "", MB_ICONEXCLAMATION);
							break;
						}
					}
					else if (temp.dia < 1 || temp.dia > 30) {
						MessageBox(hWnd, "EN EL DIA INGRESE VALORES DESDE 1-30", "", MB_ICONEXCLAMATION);
						break;
					}

					if (temp.dia > (tiempoActual->tm_mday) && temp.mes == (tiempoActual->tm_mon + 1) && temp.anio == (tiempoActual->tm_year + 1900)) {
						MessageBox(hWnd, "EL PACIENTE AUN NO HA NACIDO", "", MB_ICONEXCLAMATION);
						break;
					}
				}

				//CALCULO DE EDAD
				short edad = 0;
				if (tiempoActual->tm_year + 1900 != temp.anio) {
					edad = tiempoActual->tm_year +1900 - (temp.anio + 1);
					if (tiempoActual->tm_mon + 1 > temp.mes)
						edad++;
					if (tiempoActual->tm_mon + 1 == temp.mes && tiempoActual->tm_mday > temp.dia)
						edad++;
				}
				_itoa(edad, textoTemp, 10);
				SetDlgItemText(hWnd, IDC_EC_RP_EDAD, textoTemp);
				temp.edad = edad;

				//VALIDACIONES PRIMER MEDICO
				lenght = GetWindowTextLength(hCbRePPMed);
				GetWindowText(hCbRePPMed, textoTemp, lenght + 1);

				if (lenght == 0) {
					MessageBox(hWnd, "INGRESE TODOS LOS DATOS POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}

				temp.primerMed = ArbolMed.findNodeName(ArbolMed.getRaiz(), textoTemp)->cedula;

				//USUARIO
				strcpy(temp.usuario, nick);
				#pragma endregion

				if (!editOrDelPac) {
					//AGREGAR

					if (ListaPac->findNodeId(tempId) != 0) {
						MessageBox(hWnd, "EL ID YA ESTA SIENDO UTILIZADO POR OTRO PACIENTE", "", MB_ICONEXCLAMATION);
						break;
					}

					ListaPac->add(temp, tempId);
					MessageBox(hWnd, "PACIENTE REGISTRADO", "", MB_OK);
					SendMessage(hLbRePLista, LB_ADDSTRING, 0, (LPARAM)temp.mostrar);
					SendDlgItemMessage(hVentanaRegistroCita, IDC_CB_RC_PAC, CB_ADDSTRING, 0, (LPARAM)temp.mostrar);
				}
				else {//EDICION
					auxPac->setData(temp);
					MessageBox(hWnd, "PACIENTE EDITADO", "", MB_OK);

					SendMessage(hLbRePLista, LB_RESETCONTENT, 0, 0);
					SendMessage(hCbReCPac, CB_RESETCONTENT, 0, 0);

					auxPac = ListaPac->getHead();

					while (auxPac != NULL) {
						SendMessage(hLbRePLista, LB_ADDSTRING, 0, (LPARAM)auxPac->getData().mostrar);
						SendDlgItemMessage(hVentanaRegistroCita, IDC_CB_RC_PAC, CB_ADDSTRING, 0, (LPARAM)auxPac->getData().mostrar);
						auxPac = auxPac->getNext();
					}
					EnableWindow(GetDlgItem(hWnd, IDC_BN_BORRAR), FALSE);
					SendMessage(hEcRePID, EM_SETREADONLY, FALSE, 0);
				}

				auxPac = ListaPac->getHead();
				SetDlgItemText(hWnd, IDC_EC_RP_ID, "");
				SetDlgItemText(hWnd, IDC_EC_RP_NOMBRE, "");
				SetDlgItemText(hWnd, IDC_EC_RP_APELLIDO, "");
				SendDlgItemMessage(hWnd, IDC_CB_RP_GENERO, CB_SETCURSEL, -1, 0);
				SetDlgItemText(hWnd, IDC_EC_RP_TEL, "");
				SetDlgItemText(hWnd, IDC_EC_RP_REF, "");
				SetDlgItemText(hWnd, IDC_EC_RP_FNDIA, "");
				SetDlgItemText(hWnd, IDC_EC_RP_FNMES, "");
				SetDlgItemText(hWnd, IDC_EC_RP_FNANIO, "");
				SetDlgItemText(hWnd, IDC_EC_RP_EDAD, "");
				SendDlgItemMessage(hWnd, IDC_CB_RP_PMED, CB_SETCURSEL, -1, 0);
				ShowWindow(GetDlgItem(hWnd, IDC_ST_EDIT), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDC_ST_EDNAME), SW_HIDE);

				editOrDelPac = false;

				SetDlgItemText(hVentanaConsulta, IDC_STSEM_ID, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_FECHA, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_HORA, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_NCONSULT, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_ESPECIALIDAD, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_MEDICO, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_PACIENTE, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_TELPAC, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_ATENDIDA, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_MOTIVO, " ");
				SendMessage(hCFotoDocPc, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
				SendMessage(hLbConsulta, LB_RESETCONTENT, 0, 0);
				ShowWindow(hEcCMot, SW_HIDE);
				ShowWindow(GetDlgItem(hVentanaConsulta, IDC_BN_GUARDAR), SW_HIDE);
				ShowWindow(GetDlgItem(hVentanaConsulta, IDC_BN_REG), SW_HIDE);
				SetWindowText(hEcCMot, "");
			}

			if (LOWORD(wParam) == IDC_LB_PAC && HIWORD(wParam) == LBN_SELCHANGE) {
				EnableWindow(GetDlgItem(hWnd, IDC_BN_BORRAR), TRUE);
				int selIndex = (int)SendMessage(hLbRePLista, LB_GETCURSEL, 0, 0);
				char nombreEB[101];
				SendMessage(hLbRePLista, LB_GETTEXT, (WPARAM)selIndex, (LPARAM)nombreEB);
				auxPac = ListaPac->getHead();
				while (strcmp(auxPac->getData().mostrar, nombreEB) != 0)
				{
					auxPac = auxPac->getNext();
				}
				ShowWindow(GetDlgItem(hWnd, IDC_ST_EDIT), SW_SHOW);
				ShowWindow(GetDlgItem(hWnd, IDC_ST_EDNAME), SW_SHOW);
				SetDlgItemText(hWnd, IDC_ST_EDNAME, auxPac->getData().usuario);

				char idTemp[6];
				_itoa(auxPac->getId(), idTemp, 10);
				SetDlgItemText(hWnd, IDC_EC_RP_ID, idTemp);
				SetDlgItemText(hWnd, IDC_EC_RP_NOMBRE, auxPac->getData().nombre);
				SetDlgItemText(hWnd, IDC_EC_RP_APELLIDO, auxPac->getData().apellido);

				SendDlgItemMessage(hWnd, IDC_CB_RP_GENERO, CB_SETCURSEL, SendMessage(hCbRePGenero, CB_FINDSTRING, -1, (LPARAM)auxPac->getData().genero), 0);

				SetDlgItemText(hWnd, IDC_EC_RP_TEL, auxPac->getData().tel);
				SetDlgItemText(hWnd, IDC_EC_RP_REF, auxPac->getData().ref);

				_itoa(auxPac->getData().dia, idTemp, 10);
				SetDlgItemText(hWnd, IDC_EC_RP_FNDIA, idTemp);
				_itoa(auxPac->getData().mes, idTemp, 10);
				SetDlgItemText(hWnd, IDC_EC_RP_FNMES, idTemp);
				_itoa(auxPac->getData().anio, idTemp, 10);
				SetDlgItemText(hWnd, IDC_EC_RP_FNANIO, idTemp);
				_itoa(auxPac->getData().edad, idTemp, 10);
				SetDlgItemText(hWnd, IDC_EC_RP_EDAD, idTemp);

				SendDlgItemMessage(hWnd, IDC_CB_RP_PMED, CB_SETCURSEL, SendMessage(hCbRePPMed, CB_FINDSTRING, -1, (LPARAM)ArbolMed.findNodeCed(ArbolMed.getRaiz(),auxPac->getData().primerMed)->nombre), 0);

				SendMessage(hEcRePID, EM_SETREADONLY, TRUE, 0);
				editOrDelPac = true;
			}

			if (LOWORD(wParam) == IDC_BN_BORRAR && HIWORD(wParam) == BN_CLICKED) {
				bool usado = false;
				Node<NodeCita>* auxC;
				auxC = ListaCita->getHead();

				while (auxC != 0) {
					if (auxPac->getId() == auxC->getData().paciente) {
						MessageBox(0, "NO SE PUEDE ELIMINAR EL PACIENTE, TIENE UNA CITA REGISTRADA", auxC->getData().mostrar, MB_ICONEXCLAMATION);
						usado = true;
						break;
					}
					auxC = auxC->getNext();
				}

				if (usado)
					break;

				ListaPac->deleteNode(auxPac);

				MessageBox(hWnd, "PACIENTE ELIMINADO", "", MB_OK);
				auxPac = ListaPac->getHead();

				SendMessage(hLbRePLista, LB_RESETCONTENT, 0, 0);
				SendMessage(hCbReCPac, CB_RESETCONTENT, 0, 0);
				while (auxPac != NULL)
				{
					SendMessage(hLbRePLista, LB_ADDSTRING, 0, (LPARAM)auxPac->getData().mostrar);
					SendDlgItemMessage(hVentanaRegistroCita, IDC_CB_RC_PAC, CB_ADDSTRING, 0, (LPARAM)auxPac->getData().mostrar);
					auxPac = auxPac->getNext();
				}
				auxPac = ListaPac->getHead();

				EnableWindow(GetDlgItem(hWnd, IDC_BN_BORRAR), FALSE);
				SendMessage(hEcRePID, EM_SETREADONLY, FALSE, 0);
				editOrDelPac = false;
				SetDlgItemText(hWnd, IDC_EC_RP_ID, "");
				SetDlgItemText(hWnd, IDC_EC_RP_NOMBRE, "");
				SetDlgItemText(hWnd, IDC_EC_RP_APELLIDO, "");
				SendDlgItemMessage(hWnd, IDC_CB_RP_GENERO, CB_SETCURSEL, -1, 0);
				SetDlgItemText(hWnd, IDC_EC_RP_TEL, "");
				SetDlgItemText(hWnd, IDC_EC_RP_REF, "");
				SetDlgItemText(hWnd, IDC_EC_RP_FNDIA, "");
				SetDlgItemText(hWnd, IDC_EC_RP_FNMES, "");
				SetDlgItemText(hWnd, IDC_EC_RP_FNANIO, "");
				SetDlgItemText(hWnd, IDC_EC_RP_EDAD, "");
				SendDlgItemMessage(hWnd, IDC_CB_RP_PMED, CB_SETCURSEL, -1, 0);
				ShowWindow(GetDlgItem(hWnd, IDC_ST_EDIT), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDC_ST_EDNAME), SW_HIDE);
			}

			if (LOWORD(wParam) == IDC_BN_GREP && HIWORD(wParam) == BN_CLICKED) {
				ListaPac->_quickSort(ListaPac->getHead(), ListaPac->getTail());
				Node<NodePac>* auxTemp = ListaPac->getHead();
				
				reporte.open("Reporte_Pacientes.txt", ios::out | ios::trunc);
				if (!reporte.is_open()) {
					MessageBox(0, "NO SE HA PODIDO CREAR EL REPORTE DE PACIENTES", " ", MB_ICONEXCLAMATION);
					break;
				}
				agregarEspacios("ID", 2, 6);
				reporte << " ";
				agregarEspacios("APELLIDO", 8, 42);
				reporte << " ";
				agregarEspacios("NOMBRE", 6, 42);
				reporte << " ";
				agregarEspacios("GENERO", 6, 10);
				reporte << " ";
				agregarEspacios("TELEFONO", 8, 13);
				agregarEspacios("EDAD", 4, 5);
				agregarEspacios("AÑO", 3, 5);
				agregarEspacios("MES", 3, 4);
				agregarEspacios("DÍA", 3, 4);
				agregarEspacios("PRIMER MEDICO", 13, 56);
				reporte << endl;

				while (auxTemp != 0) {
					char numTemp[6];
					_itoa(auxTemp->getId(), numTemp, 10);
					agregarEspacios(auxTemp->getId(),strlen(numTemp),6);
					reporte << " ";
					agregarEspacios(auxTemp->getData().apellido, strlen(auxTemp->getData().apellido), 42);
					reporte << " ";
					agregarEspacios(auxTemp->getData().nombre, strlen(auxTemp->getData().nombre), 42);
					reporte << " ";
					agregarEspacios(auxTemp->getData().genero, strlen(auxTemp->getData().genero), 10);
					reporte << " ";
					agregarEspacios(auxTemp->getData().tel, strlen(auxTemp->getData().tel), 13);
					_itoa(auxTemp->getData().edad, numTemp, 10);
					agregarEspacios(auxTemp->getData().edad, strlen(numTemp), 4);

					_itoa(auxTemp->getData().anio, numTemp, 10);
					agregarEspacios(auxTemp->getData().anio, strlen(numTemp), 6);

					_itoa(auxTemp->getData().mes, numTemp, 10);
					agregarEspacios(auxTemp->getData().mes, strlen(numTemp), 4);

					_itoa(auxTemp->getData().dia, numTemp, 10);
					agregarEspacios(auxTemp->getData().dia, strlen(numTemp), 4);

					agregarEspacios(ArbolMed.findNodeCed(ArbolMed.getRaiz(), auxTemp->getData().primerMed)->nombre, strlen(ArbolMed.findNodeCed(ArbolMed.getRaiz(), auxTemp->getData().primerMed)->nombre), 56);
					reporte << endl;
					auxTemp = auxTemp->getNext();
				}
				reporte.close();
				GetCurrentDirectory(MAX_PATH, _path);
				EnableWindow(GetDlgItem(hWnd, IDC_BN_BORRAR), FALSE);
				SendMessage(hEcRePID, EM_SETREADONLY, FALSE, 0);
				SetDlgItemText(hWnd, IDC_EC_RP_ID, "");
				SetDlgItemText(hWnd, IDC_EC_RP_NOMBRE, "");
				SetDlgItemText(hWnd, IDC_EC_RP_APELLIDO, "");
				SendDlgItemMessage(hWnd, IDC_CB_RP_GENERO, CB_SETCURSEL, -1, 0);
				SetDlgItemText(hWnd, IDC_EC_RP_TEL, "");
				SetDlgItemText(hWnd, IDC_EC_RP_REF, "");
				SetDlgItemText(hWnd, IDC_EC_RP_FNDIA, "");
				SetDlgItemText(hWnd, IDC_EC_RP_FNMES, "");
				SetDlgItemText(hWnd, IDC_EC_RP_FNANIO, "");
				SetDlgItemText(hWnd, IDC_EC_RP_EDAD, "");
				SendDlgItemMessage(hWnd, IDC_CB_RP_PMED, CB_SETCURSEL, -1, 0);
				ShowWindow(GetDlgItem(hWnd, IDC_ST_EDIT), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDC_ST_EDNAME), SW_HIDE);
				SendMessage(hLbRePLista, LB_SETCURSEL, -1, 0);
				editOrDelPac = false;
				strcat(_path, "\\Reporte_Pacientes.txt");
				MessageBox(0, _path, "REPORTE GENERADO", MB_OK);
			}

			if (LOWORD(wParam) == ID_BM_RP_CONSUL && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaRegistroPac, SW_HIDE);
				ShowWindow(hVentanaConsulta, SW_SHOW);
			}
			if (LOWORD(wParam) == ID_BM_R_MEDICO && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaRegistroPac, SW_HIDE);
				ShowWindow(hVentanaRegistroMed, SW_SHOW);
			}
			if (LOWORD(wParam) == ID_BM_R_CITA && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaRegistroPac, SW_HIDE);
				ShowWindow(hVentanaRegistroCita, SW_SHOW);
			}
			if (LOWORD(wParam) == ID_BM_R_ESPECIALIDAD && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaRegistroPac, SW_HIDE);
				ShowWindow(hVentanaRegistroEsp, SW_SHOW);
			}
		}
			break;
		case WM_DESTROY:
			PostQuitMessage(10);
			break;
		case WM_CLOSE:
			hVentanaCancelar = hVentanaRegistroPac;
			ShowWindow(hWnd, SW_HIDE);
			ShowWindow(hVentanaSalir, SW_SHOW);
			//DestroyWindow(hWnd);
			break;
		}
		return FALSE;
	}


	//MENSAJE TRASPAPELE MED
	BOOL CALLBACK funcionTrasMed
	(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

		switch (msg) {
		case WM_INITDIALOG:

			break;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaTrasMed, SW_HIDE);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(10);
			break;
		case WM_CLOSE:
			//DestroyWindow(hWnd);
			break;
		}
		return FALSE;
	}


	//MENSAJE REGISTRO MEDICO
	BOOL CALLBACK funcionRegistroMed
	(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

		switch (msg) {
		case WM_INITDIALOG:
			hLbReMLista = GetDlgItem(hWnd, IDC_LB_MED);

			hEcReMCedula = GetDlgItem(hWnd, IDC_EC_RM_CEDULA);
			SendMessage(hEcReMCedula, EM_SETLIMITTEXT, 20, 0);

			hEcReMNombre = GetDlgItem(hWnd, IDC_EC_RM_NOMBRE);
			SendMessage(hEcReMNombre, EM_SETLIMITTEXT, 60, 0);

			hEcReMHora1 = GetDlgItem(hWnd, IDC_EC_RM_HORA_1);
			SendMessage(hEcReMHora1, EM_SETLIMITTEXT, 4, 0);

			hEcReMMin1 = GetDlgItem(hWnd, IDC_EC_RM_MIN1);
			SendMessage(hEcReMMin1, EM_SETLIMITTEXT, 4, 0);

			hEcReMMin2 = GetDlgItem(hWnd, IDC_EC_RM_MIN2);
			SendMessage(hEcReMMin2, EM_SETLIMITTEXT, 4, 0);

			hEcReMHora2 = GetDlgItem(hWnd, IDC_EC_RM_HORA_2);
			SendMessage(hEcReMHora2, EM_SETLIMITTEXT, 4, 0);

			hEcReMTel = GetDlgItem(hWnd, IDC_EC_RM_TEL);
			SendMessage(hEcReMTel, EM_SETLIMITTEXT, 15, 0);

			hReMFotoDocPc = GetDlgItem(hWnd, IDC_BMP_RM_FOTODOC);
			hCbReMConsul = GetDlgItem(hWnd, IDC_CB_RM_NCON);
			hCbReMEsp = GetDlgItem(hWnd, IDC_CB_RM_ESP);
			EnableWindow(GetDlgItem(hWnd, IDC_BN_BORRAR), FALSE);
			for (int i = 0; i < 5; i++) {
				char iAscci[2];
				_itoa(i + 1, iAscci, 10);
				SendDlgItemMessage(hWnd, IDC_CB_RM_NCON, CB_ADDSTRING, 0, (LPARAM)iAscci);
			}

			for (int j = 0; j < ArbolMed.getContMed(); j++) {
				SendMessage(hLbReMLista, LB_ADDSTRING, 0, (LPARAM)ArbolMed.getAuxMed()[j]->mostrar);
				SendDlgItemMessage(hVentanaRegistroPac, IDC_CB_RP_PMED, CB_ADDSTRING, 0, (LPARAM)ArbolMed.getAuxMed()[j]->nombre);
				SendDlgItemMessage(hVentanaRegistroCita, IDC_CB_RC_MED, CB_ADDSTRING, 0, (LPARAM)ArbolMed.getAuxMed()[j]->nombre);
			}
			break;

		case WM_COMMAND: {
			NodeMed temp;

			if (LOWORD(wParam) == IDC_BN_GUARDAR && HIWORD(wParam) == BN_CLICKED) {
			#pragma region VALIDACIONES RM

				char textoTemp[100];

				//VALIDACIONES CEDULA
				int lenght = GetWindowTextLength(hEcReMCedula);
				GetWindowText(hEcReMCedula, textoTemp, lenght + 1);
				strcpy(temp.mostrar, textoTemp);
				strcat(temp.mostrar, " ");
				temp.cedula = atoi(textoTemp);

				if (lenght != 8 && lenght != 0) {
					MessageBox(hWnd, "INGRESE 8 DÍGITOS EN LA CÉDULA", "", MB_ICONEXCLAMATION);
					break;
				}

				if (lenght == 0) {
					MessageBox(hWnd, "INGRESE TODOS LOS DATOS POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}

				if (textoConLetras(textoTemp, lenght)) {
					MessageBox(hWnd, "SOLO NUMEROS EN LA CEDULA POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}


				//VALIDACIONES NOMBRE
				lenght = GetWindowTextLength(hEcReMNombre);
				GetWindowText(hEcReMNombre, textoTemp, lenght + 1);
				strcat(temp.mostrar, textoTemp);
				strcat(temp.mostrar, " ");
				strcpy(temp.nombre, textoTemp);

				if (lenght > 50) {
					MessageBox(hWnd, "LIMITE DE CARACTERES ALCANZADO EN EL NOMBRE", "", MB_ICONEXCLAMATION);
					break;
				}

				if (lenght == 0) {
					MessageBox(hWnd, "INGRESE TODOS LOS DATOS POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}

				if (textoIncorrecto(textoTemp, lenght)) {
					break;
				}

				//VALIDACIONES NUMERO DE CONSULTORIO
				lenght = GetWindowTextLength(hCbReMConsul);
				GetWindowText(hCbReMConsul, textoTemp, lenght + 1);
				temp.noCon = atoi(textoTemp);

				if (lenght == 0) {
					MessageBox(hWnd, "INGRESE TODOS LOS DATOS POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}

				//VALIDACIONES ESPECIALIDAD
				lenght = GetWindowTextLength(hCbReMEsp);
				GetWindowText(hCbReMEsp, textoTemp, lenght + 1);
				strcat(temp.mostrar, textoTemp);

				if (lenght == 0) {
					MessageBox(hWnd, "INGRESE TODOS LOS DATOS POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}
				
				//Obtenemos el ID de la especialidad por medio del nombre
				auxEsp = ListaEsp->getHead();
				while (auxEsp != 0) {
					if (strcmp(textoTemp, auxEsp->getData().nombre) == 0) {
						temp.especialidad = auxEsp->getId();
						break;
					}
					auxEsp = auxEsp->getNext();
				}

				//VALIDACIONES HORA 1
				lenght = GetWindowTextLength(hEcReMHora1);
				GetWindowText(hEcReMHora1, textoTemp, lenght + 1);
				temp.h1 = atoi(textoTemp);

				if (lenght == 0) {
					MessageBox(hWnd, "INGRESE LA HORA DE INICIO POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}

				if (textoConLetras(textoTemp, lenght)) {
					MessageBox(hWnd, "SOLO NUMEROS EN LA HORA (1)", "", MB_ICONEXCLAMATION);
					break;
				}

				if (temp.h1 < 0 || temp.h1 >23){
					MessageBox(hWnd, "EN LA HORA (1) INGRESE VALORES DESDE 0-23", "", MB_ICONEXCLAMATION);
					break;
				}

				//VALIDACIONES MIN 1
				lenght = GetWindowTextLength(hEcReMMin1);
				GetWindowText(hEcReMMin1, textoTemp, lenght + 1);
				temp.m1 = atoi(textoTemp);

				if (lenght == 0) {
					MessageBox(hWnd, "INGRESE LA HORA DE INICIO COMPLETA POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}

				if (textoConLetras(textoTemp, lenght)) {
					MessageBox(hWnd, "SOLO NUMEROS EN LOS MINUTOS (1)", "", MB_ICONEXCLAMATION);
					break;
				}

				if (temp.m1 < 0 || temp.m1 >59) {
					MessageBox(hWnd, "EN LOS MINUTOS (1) INGRESE VALORES DESDE 0-59", "", MB_ICONEXCLAMATION);
					break;
				}

				if (temp.m1 % 20 != 0) {
					MessageBox(hWnd, "EN LOS MINUTOS (1) INGRESE 0, 20 o 40", "", MB_ICONEXCLAMATION);
					break;
				}


				//VALIDACIONES HORA 2
				lenght = GetWindowTextLength(hEcReMHora2);
				GetWindowText(hEcReMHora2, textoTemp, lenght + 1);
				temp.h2 = atoi(textoTemp);

				if (lenght == 0) {
					MessageBox(hWnd, "INGRESE LA HORA DE CIERRE POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}

				if (textoConLetras(textoTemp, lenght)) {
					MessageBox(hWnd, "SOLO NUMEROS EN LA HORA (2)", "", MB_ICONEXCLAMATION);
					break;
				}

				if (temp.h2 < 0 || temp.h2 >23) {
					MessageBox(hWnd, "EN LA HORA (2) INGRESE VALORES DESDE 0-23", "", MB_ICONEXCLAMATION);
					break;
				}

				//VALIDACIONES MIN 2
				lenght = GetWindowTextLength(hEcReMMin2);
				GetWindowText(hEcReMMin2, textoTemp, lenght + 1);
				temp.m2 = atoi(textoTemp);

				if (lenght == 0) {
					MessageBox(hWnd, "INGRESE LA HORA DE CIERRE COMPLETA POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}

				if (textoConLetras(textoTemp, lenght)) {
					MessageBox(hWnd, "SOLO NUMEROS EN LOS MINUTOS (2)", "", MB_ICONEXCLAMATION);
					break;
				}

				if (temp.m2 < 0 || temp.m2 >59) {
					MessageBox(hWnd, "EN LOS MINUTOS (2) INGRESE VALORES DESDE 0-59", "", MB_ICONEXCLAMATION);
					break;
				}

				if (temp.m2 % 20 != 0) {
					MessageBox(hWnd, "EN LOS MINUTOS (2) INGRESE 0, 20 o 40", "", MB_ICONEXCLAMATION);
					break;
				}

				//VALIDACIONES HORARIO
				if (temp.h1 > temp.h2) {
					MessageBox(hWnd, "LA HORA DE INICIO DEBE SER ANTES DE LA HORA DE CIERRE", "", MB_ICONEXCLAMATION);
					break;
				}

				if (temp.h1 == temp.h2) {
					if (temp.m1 == temp.m2) {
						MessageBox(hWnd, "LA HORA DE INICIO DEBE SER ANTES DE LA HORA DE CIERRE", "", MB_ICONEXCLAMATION);
						break;
					}
					if (temp.m1 > temp.m2){
						MessageBox(hWnd, "LA HORA DE INICIO DEBE SER ANTES DE LA HORA DE CIERRE", "", MB_ICONEXCLAMATION);
						break;
					}
				}


				//VALIDACIONES TELEFONO
				lenght = GetWindowTextLength(hEcReMTel);
				GetWindowText(hEcReMTel, textoTemp, lenght + 1);
				strcpy(temp.tel, textoTemp);

				if (lenght == 0)
				{
					MessageBox(hWnd, "INGRESE TODOS LOS DATOS POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}
				else {
					if (textoConLetras(textoTemp, lenght))
					{
						MessageBox(hWnd, "SOLO NUMEROS EN EL TELEFONO POR FAVOR", "", MB_ICONEXCLAMATION);
						break;
					}
					if (lenght != 8 && lenght != 10 && lenght != 12) {
						MessageBox(hWnd, "INGRESE TELEFONOS DE 8, 1O O 12 DIGITOS", "", MB_ICONEXCLAMATION);
						break;
					}
				}

			#pragma region VALIDACIONES DIAS DE ATENCION
				bool selection = false;

				if (IsDlgButtonChecked(hWnd, IDC_CB_RM_L) == BST_CHECKED) {
					temp.horario[0] = true;
					selection = true;
				}
				else
					temp.horario[0] = false;

				if (IsDlgButtonChecked(hWnd, IDC_CB_RM_MA) == BST_CHECKED) {
					temp.horario[1] = true;
					selection = true;
				}
				else
					temp.horario[1] = false;

				if (IsDlgButtonChecked(hWnd, IDC_CB_RM_MI) == BST_CHECKED) {
					temp.horario[2] = true;
					selection = true;
				}
				else
					temp.horario[2] = false;

				if (IsDlgButtonChecked(hWnd, IDC_CB_RM_J) == BST_CHECKED) {
					temp.horario[3] = true;
					selection = true;
				}
				else
					temp.horario[3] = false;

				if (IsDlgButtonChecked(hWnd, IDC_CB_RM_V) == BST_CHECKED) {
					temp.horario[4] = true;
					selection = true;
				}
				else
					temp.horario[4] = false;

				if (IsDlgButtonChecked(hWnd, IDC_CB_RM_S) == BST_CHECKED) {
					temp.horario[5] = true;
					selection = true;
				}
				else
					temp.horario[5] = false;

				if (IsDlgButtonChecked(hWnd, IDC_CB_RM_D) == BST_CHECKED) {
					temp.horario[6] = true;
					selection = true;
				}
				else
					temp.horario[6] = false;

				if (selection == false) {
					MessageBox(hWnd, "SELECCIONE POR LO MENOS UN DIA PARA ATENDER", "", MB_ICONEXCLAMATION);
					break;
				}
			#pragma endregion

				//VALIDACION DE HORARIOS
				NodeMed* aux = ArbolMed.atencionTraspapelada(ArbolMed.getRaiz(), temp);
				if (aux){
					SetDlgItemText(hVentanaTrasMed, IDC_ST_TM_MOSTRAR, aux->mostrar);
					char horat1[60] = "", horat2[10];

					_itoa(aux->h1, horat2, 10);
					if (aux->h1 < 10)
						strcat(horat1, "0");
					strcat(horat1, horat2);
					strcat(horat1, ":");
					_itoa(aux->m1, horat2, 10);
					if (aux->m1 < 10)
						strcat(horat1, "0");
					strcat(horat1, horat2);
					SetDlgItemText(hVentanaTrasMed, IDC_ST_TM_H1, horat1);

					_itoa(aux->h2, horat2, 10);
					strcpy(horat1, "");
					if (aux->h2 < 10)
						strcat(horat1, "0");
					strcat(horat1, horat2);
					strcat(horat1, ":");
					_itoa(aux->m2, horat2, 10);
					if (aux->m2 < 10)
						strcat(horat1, "0");
					strcat(horat1, horat2);
					SetDlgItemText(hVentanaTrasMed, IDC_ST_TM_H2, horat1);

					_itoa(aux->noCon, horat2, 10);
					SetDlgItemText(hVentanaTrasMed, IDC_ST_TM_CONS, horat2);

					strcpy(horat1, "");
					if (aux->horario[0])
						strcat(horat1, "LUNES ");
					if (aux->horario[1])
						strcat(horat1, "MARTES ");
					if (aux->horario[2])
						strcat(horat1, "MIÉRCOLES ");
					if (aux->horario[3])
						strcat(horat1, "JUEVES ");
					if (aux->horario[4])
						strcat(horat1, "VIERNES ");
					if (aux->horario[5])
						strcat(horat1, "SABADO ");
					if (aux->horario[6])
						strcat(horat1, "DOMINGO ");
					SetDlgItemText(hVentanaTrasMed, IDC_ST_TM_DIAS, horat1);

					ShowWindow(hVentanaTrasMed, SW_SHOW);
					break;
				}

				//VALIDACION FOTO
				if (editOrDelMed) {
					strcpy(temp.dirFoto, dirFotoEd);
				}
				else
					strcpy(temp.dirFoto, dirFotoTemp);

				if (strlen(temp.dirFoto) == 0) {
					MessageBox(hWnd, "INGRESE LA FOTO DEL DOCTOR POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}

				//ADICION DEL NICK DEL USUARIO QUE LO AGREGO/EDITO
				strcpy(temp.usuario, nick);
			#pragma endregion
			
				//AGREGAR
				if (!editOrDelMed) {

					if (ArbolMed.cedRepetido(ArbolMed.getRaiz(), temp.cedula)) {
						MessageBox(hWnd, "LA CÉDULA YA ESTA SIENDO UTILIZADA POR OTRO MÉDICO", "", MB_ICONEXCLAMATION);
						break;
					}
					
					ArbolMed.addNode(ArbolMed.getRaizDir(), temp);
					ArbolMed.parentLinker(ArbolMed.getRaiz());
					MessageBox(hWnd, "MÉDICO REGISTRADO", "", MB_OK);
					SendMessage(hLbReMLista, LB_ADDSTRING, 0, (LPARAM)temp.mostrar);
					SendDlgItemMessage(hVentanaRegistroPac, IDC_CB_RP_PMED, CB_ADDSTRING, 0, (LPARAM)temp.nombre);
					lenght = GetWindowTextLength(hCbReCEsp);
					GetWindowText(hCbReCEsp, textoTemp, lenght + 1);
					if (strcmp(ListaEsp->findNodeId(temp.especialidad)->getData().nombre, textoTemp) == 0 || strcmp("TODOS", textoTemp) == 0)
						SendDlgItemMessage(hVentanaRegistroCita, IDC_CB_RC_MED, CB_ADDSTRING, 0, (LPARAM)temp.nombre);
				}//EDITAR
				else {
					NodeMed* aM = ArbolMed.findNodeCed(ArbolMed.getRaiz(), temp.cedula);
					ArbolMed.copyNode(aM, temp);

					bool cReg = false;

					//ACTUALIZACION DE ARCHIVOS DE CITA
					while (auxCita4 != NULL) {
						if (auxCita4->getData().medico == aM->cedula) {
							updateCitaMostrar(auxCita4);
							cReg = true;
						}
						auxCita4 = auxCita4->getNext();
					}

					if (cReg) {
						SendMessage(hLbReCLista, LB_RESETCONTENT, 0, 0);
						auxCita4 = ListaCita->getHead();
						while (auxCita4 != NULL) {
							SendMessage(hLbReCLista, LB_ADDSTRING, 0, (LPARAM)auxCita4->getData().mostrar);
							auxCita4 = auxCita4->getNext();
						}
					}
					
					MessageBox(hWnd, "MÉDICO EDITADO", "", MB_OK);

					SetDlgItemText(hVentanaRegistroCita, IDC_ST_DIAS, " ");
					SetDlgItemText(hVentanaRegistroCita, IDC_ST_HORAS, " ");
					SendMessage(hReCFotoDocPc, STM_SETIMAGE, IMAGE_BITMAP, NULL);

					
					ShowWindow(GetDlgItem(hWnd, IDC_ST_EDIT), SW_HIDE);
					ShowWindow(GetDlgItem(hWnd, IDC_ST_EDNAME), SW_HIDE);

					SendMessage(hLbReMLista, LB_RESETCONTENT, 0, 0);
					SendMessage(hCbRePPMed, CB_RESETCONTENT, 0, 0);

					for (int i = 0; i < ArbolMed.getContMed(); i++) {
						SendMessage(hLbReMLista, LB_ADDSTRING, 0, (LPARAM)ArbolMed.getAuxMed()[i]->mostrar);
						SendDlgItemMessage(hVentanaRegistroPac, IDC_CB_RP_PMED, CB_ADDSTRING, 0, (LPARAM)ArbolMed.getAuxMed()[i]->nombre);
						
					}
					
					lenght = GetWindowTextLength(hCbReCEsp);
					GetWindowText(hCbReCEsp, textoTemp, lenght + 1);

					if (strcmp(ListaEsp->findNodeId(temp.especialidad)->getData().nombre, textoTemp) == 0) {
						SendMessage(hCbReCMed, CB_RESETCONTENT, 0, 0);
						for (int j = 0; j < ArbolMed.getContMed(); j++) {
							if(ArbolMed.getAuxMed()[j]->especialidad == temp.especialidad)
								SendDlgItemMessage(hVentanaRegistroCita, IDC_CB_RC_MED, CB_ADDSTRING, 0, (LPARAM)ArbolMed.getAuxMed()[j]->nombre);
						}
					}
					else if (strcmp("TODOS", textoTemp) == 0) {
						SendMessage(hCbReCMed, CB_RESETCONTENT, 0, 0);
						for (int k = 0; k < ArbolMed.getContMed(); k++) {
								SendDlgItemMessage(hVentanaRegistroCita, IDC_CB_RC_MED, CB_ADDSTRING, 0, (LPARAM)ArbolMed.getAuxMed()[k]->nombre);
						}
					}

					EnableWindow(GetDlgItem(hWnd, IDC_BN_BORRAR), FALSE);
					SendMessage(hEcReMCedula, EM_SETREADONLY, FALSE, 0);
				}

				strcpy(dirFotoTemp, "");

				SetDlgItemText(hWnd, IDC_EC_RM_NOMBRE, "");
				SetDlgItemText(hWnd, IDC_EC_RM_CEDULA, "");
				SendDlgItemMessage(hWnd, IDC_CB_RM_NCON, CB_SETCURSEL, -1, 0);
				SendDlgItemMessage(hWnd, IDC_CB_RM_ESP, CB_SETCURSEL, -1, 0);
				SetDlgItemText(hWnd, IDC_EC_RM_HORA_1, "");
				SetDlgItemText(hWnd, IDC_EC_RM_MIN1, "");
				SetDlgItemText(hWnd, IDC_EC_RM_HORA_2, "");
				SetDlgItemText(hWnd, IDC_EC_RM_MIN2, "");
				SetDlgItemText(hWnd, IDC_EC_RM_TEL, "");
				SendDlgItemMessage(hWnd, IDC_CB_RM_L, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hWnd, IDC_CB_RM_MA, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hWnd, IDC_CB_RM_MI, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hWnd, IDC_CB_RM_J, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hWnd, IDC_CB_RM_V, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hWnd, IDC_CB_RM_S, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hWnd, IDC_CB_RM_D, BM_SETCHECK, BST_UNCHECKED, 0);
				SendMessage(hReMFotoDocPc, STM_SETIMAGE, IMAGE_BITMAP, NULL);
				
				editOrDelMed = false;

				SetDlgItemText(hVentanaConsulta, IDC_STSEM_ID, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_FECHA, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_HORA, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_NCONSULT, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_ESPECIALIDAD, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_MEDICO, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_PACIENTE, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_TELPAC, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_ATENDIDA, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_MOTIVO, " ");
				SendMessage(hCFotoDocPc, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
				SendMessage(hLbConsulta, LB_RESETCONTENT, 0, 0);
				ShowWindow(hEcCMot, SW_HIDE);
				ShowWindow(GetDlgItem(hVentanaConsulta, IDC_BN_GUARDAR), SW_HIDE);
				ShowWindow(GetDlgItem(hVentanaConsulta, IDC_BN_REG), SW_HIDE);
				SetWindowText(hEcCMot, "");
			}

			if (LOWORD(wParam) == IDC_BN_RM_SUBIRF && HIWORD(wParam) == BN_CLICKED) {
				OPENFILENAME ofn;
				ZeroMemory(&ofn, sizeof(ofn));
				char chDirFile[MAX_PATH] = "";
				ofn.hwndOwner = hWnd;
				ofn.lStructSize = sizeof(ofn);
				ofn.lpstrFile = chDirFile;
				ofn.nMaxFile = MAX_PATH;
				ofn.lpstrDefExt = "txt";
				ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
				ofn.lpstrFilter = "Mapa de bits de 24 bits\0*.bmp\0";
				if (GetOpenFileName(&ofn)) {
					//hReMFotoDocPc = GetDlgItem(hWnd, IDC_BMP_RM_FOTODOC);
					hReMFotoDoc = (HBITMAP)LoadImage(NULL, chDirFile, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
					strcpy(dirFotoTemp, chDirFile);
					strcpy(dirFotoEd, chDirFile);
					SendMessage(hReMFotoDocPc, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hReMFotoDoc);
				}
			}

			if (LOWORD(wParam) == IDC_BN_GREP && HIWORD(wParam) == BN_CLICKED) {

				NodeMed arrTemp[' '];

				for (int k = 0; k < ArbolMed.getContMed(); k++) {
					ArbolMed.copyNode(&arrTemp[k], *ArbolMed.getAuxMed()[k]);
				}

				ArbolMed.heapSort(arrTemp, ArbolMed.getContMed());

				reporte.open("Reporte_Medicos.txt", ios::out | ios::trunc);
				if (!reporte.is_open()) {
					MessageBox(0, "NO SE HA PODIDO CREAR EL REPORTE DE MEDICOS", " ", MB_ICONEXCLAMATION);
					break;
				}
				agregarEspacios("CEDULA", 6, 10);
				agregarEspacios("NOMBRE", 6, 56);
				agregarEspacios("ESPECIALIDAD", 12, 32);
				agregarEspacios("NO.CONSUL", 9, 11);
				agregarEspacios("HORARIO DE ATENCION", 19, 29);
				agregarEspacios("TELEFONO", 8, 14);
				reporte << "DIRECCION FOTO";
			
				reporte << endl;

				for (int i = 0; i < ArbolMed.getContMed(); i++) {
					agregarEspacios(arrTemp[i].cedula, 8, 10);
					
					agregarEspacios(arrTemp[i].nombre, strlen(arrTemp[i].nombre), 56);

					Node<NodeEsp>* aE = ListaEsp->findNodeId(arrTemp[i].especialidad);
					agregarEspacios(aE->getData().nombre, strlen(aE->getData().nombre), 32);
					
					agregarEspacios(arrTemp[i].noCon, 1, 11);
					
					char numTemp[30] = "";
					char iT[4] =  "";
					if (arrTemp[i].h1 < 10)
						strcat(numTemp,"0");
					_itoa(arrTemp[i].h1, iT, 10);
					strcat(numTemp, iT);
					strcat(numTemp, ":");

					if (arrTemp[i].m1 < 10)
						strcat(numTemp, "0");
					_itoa(arrTemp[i].m1, iT, 10);
					strcat(numTemp, iT);
					strcat(numTemp, "-");

					if (arrTemp[i].h2 < 10)
						strcat(numTemp, "0");
					_itoa(arrTemp[i].h2, iT, 10);
					strcat(numTemp, iT);
					strcat(numTemp, ":");

					if (arrTemp[i].m2 < 10)
						strcat(numTemp, "0");
					_itoa(arrTemp[i].m2, iT, 10);
					strcat(numTemp, iT);
					strcat(numTemp, " ");

					if (arrTemp[i].horario[0]) {
						strcat(numTemp, "L ");
					}

					if (arrTemp[i].horario[1]) {
						strcat(numTemp, "Ma ");
					}

					if (arrTemp[i].horario[2]) {
						strcat(numTemp, "Mi ");
					}

					if (arrTemp[i].horario[3]) {
						strcat(numTemp, "J ");
					}

					if (arrTemp[i].horario[4]) {
						strcat(numTemp, "V ");
					}

					if (arrTemp[i].horario[5]) {
						strcat(numTemp, "S ");
					}

					if (arrTemp[i].horario[6]) {
						strcat(numTemp, "D");
					}

					agregarEspacios(numTemp, strlen(numTemp), 29);
					
					agregarEspacios(arrTemp[i].tel, strlen(arrTemp[i].tel), 14);

					reporte << arrTemp[i].dirFoto;

					reporte << endl;
				}

				reporte.close();
				GetCurrentDirectory(MAX_PATH, _path);
				strcat(_path, "\\Reporte_Medicos.txt");
				MessageBox(0, _path, "REPORTE GENERADO", MB_OK);
			}

			if (LOWORD(wParam) == IDC_BN_BORRAR && HIWORD(wParam) == BN_CLICKED) {
				bool usado = false;
				Node<NodePac>* auxP;
				auxP = ListaPac->getHead();
				while (auxP != 0) {
					if (auxM->cedula == auxP->getData().primerMed) {
						MessageBox(0, "NO SE PUEDE ELIMINAR EL MEDICO, ES USADO COMO EL PRIMER CONTACTO DE UN PACIENTE", auxP->getData().mostrar, MB_ICONEXCLAMATION);
						usado = true;
						break;
					}
					auxP = auxP->getNext();
				}

				auxCita2 = ListaCita->getHead();
				while (auxCita2 != 0) {
					if (auxM->cedula == auxCita2->getData().medico) {
						usado = true;
						MessageBox(0, "NO SE PUEDE ELIMINAR EL MEDICO, TIENE UNA CITA REGISTRADA", auxCita2->getData().mostrar, MB_ICONEXCLAMATION);
						break;
					}
					auxCita2 = auxCita2->getNext();
				}

				if (usado)
					break;

				char EspTemp[40];
				strcpy(EspTemp, ListaEsp->findNodeId(auxM->especialidad)->getData().nombre);

				short idEspTemp = ListaEsp->findNodeId(auxM->especialidad)->getId();

				ArbolMed.deleteNodeTree(auxM);
				ArbolMed.resetAuxMed(ArbolMed.getAuxMed(), ArbolMed.getContMed());
				
				MessageBox(hWnd, "MÉDICO ELIMINADO", "", MB_OK);

				SetDlgItemText(hVentanaRegistroCita, IDC_ST_DIAS, " ");
				SetDlgItemText(hVentanaRegistroCita, IDC_ST_HORAS, " ");
				SendMessage(hReCFotoDocPc, STM_SETIMAGE, IMAGE_BITMAP, NULL);

				SendMessage(hLbReMLista, LB_RESETCONTENT, 0, 0);
				SendMessage(hCbRePPMed, CB_RESETCONTENT, 0, 0);

				for (int i = 0; i < ArbolMed.getContMed(); i++) {
					SendMessage(hLbReMLista, LB_ADDSTRING, 0, (LPARAM)ArbolMed.getAuxMed()[i]->mostrar);
					SendDlgItemMessage(hVentanaRegistroPac, IDC_CB_RP_PMED, CB_ADDSTRING, 0, (LPARAM)ArbolMed.getAuxMed()[i]->nombre);

				}

				char textoTemp[60];
				int lenght = GetWindowTextLength(hCbReCEsp);
				GetWindowText(hCbReCEsp, textoTemp, lenght + 1);

				if (strcmp(EspTemp, textoTemp) == 0) {
					SendMessage(hCbReCMed, CB_RESETCONTENT, 0, 0);
					for (int j = 0; j < ArbolMed.getContMed(); j++) {
						if (ArbolMed.getAuxMed()[j]->especialidad == idEspTemp)
							SendDlgItemMessage(hVentanaRegistroCita, IDC_CB_RC_MED, CB_ADDSTRING, 0, (LPARAM)ArbolMed.getAuxMed()[j]->nombre);
					}
				}
				else if (strcmp("TODOS", textoTemp) == 0) {
					SendMessage(hCbReCMed, CB_RESETCONTENT, 0, 0);
					for (int k = 0; k < ArbolMed.getContMed(); k++) {
						SendDlgItemMessage(hVentanaRegistroCita, IDC_CB_RC_MED, CB_ADDSTRING, 0, (LPARAM)ArbolMed.getAuxMed()[k]->nombre);
					}
				}

				SetDlgItemText(hWnd, IDC_EC_RM_NOMBRE, "");
				SetDlgItemText(hWnd, IDC_EC_RM_CEDULA, "");
				SendDlgItemMessage(hWnd, IDC_CB_RM_NCON, CB_SETCURSEL, -1, 0);
				SendDlgItemMessage(hWnd, IDC_CB_RM_ESP, CB_SETCURSEL, -1, 0);
				SetDlgItemText(hWnd, IDC_EC_RM_HORA_1, "");
				SetDlgItemText(hWnd, IDC_EC_RM_MIN1, "");
				SetDlgItemText(hWnd, IDC_EC_RM_HORA_2, "");
				SetDlgItemText(hWnd, IDC_EC_RM_MIN2, "");
				SetDlgItemText(hWnd, IDC_EC_RM_TEL, "");
				SendDlgItemMessage(hWnd, IDC_CB_RM_L, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hWnd, IDC_CB_RM_MA, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hWnd, IDC_CB_RM_MI, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hWnd, IDC_CB_RM_J, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hWnd, IDC_CB_RM_V, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hWnd, IDC_CB_RM_S, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hWnd, IDC_CB_RM_D, BM_SETCHECK, BST_UNCHECKED, 0);
				SendMessage(hReMFotoDocPc, STM_SETIMAGE, IMAGE_BITMAP, 0);
				EnableWindow(GetDlgItem(hWnd, IDC_BN_BORRAR), FALSE);
				SendMessage(hEcReMCedula, EM_SETREADONLY, FALSE, 0);
				ShowWindow(GetDlgItem(hWnd, IDC_ST_EDIT), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDC_ST_EDNAME), SW_HIDE);

				editOrDelMed = false;
			}

			if (LOWORD(wParam) == IDC_LB_MED && HIWORD(wParam) == LBN_SELCHANGE) {
				EnableWindow(GetDlgItem(hWnd, IDC_BN_BORRAR), TRUE);
				int selIndex = (int)SendMessage(hLbReMLista, LB_GETCURSEL, 0, 0);
				char mostrar[60], cedTemp[10];
				SendMessage(hLbReMLista, LB_GETTEXT, (WPARAM)selIndex, (LPARAM)mostrar);
				strncpy(cedTemp, mostrar, 8);
				auxM = ArbolMed.findNodeCed(ArbolMed.getRaiz(), atoi(cedTemp));

				_itoa(auxM->cedula, cedTemp, 10);
				SetDlgItemText(hWnd, IDC_EC_RM_CEDULA, cedTemp);

				SetDlgItemText(hWnd, IDC_EC_RM_NOMBRE, auxM->nombre);

				_itoa(auxM->noCon, cedTemp, 10);
				SendDlgItemMessage(hWnd, IDC_CB_RM_NCON, CB_SETCURSEL, SendMessage(hCbReMConsul, CB_FINDSTRING, -1, (LPARAM)cedTemp), 0);
				
				SendDlgItemMessage(hWnd, IDC_CB_RM_ESP, CB_SETCURSEL, SendMessage(hCbReMEsp, CB_FINDSTRING, -1, (LPARAM)ListaEsp->findNodeId(auxM->especialidad)->getData().nombre), 0);

				_itoa(auxM->h1, cedTemp, 10);
				SetDlgItemText(hWnd, IDC_EC_RM_HORA_1, cedTemp);
				_itoa(auxM->m1, cedTemp, 10);
				SetDlgItemText(hWnd, IDC_EC_RM_MIN1, cedTemp);
				_itoa(auxM->h2, cedTemp, 10);
				SetDlgItemText(hWnd, IDC_EC_RM_HORA_2, cedTemp);
				_itoa(auxM->m2, cedTemp, 10);
				SetDlgItemText(hWnd, IDC_EC_RM_MIN2, cedTemp);

				SetDlgItemText(hWnd, IDC_EC_RM_TEL, auxM->tel);
				SendDlgItemMessage(hWnd, IDC_CB_RM_L, BM_SETCHECK, auxM->horario[0], 0);
				SendDlgItemMessage(hWnd, IDC_CB_RM_MA, BM_SETCHECK, auxM->horario[1], 0);
				SendDlgItemMessage(hWnd, IDC_CB_RM_MI, BM_SETCHECK, auxM->horario[2], 0);
				SendDlgItemMessage(hWnd, IDC_CB_RM_J, BM_SETCHECK, auxM->horario[3], 0);
				SendDlgItemMessage(hWnd, IDC_CB_RM_V, BM_SETCHECK, auxM->horario[4], 0);
				SendDlgItemMessage(hWnd, IDC_CB_RM_S, BM_SETCHECK, auxM->horario[5], 0);
				SendDlgItemMessage(hWnd, IDC_CB_RM_D, BM_SETCHECK, auxM->horario[6], 0);

				//VALIDACION NO EDITAR HORARIO
				
				bool citaReg = false;

				auxCita4 = ListaCita->getHead();
				while(auxCita4 != 0 ){
					if (auxCita4->getData().medico == auxM->cedula) {
						MessageBox(0, "EL MEDICO TIENE UNA CITA REGISTRADA, NO SE PUEDE EDITAR SU HORARIO" ,auxCita4->getData().mostrar, MB_ICONEXCLAMATION);
						citaReg = true;
						EnableWindow(GetDlgItem(hWnd, IDC_CB_RM_L), FALSE);
						EnableWindow(GetDlgItem(hWnd, IDC_CB_RM_MA), FALSE);
						EnableWindow(GetDlgItem(hWnd, IDC_CB_RM_MI), FALSE);
						EnableWindow(GetDlgItem(hWnd, IDC_CB_RM_J), FALSE);
						EnableWindow(GetDlgItem(hWnd, IDC_CB_RM_V), FALSE);
						EnableWindow(GetDlgItem(hWnd, IDC_CB_RM_S), FALSE);
						EnableWindow(GetDlgItem(hWnd, IDC_CB_RM_D), FALSE);
						EnableWindow(hEcReMHora1, FALSE);
						EnableWindow(hEcReMHora2, FALSE);
						EnableWindow(hEcReMMin1, FALSE);
						EnableWindow(hEcReMMin2, FALSE);
						break;
					}
					auxCita4 = auxCita4->getNext();
				}

				if (!citaReg) {
					EnableWindow(GetDlgItem(hWnd, IDC_CB_RM_L), TRUE);
					EnableWindow(GetDlgItem(hWnd, IDC_CB_RM_MA), TRUE);
					EnableWindow(GetDlgItem(hWnd, IDC_CB_RM_MI), TRUE);
					EnableWindow(GetDlgItem(hWnd, IDC_CB_RM_J), TRUE);
					EnableWindow(GetDlgItem(hWnd, IDC_CB_RM_V), TRUE);
					EnableWindow(GetDlgItem(hWnd, IDC_CB_RM_S), TRUE);
					EnableWindow(GetDlgItem(hWnd, IDC_CB_RM_D), TRUE);
					EnableWindow(hEcReMHora1, TRUE);
					EnableWindow(hEcReMHora2, TRUE);
					EnableWindow(hEcReMMin1, TRUE);
					EnableWindow(hEcReMMin2, TRUE);
				}

				ShowWindow(GetDlgItem(hWnd, IDC_ST_EDIT), SW_SHOW);
				ShowWindow(GetDlgItem(hWnd, IDC_ST_EDNAME), SW_SHOW);
				SetDlgItemText(hWnd, IDC_ST_EDNAME, auxM->usuario);

				strcpy(dirFotoEd, auxM->dirFoto);
				HBITMAP BmpTemp = (HBITMAP)LoadImage(NULL, auxM->dirFoto, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				SendMessage(hReMFotoDocPc, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)BmpTemp);

				SendMessage(hEcReMCedula, EM_SETREADONLY, TRUE, 0);
				editOrDelMed = true;
			}

			if (LOWORD(wParam) == ID_BM_RP_CONSUL && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaRegistroMed, SW_HIDE);
				ShowWindow(hVentanaConsulta, SW_SHOW);
			}
			if (LOWORD(wParam) == ID_BM_R_PACIENTE && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaRegistroMed, SW_HIDE);
				ShowWindow(hVentanaRegistroPac, SW_SHOW);
			}
			if (LOWORD(wParam) == ID_BM_R_CITA && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaRegistroMed, SW_HIDE);
				ShowWindow(hVentanaRegistroCita, SW_SHOW);
			}
			if (LOWORD(wParam) == ID_BM_R_ESPECIALIDAD && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaRegistroMed, SW_HIDE);
				ShowWindow(hVentanaRegistroEsp, SW_SHOW);
			}
		}
			break;
		case WM_DESTROY:
			PostQuitMessage(10);
			break;
		case WM_CLOSE:
			hVentanaCancelar = hVentanaRegistroMed;
			ShowWindow(hWnd, SW_HIDE);
			ShowWindow(hVentanaSalir, SW_SHOW);
			//DestroyWindow(hWnd);
			break;
		}
		return FALSE;
	}


	//MENSAJE REGISTRO ESPECIALIDAD
	BOOL CALLBACK funcionRegistroEsp
	(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

		switch (msg) {
		case WM_INITDIALOG: {
			hLbReELista = GetDlgItem(hWnd, IDC_LB_ESP);
			hEcReEID = GetDlgItem(hWnd, IDC_EC_RE_ID);
			SendMessage(hEcReEID, EM_SETLIMITTEXT, 10, 0);

			hEcReENom = GetDlgItem(hWnd, IDC_EC_RE_NOM);
			SendMessage(hEcReENom, EM_SETLIMITTEXT, 20, 0);

			hEcReEDesc = GetDlgItem(hWnd, IDC_EC_RE_DESC);
			SendMessage(hEcReEDesc, EM_SETLIMITTEXT, 95, 0);

			EnableWindow(GetDlgItem(hWnd, IDC_BN_BORRAR), FALSE);

			auxEsp = ListaEsp->getHead();

			SendDlgItemMessage(hVentanaRegistroCita, IDC_CB_RC_ESP, CB_ADDSTRING, 0, (LPARAM)"TODOS");
			SendDlgItemMessage(hVentanaConsulta, IDC_CB_ESP, CB_ADDSTRING, 0, (LPARAM)"TODOS");

			while (auxEsp != NULL) {
				SendMessage(hLbReELista, LB_ADDSTRING, 0, (LPARAM)auxEsp->getData().nombre);
				SendDlgItemMessage(hVentanaRegistroMed, IDC_CB_RM_ESP, CB_ADDSTRING, 0, (LPARAM)auxEsp->getData().nombre);
				SendDlgItemMessage(hVentanaRegistroCita, IDC_CB_RC_ESP, CB_ADDSTRING, 0, (LPARAM)auxEsp->getData().nombre);
				SendDlgItemMessage(hVentanaConsulta, IDC_CB_ESP, CB_ADDSTRING, 0, (LPARAM)auxEsp->getData().nombre);
				auxEsp = auxEsp->getNext();
			}

			SendDlgItemMessage(hVentanaRegistroCita, IDC_CB_RC_ESP, CB_SETCURSEL, SendMessage(hCbReCEsp, CB_FINDSTRING, -1, (LPARAM)"TODOS"), 0);
			SendDlgItemMessage(hVentanaConsulta, IDC_CB_ESP, CB_SETCURSEL, SendMessage(hCbCEsp, CB_FINDSTRING, -1, (LPARAM)"TODOS"), 0);

			auxEsp = ListaEsp->getHead();
		}
			break;

		case WM_COMMAND:
		{
			NodeEsp temp;
			short tempId;

			if (LOWORD(wParam) == IDC_BN_GUARDAR && HIWORD(wParam) == BN_CLICKED) {

			#pragma region VALIDACIONES_RE

				char textoTemp[150];
				
				//VALIDACIONES ID
				int lenght = GetWindowTextLength(hEcReEID);
				GetWindowText(hEcReEID, textoTemp, lenght + 1);
				tempId = atoi(textoTemp);

				if(atoi(textoTemp) > 32767){
					MessageBox(hWnd, "INGRESE VALORES MENORES A 32,767 EN EL ID", "", MB_ICONEXCLAMATION);
					break;
				}
				if (lenght == 0){
					MessageBox(hWnd, "INGRESE TODOS LOS DATOS POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}
				else {
					if (textoConLetras(textoTemp, lenght)){
						MessageBox(hWnd, "SOLO NUMEROS EN EL ID POR FAVOR", "", MB_ICONEXCLAMATION);
						break;
					}
				}

				//VALIDACIONES NOMBRE
				lenght = GetWindowTextLength(hEcReENom);
				GetWindowText(hEcReENom, textoTemp, lenght + 1);
				strcpy(temp.nombre, textoTemp);

				if (lenght > 30) {
					MessageBox(hWnd, "LIMITE DE CARACTERES ALCANZADO EN EL NOMBRE", "", MB_ICONEXCLAMATION);
					break;
				}

				if (lenght == 0){
					MessageBox(hWnd, "INGRESE TODOS LOS DATOS POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}
				
				if (textoIncorrecto(textoTemp,lenght)){
					break;
				}

				//VALIDACIONES DESCRIPCION
				lenght = GetWindowTextLength(hEcReEDesc);
				GetWindowText(hEcReEDesc, textoTemp, lenght + 1);
				strcpy(temp.desc, textoTemp);

				if (lenght > 100) {
					MessageBox(hWnd, "LIMITE DE CARACTERES ALCANZADO EN LA DESCRIPCION", "", MB_ICONEXCLAMATION);
					break;
				}

				if (lenght == 0)
				{
					MessageBox(hWnd, "INGRESE TODOS LOS DATOS POR FAVOR", "", MB_ICONEXCLAMATION);
					break;
				}
				
				//USUARIO
				strcpy(temp.usuario, nick);
			#pragma endregion

				//Transferencia de datos a el nuevo modulo de especialidad
				if (!editOrDelEsp) {
					//AGREGAR
					
					if (ListaEsp->findNodeId(tempId) != 0) {
						MessageBox(hWnd, "EL ID YA ESTA SIENDO UTILIZADO POR OTRA ESPECIALIDAD", "", MB_ICONEXCLAMATION);
						break;
					}

					ListaEsp->add(temp, tempId); 
					MessageBox(hWnd, "ESCPECIALIDAD REGISTRADA", "", MB_OK);
					SendMessage(hLbReELista, LB_ADDSTRING, 0, (LPARAM)temp.nombre);
					SendDlgItemMessage(hVentanaRegistroMed, IDC_CB_RM_ESP, CB_ADDSTRING, 0, (LPARAM)temp.nombre);
					SendDlgItemMessage(hVentanaRegistroCita, IDC_CB_RC_ESP, CB_ADDSTRING, 0, (LPARAM)temp.nombre);
					SendDlgItemMessage(hVentanaConsulta, IDC_CB_ESP, CB_ADDSTRING, 0, (LPARAM)temp.nombre);
				}
				else {//EDICION
					auxEsp->setData(temp);
					updateMostrar(ArbolMed.getRaiz());
					SendMessage(hLbReMLista, LB_RESETCONTENT, 0, 0);
					for (int i = 0; i < ArbolMed.getContMed(); i++)
						SendMessage(hLbReMLista, LB_ADDSTRING, 0, (LPARAM)ArbolMed.getAuxMed()[i]->mostrar);
					MessageBox(hWnd, "ESCPECIALIDAD EDITADA", "", MB_OK);

					SendMessage(hLbReELista, LB_RESETCONTENT, 0, 0);
					SendMessage(hCbReMEsp, CB_RESETCONTENT, 0, 0);
					SendMessage(hCbReCEsp, CB_RESETCONTENT, 0, 0);
					SendMessage(hCbCEsp, CB_RESETCONTENT, 0, 0);

					auxEsp = ListaEsp->getHead();

					SendDlgItemMessage(hVentanaRegistroCita, IDC_CB_RC_ESP, CB_ADDSTRING, 0, (LPARAM)"TODOS");
					SendDlgItemMessage(hVentanaConsulta, IDC_CB_ESP, CB_ADDSTRING, 0, (LPARAM)"TODOS");

					while (auxEsp != NULL) {
						SendMessage(hLbReELista, LB_ADDSTRING, 0, (LPARAM)auxEsp->getData().nombre);
						SendDlgItemMessage(hVentanaRegistroMed, IDC_CB_RM_ESP, CB_ADDSTRING, 0, (LPARAM)auxEsp->getData().nombre);
						SendDlgItemMessage(hVentanaRegistroCita, IDC_CB_RC_ESP, CB_ADDSTRING, 0, (LPARAM)auxEsp->getData().nombre);
						SendDlgItemMessage(hVentanaConsulta, IDC_CB_ESP, CB_ADDSTRING, 0, (LPARAM)auxEsp->getData().nombre);
						auxEsp = auxEsp->getNext();
					}
					SendDlgItemMessage(hVentanaRegistroCita, IDC_CB_RC_ESP, CB_SETCURSEL, SendMessage(hCbReCEsp, CB_FINDSTRING, -1, (LPARAM)"TODOS"), 0);
					SendDlgItemMessage(hVentanaConsulta, IDC_CB_ESP, CB_SETCURSEL, SendMessage(hCbCEsp, CB_FINDSTRING, -1, (LPARAM)"TODOS"), 0);


					EnableWindow(GetDlgItem(hWnd, IDC_BN_BORRAR), FALSE);
					SendMessage(hEcReEID, EM_SETREADONLY, FALSE, 0);
				}

				auxEsp = ListaEsp->getHead();
				SetDlgItemText(hWnd, IDC_EC_RE_ID, "");
				SetDlgItemText(hWnd, IDC_EC_RE_NOM, "");
				SetDlgItemText(hWnd, IDC_EC_RE_DESC, "");
				ShowWindow(GetDlgItem(hWnd, IDC_ST_EDIT), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDC_ST_EDNAME), SW_HIDE);
				editOrDelEsp = false;

				SetDlgItemText(hVentanaConsulta, IDC_STSEM_ID, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_FECHA, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_HORA, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_NCONSULT, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_ESPECIALIDAD, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_MEDICO, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_PACIENTE, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_TELPAC, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_ATENDIDA, " ");
				SetDlgItemText(hVentanaConsulta, IDC_STSEM_MOTIVO, " ");
				SendMessage(hCFotoDocPc, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
				SendMessage(hLbConsulta, LB_RESETCONTENT, 0, 0);
				ShowWindow(hEcCMot, SW_HIDE);
				ShowWindow(GetDlgItem(hVentanaConsulta, IDC_BN_GUARDAR), SW_HIDE);
				ShowWindow(GetDlgItem(hVentanaConsulta, IDC_BN_REG), SW_HIDE);
				SetWindowText(hEcCMot, "");
			}

			if (LOWORD(wParam) == IDC_BN_BORRAR && HIWORD(wParam) == BN_CLICKED) {
				
				bool usado = false;
				for (int i = 0; i < ArbolMed.getContMed(); i++) {
					if (ArbolMed.getAuxMed()[i]->especialidad == auxEsp->getId()) {
						usado = true;
						break;
					}
				}
				if (usado) {
					MessageBox(hWnd, "NO SE PUEDE ELIMINAR LA ESPECIALIDAD, ES PARTE DEL REGISTRO DE UN MEDICO", "", MB_ICONEXCLAMATION);
					break;
				}

				ListaEsp->deleteNode(auxEsp);

				MessageBox(hWnd, "ESPECIALIDAD ELIMINADA", "", MB_OK);
				auxEsp = ListaEsp->getHead();

				SendMessage(hLbReELista, LB_RESETCONTENT, 0, 0);
				SendMessage(hCbReMEsp, CB_RESETCONTENT, 0, 0);
				SendMessage(hCbReCEsp, CB_RESETCONTENT, 0, 0);
				SendMessage(hCbCEsp, CB_RESETCONTENT, 0, 0);

				SendDlgItemMessage(hVentanaRegistroCita, IDC_CB_RC_ESP, CB_ADDSTRING, 0, (LPARAM)"TODOS");
				SendDlgItemMessage(hVentanaConsulta, IDC_CB_ESP, CB_ADDSTRING, 0, (LPARAM)"TODOS");

				while (auxEsp != NULL)
				{
					SendMessage(hLbReELista, LB_ADDSTRING, 0, (LPARAM)auxEsp->getData().nombre);
					SendDlgItemMessage(hVentanaRegistroMed, IDC_CB_RM_ESP, CB_ADDSTRING, 0, (LPARAM)auxEsp->getData().nombre);
					SendDlgItemMessage(hVentanaRegistroCita, IDC_CB_RC_ESP, CB_ADDSTRING, 0, (LPARAM)auxEsp->getData().nombre);
					SendDlgItemMessage(hVentanaConsulta, IDC_CB_ESP, CB_ADDSTRING, 0, (LPARAM)auxEsp->getData().nombre);
					auxEsp = auxEsp->getNext();
				}
				SendDlgItemMessage(hVentanaRegistroCita, IDC_CB_RC_ESP, CB_SETCURSEL, SendMessage(hCbReCEsp, CB_FINDSTRING, -1, (LPARAM)"TODOS"), 0);
				SendDlgItemMessage(hVentanaConsulta, IDC_CB_ESP, CB_SETCURSEL, SendMessage(hCbCEsp, CB_FINDSTRING, -1, (LPARAM)"TODOS"), 0);
				auxEsp = ListaEsp->getHead();

				EnableWindow(GetDlgItem(hWnd, IDC_BN_BORRAR), FALSE);
				SendMessage(hEcReEID, EM_SETREADONLY, FALSE, 0);
				editOrDelEsp = false;
				SetDlgItemText(hWnd, IDC_EC_RE_ID, "");
				SetDlgItemText(hWnd, IDC_EC_RE_NOM, "");
				SetDlgItemText(hWnd, IDC_EC_RE_DESC, "");
				ShowWindow(GetDlgItem(hWnd, IDC_ST_EDIT), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDC_ST_EDNAME), SW_HIDE);
			}

			if (LOWORD(wParam) == IDC_LB_ESP && HIWORD(wParam) == LBN_SELCHANGE) {
				EnableWindow(GetDlgItem(hWnd, IDC_BN_BORRAR), TRUE);
				int selIndex = (int)SendMessage(hLbReELista, LB_GETCURSEL, 0, 0);
				char nombreEB[30];
				SendMessage(hLbReELista, LB_GETTEXT, (WPARAM)selIndex, (LPARAM)nombreEB);
				auxEsp = ListaEsp->getHead();
				while (strcmp(auxEsp->getData().nombre, nombreEB) != 0)
				{
					auxEsp = auxEsp->getNext();
				}
				ShowWindow(GetDlgItem(hWnd, IDC_ST_EDIT), SW_SHOW);
				ShowWindow(GetDlgItem(hWnd, IDC_ST_EDNAME), SW_SHOW);
				SetDlgItemText(hWnd, IDC_ST_EDNAME, auxEsp->getData().usuario);
				char idTemp[5];
				_itoa(auxEsp->getId(), idTemp, 10);
				SetDlgItemText(hWnd, IDC_EC_RE_ID, idTemp);
				SetDlgItemText(hWnd, IDC_EC_RE_NOM, auxEsp->getData().nombre);
				SetDlgItemText(hWnd, IDC_EC_RE_DESC, auxEsp->getData().desc);
				SendMessage(hEcReEID, EM_SETREADONLY, TRUE, 0);
				editOrDelEsp = true;
			}

			if (LOWORD(wParam) == ID_BM_RP_CONSUL && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaRegistroEsp, SW_HIDE);
				ShowWindow(hVentanaConsulta, SW_SHOW);
			}
			if (LOWORD(wParam) == ID_BM_R_MEDICO && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaRegistroEsp, SW_HIDE);
				ShowWindow(hVentanaRegistroMed, SW_SHOW);
			}
			if (LOWORD(wParam) == ID_BM_R_PACIENTE && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaRegistroEsp, SW_HIDE);
				ShowWindow(hVentanaRegistroPac, SW_SHOW);
			}
			if (LOWORD(wParam) == ID_BM_R_CITA && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaRegistroEsp, SW_HIDE);
				ShowWindow(hVentanaRegistroCita, SW_SHOW);
			}
		}
			break;
		case WM_DESTROY:
			PostQuitMessage(10);
			break;
		case WM_CLOSE:
			ShowWindow(hWnd, SW_HIDE);
			hVentanaCancelar = hVentanaRegistroEsp;
			ShowWindow(hVentanaSalir, SW_SHOW);
			//DestroyWindow(hWnd);
			break;
		}
		return FALSE;
	}


	//MENSAJE SALIR
	BOOL CALLBACK funcionSalir
	(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

		switch (msg) {
		case WM_INITDIALOG:
			
			break;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK && HIWORD(wParam) == BN_CLICKED) {
				delete ListaEsp;
				delete ListaPac;
				delete ListaCita;
				DestroyWindow(hWnd);
			}
			if (LOWORD(wParam) == ID_CANCELAR && HIWORD(wParam) == BN_CLICKED) {
				ShowWindow(hVentanaSalir, SW_HIDE);
				ShowWindow(hVentanaCancelar, SW_SHOW);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(10);
			break;
		case WM_CLOSE:
			//DestroyWindow(hWnd);
			break;
		}
		return FALSE;
	}


#pragma endregion

#pragma region FUNCIONES

	void cargarID() {
		//CARGADO DE CONTRASEÑA
		archivo.open("ID.txt", ios::in | ios::ate);

		if (!archivo.is_open()) {
			MessageBox(0, "NO SE HA ENCONTRADO UN ARCHIVO CON INFORMACION DEL USUARIO", "", MB_ICONEXCLAMATION);
			return;
		}
		int size = archivo.tellg();

		if (size == 0) { 
			MessageBox(0, "EL ARCHIVO DE USUARIOS ESTA VACIO", "", MB_ICONEXCLAMATION);
			return;
		}
		
		archivo.seekg(0);
		for (int i = 0; i < 5; i++)
		{
			archivo.getline(arrUsuarios[i].nick, 30);
			archivo.getline(arrUsuarios[i].nombre, 50);
			archivo.getline(arrUsuarios[i].password, 30);
		}
		
		archivo.close();
		MessageBox(0, "CARGADO DE USUARIOS EXITOSO", "", MB_OK);
	}

	bool idCmp(char temp[]) {
		for (int i = 0; i < 5; i++) {
			if (strcmp(arrUsuarios[0].nick, "") == 0)
			{
				return true;
			}
			if (strcmp(temp, arrUsuarios[i].nick) == 0)
			{
				strcpy(nomUsuario, arrUsuarios[i].nombre);
				strcpy(nick, arrUsuarios[i].nick);
				return false;
			}
		}
		return true;
	}

	bool passCmp(char temp[]) {
		for (int i = 0; i < 5; i++) {
			if (strcmp(arrUsuarios[0].password, "") == 0)
			{
				return true;
			}
			if (strcmp(temp, arrUsuarios[i].password) == 0)
			{
				return false;
			}
		}
		return true;
	}

	bool textoConLetras(char strTemp[100], int longitud)
	{
		for (int i = 0; i < longitud; i++)
		{
			if (isdigit(strTemp[i]) == false)
				return true;
		}
		return false;
	}

	bool textoIncorrecto(char strTemp[100], int longitud) {
	
		int i = 0;
		int contEsp = 0;
		bool rtn = false;

		if (strTemp[0] == ' ')
		{
			MessageBox(0, "NO SE ADMITEN ESPACIOS AL INICIO DEL NOMBRE", "", MB_ICONEXCLAMATION);
			return true;
		}
		for (i = 0; i < longitud; i++)
		{
			if (strTemp[i] == -15 || isalpha(strTemp[i]) || strTemp[i] == ' ')
			{
				if (strTemp[i] == ' ')
					contEsp++;
				if (strTemp[i + 1] == -15 || isgraph(strTemp[i + 1]))
					contEsp = 0;
				else
					contEsp++;
				if (contEsp > 1)
				{
					MessageBox(0, "NO SE EXCEDA CON LOS ESPACIOS EN EL NOMBRE POR FAVOR", "", MB_ICONEXCLAMATION);
					return true;
				}
			}
			else
			{
				MessageBox(0, "SOLO SE ADMITEN LETRAS EN EL NOMBRE", "", MB_ICONEXCLAMATION);
				return true;
			}
		}
		return rtn;
	}

	void updateMostrar(NodeMed* node) {
		if (node != NULL) {
			if (node->especialidad == auxEsp->getId()) {
				char chTemp[10];
				_itoa(node->cedula, chTemp, 10);
				strcpy(node->mostrar, chTemp);
				strcat(node->mostrar, " ");
				strcat(node->mostrar, node->nombre);
				strcat(node->mostrar, " ");
				strcat(node->mostrar, auxEsp->getData().nombre);
			}
			updateMostrar(node->izq);
			updateMostrar(node->der);
		}
	}

	void ordenarPacApe() {
		Node<NodePac>* temp = NULL;
		Node<NodePac>* temp2 = NULL;
		Node<NodePac> temporal;
		temp = ListaPac->getHead();

		while (temp->getNext() != NULL) {

			temp2 = temp->getNext();
			while (temp2 != NULL) {
				if (strcmp(temp2->getData().apellido, temp->getData().apellido) < 0) {
					temporal.setData(temp->getData());
					temporal.setId(temp->getId());
					temp->setData(temp2->getData());
					temp->setId(temp2->getId());
					temp2->setData(temporal.getData());
					temp2->setId(temporal.getId());
				}
				temp2 = temp2->getNext();
			}
			temp = temp->getNext();
		}
	}

	template <typename U>
	void agregarEspacios(U data, short dig, short digTotal) { 								
		if (!reporte.is_open()) { 
			MessageBox(0, "ERROR, ARCHIVO NO ABIERTO", " ", MB_ICONERROR);
			return;
		}

		for (int k = 0; k < (digTotal - dig); k++) {
			reporte << " ";
			if (k == (digTotal - dig) / 2)
				reporte << data;
		}
	}

	void crearArchivoCita(NodeCita temp, short tempId, NodeMed* M) {
		
		char textoTemp[130];

		strcpy(textoTemp, "Citas\\");
		strcat(textoTemp, temp.mostrar);
		strcat(textoTemp, ".txt");

		reporte.open(textoTemp, ios::out | ios::trunc);
		reporte << "ID:" << tempId << endl;
		reporte << "FECHA: " << temp.fecha.wYear << "/";
		if (temp.fecha.wMonth < 10)
			reporte << "0";
		reporte << temp.fecha.wMonth << "/";
		if (temp.fecha.wDay < 10)
			reporte << "0";
		reporte << temp.fecha.wDay << endl << "HORA: ";
		if (temp.hora < 10)
			reporte << "0";
		reporte << temp.hora << ":";
		if (temp.min < 10)
			reporte << "0";
		reporte << temp.min << endl;

		reporte << "MEDICO: " << M->nombre << endl;
		reporte << "ESPECIALIDAD: " << ListaEsp->findNodeId(M->especialidad)->getData().nombre << endl;
		reporte << "NO. CONSULTORIO: " << M->noCon << endl;
		
		Node<NodePac>* aP;
		aP = ListaPac->findNodeId(temp.paciente);
		reporte << "PACIENTE: " << aP->getData().nombre << " " << aP->getData().apellido << endl;
		reporte << "TELEFONO: " << aP->getData().tel << endl;
		reporte << "CANCELADA: ";
		if (temp.cancelada)
			reporte << "SI" << endl << "MOTIVO: " << temp.motCanc << endl;
		else
			reporte << "NO" << endl;

		reporte << "CITA ATENDIDA:";
		if (citaPasada(temp.fecha, temp.hora, temp.min) && !(temp.cancelada))
			reporte << "SI";
		else
			reporte << "NO";

		reporte.close();
	}

	bool citaPasada(SYSTEMTIME temp, short tempH, short tempM) {
		if (temp.wYear < tiempoActual->tm_year + 1900)
			return true;
		if (temp.wYear == tiempoActual->tm_year + 1900 && temp.wMonth < tiempoActual->tm_mon + 1)
			return true;
		if (temp.wYear == tiempoActual->tm_year + 1900 && temp.wMonth == tiempoActual->tm_mon + 1 && temp.wDay < tiempoActual->tm_mday)
			return true;
		if (temp.wYear == tiempoActual->tm_year + 1900 && temp.wMonth == tiempoActual->tm_mon + 1 && temp.wDay == tiempoActual->tm_mday && tempH < tiempoActual->tm_hour)
			return true;
		if (temp.wYear == tiempoActual->tm_year + 1900 && temp.wMonth == tiempoActual->tm_mon + 1 && temp.wDay == tiempoActual->tm_mday && tempH == tiempoActual->tm_hour&& tempM < tiempoActual->tm_min)
			return true;

		return false;
	}

	void titleCreator(char* textoTemp) {
		char fecha[10];
		GetCurrentDirectory(MAX_PATH, _path);
		strcat(_path, "\\");

		strcpy(textoTemp, "Reportes\\");

		if (tiempoActual->tm_mday < 10)
			strcat(textoTemp, "0");
		_itoa(tiempoActual->tm_mday, fecha, 10);
		strcat(textoTemp, fecha);
		strcat(textoTemp, "-");

		if (tiempoActual->tm_mon + 1 < 10)
			strcat(textoTemp, "0");
		_itoa(tiempoActual->tm_mon + 1, fecha, 10);
		strcat(textoTemp, fecha);
		strcat(textoTemp, "-");

		_itoa(tiempoActual->tm_year + 1900, fecha, 10);
		strcat(textoTemp, fecha);
		strcat(textoTemp, " ");

		if (tiempoActual->tm_hour < 10)
			strcat(textoTemp, "0");
		_itoa(tiempoActual->tm_hour, fecha, 10);
		strcat(textoTemp, fecha);
		strcat(textoTemp, "-");

		if (tiempoActual->tm_min < 10)
			strcat(textoTemp, "0");
		_itoa(tiempoActual->tm_min, fecha, 10);
		strcat(textoTemp, fecha);
		strcat(textoTemp, " ");

		strcat(textoTemp, "CONSULTA ");
	}

	void addNodeInfo(Node<NodeCita>* auxC) {
		if (!reporte.is_open()) {
			MessageBox(0, "ERROR, ARCHIVO NO ABIERTO", " ", MB_ICONERROR);
			return;
		}

		reporte << "ID:" << auxC->getId() << endl;
		reporte << "FECHA: " << auxC->getData().fecha.wYear << "/";
		if (auxC->getData().fecha.wMonth < 10)
			reporte << "0";
		reporte << auxC->getData().fecha.wMonth << "/";
		if (auxC->getData().fecha.wDay < 10)
			reporte << "0";
		reporte << auxC->getData().fecha.wDay << endl << "HORA: ";
		if (auxC->getData().hora < 10)
			reporte << "0";
		reporte << auxC->getData().hora << ":";
		if (auxC->getData().min < 10)
			reporte << "0";
		reporte << auxC->getData().min << endl;

		NodeMed* auxM3;
		auxM3 = ArbolMed.findNodeCed(ArbolMed.getRaiz(), auxC->getData().medico);
		reporte << "MEDICO: " << auxM3->nombre << endl;
		reporte << "ESPECIALIDAD: " << ListaEsp->findNodeId(auxM3->especialidad)->getData().nombre << endl;
		reporte << "NO. CONSULTORIO: " << auxM3->noCon << endl;

		Node<NodePac>* auxPac2;
		auxPac2 = ListaPac->findNodeId(auxC->getData().paciente);
		reporte << "PACIENTE: " << auxPac2->getData().nombre << " " << auxPac2->getData().apellido << endl;
		reporte << "TELEFONO: " << auxPac2->getData().tel << endl;

		reporte << "CANCELADA: ";
		if (auxC->getData().cancelada)
			reporte << "SI" << endl << "MOTIVO: " << auxC->getData().motCanc << endl;
		else
			reporte << "NO" << endl;

		reporte << "CITA ATENDIDA:";
		if (citaPasada(auxC->getData().fecha, auxC->getData().hora, auxC->getData().min) && !(auxC->getData().cancelada))
			reporte << "SI" << endl << endl;
		else
			reporte << "NO" << endl << endl;
	}

	void copyCita(NodeCita* des, NodeCita ori) {
		des->paciente = ori.paciente;
		des->medico = ori.medico;
		des->fecha = ori.fecha;
		strcpy(des->tel, ori.tel);
		des->hora = ori.hora;
		des->min = ori.min;
		strcpy(des->usuario, ori.usuario);
		strcpy(des->mostrar, ori.mostrar);
		strcpy(des->motCanc, ori.motCanc);
		des->cancelada = ori.cancelada;
	}

	void updateCitaMostrar(Node<NodeCita>* aux) {
		if (auxM == 0)
			return;
		
		NodeCita temp;
		char textoTemp[120];

		copyCita(&temp, aux->getData());

		_itoa(temp.fecha.wYear, temp.mostrar, 10);
		strcat(temp.mostrar, "-");

		if (temp.fecha.wMonth < 10)
			strcat(temp.mostrar, "0");
		_itoa(temp.fecha.wMonth, textoTemp, 10);
		strcat(temp.mostrar, textoTemp);
		strcat(temp.mostrar, "-");

		if (temp.fecha.wDay < 10)
			strcat(temp.mostrar, "0");
		_itoa(temp.fecha.wDay, textoTemp, 10);
		strcat(temp.mostrar, textoTemp);
		strcat(temp.mostrar, " ");

		if (temp.hora < 10)
			strcat(temp.mostrar, "0");
		_itoa(temp.hora, textoTemp, 10);
		strcat(temp.mostrar, textoTemp);
		strcat(temp.mostrar, "-");

		if (temp.min < 10)
			strcat(temp.mostrar, "0");
		_itoa(temp.min, textoTemp, 10);
		strcat(temp.mostrar, textoTemp);
		strcat(temp.mostrar, " CONS. ");

		_itoa(ArbolMed.findNodeCed(ArbolMed.getRaiz(), temp.medico)->noCon, textoTemp, 10);
		strcat(temp.mostrar, textoTemp);
		strcat(temp.mostrar, " ");

		strcat(temp.mostrar, ListaPac->findNodeId(temp.paciente)->getData().nombre);
		strcat(temp.mostrar, " ");
		strcat(temp.mostrar, ListaPac->findNodeId(temp.paciente)->getData().apellido);

		aux->setData(temp);

		crearArchivoCita(aux->getData(), aux->getId(), ArbolMed.findNodeCed(ArbolMed.getRaiz(), temp.medico));

		GetCurrentDirectory(MAX_PATH, _path);
		strcat(_path, "\\");
		strcat(_path, temp.mostrar);
		strcat(_path, ".txt");

		MessageBox(0, _path, "ARCHIVO DE CITA ACTUALIZADO", MB_OK);
	}


#pragma endregion