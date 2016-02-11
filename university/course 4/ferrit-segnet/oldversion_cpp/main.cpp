#include <iostream>                     // cin/cout.
#include <string>                       // strings
#include <fstream>                      // fin/fout
#include <stdlib.h>                     // System calls.
#include <sstream>                      // Smth for inttostr.
#include <vector>                       // For vector.
#include <algorithm>                    // For vector operations.
#include <boost/numeric/mtl/mtl.hpp>    // For mtl4 (dense*)

using namespace std;

/**********************************************************************************************************/
/*                              _                    _                               _                    */
/* / / /\ \ \ |__   __ _| |_   (_)___     __ _  ___ (_)_ __   __ _     ___  _ __    | |__   ___ _ __ ___  */
/* \ \/  \/ / '_ \ / _` | __|  | / __|   / _` |/ _ \| | '_ \ / _` |   / _ \| '_ \   | '_ \ / _ \ '__/ _ \ */
/*  \  /\  /| | | | (_| | |_   | \__ \  | (_| | (_) | | | | | (_| |  | (_) | | | |  | | | |  __/ | |  __/ */
/*   \/  \/ |_| |_|\__,_|\__|  |_|___/   \__, |\___/|_|_| |_|\__, |   \___/|_| |_|  |_| |_|\___|_|  \___| */
/*                                       |___/               |___/                                        */
/*                                                                                                        */
/*     Эта программа принимает на вход файл сетки *.mphtxt, сгенерируемый в Comsol и преобразует его в    
/* текстовый файл, который надо скормить Matlab. Чуть позже я опишу это подробнее, а пока вот что         
/* надо знать.                                                                                            
/*     # Примечание: пока не преобразует, но будет преобразовывать.                                       
/*     # Зависимости:                                                                                     
/*             * g++                                                                                      
/*             * boost                                                                                    
/*             * mtl4                                                                                     
/*             * matlab
/*             * базовые пакеты ядра типа rm и cat.
/*         Первые две решаются простой установкой пакетов из репов. Последней у меня, по крайней мере,    
/*           в репах нет. Будь внимателен: mtl и mlt - это разные пакеты.                                 
/*         Решается это так:                                                                              
/*             * устанавливаешь boost                                                                     
/*             * скачиваешь mtl4 с http://www.simunova.com/node/145                                       
/*             * распаковываешь mtl4 в куда указывает структура архива. Мне лень собирать пока в пакет,   
/*                      так что да, просто распаковываешь.                                                
/*               На том же сайте есть раздел Installation, может помочь.                                  
/*     # Код читать снизу вверх, без предопредлеления функций: я так люблю.                               
/*     # Это версия, которую лучше никому не показывать. И работать с ней внимательно.                    
/*             * Нет проверки на дурака: программа верит, что входные данные поданы корректно.            
/*             * Нет мозгов для поиска по входному файлу: пользователь делает это самостоятельно.         
/*             * По неопытности автора бывают странные решения. Я хотел бы о них узнать.                  
/*     # Синтаксис использования - ниже, в main ()                                                        
/*     # Код под GNU/GPL                                                                                  
/*     # Автор - homk                                                                                     
/**********************************************************************************************************/




////////////////////////////////////////////////////////////////////////////////////////////////




       /*
        * Функционал по мелочам.                               
        */

string inttostr(int number)
{
        stringstream out;
        out << number;
        return out.str();
}

string floattostr(float number)
{
        stringstream out;
        out << number;
        return out.str();
}

string doubletostr(double number)
{
        stringstream out;
        out << number;
        return out.str();
}



////////////////////////////////////////////////////////////////////////////////////////////////
// Закомментированные процедуры не используются пока что. Удалю перед релизом.



// int finddet(double *x, double *y)
// {

// /********************************************************/
// /*                               1 x1 y1                */
// /* Найти детерминант матрицы A = 1 x2 y2                */
// /*                               1 x3 y3                */
// /********************************************************/
//     double det = 0;
//     double A[3][3];

//     A[0][0] = 1;
//     A[0][1] = 1;
//     A[0][2] = 1;

//     A[1][0] = *x;
//     A[1][1] = *(x + 1);
//     A[1][2] = *(x + 2);

//     A[2][0] = *y;
//     A[2][1] = *(y + 1);
//     A[2][2] = *(y + 2);

//     det = A[1][1] * A[2][2] + A[1][0] * A[2][1] + A[1][2] * A[2][0] - A[1][1] * A[2][0] - A[1][0] * A[2][2] - A[1][2] * A[2][1];

//     return det;
// }




////////////////////////////////////////////////////////////////////////////////////////////////




int show_double_matrix(double *A, int n, int m)
{

       /*
        * Показать матрицу A[n][m].                         
        * Можно передавать сюда как одномерный, так и       
        * двумерный массив.                                 
        * n - количество строк, m - столбцов.               
        *                                                   
        * Возвращает ноль.                                  
        */

        int i = 0;
        int j = 0;

                for (i = 0; i < n; i++)
                {
                        cout << " ";
                                for (j = 0; j < m; j++)
                                {
                                        printf("%8.2F", 5, *(A + i*m + j));
                                }
                        cout << endl;
                }
        return 0;
}




////////////////////////////////////////////////////////////////////////////////////////////////




int multmatr(double *A, double *B, double *R, int n, int m, int k)
{

       /*
        * Умножить матрицу A[n][m] на матрицу B[m][k].     
        * Результат пишется в матрицу R[n][k] по указателю.
        *                                                  
        * Возвращает ноль.                                 
        */

        system("echo -ne \'calling multmatr...\'");

        int i       = 0;
        int j       = 0;
        int r       = 0;
        double tmp  = 0;

           for(i = 0; i < n; i++)
           {
                   for(j = 0; j < k; j++)
                   {
                           tmp = 0;
                                        for(r = 0; r < m; r++)
                                        {
                                                tmp += *(A + i*n + r) * *(B + r*k + j);
                                        }
                                *(R + i*k + j) = tmp;
                        }
                }

        system("echo -e \' done.\'");
        return 0;
}




////////////////////////////////////////////////////////////////////////////////////////////////




int transmatr(double *A, double *B, int n, int m)
{

       /*
        * Транспонировать матрицу A[n][m] в матрицу B[m][n].  
        *                                                     
        * Возвращает ноль.                                    
        */

        system("echo -ne \'calling transmatr...\'");

        int i = 0;
        int j = 0;

                for (i = 0; i < n; i++)
                {
                for (j = 0; j < m; j++)
                {
                        *(B + j*n + i) = *(A + i*m + j);
                }
                }

        system("echo -e \' done.\'");
        return 0;
}




////////////////////////////////////////////////////////////////////////////////////////////////




int findnumbofnodes(string file)
{

       /*
        * Эта функция находит количество узлов.              
        *                                                    
        * Необходимо руками задать startanchor и endanchor   
        * из вашего файла. Это строки, показывающие, где     
        * кончается и начинается раздел с координатами узлов.
        *                                                    
        * Возвращает количество узлов.                       
        */

        system("echo -ne \'calling findnumbofnodes...\'");

        int N               = 0;

        string Str          = "";

        string startanchor  = "# Mesh point coordinates";
        string endanchor    = "3 # number of element types";

        ifstream fin(file.c_str());

                while (getline(fin, Str) && (Str != startanchor))
                {
                        /* nothing*/
                }
                while (getline(fin, Str) && (Str != endanchor))
                {
                        N++;
                }

        fin.close();
        system("echo -ne \' done.\'");
        // N--;
        cout << " Result: N = " << N << endl;
        return N;
}




////////////////////////////////////////////////////////////////////////////////////////////////




int getcoords(string file, double *x, double *y)
{

       /*
        * Эта функция считывает координаты узлов.              
        *                                                      
        * Необходимо руками задать startanchor и endanchor     
        * из вашего файла. Это строки, показывающие, где       
        * кончается и начинается раздел с координатами узлов.  
        *                                                      
        * Возвращает ноль.                                     
        */

        system("echo -ne \'calling getcoords...\'");

        string Str          = "";
        string Stroutx      = "";
        string Strouty      = "";

        string startanchor  = "# Mesh point coordinates";
        string endanchor    = "3 # number of element types";

        bool k              = false;
        int i               = 0;
        int nend            = 0;
        int strlength       = 0;

        ifstream fin(file.c_str());

                while (getline(fin, Str) && (Str != startanchor))
                {
                        /* nothing */
                }

                while (getline(fin, Str) && (Str != endanchor))
                {
                        k = false;
                        strlength = Str.length();
                                for (i = 0; i < strlength; i++)                 ////////
                                {                                               ////
                                        if (Str[i] == ' ')                      //
                                        {                                       // Так как строка имеет вид x0 y0 x1 y1 x2 y2 ...
                                                k = true;                       // я записываю в массивы, чередуя.
                                        }                                       //
                                                                                //
                                        if (k == false)                         //
                                        {                                       //
                                                Stroutx += Str[i];              //
                                        }                                       //
                                        else                                    //
                                        {                                       //
                                                Strouty += Str[i];              //
                                        }                                       //
                                }                                               //
                                                                                //
                        x[nend] = atof(Stroutx.c_str());                        //
                        y[nend] = atof(Strouty.c_str());                        //
                                                                                //
                        Stroutx = "";                                           //
                        Strouty = "";                                           ////
                        nend++;                                                 ////////
                }

        fin.close();
        system("echo -e \' done.\'");
        return 0;
}




////////////////////////////////////////////////////////////////////////////////////////////////




string findsizeGlS(string file)
{
       /*
        * Эта функция находит номера граничных узлов.        
        *                                                    
        * Необходимо руками задать startanchor и endanchor   
        * из вашего файла. Это строки, показывающие, где     
        * кончается и начинается раздел с граничными узлами. 
        *                                                    
        * Возвращает количество граничных узлов.             
        */

        system("echo -ne \'calling findsizeGlS...\'");

        int i               = 0;
        int j               = 0;
        int k               = 0;
        int tmp             = 0;
        int strlength       = 0;

        vector <int> a;

        string endanchor    = "2 # number of parameter values per element";

        string Str          = "";
        string Strout       = "";
        string Out          = "";


        ifstream fin(file.c_str());                                     


        string startanchor  = " # number of elements";                  // Есть три строки, оканчивающиеся так.

            while (getline(fin,Str))
            {
                if (Str.find(startanchor) != -1)
                {
                    i++;
                        if (i == 2)
                        {
                            break;
                        }
                }
            }
        startanchor = Str;                                             // Я беру вторую.

        fin.close();
        fin.open(file.c_str());                                         //////
                                                                        ////
                while (getline(fin, Str) && (Str != startanchor))       // Узнаю, сколько у меня граничных узлов (с повторениями)       4 1
                {                                                       // записано в file.                                             0 3
                        /* nothing */                                   //                                                              1 2
                }                                                       //                                                              2 8
        getline(fin, Str);                                              //                                          Пусть там записано  3 6
                while (getline(fin, Str) && (Str != endanchor))         //                                          что-то типа         6 7
                {                                                       //                                                              7 4
                        Strout = Strout + " " + Str;                    //                                                              8 0
                        k++;                                            //                                                              4 11
                }                                                       ////                                                            11 3
        k = k * 2;                                                      //////



                for (i = 0; i < k; i++)                                 //////
                {                                                       ////
                        a.push_back(0);                                 // Переношу полученную строку в вектор.
                }                                                       // 
        j = 0;                                                          // В указанном примере будет
                                                                        // a = 4 1 0 3 1 2 2 8 3 6 6 7 7 4 8 0 4 11 11 3 
        strlength = Strout.length();                                    //
                for (i = 0; i < strlength; i++)                         // 
                {                                                       // 
                        if (Strout[i] != ' ')                           // 
                        {                                               // 
                                Out += Strout[i];                       // 
                        }                                               // 
                        else                                            // 
                        {                                               // 
                                a[j] = atoi(Out.c_str());               // 
                                j++;                                    // 
                                Out = "";                               // 
                        }                                               ////
                }                                                       //////



        sort(a.begin(), a.end());                               // Сортирую массив за N*logN
                                                                // a = 0 0 1 1 2 2 3 3 3 4 4 4 6 6 7 7 8 8 11 11

        unique(a.begin(), a.end());                             // Удаляю повторы.        Неперезаписанный мусор.
                                                                //                        v v v v v v v v v v  v 
                                                                // a = 0 1 2 3 4 6 7 8 11 4 4 4 6 6 7 7 8 8 11 11 
                                                                //                      ^
                                                                //              Вот где нужно остановиться.
                                                                //
                                                                // Но так как я не понял, как получить иттератор на последний элемент, то

        Strout = "";
        for (i = 0; i < k-1; i++)                       //////
        {                                               ////
                if (a[i] < a[i+1])                      // ищу его самостоятельно.
                {                                       // и формирую массив.
                        Strout += inttostr(a[i]);       //
                        Strout += ",";                  //
                }                                       // 
                else                                    // 
                {                                       // 
                        break;                          // 
                }                                       ////
        }                                               //////

        fin.close();
        system("echo -e \' done.\'");
        cout << Strout << endl;
        return Strout;
}




////////////////////////////////////////////////////////////////////////////////////////////////




int findnumbtr(string file)
{

       /*
        * Эта функция возвращает количество симплексов.   
        *                                                 
        * Необходимо руками задать startanchor и endanchor
        * из вашего файла. Это строки, показывающие, где  
        * кончается и начинается раздел с треугольниками. 
        *                                                 
        * Возвращает количество симплексов.               
        */

        system("echo -ne \'calling findnumbtr...\'");

        int N               = 0;
        int i               = 0;

        string Str          = "";
        string Strout       = "";

        ifstream fin(file.c_str());

        string startanchor  = " # number of elements";                  // Есть три строки, оканчивающиеся так.

            while (getline(fin,Str))
            {
                if (Str.find(startanchor) != -1)
                {
                    i++;
                        if (i == 3)
                        {
                            break;
                        }
                }
            }
        startanchor == Str;                                             // Беру третью.

                for (i = 0; i < Str.length(); i++)
                {
                        if (Str[i] != ' ')
                        {
                                Strout += Str[i];
                        }
                        else
                        {
                                break;
                        }
                }

        N = atoi(Strout.c_str());

        fin.close();
        system("echo -e \' done.\'");
        return N;
}




////////////////////////////////////////////////////////////////////////////////////////////////




string findtrangle(string file, int N)
{

       /*
        * Эта функция возвращает глобальные номера узлов  
        * треугольника с номером N в формате строки       
        *      узел1,узел2,узел3,                         
        *                                                 
        * С запятой в конце, да.                          
        *                                                 
        * Необходимо руками задать startanchor и endanchor
        * из вашего файла. Это строки, показывающие, где  
        * кончается и начинается раздел с треугольниками. 
        */

        // system("echo -ne \'calling findtrangle...\'");

        string endanchor    = "3 # number of parameter values per element";

        string Str          = "";
        string Strout       = "";

        int i               = 0;
        int strlength       = 0;

        ifstream fin(file.c_str());

        string startanchor  = " # number of elements";                  // Есть три строки, оканчивающиеся так.
                                                                        // TODO изменицть функцию так, чтобы она не считата startanchor каждый раз: это бессмысленно.
            while (getline(fin,Str))
            {
                if (Str.find(startanchor) != -1)
                {
                    i++;
                        if (i == 3)
                        {
                            break;
                        }
                }
            }
        startanchor = Str; 

        fin.close();
        fin.open(file.c_str());

                while (getline(fin, Str) && (Str != startanchor))
                {
                        /* nothing */
                }
        getline(fin, Str);
                for (i = 0; i <= N; i++)
                {
                        getline(fin, Str);
                }

        strlength = Str.length();
                for (i = 0; i < strlength; i++)
                {
                        if ((Str[i] >= '0') && (Str[i] <= '9'))
                        {
                                Strout += Str[i];
                        }
                        else
                        {
                                Strout += ",";
                        }
                }

        Strout += ",";                  // Чтобы строка кончалась запятой. Так надо.
        fin.close();
        // system("echo -e \' done.\'");
        return Strout;
}




////////////////////////////////////////////////////////////////////////////////////////////////




int find_locale_matrix(string Str, double *GlX, double *GlY, double *GlS, int GlSn, double *GlT, int GlTn)
{
        /*
         * Эта функция считает матрицы S и T              
         * (матрицы жёсткости и масс) для одного          
         * треугольника.                                  
         * После чего записывает оные в глобальные матрицы
         * GlS и GlT.                                     
         *                                                
         * Возвращает ноль.                               
         */

         // system("echo -ne \'calling find_locale_matrix...\'");

        int a[3];                       // Массив локальной нумерации
        int k                   = 0;    // и его индекс.

        int i                   = 0;
        int j                   = 0;

        vector <double> x(3);
        vector <double> y(3);

        string Strtmp           = "";

        int first_nod           = 0;
        int second_nod          = 1;
        int third_nod           = 2;

        int trianglenod_first   = 0;
        int trianglenod_second  = 1;
        int trianglenod_third   = 2;
        
        int buf                 = 0;

        double A                = 0;        // Площадь симплекса.
        double CTNG             = 0;        // Херь какая-то.
        double T[3][3];                     // Матрица массы.
        double S[3][3];                     // Матрица жесткости.

                for (i = 0; i < Str.length(); i++)              ////////
                {                                               ////
                        if (Str[i] != ',')                      // 
                        {                                       // 
                                Strtmp += Str[i];               // 
                        }                                       // Разбираю Str на глобальные координаты.
                        else                                    // Было Str = "2,3,4,"
                        {                                       // стало a = [2,3,4]
                                a[k] = atoi(Strtmp.c_str());    // Это будут, соответственно, first-, second- и third_nod
                                Strtmp = "";                    // 
                                k++;                            // 
                        }                                       ////
                }                                               ////////

        x[first_nod]  = GlX[a[0]];  // 
        x[second_nod] = GlX[a[1]];  // Координаты узлов симплекса.
        x[third_nod]  = GlX[a[2]];  // 

        y[first_nod]  = GlY[a[0]];  // 
        y[second_nod] = GlY[a[1]];  // 
        y[third_nod]  = GlY[a[2]];  // 

        A = abs(((x[second_nod] - x[first_nod]) * (y[third_nod] - y[first_nod]) - (x[third_nod] - x[first_nod]) * (y[second_nod] - y[first_nod])) / 2); // Площадь треугольника. Надо бы пересчитать через детерминант.
        
                for (i = 0; i < 3; i++)
                for (j = 0; j < 3; j++)
                {
                        S[i][j] = 0;
                        T[i][j] = 0;
                }

                for (i = 0; i < 3; i++)                         ////////
                {                                               ////
                                for (j = 0; j < 3; j++)         // 
                                {                               // Создаю матрицу T.
                                        T[i][j] = A / 12;       // 
                                }                               // 
                        T[i][i] = 2 * T[i][i];                  ////
                }                                               ////////

        GlT[a[0] * GlTn + a[0]] += T[0][0]; ////////
        GlT[a[0] * GlTn + a[1]] += T[0][1]; ////
        GlT[a[0] * GlTn + a[2]] += T[0][2]; // 
        GlT[a[1] * GlTn + a[0]] += T[1][0]; // Записываю её в
        GlT[a[1] * GlTn + a[1]] += T[1][1]; // глобалку GlT.
        GlT[a[1] * GlTn + a[2]] += T[1][2]; // 
        GlT[a[2] * GlTn + a[0]] += T[2][0]; // 
        GlT[a[2] * GlTn + a[1]] += T[2][1]; ////
        GlT[a[2] * GlTn + a[2]] += T[2][2]; ////////

                // Далее я собираю S. Что происходит - хз.

                for (i = 1; i < 3; i++)
                {
                        CTNG = (((x[second_nod] - x[first_nod]) * (x[third_nod] - x[first_nod])) + ((y[second_nod] - y[first_nod]) * (y[third_nod] - y[first_nod]))) / (4 * A);

                        S [trianglenod_second] [trianglenod_second] += CTNG;
                        S [trianglenod_second] [trianglenod_third]  -= CTNG;
                        S [trianglenod_third]  [trianglenod_second] -= CTNG;
                        S [trianglenod_third]  [trianglenod_third]  += CTNG;

                                                   buf = trianglenod_first;
                         trianglenod_first = trianglenod_second;
                        trianglenod_second = trianglenod_third;
                         trianglenod_third = buf;

                               buf = first_nod;
                         first_nod = second_nod;
                        second_nod = third_nod;
                         third_nod = buf;
                }

        GlS[a[0] * GlSn + a[0]] += S[0][0]; ////////
        GlS[a[0] * GlSn + a[1]] += S[0][1]; ////
        GlS[a[0] * GlSn + a[2]] += S[0][2]; // 
        GlS[a[1] * GlSn + a[0]] += S[1][0]; // И записываю её в
        GlS[a[1] * GlSn + a[1]] += S[1][1]; // глобалку GlS.
        GlS[a[1] * GlSn + a[2]] += S[1][2]; // 
        GlS[a[2] * GlSn + a[0]] += S[2][0]; // 
        GlS[a[2] * GlSn + a[1]] += S[2][1]; ////
        GlS[a[2] * GlSn + a[2]] += S[2][2]; ////////

        // system("echo -e \' done.\'");
        return 0;
}




////////////////////////////////////////////////////////////////////////////////////////////////




int make_identity_matrix(double *C, int Cn, int GlSn, int *nodes, int k)
{

       /* 
        * Создать единичную матрицу C[GlSn x Cn].
        */

system("echo -ne \'calling make_identity_matrix...\'");

int buf_height  = 0;
int buf_width   = 0;
int i           = 0;
int j           = 0;
vector <int> local_nodes;

        for (i = 0; i < k; i++)
        {
                local_nodes.push_back(nodes[i]);        // Я хочу работать с вектором, вектор можно сделать только так.
        }

        for (i = 0; i < GlSn; i++)      // Для каждого граничного узла
        {
                if (i <= k)
                {
                        for (j = 0; j <= k; j++)
                        {
                                if (local_nodes.size() > 0)
                                {
                                        if (i == local_nodes[j])      // Если узел лежит на границе
                                        {
                                                buf_height++;
                                                local_nodes.erase(local_nodes.begin());
                                                break;
                                        }
                                        else
                                        {
                                                C[buf_height * Cn + buf_width] = 1;
                                                buf_width++;
                                                buf_height++;
                                                break;
                                        }    
                                }
                        }
                }
                else
                {
                        C[buf_height * Cn + buf_width] = 1;
                        buf_height++;
                        buf_width++;
                }
        }

        system("echo -e \' done.\'");
        return 0;
}




////////////////////////////////////////////////////////////////////////////////////////////////




int call_matlab(double *A, int An, double *B, int Bn)
{
       /*
        * Вообще, можно было бы выполнять внешний скрипт.
        * Однако, я решил, что вариант с внутренним вызовом даст больше гибкости:
        * при внесении изменений надо будет поправить одну функцию, а не целый файл.
        * Плюс можно будет реализовать различные типы вызовов.
        *
        * Str - строка, которую я буду вызывать.
        */
        system("echo -ne \'calling matlab...\'");

        int i                   = 0;
        int j                   = 0;
        string matlabdir        = "/usr/local/MATLAB/R2014b/bin/matlab";
        string workfolderdir    = "/home/homk/prg/dip";
        string Str              = "";

        Str = "rm " + workfolderdir + "/outS.dat " + workfolderdir + "/outT.dat 2>/dev/null";
        system(Str.c_str());

        Str = workfolderdir + "/outS.dat";
        ofstream fout(Str.c_str());

                for (i = 0; i < An; i++)
                {
                                for (j = 0; j < An; j++)
                                {
                                        fout << doubletostr(A[i * An + j]);
                                        fout << " ";
                                }
                        fout << "\n ";
                }

        fout.close();
        Str = workfolderdir + "/outT.dat";
        fout.open(Str.c_str());

                for (i = 0; i < Bn; i++)
                {
                                for (j = 0; j < Bn; j++)
                                {
                                        fout << doubletostr(B[i * Bn + j]);
                                        fout << " ";
                                }
                        fout << "\n ";
                }
        fout.close();

        Str = matlabdir + " -nodisplay -nosplash -nodesktop -nojvm -r \"S = load(\'" + workfolderdir + "/outS.dat\') ; T = load(\'" + workfolderdir + "/outT.dat\') ; eigS = eig(S) ; eigT = eig(T) ; save " + workfolderdir + "/eigS.dat eigS -double -ascii ; save " + workfolderdir + "/eigT.dat eigT -double -ascii ; exit\"";;

        // cout << Str << endl;
        system(Str.c_str());
        Str = "date && echo -e 'eig(S):' && echo -e '\n' && cat " + workfolderdir + "/eigS.dat && echo -e '\neig(T):' && cat " + workfolderdir + "/eigT.dat";
        system(Str.c_str());

        system("echo -e \' done.\'");
        return 0;
}




///////////////////////////////
////////////////    _   ///////
//  _ __ ___   __ _(_)_ __   //
// | '_ ` _ \ / _` | | '_ \  //
// | | | | | | (_| | | | | | //
// |_| |_| |_|\__,_|_|_| |_| //
///////////////////////////////

        /*
         * Вызов: programm_name path_to_file_.mphtxt bound_cond
         *                                                     
         * path_to_file_.mphtxt лучше писать в полном виде     
         *                                                     
         * bound_count может принимать значения из списка ниже:
         *     * d - Дирихле,                                  
         *     * n - Нейнман,                                  
         *     * f - Флаке.                                    
         */




int main(int argc, char *argv[])
{
        int GlNoftr             = 0;
        int GlSn                = 0;
        int GlTn                = 0;
        int GlXn                = 0;
        int GlYn                = 0;
        // mtl::dense_vector <double> GlT(GlTn * GlTn);
        // mtl::dense_vector <double> GlS(GlSn * GlSn); Это объявление я сделаю позднее по причинам технического характера, а тут просто оставлю для логичности повествования.
        vector <double> GlX;
        vector <double> GlY;
        vector <int> nodes;
        double A                = 0;
        int i                   = 0;
        int j                   = 0;
        int r                   = 0;
        int k                   = 0;
        int Cn                  = 0;
        double tmp              = 0;
        string file             = argv[1];
        char* bound_cond        = argv[2];
        string Str              = "";
        string nod_str          = "";

        Str = "sed -ri \'s/\\s+$//g' " + file;
        // cout << Str << endl;
        system(Str.c_str());

                if (argc < 2)
                {
                        cout << "Not enough options." << endl << "Program terminated." << endl;
                        exit(1);
                }

        GlXn = findnumbofnodes(file);
        GlYn = GlXn;
        GlSn = GlXn;
        GlTn = GlSn;

        mtl::dense_vector <double> GlT(GlTn * GlTn);
        mtl::dense_vector <double> GlS(GlSn * GlSn);    // Я мог бы сказать GlS.push_back(), но не могу, кажется. Так как это не обычный vector, а dense_vector. Я не нашёл, как ему объяснить, что я хочу пушбэкнуть разреженный вектор. Не очень красиво, мягко сказать, объявлять переменные в середине кода, ну...пока ничего умнее не придумал.
        GlT = 0;
        GlS = 0;

                for (i = 0; i < GlXn; i++)
                {
                        GlX.push_back(0);
                }

                for (i = 0; i < GlYn; i++)
                {
                        GlY.push_back(0);
                }

        getcoords(file, &GlX[0], &GlY[0]);

        GlNoftr = findnumbtr(file);

                for (i = 0; i < GlNoftr; i++)
                {
                        Str = findtrangle(file, i);
                        // cout << "Triangle " << i << ":" << endl;
                        find_locale_matrix(Str, &GlX[0], &GlY[0], &GlS[0], GlSn, &GlT[0], GlTn);
                }

                switch (*bound_cond)
                {
                case 'd':
                        // cout << endl << "GlX" << endl;
                        // show_double_matrix(&GlX[0], 1, GlXn);
                        // cout << endl;
                        // cout << endl << "GlY" << endl;
                        // show_double_matrix(&GlY[0], 1, GlYn);
                        // cout << endl;

                        cout << endl << "Matrix S:" << endl;
                        show_double_matrix(&GlS[0], GlSn, GlSn);
                        cout << endl;

                        cout << endl << "Matrix T:" << endl;
                        show_double_matrix(&GlT[0], GlTn, GlTn);
                        cout << endl;                        

                        call_matlab(&GlS[0], GlSn, &GlT[0], GlTn);
                        break;
                case 'f':
                        cout << endl << "Be soon.";
                        cout << endl;
                        break;
                case 'n':
                        Str = findsizeGlS(file);            // Ищу граничные узлы.
                                for (i = 0; i < Str.length(); i++)                      ////////
                                {                                                       ////
                                        if (Str[i] == ',')                              // Разбираю Str вида "число,число,число" на int array[число число число]
                                        {                                               // 
                                                nodes.push_back(atof(nod_str.c_str())); // Вот тут преобразование происходит.
                                                k++;                                    // 
                                                nod_str = "";                           // 
                                        }                                               // 
                                        else                                            // 
                                        {                                               // 
                                                nod_str += Str[i];                      // 
                                        }                                               ////
                                }                                                       ////////

                        Cn = GlSn - k;      // Размер матрицы outS2 и outT2

                        mtl::dense_vector <double> C(Cn * GlSn + GlSn);         // Единичная матрица
                        mtl::dense_vector <double> C_trans(Cn * GlSn + GlSn);   // Она же, но транспонированная.

                        int outST1n = GlSn * Cn;
                        int outST2n = Cn * Cn;
                        double outS1[outST1n];    //////
                        double outS2[outST2n];      // Объявить раньше не могу, так как мне нужен размер. Объявить как вектор не могу, потому что см. ниже.
                        double outT1[outST1n];    // Вообще, это надо объявлять как dense_vector. Но у меня компилятор хоть и выполняет операции корректно, но не может освободить память. Пока что так оставлю. В принципе, ничего страшного, но не хочу не освобождать память ._.
                        double outT2[outST2n];      //////

                        C       = 0;
                        C_trans = 0;

                        make_identity_matrix(&C[0], Cn, GlSn, &nodes[0], k);
                        transmatr(&C[0], &C_trans[0], GlSn, Cn);

                        // cout << endl << "GlX" << endl;
                        // show_double_matrix(&GlX[0], 1, GlXn);
                        // cout << endl;
                        // cout << endl << "GlY" << endl;
                        // show_double_matrix(&GlY[0], 1, GlYn);
                        // cout << endl;
                        // cout << endl << "C" << endl;
                        // show_double_matrix(&C[0], GlSn, Cn);
                        // cout << endl;
                        // cout << endl << "C_trans" << endl;
                        // show_double_matrix(&C_trans[0], GlSn, Cn);
                        // cout << endl;
                        // cout << endl << "GlS" << endl;
                        // show_double_matrix(&GlS[0], GlSn, GlSn);
                        // cout << endl;
                        // cout << endl << "GlT" << endl;
                        // show_double_matrix(&GlT[0], GlTn, GlTn);
                        // cout << endl;

                        multmatr(&GlS[0], &C[0], &outS1[0], GlSn, GlSn, Cn);            // 
                        multmatr(&C_trans[0], &outS1[0], &outS2[0], GlSn, GlSn, Cn);    // Здесь двумя операциями (домножением справа и слева на единичную матрицу) матрица GlS преобразуется в матрицу outS2.


                        // cout << endl << "outS2" << endl;
                        // show_double_matrix(&outS2[0], Cn, Cn);
                        // cout << endl;


                        multmatr(&GlT[0], &C[0], &outT1[0], GlSn, GlSn, Cn);            // 
                        multmatr(&C_trans[0], &outT1[0], &outT2[0], GlSn, GlSn, Cn);    // И матрица GlT - в outT2.


                        // cout << endl << "outT2" << endl;
                        // show_double_matrix(&outT2[0], Cn, Cn);
                        // cout << endl;

                        call_matlab(&outS2[0], Cn, &outT2[0], Cn);
                }        
   return 0;
}
