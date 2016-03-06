<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ru_RU">
<context>
    <name>QObject</name>
    <message>
        <source>Emulates robot`s behaviour on TRIK Studio 2D model separately from programming environment. Passed .qrs will be interpreted just like when &apos;Run&apos; button was pressed in TRIK Studio.</source>
        <translation type="vanished">Эмулирует поведение робота на 2D модели TRIK Studio отдельно от редактора программ. Указанный файл сохранения .qrs будет исполнен, как в случае нажатия на кнопку &quot;Запуск&quot; в среде.</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/checker/twoDModelRunner/main.cpp" line="30"/>
        <source>Emulates robot`s behaviour on TRIK Studio 2D model separately from programming environment. Passed .qrs will be interpreted just like when &apos;Run&apos; button was pressed in TRIK Studio. 
In background mode the session will be terminated just after the execution ended and return code will then contain binary information about program correctness.Example: 
</source>
        <translation>Эмулирует поведение робота на 2D модели TRIK Studio отдельно от редактора программ. Указанный файл сохранения .qrs будет исполнен, как в случае нажатия на кнопку &quot;Запуск&quot; в среде. 
В режиме проверки сессия будет автоматически закрыта по факту завершения исполнения программы, в этом случае код возврата будет содержать бинарную информацию о корректности программы. Пример: 
</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/checker/twoDModelRunner/main.cpp" line="79"/>
        <source>Save file to be interpreted.</source>
        <translation>Файл сохранения, который будет исполнен.</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/checker/twoDModelRunner/main.cpp" line="80"/>
        <source>Run emulation in background.</source>
        <translation>Произвести эмуляцию в фоне.</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/checker/twoDModelRunner/main.cpp" line="82"/>
        <source>Use this option set to &quot;minimal&quot; to disable connection to X server</source>
        <translation>Используйте эту опцию, установленную в &quot;minimal&quot; для того, чтобы запустить программу без X-сервера</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/checker/twoDModelRunner/main.cpp" line="83"/>
        <source>A path to file where checker results will be written (JSON)</source>
        <translation>Путь к файлу, куда будут записаны результаты проверки (JSON)</translation>
    </message>
    <message>
        <location filename="../../../../plugins/robots/checker/twoDModelRunner/main.cpp" line="85"/>
        <source>A path to file where robot`s trajectory will be written. The writing will not be performed not immediately, each trajectory point will be written just when obtained by checker, so FIFOs are recommended to be targets for this option.</source>
        <translation>Путь к файлу, куда будет выводиться траектори робота. Запись не будет осуществлена единомоментно, каждый узел траектории будет записан по факту его просчета проверяющей системой. Поэтому разумно использования FIFO-файлов в качестве значения этого параметра.</translation>
    </message>
    <message>
        <source>A path to file where robot`s trajectory will be written. The writing will not be performed not immediately, each trajectory point will be written just when obtained by checker, engine so FIFOs are recommended to be targets for this option.</source>
        <translation type="obsolete">Путь к файлу, куда будует выводиться траектория робота. Записть не будет осуществлена одномоментно, каждый узел траектории будет записан по факту его подсчета проверяющей системой</translation>
    </message>
</context>
</TS>
