
// #include <stdio.h>
// #include <windows.h>
// #include <string.h>

// char *ss(int portnumber, const char *sa)
// {
//     char *str = malloc(strlen(sa) + 1); // Allocate memory for the modified string
//     if (str == NULL)
//     {
//         fprintf(stderr, "Memory allocation failed\n");
//         return str;
//     }

//     strcpy(str, sa);
//     printf("name: %s\n", str);

//     HANDLE hConsole;
//     CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
//     WORD saved_attributes;
//     hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//     GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
//     saved_attributes = consoleInfo.wAttributes;
//     printf("Server is running at ");
//     SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
//     printf("http://localhost:%d/\n", portnumber);
//     SetConsoleTextAttribute(hConsole, saved_attributes);
//     return str;
// }

// int main()
// {
//     char *p, *q;

//     p = ss(80, "sahvsdhgsavdvdvsvdvwdvwdwvydwvdwvdvwvdv");
//     // Free the allocated memory
//     printf("p =  %s", p);
//     strcpy(q, p);
//     printf("q =  %s", q);
//     return 0;
// }




#include <stdio.h>
#include <windows.h>   // WinApi header
#include <string.h>

// int main()
// {
//   HANDLE  hConsole;
//     int k;

//   hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

//   // you can loop k higher to see more color choices
//   for(k = 1; k < 255; k++)
//   {
//     SetConsoleTextAttribute(hConsole, k);
//     printf("%3d  %s\n", k, "I want to be nice today!");
//   }

//   getchar();  // wait
//   return 0;
// }



void ss(int portnumber,char *sa)
{
 
    HANDLE hConsole;
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;
    printf("Server is running at "); 
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("http://localhost:%d/\n", portnumber);
    SetConsoleTextAttribute(hConsole, saved_attributes);
}

int main(){ 
    printf("\x1b[38;5;46m  \x1b[0m");
    return 0;
}

// i am not getting expected output i am confused in dealing with strings in c can you help me












