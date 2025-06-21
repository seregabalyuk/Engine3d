# описание
**eng2d** - это движек для работы с 2d объектами.
Пока имеется только 2d графика.

Все классы обёрнуты в `namespace eng2d`.

Опирается на **geom**.

## сосотоит из
### графика
**Базовые примитивы** которые на прямую работают с пикселями экрана:
1) [Trapezoid](./graphics/Trapezoid.md)

**Объекты** более высокого уровня:
1) [BaseConvex](./graphics/BaseConvex.md)

Для покраски используется классы **контексты**:
1) [Context](./graphics/Context.md) - общая концепция класса.

# UML classes
```mermaid
classDiagram
class Surface {
  <<template>>
  using T
  using Pixel = T
  + size_t width
  + size_t height
  + T* pixels
  + draw(figure, args&&...)
  + context() Surface::Context
}

class Surface_Context {
  - Surface* _surface
  - T* _iterX
  - T* _iterY
  
  + set(auto&& context)
}

class BaseConvex {
  <<template>>
  using T
  using Point = geom::Vector~T, 2~
  - size_t _size
  - Point* _array

  + draw(Surf& surface, args&&...)
  + normalize()
  + normalizeLight()
  + begin() Point*
  + end() Point*
}

class Trapezoid {
  <<template>>
  using T
  using Line = geom::Line~T, 2~
  - int _yBegin
  - int _yEnd
  - const Line* _left
  - const Line* _right
  
  + draw(Surf& surface, auto&& context)
}

class Context {
  <<concept>>
  + setY(int Y)
  + setX(int X)
  + incY()
  + incX()
}
Context <|.. Surface_Context
Surface *-- Surface_Context : create
BaseConvex ..> Trapezoid : create
```

# UML sequence
Как работает Surface.draw(SimpleConvex, context):
```mermaid
sequenceDiagram
participant Surface
participant Context
participant BaseConvex
participant Trapezoid
participant Surface_Context


Surface ->> BaseConvex : call draw
BaseConvex ->> BaseConvex : create Trapezoid
BaseConvex ->> Trapezoid : call draw

Surface ->> Trapezoid : create Surface_Context

Context ->> Surface_Context: transmits Context 
Trapezoid ->> Surface_Context : call set
```

