[![Tests](https://github.com/adkostatt/Http/actions/workflows/cmake-single-platform.yml/badge.svg)](https://github.com/adkostatt/Http/actions/workflows/cmake-single-platform.yml)

Библиотека позволяет примитивно читать и создавать HTTP сообщения.

Функции в <Http.hpp> небезопасны и могут привести к UB, если пренебречь предупреждениям в комментариях.

В <HttpWrapper.hpp> есть класс Parser для безопасного использования функций Parse* и Generator для безопасного использования функций Generate*.

# Цели
- [ ] Оптимизация за счет SIMD
