<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="ru_RU">
<context>
    <name>QObject</name>
    <message>
        <location filename="nxtOSEK/nxtOSEKRobotGenerator.cpp" line="102"/>
        <location filename="nxtOSEK/nxtOSEKRobotGenerator.cpp" line="193"/>
        <source>There is nothing to generate, diagram doesn&apos;t have Initial Node</source>
        <translation>Генерация невозможна, на диаграмме нет начальных блоков</translation>
    </message>
    <message>
        <location filename="nxtOSEK/generators/simpleElementGenerator.cpp" line="48"/>
        <source>There is no outgoing connected element with no final node!</source>
        <translation>Отсутствует исходящий элемент!</translation>
    </message>
    <message>
        <location filename="nxtOSEK/generators/simpleElementGenerator.cpp" line="75"/>
        <source>There are more than 1 outgoing connected elements with simple robot element!</source>
        <translation>Обнаружено более 1 исходящей связи!</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtOSEKRobotGenerator.cpp" line="158"/>
        <source>There is no opened diagram</source>
        <translation>Сначала откройте диаграмму </translation>
    </message>
    <message>
        <location filename="nxtOSEK/generators/commentElementGenerator.cpp" line="27"/>
        <source>The comment block with incomming liks detected!</source>
        <translation>Блок коментария не может иметь входящих связей!</translation>
    </message>
    <message>
        <location filename="nxtOSEK/generators/ifElementGenerator.cpp" line="169"/>
        <location filename="nxtOSEK/generators/ifElementGenerator.cpp" line="176"/>
        <source>If block </source>
        <translation>У условного блока </translation>
    </message>
    <message>
        <location filename="nxtOSEK/generators/ifElementGenerator.cpp" line="169"/>
        <location filename="nxtOSEK/generators/ifElementGenerator.cpp" line="176"/>
        <source> has no 2 correct branches! May be you need to connect one of them to some diagram element.</source>
        <translation> найдены не присоединенные ветки!</translation>
    </message>
    <message>
        <location filename="nxtOSEK/generators/ifElementGenerator.cpp" line="209"/>
        <source>This diagram isn&apos;t structed diagram, because there are IF block with 2 back arrows!</source>
        <translation>Диаграмма не структурная т.к. существует условный блок с двумя обратными стрелками!</translation>
    </message>
    <message>
        <location filename="nxtOSEK/generators/ifElementGenerator.cpp" line="213"/>
        <source>Generator JUST does not work in this case.</source>
        <translation>Данная диаграмма слишком сложна для генерации.</translation>
    </message>
</context>
<context>
    <name>robots::generator::NxtFlashTool</name>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="37"/>
        <source>Firmware flash started. Please don&apos;t disconnect robot during the process</source>
        <translation>Начат процесс загрузки прошивки в робота. Пожалуйста, не отсоединяйте робота, пока процесс не будет завершен</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="43"/>
        <source>Some error occured. Make sure you are running QReal with superuser privileges</source>
        <translation>Ошибка. Убедитесь, что QReal:Robots запущен с правами администратора</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="50"/>
        <source>Flashing process completed.</source>
        <translation>Процесс загрузки программы завершен.</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="52"/>
        <source>flash.sh not found. Make sure it is present in QReal installation directory</source>
        <translation>Не найден скрипт flash.sh. Убедитесь, что пакет nxt-tools установлен корректно</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="54"/>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="95"/>
        <source>QReal requires superuser privileges to flash NXT robot</source>
        <translation>Для загрузки прошивки в робота требуется запустить QReal:Robots с правами администратора</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="67"/>
        <source>NXT not found. Check USB connection and make sure the robot is ON</source>
        <translation>Соединение с роботом не обнаружено. Проверьте USB-соединение и убедитесь, что робот включен</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="69"/>
        <source>NXT is not in reset mode. Please reset your NXT manually and try again</source>
        <translation>Робот находится в неправильном режиме. Переведите робота в режим перепрошивки</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="71"/>
        <source>Firmware flash complete!</source>
        <translation>Процесс загрузки прошивки в робота завершен!</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="86"/>
        <source>Uploading program started. Please don&apos;t disconnect robot during the process</source>
        <translation>Начат процесс загрузки программы в робота. Пожалуйста, не отсоединяйте робота, пока процесс не будет завершен</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="93"/>
        <source>Uploading failed. Make sure that X-server allows root to run GUI applications</source>
        <translation>Не удалось загрузить программу в робота. Убедитесь, что QReal:Robots запущен с нужными правами</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="122"/>
        <source>Could not upload program. Make sure the robot is connected and ON</source>
        <translation>Не удалось загрузить программу в робота. Убедитесь, что робот включен и подсоединен к компьютеру</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="124"/>
        <source>Uploading completed successfully</source>
        <translation>Загрузка программы завершена успешно</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="126"/>
        <source>Compilation error occured. Please check your function blocks syntax. If you sure in their validness contact developers</source>
        <translation>Произошла ошибка компиляции. Проверьте синтаксис выражений внутри блоков &quot;Функция&quot;. Если Вы уверены в их корректности, обратитесь к разработчикам</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="130"/>
        <source>QReal requires superuser privileges to upload programs on NXT robot</source>
        <translation>Для загрузки программ в робота требуется запустить QReal:Robots с правами администратора</translation>
    </message>
</context>
<context>
    <name>robots::generator::RobotsGeneratorPlugin</name>
    <message>
        <location filename="robotsGeneratorPlugin.cpp" line="37"/>
        <location filename="robotsGeneratorPlugin.cpp" line="67"/>
        <source>Generate code</source>
        <translation>Генерировать код</translation>
    </message>
    <message>
        <location filename="robotsGeneratorPlugin.cpp" line="42"/>
        <source>Flash robot</source>
        <translation>Прошить</translation>
    </message>
    <message>
        <location filename="robotsGeneratorPlugin.cpp" line="46"/>
        <source>Upload program</source>
        <translation>Загрузить программу</translation>
    </message>
    <message>
        <location filename="robotsGeneratorPlugin.cpp" line="68"/>
        <source>Upload program to robot</source>
        <translation>Загрузить программу на робота</translation>
    </message>
    <message>
        <location filename="robotsGeneratorPlugin.cpp" line="113"/>
        <source>flash.sh not found. Make sure it is present in QReal installation directory</source>
        <translation>Не найден скрипт flash.sh. Убедитесь, что пакет nxt-tools установлен корректно</translation>
    </message>
    <message>
        <location filename="robotsGeneratorPlugin.cpp" line="122"/>
        <source>upload.sh not found. Make sure it is present in QReal installation directory</source>
        <translation>Не найден скрипт upload.sh. Убедитесь, что пакет nxt-tools установлен корректно</translation>
    </message>
</context>
</TS>
