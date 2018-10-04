<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ru_RU">
<context>
    <name>QObject</name>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikPythonGeneratorLibrary/src/threadsValidator.cpp" line="136"/>
        <source>Trying to join a thread with an unknown id. Possible causes: calling fork from a subprogram or trying to merge two threads without a join</source>
        <translation>Попытка присоединиться к потоку с неизвестным идентификатором. Возможные причины: распараллеливание внутри подпрограммы или попытка склеить два потока без блока &quot;join&quot;</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikPythonGeneratorLibrary/src/threadsValidator.cpp" line="143"/>
        <source>Join block must have exactly one outgoing link</source>
        <translation>Из блок соединения потоков должна выходить в точности одна связь</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikPythonGeneratorLibrary/src/threadsValidator.cpp" line="149"/>
        <location filename="../../../../plugins/robots/generators/trik/trikPythonGeneratorLibrary/src/threadsValidator.cpp" line="194"/>
        <source>Guard property of a link outgoing from a join must contain an id of one of joined threads</source>
        <translation>На стрелке из блока соединения потоков должен быть идентификатор одного из склеиваемых потоков</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikPythonGeneratorLibrary/src/threadsValidator.cpp" line="156"/>
        <source>Joining threads in a loop is forbidden</source>
        <translation>Соединение потоков в цикле запрещено</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikPythonGeneratorLibrary/src/threadsValidator.cpp" line="218"/>
        <source>Trying to fork from a thread with an unknown id. Possible causes: calling fork from a subprogram or trying to merge two threads without a join</source>
        <translation>Попытка создать поток в потоке с неизвестным идентификатором. Возможные причины: создание нового потока в подпрограмме или попытка склеить два потока без блока Join</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikPythonGeneratorLibrary/src/threadsValidator.cpp" line="224"/>
        <source>Fork block must have at least TWO outgoing links</source>
        <translation>Блок &quot;Параллельные задачи&quot; должен иметь как минимум две исходящие связи</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikPythonGeneratorLibrary/src/threadsValidator.cpp" line="253"/>
        <source>Links outgoing from a fork block must have different thread ids</source>
        <translation>Идентификаторы на связях из блока &quot;Параллельные задачи&quot; должны быть различны</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikPythonGeneratorLibrary/src/threadsValidator.cpp" line="268"/>
        <source>Fork block must have a link marked with an id of a thread from which the fork is called, &apos;%1&apos; in this case</source>
        <translation>Из блока &quot;Параллельные задачи&quot; должна выходить связь с идентификатором текущего потока, в данном случае &quot;%1&quot;</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikPythonGeneratorLibrary/src/threadsValidator.cpp" line="279"/>
        <source>Trying to create a thread with an already occupied id &apos;%1&apos;</source>
        <translation>Попытка создать поток с уже занятым идентификатором &quot;%1&quot;</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikPythonGeneratorLibrary/src/threadsValidator.cpp" line="289"/>
        <source>Creation of threads in a cycle is forbidden, check for links to before a fork</source>
        <translation>Создание потоков в цикле запрещено, проверьте связи, ведущие в блоки перед блоком &quot;Параллельные задачи&quot;</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikPythonGeneratorLibrary/src/threadsValidator.cpp" line="325"/>
        <source>Outgoing link is not connected</source>
        <translation>Исходящая связь ни к чему не присоединена</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikPythonGeneratorLibrary/src/trikPythonControlFlowValidator.cpp" line="35"/>
        <source>There is nothing to generate, diagram doesn&apos;t have Initial Node</source>
        <translation>На диаграмме нет блока &quot;Начало&quot;</translation>
    </message>
</context>
<context>
    <name>trik::python::TrikPythonGeneratorPluginBase</name>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikPythonGeneratorLibrary/src/trikPythonGeneratorPluginBase.cpp" line="71"/>
        <source>Network operation timed out</source>
        <translation>Не удалось получить ответ от робота, проверьте настройки, проверьте, включён ли робот</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikPythonGeneratorLibrary/src/trikPythonGeneratorPluginBase.cpp" line="98"/>
        <source>Casing model mismatch, check TRIK Studio settings, &quot;Robots&quot; page. It seems that TRIK casing version selected in TRIK Studio differs from version on robot.</source>
        <translation>Модель корпуса робота не совпадает с указанной в настройках. Проверьте окно настроек, вкладка &quot;Роботы&quot;, должна быть выбрана модель ТРИКа, соответствующая корпусу робота.</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikPythonGeneratorLibrary/src/trikPythonGeneratorPluginBase.cpp" line="107"/>
        <source>Generate python code</source>
        <translation>Генерировать код на Python</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikPythonGeneratorLibrary/src/trikPythonGeneratorPluginBase.cpp" line="113"/>
        <source>Upload program</source>
        <translation>Загрузить программу</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikPythonGeneratorLibrary/src/trikPythonGeneratorPluginBase.cpp" line="119"/>
        <source>Run program</source>
        <translation>Загрузить и выполнить программу</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikPythonGeneratorLibrary/src/trikPythonGeneratorPluginBase.cpp" line="125"/>
        <source>Stop robot</source>
        <translation>Остановить робота</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikPythonGeneratorLibrary/src/trikPythonGeneratorPluginBase.cpp" line="140"/>
        <source>Generate Python code</source>
        <translation>Сгенерированть код для ТРИК на QtScript</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikPythonGeneratorLibrary/src/trikPythonGeneratorPluginBase.cpp" line="141"/>
        <source>Upload Python program</source>
        <translation>Загрузить программу</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikPythonGeneratorLibrary/src/trikPythonGeneratorPluginBase.cpp" line="142"/>
        <source>Run Python program</source>
        <translation>Исполнить программу</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikPythonGeneratorLibrary/src/trikPythonGeneratorPluginBase.cpp" line="143"/>
        <source>Stop Python program</source>
        <translation>Остановить выполнение программы для TRIK</translation>
    </message>
</context>
</TS>
