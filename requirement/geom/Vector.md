# Vector

## Структура
Все переменные лежат в памяти на стеке.
Размер задается template параметром
```
template<class T, size_t N>
struct Vector;
```


## Синтаксис

Конструкторы:
```c++
Vector a(1, 2, 3);   // -> Vector<int, 3>
Vector b(1.0, 2, 3); // -> Vector<double, 3>

Vector d = c;
Vector e = std::move(d);
```
Аргументы
```c++
a.x;  // Первый аргумент
a.y;  // Второй аргумент
a.z;  // Третий аргумент
a.t;  // Четвертый аргумент
a[i]; // Обращение по индексу
// индексируем c 0
a[0] == a.x; // всегда
```
Операторы:
```c++
// сложение векторов
operator +=; 
operator +;
// разность векторов
operator -=; 
operator -;
// умножение на скаляр
operator *=; 
operator *;
// деление на скаляр
operator /=; 
operator /;

// Сравнения
operator == 
operator != 
operator <=>
```
Функции:
```c++
dot(a, b); // -> T скалярное произведение
cross(a, b); // Для 2 и 3 мерных векоторов
normalized(a); // -> Vector
a.normalize(); // -> Vector&
a.length(); // U длина
a.sqrlength(); // T длина в квадрате
a.size(); // количество измерений constexpr

pref<2>(a);   // -> Vector<T, 2> copy
suf<2>(a);    // -> Vector<T, 2> copy
sub<0, 2>(a); // -> Vector<T, 2> copy

swap(a, b);
```
Циклы
```c++
for (auto x: point) {
  
}
```


