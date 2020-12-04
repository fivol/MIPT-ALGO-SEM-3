## Задача A “Сжатие с потерями”

### Компиляция
`g++ main.cpp -o wavcompressor.out`

### Использоние
Два обязательных агрумента и 2 необязательных:
1. Название исходного файла (required)
2. Навзание выходного файла (required)
3. Коэффициент сжатия в виде десятичного числа с точкой. 
0 - не сжимать данные, 0.2 - сжать на 20 процентов. 
По умолчанию: 0.3 (not required)
4. Размер страницы сжатия. По умлочанию 128

Примеры:
- `./wavcompressor.out input_example.wav output.wav`
- `./wavcompressor.out input_example.wav output.wav 0.6`
- `./wavcompressor.out input_example.wav output.wav 0.3567 512`