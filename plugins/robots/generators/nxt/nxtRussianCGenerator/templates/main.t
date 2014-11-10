@@VARIABLES@@

@@BMP_FILES@@

@@SUBPROGRAMS_FORWARDING@@

@@THREADS_FORWARDING@@

Инциализация
{
	инициализировать_генератор_случайных_чисел();
@@INITHOOKS@@
}

Остановка
{
@@TERMINATEHOOKS@@
}

Выполнять_Каждую_Миллисекунду
{ 
@@USERISRHOOKS@@
}

@@SUBPROGRAMS@@

@@THREADS@@

Программа
{
@@MAIN_CODE@@
}
