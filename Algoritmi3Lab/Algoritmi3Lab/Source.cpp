#include <Windows.h>
#include <wchar.h>
#include <string>
#include <vector>
#include "resource.h"
using namespace std;
LRESULT CALLBACK WndProc(HWND HWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProc2(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
bool vichek[5];
bool uichek[5];
void GlanutZnacheniaPola();
void Stoimost_final();
void ProvercaNaBalans();
void Podstanovka_znacheniy();
void ProverkanaViriBalansirovka();
void podstanovkaViCi();
wchar_t* buf5 = new wchar_t;
wchar_t suma5[300] = L"Цикл не построился, выходим: \n";
void ProvercaViCiDlaPustix(int col,int row);
void PerestanovkaZnacheniy();
int StroimChikl(int col, int row, int startcol, int startrow);
bool Chek1[5];
bool Chek2[5];
int Maini();
int peremenna = 0;
int tempcol = 0;
int temprow = 0;
int schetchik=0;
int na=0;
int nb=0;
int Vi[5];
int Ui[5];
int(*CiJ)[5]=new int[5][5];
int* mass_a=new int[5];//Значения a
int* mass_b=new int[5];//Значения b
int(*pola)[5] = new int[5][5];
int Bassize=0;
int massciclkcol[10];
int massciclrow[10];
int colvo = 0;
int raz = 0;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lcCmdLine, int nCmdShow) {
	const wchar_t class_name[] = L"main_window";
	const wchar_t app_tile[] = L"Алгоритм Транспортной задачи";
	WNDCLASS wc = {};
	HWND hWnd;
	MSG msg;
	memset(&wc, 0, sizeof(wc));
	wc.lpszClassName = class_name;
	wc.lpfnWndProc = &WndProc;
	wc.style = WS_DISABLED;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_ERROR);
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	RegisterClass(&wc);
	hWnd = CreateWindow(class_name, app_tile, WS_OVERLAPPEDWINDOW, 20, 20, 500, 500, NULL, NULL, hInstance, NULL);
	if (!hWnd) {
		MessageBox(NULL, (WCHAR*)L"Create: error", app_tile, MB_OK | MB_ICONSTOP);
		return FALSE;
	}
	HWND h_button = CreateWindow(L"button", L"Посчитать и вывести результат", WS_CHILD | WS_VISIBLE | BS_FLAT, 20, 60, 350, 100, hWnd, (HMENU)1, NULL, NULL);
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_COMMAND: {
		HINSTANCE hDLL;
		UINT id = LOWORD(wParam);
		UINT code = HIWORD(wParam);
		HWND h = (HWND)lParam;
		if (id == 1 && code == BN_CLICKED) {
			HWND normal = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);
			ShowWindow(normal, SW_SHOWNORMAL);
		}
	}break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_INITDIALOG: {
		return TRUE;
	}
	case WM_COMMAND: {
		int id = LOWORD(wParam);
		int code = HIWORD(wParam); // Код уведомления
		if (id == IDCANCEL) {
			DestroyWindow(hDlg);
			return TRUE;
		}
		if (id == IDOK) {
			wchar_t* buf= new wchar_t;
			GetDlgItemText(hDlg, IDC_EDIT1, buf, 1000);
			na = _wtoi(buf);
			GetDlgItemText(hDlg, IDC_EDIT2, buf, 1000);
			nb = _wtoi(buf);
			if(na!=0 && nb!=0){
			DestroyWindow(hDlg);
			HWND normal2 = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), NULL, DlgProc2);
			ShowWindow(normal2, SW_SHOWNORMAL);
			return TRUE;
			}
			else MessageBox(hDlg, L"Что-то не так с введенными данными, пожалуйста повторите попытку", L"Ошибка", NULL);
		}
		break;
	};
	case WM_CLOSE: {
		PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
		return TRUE;
	};
	}
	return FALSE;
}
BOOL CALLBACK DlgProc2(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_INITDIALOG: {
		return TRUE;
	}
	case WM_COMMAND: {
		int id = LOWORD(wParam);
		int code = HIWORD(wParam); // Код уведомления
		if (id == IDCANCEL) {
			DestroyWindow(hDlg);
			return TRUE;
		}
		if (id == IDOK) {
			wchar_t* buf = new wchar_t;
			wchar_t suma[300] = L"Поставщики: \n";
			int col = 0;
			for (int i = 0; i < 10; i++) {
				GetDlgItemText(hDlg, IDC_EDIT1+i, buf, 1000);
				if (i < na) {
					mass_a[col] = _wtoi(buf);
					wcscat_s(suma, buf);
					wcscat_s(suma, L" ");
					col++;
				}
				else if (i > 4 && i <= 4+nb) {
					if (i == 5) {
					wcscat_s(suma, L"\n");
					wcscat_s(suma, L"Потребители: \n");
					col = 0;
					}
					mass_b[col] = _wtoi(buf);
					wcscat_s(suma, buf);
					wcscat_s(suma, L" ");
					col++;
				}
			}
			int row = 0;
			col = 0;
			wcscat_s(suma, L"\n");
			wcscat_s(suma, L"Cij: \n");
			for (int i = 0; i < 25; i++) {
				GetDlgItemText(hDlg, IDC_EDIT51 + i, buf, 1000);
				if(col<nb && row<na){
				CiJ[row][col]= _wtoi(buf);
				wcscat_s(suma, L" ");
				wcscat_s(suma, buf);
				}
					col++;
				if (col%5 == 0 && col!=0) {
					row++;
					wcscat_s(suma, L"\n");
					col = 0;
				}
			}
			if (na != 0) {
				MessageBox(NULL, (WCHAR*)suma, L"Результат добавления", NULL);
				DestroyWindow(hDlg);
				int col = 0;
				int row = 0;
				Vi[0] = 0;
				Ui[0] = 0;
				for (int i = 0; i < (na + 1) * 5; i++) {
					pola[row][col] = -1;
					col++;
					if (col % 5 == 0 && col != 0) {
						col = 0;
						row++;
						Vi[row] = 0;
						Ui[row] = 0;
					}
				}
				Podstanovka_znacheniy();
				int tempa = na;
				int tempb = nb;
				ProvercaNaBalans();
				if (na>tempa || nb>tempb)
				Podstanovka_znacheniy();
				ProverkanaViriBalansirovka();
				while (Maini() != 1) { peremenna++; };
				col = 0;
				row = 0;
				for (int i = 0; i < na * 5; i++) {
					if (col < nb && row < na) {
						if (pola[row][col] == -2)pola[row][col] = 0;
					}
					col++;
					if (col % 5 == 0 && col != 0) {
						col = 0;
						row++;
					}
				}
				MessageBox(NULL, L"Оптимальный план найден", L"Решение найдено", NULL);
				GlanutZnacheniaPola();
				Stoimost_final();
				return TRUE;
			}
			else MessageBox(hDlg, L"Что-то не так с введенными данными, пожалуйста повторите попытку", L"Ошибка", NULL);
		}
		break;
	};
	case WM_CLOSE: {
		PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
		return TRUE;
	};
	}
	return FALSE;
}
int Maini() {
	int col = 0;
	int row = 0;
	wchar_t* buf2 = new wchar_t;
	wchar_t suma2[300] = L"Vij: \n";
	for (int i = 0; i < 5; i++) {
		Chek2[i] = false;
		Chek1[i] = false;
		uichek[i] = false;
		vichek[i] = false;
	}
	peremenna = 0;
	podstanovkaViCi();
	for (int i = 0; i < nb;i++){
	_itow(Vi[i], buf2, 10);
	wcscat_s(suma2, buf2);
	wcscat_s(suma2, L" ");
	}
	wcscat_s(suma2, L"\n Uij \n");
	for (int i = 0; i < na; i++) {
		_itow(Ui[i], buf2, 10);
		wcscat_s(suma2, buf2);
		wcscat_s(suma2, L" ");
	}
	MessageBox(NULL, (WCHAR*)suma2, L"Результат добавления 2", NULL);
	tempcol = 0;
	temprow = 0;
	ProvercaViCiDlaPustix(col, row);
	if (tempcol == -1 && temprow == -1)return 1;
	wchar_t* buf3 = new wchar_t;
	wchar_t suma3[300] = L"Ячейка для изменения: \n";
	_itow(temprow, buf3, 10);
	wcscat_s(suma3, buf3);
	wcscat_s(suma3, L" ");
	_itow(tempcol, buf3, 10);
	wcscat_s(suma3, buf3);
	MessageBox(NULL, (WCHAR*)suma3, L"Результат добавления 3", NULL);
	for (int i = 0; i < 5; i++) {
		Chek2[i] = false;
		Chek1[i] = false;
	}
	schetchik = 0;
	int k=StroimChikl(tempcol, temprow, tempcol, temprow);
	wcscat_s(suma5, L"END -----\n");
	if ( k== 1) {
	wchar_t* buf4 = new wchar_t;
	wchar_t suma4[300] = L"Цикл построился: \n";
	for (int i = 0; i < schetchik; i++) {
		_itow(massciclrow[i], buf4, 10);
		wcscat_s(suma4, buf4);
		wcscat_s(suma4, L" ");
		_itow(massciclkcol[i], buf4, 10);
		wcscat_s(suma4, buf4);
		wcscat_s(suma4, L"\n");
	}
	MessageBox(NULL, (WCHAR*)suma4, L"Построение цикла", NULL);
	}
	else {
		MessageBox(NULL, (WCHAR*)suma5, L"Ошибка с построением цикла", NULL);
		return 0 ;
	}
	GlanutZnacheniaPola();
	PerestanovkaZnacheniy();
	return 0;
}
void ProvercaNaBalans(){
	int sum1 = 0;
	int sum2 = 0;
	for (int i = 0; i < na; i++) {
		sum1 += mass_a[i];
	}
	for (int i = 0; i < nb; i++) {
		sum2 += mass_b[i];
	}
	if (sum1 > sum2) { mass_b[nb] = sum1 - sum2;
	nb++;
	for (int i = 0; i < na; i++)
		CiJ[i][nb-1] = 0;
	}
	if (sum1 < sum2) {
		mass_a[na] = sum2 - sum1;
		na++;
		for (int i = 0; i < nb; i++)
			CiJ[na - 1][i] = 0;
	}
}
void Podstanovka_znacheniy() {
	int rowmin = 0;
	int colmin = 0;
	int col = 0;
	int row = 0;
	int max = 0;
	for (int b = 0; b < na * 5; b++) {
		if (col < nb && row < na){
		if (max <= CiJ[row][col])max = CiJ[row][col];
		}
		col++;
		if (col % 5 == 0 && col != 0) {
			col = 0;
			row++;
		}
	}
	col = 0;
	row = 0;
	for (int i = 0; i < na * 5; i++) {
		int min = max;
		int sum = 0;
		int sum2 = 0;
		int col2 = 0;
		int row2 = 0;
		for (int k = 0; k < na * 5; k++) {
			if (col2 < nb && row2 < na) {
				if (pola[row2][col2]==-1 && min >= CiJ[row2][col2]) {
					min = CiJ[row2][col2];
					rowmin = row2;
					colmin = col2;
				}
			}
			col2++;
			if (col2 % 5 == 0 && col2 != 0) {
				col2 = 0;
				row2++;
			}
		}
		if (col < nb && row < na){
		for (int m = 0; m < nb; m++) {
			if (pola[rowmin][m] >= 0) sum += pola[rowmin][m];
			else sum += 0;
		}
		for (int m2 = 0; m2 < na; m2++) {
			if (pola[m2][colmin] >= 0) sum2 += pola[m2][colmin];
			else sum2 += 0;
		}
		if (sum != mass_a[rowmin] && sum2 != mass_b[colmin]) {
			if ((mass_a[rowmin]-sum) > (mass_b[colmin]-sum2))pola[rowmin][colmin]=(mass_b[colmin] - sum2);
			else pola[rowmin][colmin] = (mass_a[rowmin] - sum);
		}
		else pola[rowmin][colmin] = -2;
		}
		col++;
		if (col % 5 == 0 && col!=0) {
			col = 0;
			row++;
		}
	}
}
	void ProverkanaViriBalansirovka() {
		Bassize = 0;
		int col2 = 0;
		int row2 = 0;
		for (int k = 0; k < na * 5; k++) {
			if (col2 < nb && row2 < na && pola[row2][col2]!=-2) Bassize++;
			col2++;
			if (col2 % 5 == 0 && col2 != 0) {
				col2 = 0;
				row2++;
			}
		}
		if (Bassize < (na + nb - 1)) {
			wchar_t* buf = new wchar_t;
			wchar_t suma[300] = L"Vij: \n";
			int col = 0;
			int row = 0;
			while(true){
				col = rand() % 10;
				row = rand() % 10;
				_itow(row, buf, 10);
				wcscat_s(suma, buf);
				wcscat_s(suma, L" ");
				_itow(col, buf, 10);
				wcscat_s(suma, buf);
				wcscat_s(suma, L"\n");
				if(col>=0 && row>=0 && col < nb && row < na && pola[row][col] == -2) {
					pola[row][col] = 0;
					MessageBox(NULL, (WCHAR*)suma, L"Рандом", NULL);
					return;
				}
			}
		}
	}
	void podstanovkaViCi() {
		int col = 0;
		int row = 0;
		for (int i = 0; i<36; i++) {
			if (col < nb && row < na) {
				if (Chek1[col] == false && Chek2[row] == false && pola[row][col] != -2) {
					Vi[col] = CiJ[row][col];
					Ui[row] = 0;
					Chek1[col] = true;
					Chek2[row] = true;
				}
				else if (Chek1[col] == true && Chek2[row] == false && pola[row][col] != -2) {
					Ui[row] = CiJ[row][col] - Vi[col];
					Chek2[row] = true;
				}
				else if (Chek1[col] == false && Chek2[row] == true && pola[row][col] != -2) {
					Vi[col] = CiJ[row][col] - Ui[row];
					Chek1[col] = true;
				}
				else if (Chek1[col] == true && Chek2[row] == true && pola[row][col] != -2) {
					if ((Vi[col] + Ui[row]) != CiJ[row][col]) {
						if((Vi[col] + Ui[row])<CiJ[row][col]){
							if (Vi[col]<Ui[row] && vichek[col] == false) {
								Vi[col] += CiJ[row][col] - (Vi[col] + Ui[row]);
								if (uichek[row] == false)
									vichek[col] = true;
							}
							else { Ui[row] += CiJ[row][col] - (Vi[col] + Ui[row]); 
							if (vichek[col] == false)
								uichek[row] == true;
							}
						}
						else if ((Vi[col] + Ui[row])>CiJ[row][col]){
							if (Vi[col] > Ui[row] && vichek[col] == false) { Vi[col] -= (Vi[col] + Ui[row]) - CiJ[row][col]; 
							if(uichek[row]==false)
							vichek[col] = true;
							}
							else {
								if (vichek[col] == false)
									uichek[row] == true;
								Ui[row] += CiJ[row][col] - (Vi[col] + Ui[row]); }
						}
						for (int i = 0; i < na; i++) {
							if(i!=row)
							Chek2[i] = false;
							if(i!=col)
							Chek1[i] = false;
						}
						peremenna++;
						podstanovkaViCi();
					}
				}
			}
			row++;
			if (row % 5 == 0 && row != 0) {
				row = 0;
				col++;
			}
		}
		return;
	}
	void ProvercaViCiDlaPustix(int col, int row) {
		int maxdelta = 0;
		for (int i = 0; i < 36; i++) {
			if (col < nb && row < na && pola[row][col] == -2) {
				if(((Ui[row]+Vi[col])>CiJ[row][col]) && (Ui[row] + Vi[col]-CiJ[row][col]) > maxdelta){
					maxdelta = Ui[row] + Vi[col] - CiJ[row][col];
					tempcol = col;
					temprow = row;
				}
			}
			row++;
			if (row % 5 == 0 && row != 0) {
				row = 0;
				col++;
			}
		}
		if (maxdelta == 0) {
			tempcol = -1;
			temprow = -1;
		}
		return;
	
	}
	int StroimChikl(int col, int row, int startcol, int startrow) {
		_itow(row, buf5, 10);
		wcscat_s(suma5, buf5);
		wcscat_s(suma5, L" ");
		_itow(col, buf5, 10);
		wcscat_s(suma5, buf5);
		wcscat_s(suma5, L"\n");
	   if(col != startcol || row != startrow){
		   if (col == startcol && Chek1[col] == false) { 
			   massciclrow[schetchik] = row;
			   massciclkcol[schetchik] = col;
			   schetchik++;
			   return 1; }
		   if (row == startrow && Chek2[row] == false) { 
			   massciclrow[schetchik] = row;
			   massciclkcol[schetchik] = col;
			   schetchik++;
			   return 1; }
	   }
		if (Chek1[col] == false) {
			for (int i = 1; row - i >= 0; i++) {
      				Chek1[col] = true;
					if (pola[row-i][col]!=-2){
						if (StroimChikl(col, row - i, startcol, startrow) == 1) { 
							massciclrow[schetchik] = row;
							massciclkcol[schetchik] = col;
							schetchik++;
							return 1; }
					}
					Chek1[col] = false;
				}}
		if (Chek1[col] == false) {
			for (int i = 1; row + i < na; i++) {
					Chek1[col] = true;
					if (pola[row+i][col] != -2) {
						if (StroimChikl(col, row + i, startcol, startrow) == 1) {
							massciclrow[schetchik] = row;
							massciclkcol[schetchik] = col;
							schetchik++;
							return 1; }
					}
					Chek1[col] = false;
			}
		}
		if (Chek2[row] == false){
				for (int i = 1; col - i >= 0; i++) {
						Chek2[row] = true;
						if (pola[row][col-i] != -2){
							if (StroimChikl(col - i, row, startcol, startrow) == 1) { 
								massciclrow[schetchik] = row;
								massciclkcol[schetchik] = col;
								schetchik++;
								return 1; 
							}
						}
						Chek2[row] = false;
				}}
		if (Chek2[row] == false) {
			for (int i = 1; col + i < nb; i++) {
					{Chek2[row] = true;
						if (pola[row][col+i] != -2){
							if (StroimChikl(col + i, row, startcol, startrow) == 1) {
								massciclrow[schetchik] = row;
								massciclkcol[schetchik] = col;
								schetchik++;
								return 1; }
						}
						Chek2[row] = false;
				}
			}}
			return 0;
	}
	void PerestanovkaZnacheniy() {
		wchar_t* buf = new wchar_t;
		wchar_t suma[300] = L"Список изменений: \n";
		temprow=massciclrow[0];
		tempcol=massciclkcol[0];
		int min = pola[massciclrow[0]][massciclkcol[0]];
		bool chet = true;
		for (int i = 0; i < schetchik; i++){
			if (pola[massciclrow[i]][massciclkcol[i]] == -2)pola[massciclrow[i]][massciclkcol[i]] += 2;
			if (chet==true) {
				if(pola[massciclrow[i]][massciclkcol[i]] <= min){
				min = pola[massciclrow[i]][massciclkcol[i]];
				temprow = massciclrow[i];
				tempcol = massciclkcol[i];
				}
				chet = false;
			}
			else chet = true;
		}
		wcscat_s(suma, L"Минимум: (");
		_itow(temprow, buf, 10);
		wcscat_s(suma, buf);
		wcscat_s(suma, L", ");
		_itow(tempcol, buf, 10);
		wcscat_s(suma, buf);
		wcscat_s(suma, L")=");
		_itow(min, buf, 10);
		wcscat_s(suma, buf);
		wcscat_s(suma, L"\n");
		chet = true;
		for (int r = 0; r < schetchik; r++) {
			_itow((pola[massciclrow[r]][massciclkcol[r]]), buf, 10);
			wcscat_s(suma, buf);
			wcscat_s(suma, L"\n"); 
				if (chet==true) {
					pola[massciclrow[r]][massciclkcol[r]] -= min;
					chet = false;
				}
				else {
					chet = true;
					(pola[massciclrow[r]][massciclkcol[r]]) += min; 
			}
			wcscat_s(suma, L"(");
			_itow(massciclrow[r], buf, 10);
			wcscat_s(suma, buf);
			wcscat_s(suma, L", ");
			_itow(massciclkcol[r], buf, 10);
			wcscat_s(suma, buf);
			wcscat_s(suma, L")=");
			_itow((pola[massciclrow[r]][massciclkcol[r]]), buf, 10);
			wcscat_s(suma, buf);
			wcscat_s(suma, L"\n");
		}
		MessageBox(NULL, (WCHAR*)suma, L"Конечная", NULL);
		pola[temprow][tempcol] = -2;
	}
	void GlanutZnacheniaPola() {
		int col = 0;
		int row = 0;
		wchar_t* buf = new wchar_t;
		wchar_t suma[300] = L"Результат: \n";
		for (int i = 0; i < 5 * 5; i++) {
			if (col < nb && row < na) {
				_itow(pola[row][col], buf, 10);
				wcscat_s(suma, buf);
				wcscat_s(suma, L" ");
			}
			col++;
			if (col % 5 == 0) {
				col = 0;
				row++;
				wcscat_s(suma, L"\n");
			}
		}
		MessageBox(NULL, (WCHAR*)suma, L"Поля значений", NULL);
	}
	void Stoimost_final() {
		int sum=0;
		int col = 0;
		int row = 0;
		wchar_t* buf = new wchar_t;
		wchar_t suma[300] = L"Общая стоимость доставки: ";
		for (int i = 0; i < 5 * 5; i++) {
			if (col < nb && row < na) {
				if (pola[row][col] != -2) sum += CiJ[row][col]*pola[row][col];
			}
			col++;
			if (col % 5 == 0) {
				col = 0;
				row++;
			}
		}
		_itow(sum, buf, 10);
		wcscat_s(suma, buf);
		wcscat_s(suma, L" условных единиц");
		MessageBox(NULL, (WCHAR*)suma, L"Результат доставки", NULL);
	}