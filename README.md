# MapReduceWebScraper


С помощью скрипта можно узнать, на каких страницы можно перейти 
из имеющихся при заданной глубине обхода.

Перед запуском нужно скомпилировать с++:
```commandline
bash BuildScripts.sh
```
(а можно просто переместить из папки build)

Для запуска скрапера достаточно выполнить в командной строке
```commandline
bash WebScraperMainScript input.txt output.txt depth
```

где depth - глубина захода,
input.txt, output.txt - входной и выходной файлы.

Формат данных:
``Key\tValue\n``
где Key - ссылка, а Value=1, если мы 
еще не обрабатывали ссылку, и =0 иначе.

Скрипт запускает:

1) "map". В качестве map_script передается scraper.py, 
который выполняется питоном. "Map" делит файл на файлики, содержащие не более
LENGTH_TO_DIVIDE_MAP (можно изменять в Constants.h) строк, 
и запускает скрипт на каждом из них. Потом мерджит выводы в один temp.txt
 
1) "reduce".
   
   Сортировка. Используется внешняя сортировка обычного файла
   (т.е.значения разделены\`\n\`) методом merge sort.
   
   В качестве скрипта передается исполняемый файл, сгенерированный 
   ReduceScript.cpp, 
   
   Reduce запускает по одному процессу на каждую группу соседних равных строк
   (при равенстве ключей).
   
Для работы необходимы следующие библиотеки:
- python3 bs4

Для генерации можно использовать исполняемый файл из generate.cpp:
```commandline
./generate number_of_lines > file_name.txt
```
