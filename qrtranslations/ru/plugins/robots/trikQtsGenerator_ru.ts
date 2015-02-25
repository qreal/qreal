<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ru_RU">
<context>
    <name>QObject</name>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="115"/>
        <source>Trying to join a thread with an unknown id. Possible causes: calling fork from a subprogram or trying to merge two threads without a join</source>
        <translation>В блок &quot;Слияние задач&quot; входит задача с неопределённым идентификатором. Возможные причины: вызов блока &quot;Параллельные задачи&quot; из подпрограммы либо попытка слить задачи без блока &quot;Слияние задач&quot;</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="122"/>
        <source>Join block must have exactly one outgoing link</source>
        <translation>Блок &quot;Слияние задач&quot; должен иметь ровно одну исходящую связь</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="128"/>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="167"/>
        <source>Guard property of a link outgoing from a join must contain an id of one of joined threads</source>
        <translation>Свойство &quot;Условие&quot; связи, исходящей из блока &quot;Слияние задач&quot;, должно содержать идентификатор одной из сливаемых задач</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="135"/>
        <source>Joining threads in a loop is forbidden</source>
        <translation>Вызов блока &quot;Слияние задач&quot; в цикле запрещён</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="191"/>
        <source>Trying to fork from a thread with an unknown id. Possible causes: calling fork from a subprogram or trying to merge two threads without a join</source>
        <translation>Вызов блока &quot;Параллельные задачи&quot; в задаче, имеющей неопределённый идентификатор. Возможные причины: вызов блока &quot;Параллельные задачи&quot; из подпрограммы либо попытка слить задачи без блока &quot;Слияние задач&quot;</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="197"/>
        <source>Fork block must have at least TWO outgoing links</source>
        <translation>Блок &quot;Параллельные задачи&quot; должен иметь как минимум две исходящие связи</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="203"/>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="215"/>
        <source>All links outgoing from a fork block must have a thread id specified in a Guard property</source>
        <translation>Свойство &quot;Условие&quot; каждой связи, исходящей из блока &quot;Параллельные задачи&quot;, должно содержать идентификатор задачи</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="221"/>
        <source>Links outgoing from a fork block must have different thread ids</source>
        <translation>Все связи, исходящие из блока &quot;Параллельные задачи&quot;, должны содержать в свойстве &quot;Условие&quot; различные идентификаторы задач</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="236"/>
        <source>Fork block must have a link marked with an id of a thread from which the fork is called, &apos;%1&apos; in this case</source>
        <translation>Блок &quot;Параллельные задачи&quot; должен иметь исходящую связь, отмеченную идентификатором задачи, из которой этот блок вызван, в данном случае &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="247"/>
        <source>Trying to create a thread with an already occupied id &apos;%1&apos;</source>
        <translation>Попытка создать задачу с уже занятым идентификатором &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="257"/>
        <source>Creation of threads in a cycle is forbidden, check for links to before a fork</source>
        <translation>Создание задач в цикле запрещено, проверьте связи, ведущие к блокам до блока &quot;Параллельные задачи&quot;</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="293"/>
        <source>Outgoing link is not connected</source>
        <translation>Исходящая связь ни к чему не подключена</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/trikQtsControlFlowValidator.cpp" line="21"/>
        <source>There is nothing to generate, diagram doesn&apos;t have Initial Node</source>
        <translation>Генерация невозможна, на диаграмме нет начальных блоков</translation>
    </message>
</context>
<context>
    <name>trik::qts::TrikQtsGeneratorPlugin</name>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/trikQtsGeneratorPlugin.cpp" line="44"/>
        <source>Generate TRIK code</source>
        <translation>Генерировать код на QtScript</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/trikQtsGeneratorPlugin.cpp" line="49"/>
        <source>Upload program</source>
        <translation>Загрузить программу</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/trikQtsGeneratorPlugin.cpp" line="54"/>
        <source>Run program</source>
        <translation>Загрузить и выполнить программу</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/trikQtsGeneratorPlugin.cpp" line="59"/>
        <source>Stop robot</source>
        <translation>Остановить робота</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/trikQtsGeneratorPlugin.cpp" line="74"/>
        <source>Generate TRIK Code</source>
        <translation>Сгенерированть код для ТРИК на QtScript</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/trikQtsGeneratorPlugin.cpp" line="75"/>
        <source>Upload TRIK Program</source>
        <translation>Загрузить программу</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/trikQtsGeneratorPlugin.cpp" line="76"/>
        <source>Run TRIK Program</source>
        <translation>Исполнить программу</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/trikQtsGeneratorPlugin.cpp" line="77"/>
        <source>Stop TRIK Robot</source>
        <translation>Остановить выполнение программы для TRIK</translation>
    </message>
    <message>
        <source>QtScript Source File</source>
        <translation type="vanished">Файл с исходным кодом на QtScript</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/trikQtsGeneratorPlugin.cpp" line="115"/>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/trikQtsGeneratorPlugin.cpp" line="138"/>
        <source>No connection to robot</source>
        <translation>Нет соединения с роботом, проверьте настройки, проверьте, включён ли робот</translation>
    </message>
</context>
</TS>
