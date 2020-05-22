# MapReduceWebScraper

Для запуска скрапера достаточно выполнить в командной строке
```commandline
bash WebScraperMainScript input.txt output.txt depth
```
где depth - глубина захода.

Формат данных:
``Key\tValue\n``
где Key - ссылка, а Value=1, если мы 
еще не обрабатывали ссылку, и =0 иначе.

Скрипт запускает:

1) "map". В качестве map_script передается scraper.py, 
который выполняется питоном. "Map" делит файл на файлики, содержащие 
LENGTH_TO_DIVIDE_MAP строк, и запускает скрипт
 на каждом из них. Потом мерджит выводы в один temp.txt
 
1) "reduce".
   
   Сортировка. Используется внешняя сортировка обычного файла
   (т.е.значения разделены\`\n\`) методом merge sort.
   
   В качестве скрипта передается исполняемый файл, сгенерированный 
   ReduceScript.cpp, 
   
   Reduce запускает по одному процессу на каждую группу соседних равных строк
   (при равенстве ключей).
   
   Скрипт также учитывает особенности ссылок, например, следующие ссылки считаются за одну:
     - https://ya.ru, 
     - https://ya.ru/.    
   
  