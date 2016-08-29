//---------------------------------------------------------------------------

#include "TransposCipher.h"


TTransposCipher::TTransposCipher()
{
}

TTransposCipher::~TTransposCipher()
{
}

//---------------------------------------------------------------------------
// Шифрование
AnsiString __fastcall TTransposCipher::Encrypt(AnsiString SrcStr, const int* _pRouteRow, const int* _pRouteCol, AnsiString Extension)
{
    char* Src = SrcStr.c_str();
    int n_src = strlen(Src);        // Длина исходной строки

    int size = ceil(sqrt(n_src));   // Размер двумерного квадратного массива
    int n_dst = pow(size,2);        // Размер строки-результата

    int* pRouteRow = new int[size];
    memcpy(pRouteRow, _pRouteRow, size * sizeof(_pRouteRow));

    int* pRouteCol = new int[size];
    memcpy(pRouteCol, _pRouteCol, size * sizeof(_pRouteCol));


    // Формируем удлиненную строку для этого формируем строку - источник доп.символов
    char* Ext;
    int n_ext;
    if (Extension != "") {
        Ext = Extension.c_str();
        n_ext = strlen(Ext);
    } else {
        Ext = Src;
        n_ext = n_src;
    }

    // Длина новой строки должна быть равна квадрату числа
    char* SrcExt = new char[n_dst+1];
    SrcExt[n_ext] = '\0';
    for (int i = 0; i < n_src; i++) {
        SrcExt[i] = Src[i];
    }
    for (int i= n_src; i < n_dst; i++) {
        SrcExt[i] = Ext[i % n_ext];
    }


    // Формируем укороченный маршрут перестановки по строкам
    int* RouteRow = new int[size];
    for (int i=0; i<size; i++) {
        int minindex = i;
        for (int j = 0; j < size; j++) {    // Поиск минимального элемента
            if (pRouteRow[j] < pRouteRow[minindex]) {
                minindex = j;
            }
        }
        RouteRow[minindex] = i;
        pRouteRow[minindex] = 9999;
    }

    // Формируем укороченный маршрут перестановки по столбцам
    int* RouteCol = new int[size];
    for (int i=0; i < size; i++) {
        int minindex = i;
        for (int j = 0; j < size; j++) {    // Поиск минимального элемента
            if (pRouteCol[j] < pRouteCol[minindex]) {
                minindex = j;
            }
        }
        RouteCol[minindex] = i;
        pRouteCol[minindex] = 9999;
    }

    // Перестановка
    char** TmpArr = new char*[size];    // Промежуточный массив, для хранения между перестановками

    // Преобразование одномерного массива в двумерный массив
    int k = 0;
    for(int i=0; i< size; i++) {
        TmpArr[i] = new char[size];
        for(int j=0; j < size; j++) {
            TmpArr[i][j] = SrcExt[k++];
        }
    }

    // Создаем новый массив для хранения результатов
    char** DstArr = new char*[size];

    // Перестановка строк
    for(int i=0; i < size; i++) {
        DstArr[i] = new char[size];
        for(int j=0; j < size; j++) {
            DstArr[i][j] = TmpArr[RouteRow[i]][j];
        }
    }

    // Меняем местами временный массив с
    char** pTmp = DstArr;
    DstArr = TmpArr;
    TmpArr = pTmp;

    // Перестановка столбцов
    for(int j=0; j < size; j++) {
        for(int i=0; i < size; i++) {
            DstArr[i][j] = TmpArr[i][RouteCol[j]];
        }
    }

    // Преобразование из квадратного массива в линейный
    char* Dst = new char[n_dst+1];
    Dst[n_dst] = '\0';
    k = 0;
    for(int i=0; i < size; i++) {
        for(int j=0; j < size; j++) {
            Dst[k++] = DstArr[i][j];
        }
    }

    AnsiString result = AnsiString(Dst);

    // Очистка памяти
    for(int i=0; i < size; i++) {
        delete []TmpArr[i];
        delete []DstArr[i];
    }

    delete []TmpArr;
    delete []DstArr;    // Тут нужен цикл для очистки массива
    delete Dst;
    delete SrcExt;
    delete []pRouteRow;
    delete []pRouteCol;

    return result;
}

//---------------------------------------------------------------------------
// Расшифровка
AnsiString __fastcall TTransposCipher::Decrypt(AnsiString SrcStr, const int* _pRouteRow, const int* _pRouteCol)
{
    char* SrcExt = SrcStr.c_str();
    int n_dst = strlen(SrcExt);        // Длина исходной строки

    //int n_ext = pow(size,2);        // Новый размер строки
    double tmp_size = sqrt(n_dst);

    int ceil_tmp_size = ceil(tmp_size);
    if (tmp_size != ceil_tmp_size)
        throw Exception("The length of the string is not equal to the square of any number.");

    int size = (int)tmp_size;   // Размер двумерного квадратного массива

    int* pRouteRow = new int[size];
    memcpy(pRouteRow, _pRouteRow, size * sizeof(_pRouteCol));
    int* pRouteCol = new int[size];
    memcpy(pRouteCol, _pRouteCol, size * sizeof(_pRouteCol));

    // Формируем укороченный маршрут перестановки по строкам
    int* RouteRow = new int[size];
    for (int i=0; i < size; i++) {
        //int min = pRouteRow[i];
        int minindex = i;
        for (int j = 0; j < size; j++) {    // Поиск минимального элемента
            if (pRouteRow[j] < pRouteRow[minindex]) {
                //min = pRouteRow[j];
                minindex = j;
            }
        }
        RouteRow[minindex] = i;
        pRouteRow[minindex] = 9999;
    }

    // Формируем укороченный маршрут перестановки по столбцам
    int* RouteCol = new int[size];
    for (int i=0; i < size; i++) {
        int minindex = i;
        for (int j = 0; j < size; j++) {    // Поиск минимального элемента
            if (pRouteCol[j] < pRouteCol[minindex]) {
                minindex = j;
            }
        }
        RouteCol[minindex] = i;
        pRouteCol[minindex] = 9999;
    }

    // Перестановка
    char** TmpArr = new char*[size];    // Промежуточный массив, для хранения между перестановками

    // Преобразование одномерного массива в двумерный массив
    int k = 0;
    for(int i=0; i < size; i++) {
        TmpArr[i] = new char[size];
        for(int j=0; j < size; j++) {
            TmpArr[i][j] = SrcExt[k++];
        }
    }


    // Создаем новый массив для хранения результатов
    char** DstArr = new char*[size];
    for(int i=0; i < size; i++)
        DstArr[i] = new char[size];


    // Перестановка строк
    for(int i=0; i < size; i++) {
        for(int j=0; j < size; j++) {
            DstArr[RouteRow[i]][j] = TmpArr[i][j];
        }
    }


    // Меняем местами временный массив с
    char** pTmp = DstArr;
    DstArr = TmpArr;
    TmpArr = pTmp;

    // Перестановка столбцов
    for(int j=0; j < size; j++) {
        for(int i=0; i < size; i++) {
            DstArr[i][RouteCol[j]] = TmpArr[i][j];
        }
    }


    // Преобразование из квадратного массива в линейный
    char* Dst = new char[n_dst+1];
    Dst[n_dst] = '\0';

    k = 0;
    for(int i=0; i < size; i++) {
        for(int j=0; j < size; j++) {
            Dst[k++] = DstArr[i][j];
        }
    }

    AnsiString result = AnsiString(Dst);

    // Очистка памяти
    for(int i=0; i < size; i++) {
        delete []TmpArr[i];
        delete []DstArr[i];
    }

    delete []TmpArr;
    delete []DstArr;
    delete Dst;
    delete []pRouteRow;
    delete []pRouteCol;

    return result;
}

