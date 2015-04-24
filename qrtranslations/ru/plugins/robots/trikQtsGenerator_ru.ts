<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ru_RU">
<context>
    <name>QObject</name>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="116"/>
        <source>Trying to join a thread with an unknown id. Possible causes: calling fork from a subprogram or trying to merge two threads without a join</source>
        <translation>Попытка присоединиться к потоку с неизвестным идентификатором. Возможные причины: распараллеливание внутри подпрограммы или попытка склеить два потока без блока &quot;join&quot;</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="123"/>
        <source>Join block must have exactly one outgoing link</source>
        <translation>Из блок соединения потоков должна выходить в точности одна связь</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="129"/>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="171"/>
        <source>Guard property of a link outgoing from a join must contain an id of one of joined threads</source>
        <translation>На стрелке из блока соединения потоков должен быть идентификатор одного из склеиваемых потоков</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="136"/>
        <source>Joining threads in a loop is forbidden</source>
        <translation>Соединение потоков в цикле запрещено</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="195"/>
        <source>Trying to fork from a thread with an unknown id. Possible causes: calling fork from a subprogram or trying to merge two threads without a join</source>
        <translation>Попытка создать поток в потоке с неизвестным идентификатором. Возможные причины: создание нового потока в подпрограмме или попытка склеить два потока без блока Join</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="201"/>
        <source>Fork block must have at least TWO outgoing links</source>
        <translation>Блок &quot;Параллельные задачи&quot; должен иметь как минимум две исходящие связи</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="207"/>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="219"/>
        <source>All links outgoing from a fork block must have a thread id specified in a Guard property</source>
        <translation>На всех связях, исходящих из блока &quot;Параллельные задачи&quot; должнен быть проставлен идентификатор потока</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="225"/>
        <source>Links outgoing from a fork block must have different thread ids</source>
        <translation>Идентификаторы на связях из блока &quot;Параллельные задачи&quot; должны быть различны</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="240"/>
        <source>Fork block must have a link marked with an id of a thread from which the fork is called, &apos;%1&apos; in this case</source>
        <translation>Из блока &quot;Параллельные задачи&quot; должна выходить связь с идентификатором текущего потока, в данном случае &quot;%1&quot;</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="251"/>
        <source>Trying to create a thread with an already occupied id &apos;%1&apos;</source>
        <translation>Попытка создать поток с уже занятым идентификатором &quot;%1&quot;</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="261"/>
        <source>Creation of threads in a cycle is forbidden, check for links to before a fork</source>
        <translation>Создание потоков в цикле запрещено, проверьте связи, ведущие в блоки перед блоком &quot;Параллельные задачи&quot;</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/threadsValidator.cpp" line="297"/>
        <source>Outgoing link is not connected</source>
        <translation>Исходящая связь ни к чему не присоединена</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/trikQtsControlFlowValidator.cpp" line="21"/>
        <source>There is nothing to generate, diagram doesn&apos;t have Initial Node</source>
        <translation>На диаграмме нет блока &quot;Начало&quot;</translation>
    </message>
</context>
<context>
    <name>trik::qts::TrikQtsGeneratorPlugin</name>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/trikQtsGeneratorPlugin.cpp" line="16"/>
        <source>Generation (QtScript)</source>
        <translation>Генерация (QtScript)</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/trikQtsGeneratorPlugin.cpp" line="39"/>
        <source>Generate TRIK code</source>
        <translation>Генерировать код на QtScript</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/trikQtsGeneratorPlugin.cpp" line="44"/>
        <source>Upload program</source>
        <translation>Загрузить программу</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/trikQtsGeneratorPlugin.cpp" line="49"/>
        <source>Run program</source>
        <translation>Загрузить и выполнить программу</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/trikQtsGeneratorPlugin.cpp" line="54"/>
        <source>Stop robot</source>
        <translation>Остановить робота</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/trikQtsGeneratorPlugin.cpp" line="69"/>
        <source>Generate TRIK Code</source>
        <translation>Сгенерированть код для ТРИК на QtScript</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/trikQtsGeneratorPlugin.cpp" line="70"/>
        <source>Upload TRIK Program</source>
        <translation>Загрузить программу</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/trikQtsGeneratorPlugin.cpp" line="71"/>
        <source>Run TRIK Program</source>
        <translation>Исполнить программу</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/trikQtsGeneratorPlugin.cpp" line="72"/>
        <source>Stop TRIK Robot</source>
        <translation>Остановить выполнение программы для TRIK</translation>
    </message>
    <message>
        <source>QtScript Source File</source>
        <translation type="vanished">Файл с исходным кодом на QtScript</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/trikQtsGeneratorPlugin.cpp" line="116"/>
        <location filename="../../../../plugins/robots/generators/trik/trikQtsGenerator/trikQtsGeneratorPlugin.cpp" line="139"/>
        <source>No connection to robot</source>
        <translation>Нет соединения с роботом, проверьте настройки, проверьте, включён ли робот</translation>
    </message>
</context>
</TS>
