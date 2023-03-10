#include <dirent.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define NMAX 1024
#define IFDIR 0040000 // Каталог
#define MASK 0170000  // Битовая маска битового поля для типа файла

// Функция "склейки" двух строк
char* Concat(char* str1, char* str2)
{
   // Получить длину каждой из входной строки
   const size_t len1 = strlen(str1);
   const size_t len2 = strlen(str2);

   // Выделить динамическиую память для хранения "склеенной" строки + 1 символ для нуль терминатора (\0)
   char* result = (char*)malloc((len1 + len2 + 1) * sizeof(char));

   if (!result)
      return NULL;
   
   // Скопировать строку str1 в result
   strcpy(result, str1);
   // Добаить к строке result строку str2
   strcat(result, str2);
   
   return result;
}

// Функция проверки директории на отсутствие вложенных директорий
int IsEmptyDir(DIR* dirToCheck, char* dirName)
{
   // Структура, содержащая о запись внутри директории
   struct dirent* record;

   // Читаем записи директории пока существуют
   while ((record = readdir(dirToCheck)) != NULL)
   {
      // Структура, хранящая информацию о файле
      struct stat st;
      
      // Если не удалось взять информацию о файле -> идем к следующей записи
      if (stat(Concat(dirName, record->d_name), &st) != 0)
      {
         fprintf(stderr, "%s -> ", Concat(dirName, record->d_name));
         perror("stat()");
         continue;
      }

      // Если это особые директории -> идем к следующей записи
      if (strcmp(record->d_name, "..") == 0 || strcmp(record->d_name, ".") == 0)
	 continue;
      
      // Если директория -> выходим из функции, возвращая резултат о непустоте директории
      if ((st.st_mode & MASK) == IFDIR)
	 return 0;
   }
   // Если текущая директория не содержит в себе вложенных директорий
   return 1;
}

// Обход директории начиная с dirPath
void DirectoryTraversal(DIR* startDir, char* dirPath)
{
   // Структура, содержащая информацию о директории
   struct dirent* record;

   int i = 0;
   printf("========================\n");
   printf("N   Name Directory");

   // Читаем записи директории пока существуют
   while ((record = readdir(startDir)) != NULL)
   {
      // Структура, хранящая информацию о файле
      struct stat st;

      // Если не удалось взять информацию о файле -> идем к следующей записи
      if (stat(Concat(dirPath, record->d_name), &st) != 0)
      {
         fprintf(stderr, "%s -> ", Concat(dirPath, record->d_name));
         perror("stat()");
         continue;
      }

      // Если не директория -> идем к следующей записи
      if ((st.st_mode & MASK) != IFDIR)
	 continue;

      // Открывает директорию
      DIR* nestedDirectory = opendir(Concat(dirPath, record->d_name));

      // При неуспешном открытии директории -> идем к следующей записи
      if (nestedDirectory == NULL)
      {
         fprintf(stderr, "%s -> ", Concat(dirPath,record->d_name));
         perror("opendir()");
         continue;
      }
     
      // Если пустая директория -> выводим ее имя
      if (IsEmptyDir(nestedDirectory, Concat(Concat(dirPath, record->d_name), "/")))
         printf("\n[%d] %s", ++i, &record->d_name);

      // Закрывает открытую ранее директорию
      closedir(nestedDirectory);
   }
   
   printf("\n========================\n");
}

int main(int numberOfArguments, char *arguments[])
{
   // Если не была введена дирректория
   if (numberOfArguments < 2)
   {
      fprintf(stderr, "Arguments too few (The directory name is missing)\n");
      return EXIT_FAILURE;
   }

   // Выделяем динамическую память для хранения пути к стартовому каталогу
   char* dirName = (char*)malloc(NMAX * sizeof(char));
  
   dirName = Concat(arguments[1], "/");
   
   // Открывает директорию
   DIR* d = opendir(dirName);

   // При неуспешном открытии директории
   if (d == NULL)
   {  
      fprintf(stderr, "%s -> ", arguments[1]);
      perror("opendir()");
      return EXIT_FAILURE;
   }

   DirectoryTraversal(d, dirName);

   // printf("Красава!\n");

   // Освобождаем память выделенную malloc
   free(dirName);
   // Закрывает открытую ранее директорию
   closedir(d);

   return EXIT_SUCCESS;
}
